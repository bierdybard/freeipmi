/*****************************************************************************\
 *  $Id: ipmi_sdr_cache.h,v 1.1 2007-01-30 21:52:57 chu11 Exp $
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

#ifndef _IPMI_SDR_CACHE_H
#define _IPMI_SDR_CACHE_H

#include <stdint.h>

#define IPMI_SDR_CACHE_ERR_SUCCESS                                      0
#define IPMI_SDR_CACHE_ERR_CONTEXT_NULL                                 1
#define IPMI_SDR_CACHE_ERR_CONTEXT_INVALID                              2
#define IPMI_SDR_CACHE_ERR_PARAMETERS                                   3
#define IPMI_SDR_CACHE_ERR_OUT_OF_MEMORY                                4
#define IPMI_SDR_CACHE_ERR_FILENAME_INVALID                             5
#define IPMI_SDR_CACHE_ERR_FILESYSTEM                                   6
#define IPMI_SDR_CACHE_ERR_PERMISSION                                   7
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_CACHE_EXISTS                    8
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_ALREADY_INITIALIZED             9
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_CTX_SET_TO_READ                 10
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_INITIALIZATION                  11
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_RECORD_COUNT_REACHED            12
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_INVALID_RECORD_LENGTH           13
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_DUPLICATE_RECORD_ID             14
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_DUPLICATE_SENSOR_NUMBER         15
#define IPMI_SDR_CACHE_ERR_CACHE_CREATE_INCOMPLETE_RECORD_COUNT_WRITTEN 16
#define IPMI_SDR_CACHE_ERR_CACHE_READ_ALREADY_INITIALIZED               17
#define IPMI_SDR_CACHE_ERR_CACHE_READ_CTX_SET_TO_CREATE                 18
#define IPMI_SDR_CACHE_ERR_CACHE_READ_INITIALIZATION                    19
#define IPMI_SDR_CACHE_ERR_CACHE_READ_CACHE_DOES_NOT_EXIST              20
#define IPMI_SDR_CACHE_ERR_CACHE_DELETE_CTX_SET_TO_CREATE               21
#define IPMI_SDR_CACHE_ERR_CACHE_DELETE_CTX_SET_TO_READ                 22
#define IPMI_SDR_CACHE_ERR_CACHE_INVALID                                23
#define IPMI_SDR_CACHE_ERR_NOT_FOUND                                    24
#define IPMI_SDR_CACHE_ERR_OVERFLOW                                     25
#define IPMI_SDR_CACHE_ERR_INTERNAL                                     26
#define IPMI_SDR_CACHE_ERR_ERRNUMRANGE                                  27

#define IPMI_SDR_CACHE_CREATE_FLAGS_DEFAULT            0x0
#define IPMI_SDR_CACHE_CREATE_FLAGS_OVERWRITE          0x1

#define IPMI_SDR_CACHE_VALIDATION_FLAGS_DEFAULT        0x0
#define IPMI_SDR_CACHE_VALIDATION_FLAGS_DUPLICATE_RECORD_ID 0x1
#define IPMI_SDR_CACHE_VALIDATION_FLAGS_DUPLICATE_SENSOR_NUMBER 0x2

typedef struct ipmi_sdr_cache_ctx *ipmi_sdr_cache_ctx_t;

/* SDR Cache Context Functions */
ipmi_sdr_cache_ctx_t ipmi_sdr_cache_ctx_create(void);
void ipmi_sdr_cache_ctx_destroy(ipmi_sdr_cache_ctx_t c);
int ipmi_sdr_cache_ctx_errnum(ipmi_sdr_cache_ctx_t c);
char * ipmi_sdr_cache_ctx_strerror(int errnum);

/* SDR Cache Creation Functions */
int ipmi_sdr_cache_create(ipmi_sdr_cache_ctx_t c, 
			  char *filename, 
			  uint8_t version,
			  uint16_t record_count,
			  int create_flags,
			  int validation_flags);
int ipmi_sdr_cache_record_write(ipmi_sdr_cache_ctx_t c, 
				char *buf,
				unsigned int buflen);
int ipmi_sdr_cache_complete(ipmi_sdr_cache_ctx_t c);

/* SDR Cache Reading Functions */
int ipmi_sdr_cache_open(ipmi_sdr_cache_ctx_t c, char *filename);
int ipmi_sdr_cache_version(ipmi_sdr_cache_ctx_t c, uint8_t *version);
int ipmi_sdr_cache_record_count(ipmi_sdr_cache_ctx_t c, uint16_t *record_count);

int ipmi_sdr_cache_first(ipmi_sdr_cache_ctx_t c);
int ipmi_sdr_cache_next(ipmi_sdr_cache_ctx_t c);
int ipmi_sdr_cache_seek(ipmi_sdr_cache_ctx_t c, unsigned int index);
int ipmi_sdr_cache_search_record_id(ipmi_sdr_cache_ctx_t c, uint16_t record_id);

int ipmi_sdr_cache_record_read(ipmi_sdr_cache_ctx_t c, 
			       char *buf,
			       unsigned int buflen);

int ipmi_sdr_cache_close(ipmi_sdr_cache_ctx_t c);

/* SDR Cache Delete Functions */
int ipmi_sdr_cache_delete(ipmi_sdr_cache_ctx_t c, char *filename);


#endif /* _IPMI_SDR_CACHE_H */
