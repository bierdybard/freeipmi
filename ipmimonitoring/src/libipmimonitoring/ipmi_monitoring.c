/*****************************************************************************\
 *  $Id: ipmi_monitoring.c,v 1.4 2007-02-17 01:40:52 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2006 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-222073
 *
 *  This file is part of Ipmimonitoring, an IPMI sensor monitoring
 *  library.  For details, see http://www.llnl.gov/linux/.
 *
 *  Ipmimonitoring is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  Ipmimonitoring is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmimonitoring; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <assert.h>
#include <errno.h>

#include "ipmi_monitoring.h"
#include "ipmi_monitoring_defs.h"
#include "ipmi_monitoring_debug.h"
#include "ipmi_monitoring_ipmi_communication.h"
#include "ipmi_monitoring_sdr_cache.h"
#include "ipmi_monitoring_sensor_config.h"
#include "ipmi_monitoring_sensor_reading.h"

#include "secure.h"

static char *ipmi_monitoring_errmsgs[] =
  {
    "success",
    "context null",
    "context invalid",
    "invalid parmaeters",
    "permission denied",
    "library uninitialized",
    "config file parse error",
    "sensor config file parse error",
    "sdr cache permission error",
    "sdr cache filesystem error",
    "hostname invalid",
    "sensor not found",
    "no sensor readings available",
    "end of sensor readings list reached",
    "session timeout",
    "invalid username",
    "invalid password",
    "invalid privilege level",
    "invalid authentication type",
    "BMC busy",
    "internal IPMI error",
    "out of memory",
    "internal system error",
    "internal error",
    "errnum out of range",
    NULL
  };

static int _ipmi_monitoring_initialized = 0;

uint32_t _ipmi_monitoring_flags = 0;

static void
_init_ctx(ipmi_monitoring_ctx_t c)
{
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);

  c->sc = NULL;
}

static void
_destroy_ctx(ipmi_monitoring_ctx_t c)
{
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);

  if (c->sc)
    {
      ipmi_sdr_cache_ctx_destroy(c->sc);
      c->sc = NULL;
    }

  /* Note: destroy iterator first */
  if (c->sensor_readings_itr)
    {
      list_iterator_destroy(c->sensor_readings_itr);
      c->sensor_readings_itr = NULL;
    }

  if (c->sensor_readings)
    {
      list_destroy(c->sensor_readings);
      c->sensor_readings = NULL;
    }

  c->current_sensor_reading = NULL; 

  c->magic = ~IPMI_MONITORING_MAGIC;
  if (_ipmi_monitoring_flags & IPMI_MONITORING_FLAGS_DO_NOT_LOCK_MEMORY)
    free(c);
  else
    secure_free(c, sizeof(struct ipmi_monitoring_ctx));
}

ipmi_monitoring_ctx_t 
ipmi_monitoring_ctx_create(void)
{
  struct ipmi_monitoring_ctx *c = NULL;

  if (_ipmi_monitoring_flags & IPMI_MONITORING_FLAGS_DO_NOT_LOCK_MEMORY)
    {
      if (!(c = (ipmi_monitoring_ctx_t)malloc(sizeof(struct ipmi_monitoring_ctx))))
        return NULL;
    }
  else
    {
      if (!(c = (ipmi_monitoring_ctx_t)secure_malloc(sizeof(struct ipmi_monitoring_ctx))))
        return NULL;
    }
  c->magic = IPMI_MONITORING_MAGIC;

  if (!(c->sensor_readings = list_create((ListDelF)free)))
    {
      if (_ipmi_monitoring_flags & IPMI_MONITORING_FLAGS_DO_NOT_LOCK_MEMORY)
        free(c);
      else
        secure_free(c, sizeof(struct ipmi_monitoring_ctx));
      return NULL;
    }

  _init_ctx(c);
  return c;
}

void 
ipmi_monitoring_ctx_destroy(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return;

  _destroy_ctx(c);
}

