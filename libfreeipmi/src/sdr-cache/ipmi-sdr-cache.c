/*****************************************************************************\
 *  $Id: ipmi-sdr-cache.c,v 1.2 2007-12-29 17:20:35 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2006-2007 The Regents of the University of California.
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
 *  with Ipmimonitoring.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <sys/mman.h>
#include <assert.h>
#include <errno.h>

#include "freeipmi/sdr-cache/ipmi-sdr-cache.h"

#include "ipmi-sdr-cache-common.h"
#include "ipmi-sdr-cache-defs.h"

#include "libcommon/ipmi-err-wrappers.h"

static char *ipmi_sdr_cache_errmsgs[] =
  {
    "success",
    "context null",
    "context invalid",
    "invalid parmaeters",
    "out of memory",
    "filename invalid",
    "file system error",
    "filename path permission error",
    "SDR cache exists",
    "SDR cache context set for reading",
    "SDR record with an identical record id already written",
    "SDR record with an identical sensor number already written",
    "SDR record length invalid",
    "SDR record count invalid",
    "SDR cache reading initialization already called",
    "cache reading not initialized",
    "SDR cache does not exist",
    "SDR cache context set for reading",
    "SDR cache invalid",
    "SDR cache out of date",
    "not found",
    "internal IPMI error",
    "internal system error",
    "buffer overflow",
    "internal error",
    "errnum out of range",
    NULL
  };

ipmi_sdr_cache_ctx_t
ipmi_sdr_cache_ctx_create(void)
{
  struct ipmi_sdr_cache_ctx *ctx = NULL;

  ERR_CLEANUP((ctx = (ipmi_sdr_cache_ctx_t)malloc(sizeof(struct ipmi_sdr_cache_ctx))));
  ctx->magic = IPMI_SDR_CACHE_MAGIC;
  ipmi_sdr_cache_init_ctx(ctx);
  return ctx;

 cleanup:
  if (ctx)
    free(ctx);
  return NULL;
}

void
ipmi_sdr_cache_ctx_destroy(ipmi_sdr_cache_ctx_t ctx)
{
  if (!ctx || ctx->magic != IPMI_SDR_CACHE_MAGIC)
    return;

  if (ctx->fd >= 0)
    close(ctx->fd);
  if (ctx->sdr_cache)
    munmap(ctx->sdr_cache, ctx->file_size);

  ctx->magic = ~IPMI_SDR_CACHE_MAGIC;
  ctx->operation = IPMI_SDR_CACHE_OPERATION_UNINITIALIZED;
  free(ctx);
}

int 
ipmi_sdr_cache_ctx_errnum(ipmi_sdr_cache_ctx_t ctx)
{
  if (!ctx)
    return IPMI_SDR_CACHE_CTX_ERR_CONTEXT_NULL;
  else if (ctx->magic != IPMI_SDR_CACHE_MAGIC)
    return IPMI_SDR_CACHE_CTX_ERR_CONTEXT_INVALID;
  else
    return ctx->errnum;
}

char *
ipmi_sdr_cache_ctx_strerror(int errnum)
{
  if (errnum >= IPMI_SDR_CACHE_CTX_ERR_SUCCESS && errnum <= IPMI_SDR_CACHE_CTX_ERR_ERRNUMRANGE)
    return ipmi_sdr_cache_errmsgs[errnum];
  else
    return ipmi_sdr_cache_errmsgs[IPMI_SDR_CACHE_CTX_ERR_ERRNUMRANGE];
}
