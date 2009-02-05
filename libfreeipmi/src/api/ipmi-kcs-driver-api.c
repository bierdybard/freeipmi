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
#if HAVE_ALLOCA_H
#include <alloca.h>
#endif /* HAVE_ALLOCA_H */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <assert.h>
#include <errno.h>

#include "freeipmi/api/ipmi-messaging-support-cmds-api.h"
#include "freeipmi/debug/ipmi-debug.h"
#include "freeipmi/driver/ipmi-kcs-driver.h"
#include "freeipmi/interface/ipmi-ipmb-interface.h"
#include "freeipmi/interface/ipmi-kcs-interface.h"
#include "freeipmi/util/ipmi-ipmb-util.h"
#include "freeipmi/spec/ipmi-channel-spec.h"
#include "freeipmi/spec/ipmi-cmd-spec.h"
#include "freeipmi/spec/ipmi-ipmb-lun-spec.h"
#include "freeipmi/spec/ipmi-slave-address-spec.h"

#include "ipmi-ctx.h"
#include "ipmi-err-wrappers-api.h"
#include "ipmi-fiid-wrappers-api.h"
#include "ipmi-kcs-driver-api.h"

#include "freeipmi-portability.h"
#include "debug-util.h"

/* achu: I dunno what's a good number, ten seems good.  Similar to the
 * inband "TIMEOUT", the purpose is to just not hang any user code
 * trying to do ipmb.  You gotta give up at some point.
 */
#define IPMI_KCS_IPMB_RETRANSMISSION_COUNT   10
#define IPMI_KCS_IPMB_REREAD_COUNT           10

fiid_template_t tmpl_kcs_raw =
  {
    {8192, "raw_data", FIID_FIELD_OPTIONAL | FIID_FIELD_LENGTH_VARIABLE},
    {0, "", 0}
  };

static void
_ipmi_kcs_dump (ipmi_ctx_t ctx,
                uint8_t *pkt,
                uint32_t pkt_len,
                uint8_t cmd,
                uint8_t net_fn,
                unsigned int debug_direction,
                fiid_template_t tmpl_cmd,
                fiid_template_t tmpl_ipmb_msg_hdr,
                fiid_template_t tmpl_ipmb_cmd)
{
  char hdrbuf[DEBUG_UTIL_HDR_BUFLEN];

  assert (ctx
          && ctx->magic == IPMI_CTX_MAGIC
          && (ctx->flags & IPMI_FLAGS_DEBUG_DUMP)
          && pkt
          && pkt_len
          && tmpl_cmd
          && (debug_direction == DEBUG_UTIL_DIRECTION_REQUEST
              || debug_direction == DEBUG_UTIL_DIRECTION_RESPONSE));

  /* Don't cleanup/return an error here.  It's just debug code. */

  debug_hdr_cmd(DEBUG_UTIL_TYPE_INBAND,
                debug_direction,
                net_fn,
                cmd,
                hdrbuf,
                DEBUG_UTIL_HDR_BUFLEN);
  
  if (tmpl_ipmb_cmd
      && ((cmd == IPMI_CMD_SEND_MESSAGE
           && debug_direction == DEBUG_UTIL_DIRECTION_REQUEST)
          || (cmd == IPMI_CMD_GET_MESSAGE
              && debug_direction == DEBUG_UTIL_DIRECTION_RESPONSE)))
    ipmi_dump_kcs_packet_ipmb (STDERR_FILENO,
                               NULL,
                               hdrbuf,
                               NULL,
                               pkt,
                               pkt_len,
                               tmpl_cmd,
                               tmpl_ipmb_msg_hdr,
                               tmpl_ipmb_cmd);
  else
    ipmi_dump_kcs_packet (STDERR_FILENO,
                          NULL,
                          hdrbuf,
                          NULL,
                          pkt,
                          pkt_len,
                          tmpl_cmd);
  
}