int 
ipmi_monitoring_ctx_errnum(ipmi_monitoring_ctx_t c)
{
  if (!c)
    return IPMI_MONITORING_ERR_CONTEXT_NULL;
  else if (c->magic != IPMI_MONITORING_MAGIC)
    return IPMI_MONITORING_ERR_CONTEXT_INVALID;
  else
    return c->errnum;
}

char *
ipmi_monitoring_ctx_strerror(int errnum)
{
  if (errnum >= IPMI_MONITORING_ERR_SUCCESS && errnum <= IPMI_MONITORING_ERR_ERRNUMRANGE)
    return ipmi_monitoring_errmsgs[errnum];
  else
    return ipmi_monitoring_errmsgs[IPMI_MONITORING_ERR_ERRNUMRANGE];
}

int 
ipmi_monitoring_init(unsigned int flags, int *errnum)
{
  if (flags & ~IPMI_MONITORING_FLAGS_MASK)
    {
      if (errnum)
        *errnum = IPMI_MONITORING_ERR_PARAMETERS;
      return -1;
    }

  if (_ipmi_monitoring_initialized)
    return 0;

  if (ipmi_monitoring_sensor_config(errnum) < 0)
    return -1;

  _ipmi_monitoring_flags = flags;
  _ipmi_monitoring_initialized++;
  if (errnum)
    *errnum = IPMI_MONITORING_ERR_SUCCESS;
  return 0;
}

int 
ipmi_monitoring_sensor_readings_by_record_id(ipmi_monitoring_ctx_t c,
                                             char *hostname,
                                             struct ipmi_monitoring_ipmi_config *config,
                                             unsigned int sensor_reading_flags,
                                             unsigned int *record_ids,
                                             unsigned int record_ids_len)
{
  uint16_t record_count;
  int i, rv = -1;

  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!_ipmi_monitoring_initialized)
    {
      c->errnum = IPMI_MONITORING_ERR_LIBRARY_UNINITIALIZED;
      return -1;
    }

  if (sensor_reading_flags & ~IPMI_MONITORING_SENSOR_READING_FLAGS_MASK)
    {
      c->errnum = IPMI_MONITORING_ERR_PARAMETERS;
      return -1;
    }

  ipmi_monitoring_iterator_destroy(c);

  if (ipmi_monitoring_ipmi_communication_init(c, hostname, config) < 0)
    goto cleanup;

  if (sensor_reading_flags & IPMI_MONITORING_SENSOR_READING_FLAGS_REREAD_SDR_CACHE)
    {
      if (ipmi_monitoring_sdr_cache_flush(c, hostname) < 0)
        goto cleanup;
    }
      
  if (ipmi_monitoring_sdr_cache_load(c, hostname) < 0)
    goto cleanup;
      
  if (!record_ids)
    {
      if (ipmi_sdr_cache_record_count(c->sc, &record_count) < 0)
        {
          IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_record_count: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
          c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          goto cleanup;
        }
      
      for (i = 0; i < record_count; i++, ipmi_sdr_cache_next(c->sc))
        {
          char sdr_record[IPMI_MONITORING_MAX_SDR_RECORD_LENGTH];
          int sdr_record_len;
          
          memset(sdr_record, '\0', IPMI_MONITORING_MAX_SDR_RECORD_LENGTH);
          if ((sdr_record_len = ipmi_sdr_cache_record_read(c->sc,
                                                           sdr_record,
                                                           IPMI_MONITORING_MAX_SDR_RECORD_LENGTH)) < 0)
            {
              IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_record_read: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
              c->errnum = IPMI_MONITORING_ERR_INTERNAL;
              goto cleanup;
            }

          if (ipmi_monitoring_get_sensor_reading(c,
                                                 sensor_reading_flags,
                                                 sdr_record,
                                                 sdr_record_len,
                                                 NULL, 
                                                 0) < 0)
            goto cleanup;
        }
    }
  else
    {
      for (i = 0; i < record_ids_len; i++)
        {
          char sdr_record[IPMI_MONITORING_MAX_SDR_RECORD_LENGTH];
          int sdr_record_len;

          if (ipmi_sdr_cache_search_record_id(c->sc, record_ids[i]) < 0)
            {
              if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_ERR_NOT_FOUND)
                {
                  c->errnum = IPMI_MONITORING_ERR_SENSOR_NOT_FOUND;
                  goto cleanup;
                }
              IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_search_record_id: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
              c->errnum = IPMI_MONITORING_ERR_INTERNAL;
              goto cleanup;
            }
          
          memset(sdr_record, '\0', IPMI_MONITORING_MAX_SDR_RECORD_LENGTH);
          if ((sdr_record_len = ipmi_sdr_cache_record_read(c->sc,
                                                           sdr_record,
                                                           IPMI_MONITORING_MAX_SDR_RECORD_LENGTH)) < 0)
            {
              IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_record_read: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
              c->errnum = IPMI_MONITORING_ERR_INTERNAL;
              goto cleanup;
            }
          
          if (ipmi_monitoring_get_sensor_reading(c,
                                                 sensor_reading_flags,
                                                 sdr_record,
                                                 sdr_record_len,
                                                 NULL,
                                                 0) < 0)
            goto cleanup;
        }
    }

  if ((rv = list_count(c->sensor_readings)) > 0)
    {
      if (!(c->sensor_readings_itr = list_iterator_create(c->sensor_readings)))
        {
          IPMI_MONITORING_DEBUG(("list_iterator_create: %s", strerror(errno)));
          c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          goto cleanup;
        }
      c->current_sensor_reading = list_next(c->sensor_readings_itr);
    }

  ipmi_monitoring_ipmi_communication_cleanup(c);
  c->errnum = IPMI_MONITORING_ERR_SUCCESS;
  return rv;

 cleanup:
  ipmi_monitoring_iterator_destroy(c);
  ipmi_monitoring_ipmi_communication_cleanup(c);
  return -1;
}

