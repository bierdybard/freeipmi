/* 
   Copyright (C) 2003-2009 FreeIPMI Core Team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#ifdef STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>

#include "freeipmi/api/ipmi-event-cmds-api.h"
#include "freeipmi/cmds/ipmi-event-cmds.h"
#include "freeipmi/spec/ipmi-ipmb-lun-spec.h"
#include "freeipmi/spec/ipmi-netfn-spec.h"

#include "ipmi-ctx.h"
#include "ipmi-err-wrappers-api.h"
#include "ipmi-fiid-wrappers-api.h"

#include "freeipmi-portability.h"

int8_t 
ipmi_cmd_set_event_receiver (ipmi_ctx_t ctx, 
                             uint8_t event_receiver_slave_address,
                             uint8_t event_receiver_lun,
                             fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq = NULL;
  int8_t rv = -1;

  if (!ctx || ctx->magic != IPMI_CTX_MAGIC)
    {
      API_TRACE("invalid ctx", 0);
      return (-1);
    }

  if (!IPMI_BMC_LUN_VALID(event_receiver_lun)
      || !fiid_obj_valid(obj_cmd_rs))
    {
      API_SET_ERRNUM(IPMI_ERR_PARAMETERS);
      return (-1);
    }
  
  API_FIID_OBJ_TEMPLATE_COMPARE(obj_cmd_rs, tmpl_cmd_set_event_receiver_rs);

  API_FIID_OBJ_CREATE(obj_cmd_rq, tmpl_cmd_set_event_receiver_rq);

  if (fill_cmd_set_event_receiver (event_receiver_slave_address,
                                   event_receiver_lun,
                                   obj_cmd_rq) < 0)
    {
      API_ERRNO_TO_API_ERRNUM(ctx, errno);
      goto cleanup;
    }

  API_ERR_IPMI_CMD_CLEANUP (ctx, 
			    IPMI_BMC_IPMB_LUN_BMC, 
			    IPMI_NET_FN_APP_RQ, 
			    obj_cmd_rq, 
			    obj_cmd_rs);

  rv = 0;
 cleanup:
  API_FIID_OBJ_DESTROY(obj_cmd_rq);
  return (rv);
}

int8_t 
ipmi_cmd_set_event_receiver_ipmb (ipmi_ctx_t ctx, 
                                  uint8_t slave_address,
                                  uint8_t event_receiver_slave_address,
                                  uint8_t event_receiver_lun,
                                  fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq = NULL;
  int8_t rv = -1;

  if (!ctx || ctx->magic != IPMI_CTX_MAGIC)
    {
      API_TRACE("invalid ctx", 0);
      return (-1);
    }

  if (!fiid_obj_valid(obj_cmd_rs))
    {
      API_SET_ERRNUM(IPMI_ERR_PARAMETERS);
      return (-1);
    }

  API_FIID_OBJ_TEMPLATE_COMPARE(obj_cmd_rs, tmpl_cmd_set_event_receiver_rs);

  API_FIID_OBJ_CREATE(obj_cmd_rq, tmpl_cmd_set_event_receiver_rq);
  
  if (fill_cmd_set_event_receiver (event_receiver_slave_address,
                                   event_receiver_lun,
                                   obj_cmd_rq) < 0)
    {
      API_ERRNO_TO_API_ERRNUM(ctx, errno);
      goto cleanup;
    }

  API_ERR_IPMI_CMD_IPMB_CLEANUP (ctx, 
                                 slave_address,
                                 IPMI_BMC_IPMB_LUN_BMC, 
                                 IPMI_NET_FN_SENSOR_EVENT_RQ, 
                                 obj_cmd_rq, 
                                 obj_cmd_rs);

  rv = 0;
 cleanup:
  API_FIID_OBJ_DESTROY(obj_cmd_rq);
  return (rv);
}

int8_t 
ipmi_cmd_get_event_receiver (ipmi_ctx_t ctx, fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_cmd_rq = NULL;
  int8_t rv = -1;

  if (!ctx || ctx->magic != IPMI_CTX_MAGIC)
    {
      API_TRACE("invalid ctx", 0);
      return (-1);
    }

  if (!fiid_obj_valid(obj_cmd_rs))
    {
      API_SET_ERRNUM(IPMI_ERR_PARAMETERS);
      return (-1);
    }
  
  API_FIID_OBJ_TEMPLATE_COMPARE(obj_cmd_rs, tmpl_cmd_get_event_receiver_rs);

  API_FIID_OBJ_CREATE(obj_cmd_rq, tmpl_cmd_get_event_receiver_rq);

  if (fill_cmd_get_event_receiver (obj_cmd_rq) < 0)
    {
      API_ERRNO_TO_API_ERRNUM(ctx, errno);
      goto cleanup;
    }

  API_ERR_IPMI_CMD_CLEANUP (ctx, 
			    IPMI_BMC_IPMB_LUN_BMC, 
			    IPMI_NET_FN_APP_RQ, 
			    obj_cmd_rq, 
			    obj_cmd_rs);

  rv = 0;
 cleanup:
  API_FIID_OBJ_DESTROY(obj_cmd_rq);
  return (rv);
}