static void
_ipmi_kcs_dump_rq (ipmi_ctx_t ctx,
                   uint8_t *pkt,
                   uint32_t pkt_len,
                   uint8_t cmd,
                   uint8_t net_fn,
                   fiid_obj_t obj_cmd_rq)
{
  fiid_field_t *tmpl_cmd = NULL;

  if ((tmpl_cmd = fiid_obj_template(obj_cmd_rq)))
    {
      _ipmi_kcs_dump (ctx,
                      pkt,
                      pkt_len,
                      cmd,
                      net_fn,
                      DEBUG_UTIL_DIRECTION_REQUEST,
                      tmpl_cmd,
                      tmpl_ipmb_msg_hdr_rq,
                      ctx->tmpl_ipmb_cmd_rq);
      fiid_template_free (tmpl_cmd);
    }
}

static void
_ipmi_kcs_dump_rs (ipmi_ctx_t ctx,
                   uint8_t *pkt,
                   uint32_t pkt_len,
                   uint8_t cmd,
                   uint8_t net_fn,
                   fiid_obj_t obj_cmd_rs)
{
  fiid_field_t *tmpl_cmd = NULL;

  if ((tmpl_cmd = fiid_obj_template(obj_cmd_rs)))
    {
      _ipmi_kcs_dump (ctx,
                      pkt,
                      pkt_len,
                      cmd,
                      net_fn,
                      DEBUG_UTIL_DIRECTION_RESPONSE,
                      tmpl_cmd,
                      tmpl_ipmb_msg_hdr_rs,
                      ctx->tmpl_ipmb_cmd_rs);
      fiid_template_free (tmpl_cmd);
    }
}

static void
_ipmi_kcs_dump_raw_rq (ipmi_ctx_t ctx,
                       uint8_t *pkt,
                       uint32_t pkt_len,
                       uint8_t cmd,
                       uint8_t net_fn)
{
  _ipmi_kcs_dump (ctx,
                  pkt,
                  pkt_len,
                  cmd,
                  net_fn,
                  DEBUG_UTIL_DIRECTION_REQUEST,
                  tmpl_kcs_raw,
                  tmpl_ipmb_msg_hdr_rq,
                  ctx->tmpl_ipmb_cmd_rq);
}

static void
_ipmi_kcs_dump_raw_rs (ipmi_ctx_t ctx,
                       uint8_t *pkt,
                       uint32_t pkt_len,
                       uint8_t cmd,
                       uint8_t net_fn)
{
  _ipmi_kcs_dump (ctx,
                  pkt,
                  pkt_len,
                  cmd,
                  net_fn,
                  DEBUG_UTIL_DIRECTION_RESPONSE,
                  tmpl_kcs_raw,
                  tmpl_ipmb_msg_hdr_rs,
                  ctx->tmpl_ipmb_cmd_rs);
}