int 
ipmi_monitoring_sensor_readings_by_sensor_group(ipmi_monitoring_ctx_t c,
                                                char *hostname,
                                                struct ipmi_monitoring_ipmi_config *config,
                                                unsigned int sensor_reading_flags,
                                                unsigned int *sensor_groups,
                                                unsigned int sensor_groups_len)
{
  uint16_t record_count;
  int i, rv = -1;

  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!_ipmi_monitoring_initialized)
    {
      c->errnum = IPMI_MONITORING_ERR_LIBRARY_UNINITIALIZED;
      return -1;
    }

  if ((sensor_reading_flags & ~IPMI_MONITORING_SENSOR_READING_FLAGS_MASK)
      || (sensor_groups && !sensor_groups_len))
    {
      c->errnum = IPMI_MONITORING_ERR_PARAMETERS;
      return -1;
    }

  ipmi_monitoring_iterator_destroy(c);

  if (ipmi_monitoring_ipmi_communication_init(c, hostname, config) < 0)
    goto cleanup;

  if (sensor_reading_flags & IPMI_MONITORING_SENSOR_READING_FLAGS_REREAD_SDR_CACHE)
    {
      if (ipmi_monitoring_sdr_cache_flush(c, hostname) < 0)
        goto cleanup;
    }
      
  if (ipmi_monitoring_sdr_cache_load(c, hostname) < 0)
    goto cleanup;
      
  if (ipmi_sdr_cache_record_count(c->sc, &record_count) < 0)
    {
      IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_record_count: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
      c->errnum = IPMI_MONITORING_ERR_INTERNAL;
      goto cleanup;
    }
      
  for (i = 0; i < record_count; i++, ipmi_sdr_cache_next(c->sc))
    {
      char sdr_record[IPMI_MONITORING_MAX_SDR_RECORD_LENGTH];
      int sdr_record_len;
      
      memset(sdr_record, '\0', IPMI_MONITORING_MAX_SDR_RECORD_LENGTH);
      if ((sdr_record_len = ipmi_sdr_cache_record_read(c->sc,
                                                       sdr_record,
                                                       IPMI_MONITORING_MAX_SDR_RECORD_LENGTH)) < 0)
        {
          IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_record_read: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
          c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          goto cleanup;
        }
      
      if (ipmi_monitoring_get_sensor_reading(c,
                                             sensor_reading_flags,
                                             sdr_record,
                                             sdr_record_len,
                                             sensor_groups,
                                             sensor_groups_len) < 0)
        goto cleanup;
    }

  if ((rv = list_count(c->sensor_readings)) > 0)
    {
      if (!(c->sensor_readings_itr = list_iterator_create(c->sensor_readings)))
        {
          IPMI_MONITORING_DEBUG(("list_iterator_create: %s", strerror(errno)));
          c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          goto cleanup;
        }
      c->current_sensor_reading = list_next(c->sensor_readings_itr);
    }

  ipmi_monitoring_ipmi_communication_cleanup(c);
  c->errnum = IPMI_MONITORING_ERR_SUCCESS;
  return rv;

 cleanup:
  ipmi_monitoring_iterator_destroy(c);
  ipmi_monitoring_ipmi_communication_cleanup(c);
  return -1;
}

