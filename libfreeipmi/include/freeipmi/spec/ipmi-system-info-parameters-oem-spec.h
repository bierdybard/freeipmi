/*
 * Copyright (C) 2003-2012 FreeIPMI Core Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef IPMI_SYSTEM_INFO_PARAMETERS_OEM_SPEC_H
#define IPMI_SYSTEM_INFO_PARAMETERS_OEM_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************
 * Dell                                    *
 *******************************************/
  
/*
 * Dell Poweredge 2900
 * Dell Poweredge 2950
 * Dell Poweredge R610
 * Dell Poweredge R710
 * Dell Poweredge M600
 * Dell Poweredge M610
 * Dell Poweredge M610X
 * Dell Poweredge M910
 *
 * Support of below varies by system.
 */

#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_LCD_STRING                           0xC1
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_LCD_CONFIGURATION                    0xC2
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_SYSTEM_GUID                          0xC3
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_SYSTEM_ASSET_TAG                     0xC4
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_SYSTEM_SERVICE_TAG                   0xC5
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CHASSIS_SERVICE_TAG                  0xC6
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CHASSIS_RELATED_SERVICE_TAG          0xC7
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_BOARD_REVISION                       0xC8
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_SYSTEM_ID                            0xC9
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_BIOS_FEATURE                         0xCA
/* Only for 10G systems */
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_EMBEDDED_NICS_MAC_ADDRESSES          0xCB
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_EMBEDDED_NICS_CAPABILITY             0xCE
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_PLATFORM_MODEL_NAME                  0xD1
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_LOCAL_CONSOLE_LOCKOUT                0xD6
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_POWER_STAGGERING_AC_RECOVERY         0xD8
/* achu: this one is taken from code, is correct name? */
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_11G_MAC_ADDRESSES                    0xDA
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_12G_MAC_ADDRESSES                    IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_11G_MAC_ADDRESSES
/* achu: this one is through reverse engineering, is correct name? */ 
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_SLOT_NUMBER                          0xDC
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IDRAC_INFO                           0xDD
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IDRAC_IPV4_URL                       0xDE
/* CMC = Chassis Management Controller */
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CMC_IPV4_URL                         0xE0
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IDRAC_GUI_WEBSERVER_CONTROL          0xE1
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_PLATFORM_SPECIFIC_DEVICE_INFORMATION 0xE3
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_LCD_STATUS                           0xE7
/* achu: this one is taken from code, is correct name? */
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_POWER_CAPACITY                       0xEA
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_AVERAGE_POWER_CONSUMPTION_STATISTICS 0xEB
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_MAX_POWER_CONSUMPTION_STATISTICS     0xEC
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_MIN_POWER_CONSUMPTION_STATISTICS     0xED
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_EMBEDDED_VIDEO_STATUS                0xEE
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_ISCSI_NICS_MAC_ADDRESSES             0xEF
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IPV6_SNMP_TRAP_DESTINATION_ADDRESS   0xF0
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_INTERNAL_STORAGE_SLOT_INFO           0xF1
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CMC_IPV6_INFO                        0xF2
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CMC_IPV6_URL                         0xF3
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_SYSTEM_REVISION                      0xF4
#define IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_REDUNDANCY_POLICY                    0xFE

/* Dell System Info Parameter macros */

/* 256 b/c length is 8 bit field */
#define IPMI_OEM_DELL_SYSTEM_INFO_MAX_STRING_BYTES 256

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_PLATFORM_MODEL_NAME,
 * IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_SLOT_NUMBER,
 * IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IDRAC_IPV4_URL,
 * IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IDRAC_INFO, 
 * IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CMC_IPV4_URL,
 * IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CMC_IPV6_INFO,
 * IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CMC_IPV6_URL
 */
#define IPMI_OEM_DELL_SYSTEM_INFO_STRING_ENCODING_BITMASK 0xF
#define IPMI_OEM_DELL_SYSTEM_INFO_STRING_ENCODING_SHIFT   0

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_EMBEDDED_NICS_MAC_ADDRESSES */
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_LENGTH 6

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_11G_MAC_ADDRESSES */
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_TYPE_BITMASK 0x30
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_TYPE_SHIFT   4

#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_NIC_STATUS_BITMASK 0xC0
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_NIC_STATUS_SHIFT   6

#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_NIC_NUMBER_BITMASK 0x1F
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_NIC_NUMBER_SHIFT   0