int8_t 
ipmi_kcs_cmd_api (ipmi_ctx_t ctx,
		  fiid_obj_t obj_cmd_rq,
		  fiid_obj_t obj_cmd_rs)
{
  uint64_t cmd = 0;             /* used for debugging */

  if (!ctx || ctx->magic != IPMI_CTX_MAGIC)
    {
      API_TRACE("invalid ctx", 0);
      return (-1);
    }

  if (!fiid_obj_valid(obj_cmd_rq)
      || !fiid_obj_valid(obj_cmd_rs))
    {
      API_ERR_SET_ERRNUM(IPMI_ERR_PARAMETERS);
      return (-1);
    }

  API_FIID_OBJ_PACKET_VALID(obj_cmd_rq);

  if (ctx->type != IPMI_DEVICE_KCS)
    {
      API_ERR_SET_ERRNUM(IPMI_ERR_INTERNAL_ERROR);
      return (-1);
    }

  if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP)
    API_FIID_OBJ_GET_NO_RETURN(obj_cmd_rq, "cmd", &cmd);

  {
    uint8_t *pkt;
    uint32_t pkt_len;
    int32_t hdr_len, cmd_len;

    API_FIID_TEMPLATE_LEN_BYTES(hdr_len, tmpl_hdr_kcs);
    API_FIID_OBJ_LEN_BYTES (cmd_len, obj_cmd_rq);
    pkt_len = hdr_len + cmd_len;
    pkt = alloca (pkt_len);
    memset (pkt, 0, pkt_len);
    API_ERR (pkt);

    API_ERR (fill_hdr_ipmi_kcs (ctx->lun,
				ctx->net_fn,
				ctx->io.inband.rq.obj_hdr) == 0);
    API_ERR (assemble_ipmi_kcs_pkt (ctx->io.inband.rq.obj_hdr,
				    obj_cmd_rq,
				    pkt,
				    pkt_len) > 0);

    if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP && pkt_len)
      _ipmi_kcs_dump_rq (ctx, pkt, pkt_len, cmd, ctx->net_fn, obj_cmd_rq);

    if (ipmi_kcs_write (ctx->io.inband.kcs_ctx, pkt, pkt_len) < 0)
      {
        API_KCS_ERRNUM_TO_API_ERRNUM(ipmi_kcs_ctx_errnum(ctx->io.inband.kcs_ctx));
        return (-1);
      }
  }

  {
    uint8_t *pkt;
    uint32_t pkt_len;
    int32_t hdr_len, cmd_len;
    int32_t read_len;
    fiid_field_t *tmpl = NULL;
    int8_t rv = -1;

    API_FIID_TEMPLATE_LEN_BYTES_CLEANUP(hdr_len, tmpl_hdr_kcs);
    API_FIID_OBJ_TEMPLATE_CLEANUP(tmpl, obj_cmd_rs);
    API_FIID_TEMPLATE_LEN_BYTES_CLEANUP(cmd_len, tmpl);
    pkt_len = hdr_len + cmd_len;

    API_ERR_CLEANUP ((pkt = alloca (pkt_len)));
    memset (pkt, 0, pkt_len);

    if ((read_len = ipmi_kcs_read (ctx->io.inband.kcs_ctx, 
                                   pkt, 
                                   pkt_len)) < 0)
      {
        API_KCS_ERRNUM_TO_API_ERRNUM(ipmi_kcs_ctx_errnum(ctx->io.inband.kcs_ctx));
        goto cleanup;
      }

    if (!read_len)
      {
        API_ERR_SET_ERRNUM(IPMI_ERR_SYSTEM_ERROR);
        goto cleanup;
      }

    if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP && read_len)
      _ipmi_kcs_dump_rs (ctx, pkt, read_len, cmd, ctx->net_fn, obj_cmd_rs);

    API_ERR_CLEANUP (!(unassemble_ipmi_kcs_pkt (pkt,
						read_len,
						ctx->io.inband.rs.obj_hdr,
						obj_cmd_rs) < 0));

    rv = 0;
  cleanup:
    API_FIID_TEMPLATE_FREE(tmpl);
    if (rv < 0)
      return (rv);
  }

  return (0);
}