int 
ipmi_monitoring_iterator_first(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  list_iterator_reset(c->sensor_readings_itr);
  c->current_sensor_reading = list_next(c->sensor_readings_itr);
  return 0;
}

int 
ipmi_monitoring_iterator_next(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  c->current_sensor_reading = list_next(c->sensor_readings_itr);
  return ((c->current_sensor_reading) ? 1 : 0);
}

int 
ipmi_monitoring_iterator_record_id(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return -1;
    }

  return c->current_sensor_reading->record_id;
}

int 
ipmi_monitoring_iterator_sensor_group(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return -1;
    }

  return c->current_sensor_reading->sensor_group;
}

char *
ipmi_monitoring_iterator_sensor_name(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return NULL;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return NULL;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return NULL;
    }

  return c->current_sensor_reading->sensor_name;
}


int 
ipmi_monitoring_iterator_sensor_state(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return -1;
    }

  return c->current_sensor_reading->sensor_state;
}

int 
ipmi_monitoring_iterator_sensor_units(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return -1;
    }

  return c->current_sensor_reading->sensor_units;
}

int 
ipmi_monitoring_iterator_sensor_reading_type(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return -1;
    }
  
  return c->current_sensor_reading->sensor_reading_type;
}

int 
ipmi_monitoring_iterator_sensor_bitmask_type(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return -1;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return -1;
    }
  
  return c->current_sensor_reading->sensor_bitmask_type;
}

void *
ipmi_monitoring_iterator_sensor_reading(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return NULL;

  if (!c->sensor_readings_itr)
    {
      c->errnum = IPMI_MONITORING_ERR_NO_SENSOR_READINGS;
      return NULL;
    }

  if (!c->current_sensor_reading)
    {
      c->errnum = IPMI_MONITORING_ERR_SENSOR_READINGS_LIST_END;
      return NULL;
    }
  
  if (c->current_sensor_reading->sensor_reading_type == IPMI_MONITORING_SENSOR_READING_TYPE_UNSIGNED_INTEGER8_BOOL)
    return &(c->current_sensor_reading->sensor_reading.bool_val);
  else if (c->current_sensor_reading->sensor_reading_type == IPMI_MONITORING_SENSOR_READING_TYPE_UNSIGNED_INTEGER32)
    return &(c->current_sensor_reading->sensor_reading.integer_val);
  else if (c->current_sensor_reading->sensor_reading_type == IPMI_MONITORING_SENSOR_READING_TYPE_DOUBLE)
    return &(c->current_sensor_reading->sensor_reading.double_val);
  else if (c->current_sensor_reading->sensor_reading_type == IPMI_MONITORING_SENSOR_READING_TYPE_UNSIGNED_INTEGER16_BITMASK)
    return &(c->current_sensor_reading->sensor_reading.integer_bitmask_val);

  return NULL;
}

static int
_sensor_readings_delete_all(void *x, void *y)
{
  return 1;
}

