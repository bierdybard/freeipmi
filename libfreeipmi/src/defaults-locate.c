/* 
   defaults-locate.c - Return default locate info for IPMI interfaces.

   Copyright (C) 2005 FreeIPMI Core Team

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
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  
*/

#include "freeipmi.h"

ipmi_locate_info_t*
defaults_get_dev_info (ipmi_interface_type_t type, ipmi_locate_info_t* pinfo)
{
  pinfo->locate_driver_type = IPMI_LOCATE_DRIVER_DEFAULTS;
  switch (type){
  case IPMI_INTERFACE_KCS:
    pinfo->ipmi_ver_major = 1;
    pinfo->ipmi_ver_minor = 5;
    pinfo->interface_type = IPMI_INTERFACE_KCS;
    pinfo->addr_space_id = IPMI_ADDRESS_SPACE_ID_SYSTEM_IO;
    pinfo->base_addr.bmc_iobase_addr = IPMI_KCS_SMS_IO_BASE_DEFAULT;
    pinfo->reg_space = 1;
    return (pinfo);
  case IPMI_INTERFACE_SMIC:
    pinfo->ipmi_ver_major = 1;
    pinfo->ipmi_ver_minor = 5;
    pinfo->interface_type = IPMI_INTERFACE_SMIC;
    pinfo->addr_space_id = IPMI_ADDRESS_SPACE_ID_SYSTEM_IO;
    pinfo->base_addr.bmc_iobase_addr = IPMI_SMIC_SMS_IO_BASE_DEFAULT;
    pinfo->reg_space = 1;
    return (pinfo);
  case IPMI_INTERFACE_SSIF:
    pinfo->ipmi_ver_major = 1;
    pinfo->ipmi_ver_minor = 5;
    pinfo->interface_type = IPMI_INTERFACE_SSIF;
    pinfo->addr_space_id = IPMI_ADDRESS_SPACE_ID_SMBUS;
    pinfo->base_addr.bmc_smbus_slave_addr = IPMI_SSIF_SMBUS_SLAVE_ADDR;
    pinfo->bmc_i2c_dev_name = strdup (IPMI_DEFAULT_I2C_DEVICE);
    pinfo->reg_space = 1;
    return (pinfo);
  case IPMI_INTERFACE_LAN:
  case IPMI_INTERFACE_BT:
  case IPMI_INTERFACE_RESERVED:
    return (NULL);
  }
  return (NULL);
}
