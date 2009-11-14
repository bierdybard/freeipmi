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

#ifndef _IPMI_NETFN_OEM_SPEC_H
#define _IPMI_NETFN_OEM_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Dell
 */

/*
 * Dell Poweredge R610
 * Dell Poweredge R710
 */
/* achu: not official names, named based on use context */
#define IPMI_NET_FN_OEM_DELL_GENERIC_RQ 0x30
#define IPMI_NET_FN_OEM_DELL_GENERIC_RS 0x31

/*
 * Dell Xanadu2
 */
/* achu: not official names, named based on use context */
#define IPMI_NET_FN_OEM_DELL_XANADU2_RQ 0x34
#define IPMI_NET_FN_OEM_DELL_XANADU2_RS 0x35

/*
 * Fujitsu
 */

/*
 * Fujitsu RX100 S5
 */
#define IPMI_NET_FN_OEM_FUJITSU_GENERIC_RQ 0x30
#define IPMI_NET_FN_OEM_FUJITSU_GENERIC_RS 0x31

/* 
 * Inventec
 */

/*
 * Inventec 5441/Dell Xanadu2
 */
/* achu: not official names, named based on use context */
#define IPMI_NET_FN_OEM_INVENTEC_GENERIC_RQ 0x30
#define IPMI_NET_FN_OEM_INVENTEC_GENERIC_RS 0x31

/*
 * Supermicro
 */

/*
 * Supermicro H8QME
 */
/* achu: not official names, named based on use context */
#define IPMI_NET_FN_OEM_SUPERMICRO_GENERIC_RQ 0x30
#define IPMI_NET_FN_OEM_SUPERMICRO_GENERIC_RS 0x31

#define IPMI_NET_FN_OEM_SUPERMICRO_PEPPERCON_RQ 0x3C
#define IPMI_NET_FN_OEM_SUPERMICRO_PEPPERCON_RS 0x3D

#ifdef __cplusplus
}
#endif

#endif /* ipmi-netfn-oem-spec.h */