static int8_t
_ipmi_kcs_ipmb_send (ipmi_ctx_t ctx,
		     fiid_obj_t obj_cmd_rq)
{
  uint8_t buf[IPMI_MAX_PKT_LEN];
  fiid_obj_t obj_ipmb_msg_hdr_rq = NULL;
  fiid_obj_t obj_ipmb_msg_rq = NULL;
  fiid_obj_t obj_send_cmd_rs = NULL;
  int32_t len;
  int8_t rv = -1;

  assert (ctx
	  && ctx->magic == IPMI_CTX_MAGIC
	  && ctx->type == IPMI_DEVICE_KCS
	  && fiid_obj_valid(obj_cmd_rq)
	  && fiid_obj_packet_valid(obj_cmd_rq));

  API_FIID_OBJ_CREATE_CLEANUP(obj_ipmb_msg_hdr_rq, tmpl_ipmb_msg_hdr_rq);
  API_FIID_OBJ_CREATE_CLEANUP(obj_ipmb_msg_rq, tmpl_ipmb_msg);
  API_FIID_OBJ_CREATE_CLEANUP(obj_send_cmd_rs, tmpl_cmd_send_message_rs);
  
  API_ERR_CLEANUP (fill_ipmb_msg_hdr (ctx->rs_addr,
				      ctx->net_fn,
				      ctx->lun,
				      IPMI_SLAVE_ADDRESS_BMC,
				      IPMI_BMC_IPMB_LUN_SMS_MSG_LUN,
				      ctx->io.inband.rq_seq,
				      obj_ipmb_msg_hdr_rq) != -1);
  
  API_ERR_CLEANUP (assemble_ipmi_ipmb_msg (obj_ipmb_msg_hdr_rq,
					   obj_cmd_rq,
					   obj_ipmb_msg_rq) != -1);
  
  memset(buf, '\0', IPMI_MAX_PKT_LEN);
  API_FIID_OBJ_GET_ALL_LEN_CLEANUP (len,
				    obj_ipmb_msg_rq,
				    buf,
				    IPMI_MAX_PKT_LEN);
  
  if (ipmi_cmd_send_message (ctx,
			     IPMI_CHANNEL_NUMBER_PRIMARY_IPMB,
			     IPMI_SEND_MESSAGE_AUTHENTICATION_NOT_REQUIRED,
			     IPMI_SEND_MESSAGE_ENCRYPTION_NOT_REQUIRED,
			     IPMI_SEND_MESSAGE_TRACKING_OPERATION_NO_TRACKING,
			     buf,
			     len,
			     obj_send_cmd_rs) < 0)
    {
      API_BAD_COMPLETION_CODE_TO_API_ERRNUM(ctx, obj_send_cmd_rs);
      goto cleanup;
    }
  
  rv = 0;
 cleanup:
  API_FIID_OBJ_DESTROY(obj_ipmb_msg_hdr_rq);
  API_FIID_OBJ_DESTROY(obj_ipmb_msg_rq);
  API_FIID_OBJ_DESTROY(obj_send_cmd_rs);
  return (rv);
}

static int8_t
_ipmi_kcs_ipmb_recv (ipmi_ctx_t ctx,
		     fiid_obj_t obj_ipmb_msg_hdr_rs,
		     fiid_obj_t obj_ipmb_msg_trlr,
		     fiid_obj_t obj_cmd_rs)
{
  uint8_t buf[IPMI_MAX_PKT_LEN];
  fiid_obj_t obj_ipmb_msg_rs = NULL;
  fiid_obj_t obj_get_cmd_rs = NULL;
  int32_t len;
  int8_t rv = -1;

  assert (ctx
	  && ctx->magic == IPMI_CTX_MAGIC
	  && ctx->type == IPMI_DEVICE_KCS
	  && fiid_obj_valid(obj_ipmb_msg_hdr_rs)
	  && fiid_obj_valid(obj_ipmb_msg_trlr)
	  && fiid_obj_valid(obj_cmd_rs));

  API_FIID_OBJ_CREATE_CLEANUP(obj_ipmb_msg_rs, tmpl_ipmb_msg);
  API_FIID_OBJ_CREATE_CLEANUP(obj_get_cmd_rs, tmpl_cmd_get_message_rs);

  if (ipmi_cmd_get_message (ctx, obj_get_cmd_rs) < 0)
    {
      if (ipmi_check_completion_code (obj_get_cmd_rs, IPMI_COMP_CODE_DATA_NOT_AVAILABLE) == 1)
        API_ERR_SET_ERRNUM(IPMI_ERR_MESSAGE_TIMEOUT);
      else
	API_BAD_COMPLETION_CODE_TO_API_ERRNUM(ctx, obj_get_cmd_rs);
      goto cleanup;
    }

  API_FIID_OBJ_GET_DATA_LEN_CLEANUP (len,
				     obj_get_cmd_rs,
				     "message_data",
				     buf,
				     IPMI_MAX_PKT_LEN);

  API_FIID_OBJ_SET_ALL_CLEANUP (obj_ipmb_msg_rs,
				buf,
				len);

  API_ERR_CLEANUP (unassemble_ipmi_ipmb_msg (obj_ipmb_msg_rs,
					     obj_ipmb_msg_hdr_rs,
					     obj_cmd_rs,
					     obj_ipmb_msg_trlr) != -1);

  rv = 0;
 cleanup:
  API_FIID_OBJ_DESTROY(obj_ipmb_msg_rs);
  API_FIID_OBJ_DESTROY(obj_get_cmd_rs);
  return (rv);
}

