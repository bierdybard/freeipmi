/* 
   ipmi-netfn-spec.h - IPMI Network Function Specification

   Copyright (C) 2003 FreeIPMI Core Team

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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
*/

#ifndef _IPMI_NETFN_SPEC_H
#define	_IPMI_NETFN_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

#define IPMI_NET_FN_LUN(net_fn)  (net_fn & 0x03)
#define IPMI_NET_FN_FN(net_fn)   (net_fn >> 2)

/* Notes:
   Refer to IPMIv1_5_rev1_1.pdf Table 5-1, Network Function Codes
   for complete description
*/
#define IPMI_NET_FN_CHASSIS_RQ         0x00
#define IPMI_NET_FN_CHASSIS_RS	       0x01
#define IPMI_NET_FN_BRIDGE_RQ	       0x02
#define IPMI_NET_FN_BRIDGE_RS	       0x03
#define IPMI_NET_FN_SENSOR_EVENT_RQ    0x04
#define IPMI_NET_FN_SENSOR_EVENT_RS    0x05
#define IPMI_NET_FN_APP_RQ	       0x06
#define IPMI_NET_FN_APP_RS	       0x07
#define IPMI_NET_FN_FIRMWARE_RQ	       0x08
#define IPMI_NET_FN_FIRMWARE_RS	       0x09
#define IPMI_NET_FN_STORAGE_RQ	       0x0A
#define IPMI_NET_FN_STORAGE_RS	       0x0B
#define IPMI_NET_FN_TRANSPORT_RQ       0x0C
#define IPMI_NET_FN_TRANSPORT_RS       0x0D
/*
   0x0E to 0x2B RESERVED
   0x2C to 0x2D IPMI_NET_FN_GRP
   0x2E to 0x2F IPMI_NET_FN_OEM_GRP
   0x30 to 0x3F IPMI_NET_FN_CNTRLR_OEM_GRP
*/

typedef struct net_fn
{
  u_int8_t lun:2;
  u_int8_t fn:6;
} net_fn_t;

u_int8_t ipmi_netfn2byte (net_fn_t net_fn);

#ifdef __cplusplus
}
#endif

#endif /* ipmi-cmd-spec.h */