void 
ipmi_monitoring_iterator_destroy(ipmi_monitoring_ctx_t c)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return;

  list_delete_all(c->sensor_readings, _sensor_readings_delete_all, "dummyvalue");

  if (c->sensor_readings_itr)
    {
      list_iterator_destroy(c->sensor_readings_itr);
      c->sensor_readings_itr = NULL;
    }

  c->current_sensor_reading = NULL;
}

int
ipmi_monitoring_bitmask_string(ipmi_monitoring_ctx_t c,
                               int bitmask_type,
                               unsigned int bitmask,
                               char *buffer,
                               unsigned int buflen)
{
  if (!c || c->magic != IPMI_MONITORING_MAGIC)
    return -1;

  if (!IPMI_MONITORING_SENSOR_BITMASK_TYPE_VALID(bitmask_type)
      || !buffer
      || !buflen)
    {
      c->errnum = IPMI_MONITORING_ERR_PARAMETERS;
      return -1;
    }
  
  if (!bitmask)
    return -1;

  memset(buffer, '\0', buflen);
  
  if (bitmask_type >= IPMI_MONITORING_SENSOR_BITMASK_TYPE_TRANSITION
      && bitmask_type <= IPMI_MONITORING_SENSOR_BITMASK_TYPE_POWER_STATE)
    {
      uint8_t event_reading_type_code;

      switch (bitmask_type)
        {
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_TRANSITION:
          event_reading_type_code = 0x02;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_STATE:
          event_reading_type_code = 0x03;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_PREDICTIVE_FAILURE:
          event_reading_type_code = 0x04;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_LIMIT:
          event_reading_type_code = 0x05;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_PERFORMANCE:
          event_reading_type_code = 0x06;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_TRANSITION_SEVERITY:
          event_reading_type_code = 0x07;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_DEVICE_INSTALL:
          event_reading_type_code = 0x08;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_DEVICE_STATE:
          event_reading_type_code = 0x09;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_TRANSITION_DEVICE:
          event_reading_type_code = 0x0A;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_REDUNDANCY:
          event_reading_type_code = 0x0B;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_POWER_STATE:
          event_reading_type_code = 0x0C;
          break;
        default:
          c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          return -1;
        }

      if (ipmi_get_generic_event_message (event_reading_type_code,
                                          bitmask,
                                          buffer,
                                          buflen) < 0)
        {
          if (errno == EINVAL)
            c->errnum = IPMI_MONITORING_ERR_PARAMETERS;
          else
            c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          return -1;
        }
    }
  else
    {
      uint8_t sensor_type_code;

      switch (bitmask_type)
        {
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_PHYSICAL_SECURITY:
          sensor_type_code = 0x05;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_PLATFORM_SECURITY_VIOLATION_ATTEMPT:
          sensor_type_code = 0x06;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_PROCESSOR:
          sensor_type_code = 0x07;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_POWER_SUPPLY:
          sensor_type_code = 0x08;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_POWER_UNIT:
          sensor_type_code = 0x09;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_MEMORY:
          sensor_type_code = 0x0C;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_DRIVE_SLOT:
          sensor_type_code = 0x0D;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_SYSTEM_FIRMWARE_PROGRESS:
          sensor_type_code = 0x0F;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_EVENT_LOGGING_DISABLED:
          sensor_type_code = 0x10;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_SYSTEM_EVENT:
          sensor_type_code = 0x12;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_CRITICAL_INTERRUPT:
          sensor_type_code = 0x13;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_MODULE_BOARD:
          sensor_type_code = 0x15;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_SLOT_CONNECTOR:
          sensor_type_code = 0x21;
          break;
        case IPMI_MONITORING_SENSOR_BITMASK_TYPE_WATCHDOG2:
          sensor_type_code = 0x23;
          break;
        default:
          c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          return -1;
        }
      
      if (ipmi_get_sensor_type_code_message (sensor_type_code,
                                             bitmask,
                                             buffer,
                                             buflen) < 0)
        {
          if (errno == EINVAL)
            c->errnum = IPMI_MONITORING_ERR_PARAMETERS;
          else
            c->errnum = IPMI_MONITORING_ERR_INTERNAL;
          return -1;
        }
    }

  return 0;
}