int8_t 
ipmi_kcs_cmd_api_ipmb (ipmi_ctx_t ctx,
		       fiid_obj_t obj_cmd_rq,
		       fiid_obj_t obj_cmd_rs)
{
  fiid_obj_t obj_ipmb_msg_hdr_rs = NULL;
  fiid_obj_t obj_ipmb_msg_trlr = NULL;
  unsigned retransmission_count = 0;
  unsigned reread_count = 0;
  int8_t rv = -1;
  int ret;

  if (!ctx || ctx->magic != IPMI_CTX_MAGIC)
    {
      API_TRACE("invalid ctx", 0);
      return (-1);
    }

  if (!fiid_obj_valid(obj_cmd_rq)
      || !fiid_obj_valid(obj_cmd_rs))
    {
      API_ERR_SET_ERRNUM(IPMI_ERR_PARAMETERS);
      return (-1);
    }

  API_FIID_OBJ_PACKET_VALID(obj_cmd_rq);

  if (ctx->type != IPMI_DEVICE_KCS)
    {
      API_ERR_SET_ERRNUM(IPMI_ERR_INTERNAL_ERROR);
      return (-1);
    }

  API_FIID_OBJ_CREATE_CLEANUP(obj_ipmb_msg_hdr_rs, tmpl_ipmb_msg_hdr_rs);
  API_FIID_OBJ_CREATE_CLEANUP(obj_ipmb_msg_trlr, tmpl_ipmb_msg_trlr);

  /* for debugging */
  ctx->tmpl_ipmb_cmd_rq = fiid_obj_template(obj_cmd_rq);
  ctx->tmpl_ipmb_cmd_rs = fiid_obj_template(obj_cmd_rs);

  if (_ipmi_kcs_ipmb_send (ctx, obj_cmd_rq) < 0)
    goto cleanup;

  while (1)
    {
      if (_ipmi_kcs_ipmb_recv (ctx, 
			       obj_ipmb_msg_hdr_rs,
			       obj_ipmb_msg_trlr,
			       obj_cmd_rs) < 0)
        {
          if (ctx->errnum == IPMI_ERR_MESSAGE_TIMEOUT)
            {
	      reread_count++;
	      
	      if (reread_count > IPMI_KCS_IPMB_REREAD_COUNT)
                {
                  API_ERR_SET_ERRNUM(IPMI_ERR_MESSAGE_TIMEOUT);
                  goto cleanup;
                }
	      
	      continue;
            }
          goto cleanup;
        }
      
      API_ERR_CLEANUP (!((ret = ipmi_ipmb_check_rq_seq (obj_ipmb_msg_hdr_rs, 
							ctx->io.inband.rq_seq)) < 0));

      /* if it's the wrong rq_seq, get another packet */
      if (!ret)
	continue;
      
      API_ERR_CLEANUP (!((ret = ipmi_ipmb_check_checksum (IPMI_SLAVE_ADDRESS_BMC,
							  obj_ipmb_msg_hdr_rs,
							  obj_cmd_rs,
							  obj_ipmb_msg_trlr)) < 0));
      
      /* if the checksum is wrong, assume an error and resend */
      if (!ret)
	{
	  retransmission_count++;
	  
	  if (retransmission_count > IPMI_KCS_IPMB_RETRANSMISSION_COUNT)
            {
              API_ERR_SET_ERRNUM(IPMI_ERR_MESSAGE_TIMEOUT);
              goto cleanup;
            }
	  
	  ctx->io.inband.rq_seq = ((ctx->io.inband.rq_seq) + 1) % (IPMI_IPMB_REQUESTER_SEQUENCE_NUMBER_MAX + 1);

	  if (_ipmi_kcs_ipmb_send (ctx, obj_cmd_rq) < 0)
	    goto cleanup;

	  continue;
	}
      
      break;
    }

  rv = 0;
 cleanup:
  ctx->io.inband.rq_seq = ((ctx->io.inband.rq_seq) + 1) % (IPMI_IPMB_REQUESTER_SEQUENCE_NUMBER_MAX + 1);
  API_FIID_OBJ_DESTROY(obj_ipmb_msg_hdr_rs);
  API_FIID_OBJ_DESTROY(obj_ipmb_msg_trlr);
  API_FIID_TEMPLATE_FREE (ctx->tmpl_ipmb_cmd_rq);
  ctx->tmpl_ipmb_cmd_rq = NULL;
  API_FIID_TEMPLATE_FREE (ctx->tmpl_ipmb_cmd_rs);
  ctx->tmpl_ipmb_cmd_rs = NULL;
  return (rv);
}               