#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_TYPE_ETHERNET  0
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_TYPE_ISCSI     1
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_TYPE_RESERVED  3

/* achu: Dell calls an ethernet port that has been PCI disabled in the
 * BIOS, but has an active service processor "playing dead"
 */
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_STATUS_ENABLED      0
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_STATUS_DISABLED     1
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_STATUS_PLAYING_DEAD 2
#define IPMI_OEM_DELL_SYSTEM_INFO_MAC_ADDRESS_STATUS_RESERVED     3

#define IPMI_OEM_DELL_SYSTEM_INFO_11G_OR_12G_MAC_ADDRESS_LENGTH   8 

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IDRAC_INFO */
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_MIN_LEN 41

#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IP_ADDRESS_FORMAT_IPV4 0x00
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IP_ADDRESS_FORMAT_IPV6 0x01

#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IP_ADDRESS_CONFIG_DHCP   0x00
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IP_ADDRESS_CONFIG_STATIC 0x01

#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_FIRMWARE_VERSION_STRING_LENGTH 20

#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_TYPE_10G            0x08
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_TYPE_CMC            0x09
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_TYPE_11G_MONOLITHIC 0x0A
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_TYPE_11G_MODULAR    0x0B
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_TYPE_MASER_LITE_BMC 0x0D
/* From ipmitool, http://ipmitool.sourceforge.net/ */
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_TYPE_12G_MONOLITHIC 0x10
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_INFO_IDRAC_TYPE_12G_MODULAR    0x11

/* w/  IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IDRAC_GUI_WEBSERVER_CONTROL */
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_WEB_GUI_SERVER_CONTROL_DISABLED 0x00
#define IPMI_OEM_DELL_SYSTEM_INFO_IDRAC_WEB_GUI_SERVER_CONTROL_ENABLED  0x01

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_POWER_CAPACITY */
#define IPMI_OEM_DELL_SYSTEM_INFO_POWER_CAPACITY_UNITS_WATTS   0x00
#define IPMI_OEM_DELL_SYSTEM_INFO_POWER_CAPACITY_UNITS_BTUPHR  0x01
#define IPMI_OEM_DELL_SYSTEM_INFO_POWER_CAPACITY_UNITS_PERCENT 0x03

#define IPMI_OEM_DELL_SYSTEM_INFO_GET_POWER_CAPACITY_SYSTEM_THROTTLING_NORMAL_SYSTEM_OPERATION      0
#define IPMI_OEM_DELL_SYSTEM_INFO_GET_POWER_CAPACITY_SYSTEM_THROTTLING_SYSTEM_NEEDS_TO_BE_THROTTLED 1
#define IPMI_OEM_DELL_SYSTEM_INFO_GET_POWER_CAPACITY_SYSTEM_THROTTLING_SYSTEM_IS_OVERCONFIGURED     2

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_EMBEDDED_VIDEO_STATUS */
#define IPMI_OEM_DELL_SYSTEM_INFO_EMBEDDED_VIDEO_STATUS_DISABLED 0x00
#define IPMI_OEM_DELL_SYSTEM_INFO_EMBEDDED_VIDEO_STATUS_ENABLED  0x01

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_IPV6_SNMP_TRAP_DESTINATION_ADDRESS */
#define IPMI_OEM_DELL_SYSTEM_INFO_IPV6_SNMP_TRAP_DESTINATION_ADDRESS_MIN_LEN 6

/* w/ IPMI_SYSTEM_INFO_PARAMETER_OEM_DELL_CMC_IPV6_INFO */
#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_MIN_LEN 57

#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_IPV6_ADDRESS_STRING_LENGTH 39

#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_IPV6_STATUS_BITMASK 0x1
#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_IPV6_STATUS_SHIFT   0

#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_IPV6_STATUS_DISABLED 0
#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_IPV6_STATUS_ENABLED  1

#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_AUTOCONFIGURATION_BITMASK 0x1
#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_AUTOCONFIGURATION_SHIFT   0

#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_AUTOCONFIGURATION_DISABLED 0
#define IPMI_OEM_DELL_SYSTEM_INFO_CMC_IPV6_INFO_AUTOCONFIGURATION_ENABLED  1

#ifdef __cplusplus
}
#endif
#endif /* IPMI_SYSTEM_INFO_PARAMETERS_SPEC_H */