int32_t
ipmi_kcs_cmd_raw_api (ipmi_ctx_t ctx,
		      uint8_t *buf_rq,
		      size_t buf_rq_len,
		      uint8_t *buf_rs,
		      size_t buf_rs_len)
{
  uint8_t *pkt = NULL;
  uint32_t pkt_len;
  uint8_t *readbuf = NULL;
  int32_t bytes_read = 0;
  int32_t hdr_len;
  int32_t rv = -1;
  uint64_t cmd = 0;             /* used for debugging */

  if (!ctx || ctx->magic != IPMI_CTX_MAGIC)
    {
      API_TRACE("invalid ctx", 0);
      return (-1);
    }

  if (!buf_rq 
      || !buf_rq_len
      || !buf_rs 
      || !buf_rs_len)
    {
      API_ERR_SET_ERRNUM(IPMI_ERR_PARAMETERS);
      return (-1);
    }

  if (ctx->type != IPMI_DEVICE_KCS)
    {
      API_ERR_SET_ERRNUM(IPMI_ERR_INTERNAL_ERROR);
      return (-1);
    }

  if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP)
    cmd = buf_rq[0];

  API_FIID_TEMPLATE_LEN_BYTES(hdr_len, tmpl_hdr_kcs);
  pkt_len = hdr_len + buf_rq_len;

  pkt = alloca(pkt_len);
  API_ERR (pkt);
  readbuf = alloca(buf_rs_len);
  API_ERR (readbuf);

  API_ERR (fill_hdr_ipmi_kcs (ctx->lun,
			      ctx->net_fn,
			      ctx->io.inband.rq.obj_hdr) == 0);
  API_FIID_OBJ_GET_ALL(ctx->io.inband.rq.obj_hdr, pkt, pkt_len);
  memcpy(pkt + hdr_len, buf_rq, buf_rq_len);

  if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP && pkt_len)
    _ipmi_kcs_dump_raw_rq (ctx, pkt, pkt_len, cmd, ctx->net_fn);

  /* Request Block */
  if (ipmi_kcs_write (ctx->io.inband.kcs_ctx, pkt, pkt_len) < 0)
    {
      API_KCS_ERRNUM_TO_API_ERRNUM(ipmi_kcs_ctx_errnum(ctx->io.inband.kcs_ctx));
      return (-1);
    }

  /* Response Block */
  if ((bytes_read = ipmi_kcs_read (ctx->io.inband.kcs_ctx, readbuf, buf_rs_len)) < 0)
    {
      API_KCS_ERRNUM_TO_API_ERRNUM(ipmi_kcs_ctx_errnum(ctx->io.inband.kcs_ctx));
      return (-1);
    }

  if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP && bytes_read)
    _ipmi_kcs_dump_raw_rs (ctx, pkt, bytes_read, cmd, ctx->net_fn);

  if (!bytes_read)
    {
      API_ERR_SET_ERRNUM(IPMI_ERR_SYSTEM_ERROR);
      return -1;
    }

  if ((bytes_read - hdr_len) > 0)
    {
      memcpy(buf_rs, readbuf + hdr_len, bytes_read - hdr_len);
      rv = bytes_read - hdr_len;
    }
  else
    rv = 0;

  return (rv);
}
