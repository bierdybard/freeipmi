/* 
   bmc-conf2.c: BMC Config functions
   
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
*/

#include "common.h"

static int8_t 
set_bmc_user_access (ipmi_device_t *dev, 
		     uint8_t userid, 
		     uint8_t channel_number, 
		     uint8_t enable_ipmi_msgs, 
		     uint8_t enable_link_auth, 
		     uint8_t enable_restrict_to_callback, 
		     uint8_t privilege_limit, 
		     uint8_t session_limit)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_user_access_rs);
  if (ipmi_cmd_set_user_access2 (dev, 
				 channel_number, 
				 userid, 
				 enable_restrict_to_callback, 
				 enable_link_auth, 
				 enable_ipmi_msgs, 
				 privilege_limit, 
				 session_limit, 
				 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

static int8_t 
set_bmc_channel_access (ipmi_device_t *dev, 
			uint8_t channel_number, 
			uint8_t set_option, 
			uint8_t access_mode, 
			uint8_t enable_user_level_auth, 
			uint8_t enable_per_message_auth, 
			uint8_t enable_pef_alerting, 
			uint8_t channel_privilege_limit)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_channel_access_rs);
  if (ipmi_cmd_set_channel_access2 (dev, 
				    channel_number, 
				    access_mode, 
				    enable_user_level_auth, 
				    enable_per_message_auth, 
				    enable_pef_alerting, 
				    (set_option ? IPMI_CHANNEL_ACCESS_SET_VOLATILE : 
				     IPMI_CHANNEL_ACCESS_SET_NON_VOLATILE), 
				    channel_privilege_limit, 
				    (set_option ? IPMI_PRIV_LEVEL_LIMIT_SET_VOLATILE : 
				     IPMI_PRIV_LEVEL_LIMIT_SET_NON_VOLATILE), 
				    obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_username (ipmi_device_t *dev, 
		  uint8_t userid, 
		  uint8_t *username)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  if (userid == 1)
    return (0);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_user_name_rs);
  if (ipmi_cmd_set_user_name2 (dev, 
			       userid, 
			       (char *)username, 
			       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_enable_user (ipmi_device_t *dev, 
		     uint8_t userid, 
		     int user_status)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t password[IPMI_USER_PASSWORD_MAX_LENGTH];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_user_password_rs);
  memset (password, 0, IPMI_USER_PASSWORD_MAX_LENGTH);
  if (ipmi_cmd_set_user_password2 (dev, 
				   userid, 
				   (user_status ? IPMI_PASSWORD_OPERATION_ENABLE_USER :
				    IPMI_PASSWORD_OPERATION_DISABLE_USER), 
				   (char *)password, 
				   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_user_password (ipmi_device_t *dev, 
		       uint8_t userid, 
		       uint8_t *password)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_user_password_rs);
  if (ipmi_cmd_set_user_password2 (dev, 
				   userid, 
				   IPMI_PASSWORD_OPERATION_SET_PASSWORD, 
				   (char *)password, 
				   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_user_lan_channel_access (ipmi_device_t *dev, 
				 uint8_t userid, 
				 uint8_t lan_enable_ipmi_msgs, 
				 uint8_t lan_enable_link_auth, 
				 uint8_t lan_enable_restrict_to_callback, 
				 uint8_t lan_privilege_limit, 
				 uint8_t lan_session_limit)
{
  return set_bmc_user_access (dev, 
			      userid, 
			      get_lan_channel_number (), 
			      lan_enable_ipmi_msgs, 
			      lan_enable_link_auth, 
			      lan_enable_restrict_to_callback, 
			      lan_privilege_limit, 
			      lan_session_limit);
}

int8_t 
set_bmc_user_serial_channel_access (ipmi_device_t *dev, 
				    uint8_t userid, 
				    uint8_t serial_enable_ipmi_msgs, 
				    uint8_t serial_enable_link_auth, 
				    uint8_t serial_enable_restrict_to_callback, 
				    uint8_t serial_privilege_limit, 
				    uint8_t serial_session_limit)
{
  return set_bmc_user_access (dev, 
			      userid, 
			      get_serial_channel_number (), 
			      serial_enable_ipmi_msgs, 
			      serial_enable_link_auth, 
			      serial_enable_restrict_to_callback, 
			      serial_privilege_limit, 
			      serial_session_limit);
}

int8_t 
set_bmc_lan_channel_volatile_access (ipmi_device_t *dev, 
				     uint8_t access_mode, 
				     uint8_t enable_user_level_auth, 
				     uint8_t enable_per_message_auth, 
				     uint8_t enable_pef_alerting, 
				     uint8_t channel_privilege_limit)
{
  return set_bmc_channel_access (dev, 
				 get_lan_channel_number (), 
				 1, 
				 access_mode, 
				 (enable_user_level_auth ? 0 : 1), 
				 (enable_per_message_auth ? 0 : 1), 
				 (enable_pef_alerting ? 0 : 1), 
				 channel_privilege_limit);
}

int8_t 
set_bmc_lan_channel_non_volatile_access (ipmi_device_t *dev, 
					 uint8_t access_mode, 
					 uint8_t enable_user_level_auth, 
					 uint8_t enable_per_message_auth, 
					 uint8_t enable_pef_alerting, 
					 uint8_t channel_privilege_limit)
{
  return set_bmc_channel_access (dev, 
				 get_lan_channel_number (), 
				 0, 
				 access_mode, 
				 (enable_user_level_auth ? 0 : 1), 
				 (enable_per_message_auth ? 0 : 1), 
				 (enable_pef_alerting ? 0 : 1), 
				 channel_privilege_limit);
}

int8_t 
set_bmc_lan_conf_ip_addr_source (ipmi_device_t *dev, 
				 uint8_t ip_addr_source)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_ip_addr_source2 (dev, 
					get_lan_channel_number (), 
					ip_addr_source, 
					obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_ip_addr (ipmi_device_t *dev, 
			  char *ip_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  unsigned int b1, b2, b3, b4;
  uint64_t ip_address = 0;
  
  sscanf (ip_addr, "%u.%u.%u.%u", &b1, &b2, &b3, &b4);
  ip_address = bits_merge (ip_address, 0,  8,  b1);
  ip_address = bits_merge (ip_address, 8,  16, b2);
  ip_address = bits_merge (ip_address, 16, 24, b3);
  ip_address = bits_merge (ip_address, 24, 32, b4);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_ip_addr2 (dev, 
				 get_lan_channel_number (), 
				 ip_address, 
				 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_mac_addr (ipmi_device_t *dev, 
			   char *mac_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  unsigned int b1, b2, b3, b4, b5, b6;
  uint64_t mac_address = 0;
  
  sscanf (mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", &b1, &b2, &b3, &b4, &b5, &b6);
  mac_address = bits_merge (mac_address, 0,  8,  b1);
  mac_address = bits_merge (mac_address, 8,  16, b2);
  mac_address = bits_merge (mac_address, 16, 24, b3);
  mac_address = bits_merge (mac_address, 24, 32, b4);
  mac_address = bits_merge (mac_address, 32, 40, b5);
  mac_address = bits_merge (mac_address, 40, 48, b6);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_mac_addr2 (dev, 
				  get_lan_channel_number (), 
				  mac_address, 
				  obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_subnet_mask (ipmi_device_t *dev, 
			      char *subnet_mask)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  unsigned int b1, b2, b3, b4;
  uint64_t subnetmask = 0;
  
  sscanf (subnet_mask, "%u.%u.%u.%u", &b1, &b2, &b3, &b4);
  subnetmask = bits_merge (subnetmask, 0,  8,  b1);
  subnetmask = bits_merge (subnetmask, 8,  16, b2);
  subnetmask = bits_merge (subnetmask, 16, 24, b3);
  subnetmask = bits_merge (subnetmask, 24, 32, b4);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_subnet_mask2 (dev, 
				     get_lan_channel_number (), 
				     subnetmask, 
				     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_default_gw_ip_addr (ipmi_device_t *dev, 
				     char *default_gw_ip_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  unsigned int b1, b2, b3, b4;
  uint64_t ip_address = 0;
  
  sscanf (default_gw_ip_addr, "%u.%u.%u.%u", &b1, &b2, &b3, &b4);
  ip_address = bits_merge (ip_address, 0,  8,  b1);
  ip_address = bits_merge (ip_address, 8,  16, b2);
  ip_address = bits_merge (ip_address, 16, 24, b3);
  ip_address = bits_merge (ip_address, 24, 32, b4);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_default_gw_ip_addr2 (dev, 
					    get_lan_channel_number (), 
					    ip_address, 
					    obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_default_gw_mac_addr (ipmi_device_t *dev, 
				      char *default_gw_mac_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  unsigned int b1, b2, b3, b4, b5, b6;
  uint64_t mac_address = 0;
  
  sscanf (default_gw_mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", 
	  &b1, &b2, &b3, &b4, &b5, &b6);
  mac_address = bits_merge (mac_address, 0,  8,  b1);
  mac_address = bits_merge (mac_address, 8,  16, b2);
  mac_address = bits_merge (mac_address, 16, 24, b3);
  mac_address = bits_merge (mac_address, 24, 32, b4);
  mac_address = bits_merge (mac_address, 32, 40, b5);
  mac_address = bits_merge (mac_address, 40, 48, b6);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_default_gw_mac_addr2 (dev, 
					     get_lan_channel_number (), 
					     mac_address, 
					     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_backup_gw_ip_addr (ipmi_device_t *dev, 
				    char *backup_gw_ip_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  unsigned int b1, b2, b3, b4;
  uint64_t ip_address = 0;
  
  sscanf (backup_gw_ip_addr, "%u.%u.%u.%u", &b1, &b2, &b3, &b4);
  ip_address = bits_merge (ip_address, 0,  8,  b1);
  ip_address = bits_merge (ip_address, 8,  16, b2);
  ip_address = bits_merge (ip_address, 16, 24, b3);
  ip_address = bits_merge (ip_address, 24, 32, b4);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_backup_gw_ip_addr2 (dev, 
					   get_lan_channel_number (), 
					   ip_address, 
					   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_backup_gw_mac_addr (ipmi_device_t *dev, 
				     char *backup_gw_mac_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  unsigned int b1, b2, b3, b4, b5, b6;
  uint64_t mac_address = 0;
  
  sscanf (backup_gw_mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", 
	  &b1, &b2, &b3, &b4, &b5, &b6);
  mac_address = bits_merge (mac_address, 0,  8,  b1);
  mac_address = bits_merge (mac_address, 8,  16, b2);
  mac_address = bits_merge (mac_address, 16, 24, b3);
  mac_address = bits_merge (mac_address, 24, 32, b4);
  mac_address = bits_merge (mac_address, 32, 40, b5);
  mac_address = bits_merge (mac_address, 40, 48, b6);
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_backup_gw_mac_addr2 (dev, 
					    get_lan_channel_number (), 
					    mac_address, 
					    obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_vlan_id (ipmi_device_t *dev, 
			  uint8_t vlan_id_flag, 
			  uint32_t vlan_id)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_vlan_id2 (dev, 
				 get_lan_channel_number (), 
				 vlan_id_flag, 
				 vlan_id, 
				 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_vlan_priority (ipmi_device_t *dev, 
				uint8_t vlan_priority)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_vlan_priority2 (dev, 
				       get_lan_channel_number (), 
				       vlan_priority, 
				       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

static int8_t
_fill_lan_set_auth_type_enables (fiid_obj_t obj_data_rq,
                                 fiid_template_t l_tmpl_set_auth_auth_type_enables,
                                 uint8_t channel_number,
                                 uint8_t max_privilege_auth_type_callback_level,
                                 uint8_t max_privilege_auth_type_user_level,
                                 uint8_t max_privilege_auth_type_operator_level,
                                 uint8_t max_privilege_auth_type_admin_level,
                                 uint8_t max_privilege_auth_type_oem_level)
{
  if (obj_data_rq == NULL
      || !IPMI_CHANNEL_NUMBER_VALID(channel_number))
    {
      errno = EINVAL;
      return (-1);
    }

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"cmd",
                    IPMI_CMD_SET_LAN_CONF_PARAMS) < 0)
    return (-1);

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"channel_number",
                    channel_number) < 0)
    return (-1);

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"parameter_selector",
                    IPMI_LAN_PARAM_AUTH_TYPE_ENABLES) < 0)
    return (-1);

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"max_privilege_auth_type_callback_level",
                    max_privilege_auth_type_callback_level) < 0)
    return (-1);

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"max_privilege_auth_type_user_level",
                    max_privilege_auth_type_user_level) < 0)
    return (-1);

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"max_privilege_auth_type_operator_level",
                    max_privilege_auth_type_operator_level) < 0)
    return (-1);

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"max_privilege_auth_type_admin_level",
                    max_privilege_auth_type_admin_level) < 0)
    return (-1);

  if (fiid_obj_set (obj_data_rq,
                    l_tmpl_set_auth_auth_type_enables,
                    (uint8_t *)"max_privilege_auth_type_oem_level",
                    max_privilege_auth_type_oem_level) < 0)
    return (-1);

  return 0;
}

int8_t 
set_bmc_lan_conf_auth_type_enables (ipmi_device_t *dev, 
				    struct bmc_auth_level *bmc_auth_level)
{
  fiid_obj_t obj_cmd_rq = NULL;
  fiid_obj_t obj_cmd_rs = NULL;
  fiid_template_t l_tmpl_set_lan_conf_param_auth_type_enables_rq =
    {
      {8, "cmd"},
      {4, "channel_number"},
      {4, "reserved1"},
      {8, "parameter_selector"},
      {6, "max_privilege_auth_type_callback_level"},
      {2, "max_privilege_auth_type_callback_level.reserved2"},
      {6, "max_privilege_auth_type_user_level"},
      {2, "max_privilege_auth_type_user_level.reserved2"},
      {6, "max_privilege_auth_type_operator_level"},
      {2, "max_privilege_auth_type_operator_level.reserved2"},
      {6, "max_privilege_auth_type_admin_level"},
      {2, "max_privilege_auth_type_admin_level.reserved2"},
      {6, "max_privilege_auth_type_oem_level"},
      {2, "max_privilege_auth_type_oem_level.reserved2"},
      {0, ""}
    };
  uint8_t auth_type_callback_level = 0;
  uint8_t auth_type_user_level = 0;
  uint8_t auth_type_operator_level = 0;
  uint8_t auth_type_admin_level = 0;
  uint8_t auth_type_oem_level = 0;
  
  if (bmc_auth_level->callback.type_none)
    auth_type_callback_level = BIT_SET (auth_type_callback_level, 0);
  if (bmc_auth_level->callback.type_md2)
    auth_type_callback_level = BIT_SET (auth_type_callback_level, 1);
  if (bmc_auth_level->callback.type_md5)
    auth_type_callback_level = BIT_SET (auth_type_callback_level, 2);
  if (bmc_auth_level->callback.type_straight_password)
    auth_type_callback_level = BIT_SET (auth_type_callback_level, 4);
  if (bmc_auth_level->callback.type_oem_proprietary)
    auth_type_callback_level = BIT_SET (auth_type_callback_level, 5);
  
  if (bmc_auth_level->user.type_none)
    auth_type_user_level = BIT_SET (auth_type_user_level, 0);
  if (bmc_auth_level->user.type_md2)
    auth_type_user_level = BIT_SET (auth_type_user_level, 1);
  if (bmc_auth_level->user.type_md5)
    auth_type_user_level = BIT_SET (auth_type_user_level, 2);
  if (bmc_auth_level->user.type_straight_password)
    auth_type_user_level = BIT_SET (auth_type_user_level, 4);
  if (bmc_auth_level->user.type_oem_proprietary)
    auth_type_user_level = BIT_SET (auth_type_user_level, 5);
  
  if (bmc_auth_level->operator.type_none)
    auth_type_operator_level = BIT_SET (auth_type_operator_level, 0);
  if (bmc_auth_level->operator.type_md2)
    auth_type_operator_level = BIT_SET (auth_type_operator_level, 1);
  if (bmc_auth_level->operator.type_md5)
    auth_type_operator_level = BIT_SET (auth_type_operator_level, 2);
  if (bmc_auth_level->operator.type_straight_password)
    auth_type_operator_level = BIT_SET (auth_type_operator_level, 4);
  if (bmc_auth_level->operator.type_oem_proprietary)
    auth_type_operator_level = BIT_SET (auth_type_operator_level, 5);
  
  if (bmc_auth_level->admin.type_none)
    auth_type_admin_level = BIT_SET (auth_type_admin_level, 0);
  if (bmc_auth_level->admin.type_md2)
    auth_type_admin_level = BIT_SET (auth_type_admin_level, 1);
  if (bmc_auth_level->admin.type_md5)
    auth_type_admin_level = BIT_SET (auth_type_admin_level, 2);
  if (bmc_auth_level->admin.type_straight_password)
    auth_type_admin_level = BIT_SET (auth_type_admin_level, 4);
  if (bmc_auth_level->admin.type_oem_proprietary)
    auth_type_admin_level = BIT_SET (auth_type_admin_level, 5);
  
  if (bmc_auth_level->oem.type_none)
    auth_type_oem_level = BIT_SET (auth_type_oem_level, 0);
  if (bmc_auth_level->oem.type_md2)
    auth_type_oem_level = BIT_SET (auth_type_oem_level, 1);
  if (bmc_auth_level->oem.type_md5)
    auth_type_oem_level = BIT_SET (auth_type_oem_level, 2);
  if (bmc_auth_level->oem.type_straight_password)
    auth_type_oem_level = BIT_SET (auth_type_oem_level, 4);
  if (bmc_auth_level->oem.type_oem_proprietary)
    auth_type_oem_level = BIT_SET (auth_type_oem_level, 5);
  
  fiid_obj_alloca (obj_cmd_rq, l_tmpl_set_lan_conf_param_auth_type_enables_rq);
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (_fill_lan_set_auth_type_enables (obj_cmd_rq,
                                       l_tmpl_set_lan_conf_param_auth_type_enables_rq,
                                       get_lan_channel_number (), 
                                       auth_type_callback_level,
                                       auth_type_user_level,
                                       auth_type_operator_level,
                                       auth_type_admin_level,
                                       auth_type_oem_level) < 0)
    return (-1);
  if (ipmi_cmd (dev,
                IPMI_BMC_IPMB_LUN_BMC,
                IPMI_NET_FN_TRANSPORT_RQ,
                obj_cmd_rq,
                l_tmpl_set_lan_conf_param_auth_type_enables_rq,
                obj_cmd_rs,
                tmpl_set_lan_conf_param_rs) < 0)
    return (-1);
  if (ipmi_comp_test (obj_cmd_rs) != 1)
    return (-1);
  
  return (0);
}

int8_t 
set_bmc_lan_conf_arp_control (ipmi_device_t *dev, 
			      uint8_t enable_gratuitous_arps, 
			      uint8_t enable_arp_response)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_cmd_lan_set_arp2 (dev, 
			     get_lan_channel_number (), 
			     enable_gratuitous_arps, 
			     enable_arp_response, 
			     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_lan_conf_gratuitous_arp (ipmi_device_t *dev, 
				 uint8_t gratuitous_arp_interval)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_lan_conf_param_rs);
  if (ipmi_lan_set_gratuitous_arp_interval2 (dev, 
					     get_lan_channel_number (), 
					     gratuitous_arp_interval, 
					     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_serial_channel_volatile_access (ipmi_device_t *dev, 
					uint8_t access_mode, 
					uint8_t enable_user_level_auth, 
					uint8_t enable_per_message_auth, 
					uint8_t enable_pef_alerting, 
					uint8_t channel_privilege_limit)
{
  return set_bmc_channel_access (dev, 
				 get_serial_channel_number (), 
				 1, 
				 access_mode, 
				 (enable_user_level_auth ? 0 : 1), 
				 (enable_per_message_auth ? 0 : 1), 
				 (enable_pef_alerting ? 0 : 1), 
				 channel_privilege_limit);
}

int8_t 
set_bmc_serial_channel_non_volatile_access (ipmi_device_t *dev, 
					    uint8_t access_mode, 
					    uint8_t enable_user_level_auth, 
					    uint8_t enable_per_message_auth, 
					    uint8_t enable_pef_alerting, 
					    uint8_t channel_privilege_limit)
{
  return set_bmc_channel_access (dev, 
				 get_serial_channel_number (), 
				 0, 
				 access_mode, 
				 (enable_user_level_auth ? 0 : 1), 
				 (enable_per_message_auth ? 0 : 1), 
				 (enable_pef_alerting ? 0 : 1), 
				 channel_privilege_limit);
}

int8_t 
set_bmc_serial_conf_conn_mode (ipmi_device_t *dev, 
			       uint8_t enable_basic_mode, 
			       uint8_t enable_ppp_mode, 
			       uint8_t enable_terminal_mode, 
			       uint8_t connect_mode)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_serial_conf_param_rs);
  if (ipmi_cmd_set_serial_connmode2 (dev, 
				     get_serial_channel_number (), 
				     enable_basic_mode, 
				     enable_ppp_mode, 
				     enable_terminal_mode, 
				     connect_mode, 
				     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_serial_conf_page_blackout_interval (ipmi_device_t *dev, 
					    uint8_t page_blackout_interval)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_serial_conf_param_rs);
  if (ipmi_cmd_set_serial_page_blackout_interval2 (dev, 
						   get_serial_channel_number (), 
						   page_blackout_interval, 
						   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_serial_conf_call_retry_time (ipmi_device_t *dev, 
				     uint8_t call_retry_time)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_serial_conf_param_rs);
  if (ipmi_cmd_set_serial_retry_time2 (dev, 
				       get_serial_channel_number (), 
				       call_retry_time, 
				       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_serial_conf_ipmi_msg_comm_settings (ipmi_device_t *dev, 
					    uint8_t dtr_hangup, 
					    uint8_t flow_control, 
					    uint8_t bit_rate)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_serial_conf_param_rs);
  if (ipmi_cmd_set_serial_comm_bits2 (dev, 
				      get_serial_channel_number (), 
				      dtr_hangup, 
				      flow_control, 
				      bit_rate, 
				      obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_bmc_power_restore_policy (ipmi_device_t *dev, 
			      uint8_t power_restore_policy)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_power_restore_policy_rs);
  if (ipmi_cmd_set_power_restore_policy2 (dev, 
					  power_restore_policy, 
					  obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_pef_control (ipmi_device_t *dev, 
		 uint8_t pef_enable, 
		 uint8_t pef_event_msgs_enable, 
		 uint8_t pef_startup_delay_enable, 
		 uint8_t pef_alert_startup_delay_enable)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_pef_conf_param_rs);
  if (ipmi_cmd_set_pef_control2 (dev, 
				 pef_enable, 
				 pef_event_msgs_enable, 
				 pef_startup_delay_enable, 
				 pef_alert_startup_delay_enable, 
				 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_pef_global_action_control (ipmi_device_t *dev, 
			       uint8_t alert_action_enable, 
			       uint8_t powerdown_action_enable, 
			       uint8_t reset_action_enable, 
			       uint8_t powercycle_action_enable, 
			       uint8_t oem_action_enable, 
			       uint8_t diag_interrupt_enable)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_pef_conf_param_rs);
  if (ipmi_cmd_set_global_action_control2 (dev, 
					   alert_action_enable, 
					   powerdown_action_enable, 
					   reset_action_enable, 
					   powercycle_action_enable, 
					   oem_action_enable, 
					   diag_interrupt_enable, 
					   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_pef_startup_delay (ipmi_device_t *dev, 
		       uint8_t pef_startup_delay)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_pef_conf_param_rs);
  if (ipmi_cmd_set_startup_delay2 (dev, 
				   pef_startup_delay, 
				   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

int8_t 
set_pef_alert_startup_delay (ipmi_device_t *dev, 
			     uint8_t pef_alert_startup_delay)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_pef_conf_param_rs);
  if (ipmi_cmd_set_alert_startup_delay2 (dev, 
					 pef_alert_startup_delay, 
					 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  return (0);
}

static int8_t 
get_bmc_user_access (ipmi_device_t *dev, 
		     uint8_t userid, 
		     uint8_t channel_number, 
		     uint8_t *enable_ipmi_msgs, 
		     uint8_t *enable_link_auth, 
		     uint8_t *enable_restrict_to_callback, 
		     uint8_t *privilege_limit, 
		     uint8_t *session_limit)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_user_access_rs);
  if (ipmi_cmd_get_user_access2 (dev, 
				 channel_number, 
				 userid, 
				 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_user_access_rs, 
		(uint8_t *)"user_privilege_level_limit", 
		&val);
  *privilege_limit = (uint8_t) val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_user_access_rs, 
		(uint8_t *)"user_flags.enable_ipmi_msgs", 
		&val);
  *enable_ipmi_msgs = (uint8_t) val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_user_access_rs, 
		(uint8_t *)"user_flags.enable_link_auth", 
		&val);
  *enable_link_auth = (uint8_t) val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_user_access_rs, 
		(uint8_t *)"user_flags.restrict_to_callback", 
		&val);
  *enable_restrict_to_callback = (uint8_t) val;
  
  *session_limit = 0;
  
  return (0);
}

static int8_t 
get_bmc_channel_access (ipmi_device_t *dev, 
			uint8_t channel_number, 
			uint8_t access_type, 
			uint8_t *access_mode, 
			uint8_t *user_level_auth, 
			uint8_t *per_message_auth, 
			uint8_t *pef_alerting, 
			uint8_t *privilege_limit)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_channel_access_rs);
  if (ipmi_cmd_get_channel_access2 (dev, 
				    channel_number, 
				    (access_type ? IPMI_CHANNEL_ACCESS_GET_VOLATILE :
				     IPMI_CHANNEL_ACCESS_GET_NON_VOLATILE), 
				    obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_channel_access_rs, 
		(uint8_t *)"ipmi_messaging_access_mode", 
		&val);
  *access_mode = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_channel_access_rs, 
		(uint8_t *)"user_level_authentication", 
		&val);
  *user_level_auth = (val ? 0 : 1);
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_channel_access_rs, 
		(uint8_t *)"per_message_authentication", 
		&val);
  *per_message_auth = (val ? 0 : 1);
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_channel_access_rs, 
		(uint8_t *)"pef_alerting", 
		&val);
  *pef_alerting = (val ? 0 : 1);
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_channel_access_rs, 
		(uint8_t *)"channel_privilege_level_limit", 
		&val);
  *privilege_limit = val;
  
  return (0);
}

int8_t 
get_bmc_username (ipmi_device_t *dev, 
		  uint8_t userid, 
		  uint8_t *username,
                  uint32_t username_len)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  if (userid == 1)
    {
      strcpy ((char *)username, "NULL");
      return (0);
    }
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_user_name_rs);
  if (ipmi_cmd_get_user_name2 (dev, 
			       userid, 
			       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_user_name_rs, 
		     (uint8_t *)"user_name", 
		     username,
                     username_len);
  
  return (0);
}

int8_t 
get_bmc_user_lan_channel_access (ipmi_device_t *dev, 
				 uint8_t userid, 
				 uint8_t *enable_ipmi_msgs, 
				 uint8_t *enable_link_auth, 
				 uint8_t *enable_restrict_to_callback, 
				 uint8_t *privilege_limit, 
				 uint8_t *session_limit)
{
  return get_bmc_user_access (dev, 
			      userid, 
			      get_lan_channel_number (), 
			      enable_ipmi_msgs, 
			      enable_link_auth, 
			      enable_restrict_to_callback, 
			      privilege_limit, 
			      session_limit);
}

int8_t 
get_bmc_user_serial_channel_access (ipmi_device_t *dev, 
				    uint8_t userid, 
				    uint8_t *enable_ipmi_msgs, 
				    uint8_t *enable_link_auth, 
				    uint8_t *enable_restrict_to_callback, 
				    uint8_t *privilege_limit, 
				    uint8_t *session_limit)
{
  return get_bmc_user_access (dev, 
			      userid, 
			      get_serial_channel_number (), 
			      enable_ipmi_msgs, 
			      enable_link_auth, 
			      enable_restrict_to_callback, 
			      privilege_limit, 
			      session_limit);
}

int8_t 
get_bmc_lan_channel_volatile_access (ipmi_device_t *dev, 
				     uint8_t *access_mode, 
				     uint8_t *user_level_auth, 
				     uint8_t *per_message_auth, 
				     uint8_t *pef_alerting, 
				     uint8_t *privilege_limit)
{
  return get_bmc_channel_access (dev, 
				 get_lan_channel_number (), 
				 1, 
				 access_mode, 
				 user_level_auth, 
				 per_message_auth, 
				 pef_alerting, 
				 privilege_limit);
}

int8_t 
get_bmc_lan_channel_non_volatile_access (ipmi_device_t *dev, 
					 uint8_t *access_mode, 
					 uint8_t *user_level_auth, 
					 uint8_t *per_message_auth, 
					 uint8_t *pef_alerting, 
					 uint8_t *privilege_limit)
{
  return get_bmc_channel_access (dev, 
				 get_lan_channel_number (), 
				 0, 
				 access_mode, 
				 user_level_auth, 
				 per_message_auth, 
				 pef_alerting, 
				 privilege_limit);
}

int8_t 
get_bmc_lan_conf_ip_addr_source (ipmi_device_t *dev, 
				 uint8_t *ip_addr_source)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_ip_addr_source_rs);
  if (ipmi_cmd_lan_get_ip_addr_source2 (dev, 
					get_lan_channel_number (), 
					IPMI_GET_LAN_PARAMETER, 
					SET_SELECTOR, 
					BLOCK_SELECTOR, 
					obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_ip_addr_source_rs, 
		(uint8_t *)"ip_addr_source", 
		&val);
  *ip_addr_source = val;
  
  return (0);
}

int8_t 
get_bmc_lan_conf_ip_addr (ipmi_device_t *dev, 
			  char *ip_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t ip_addr_bytes[4];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_ip_addr_rs);
  if (ipmi_cmd_lan_get_ip_addr2 (dev, 
				 get_lan_channel_number (), 
				 IPMI_GET_LAN_PARAMETER, 
				 SET_SELECTOR, 
				 BLOCK_SELECTOR, 
				 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_lan_conf_param_ip_addr_rs, 
		     (uint8_t *)"ip_addr", 
		     ip_addr_bytes,
                     4);
  sprintf (ip_addr, 
	   "%u.%u.%u.%u", 
	   ip_addr_bytes[0], 
	   ip_addr_bytes[1], 
	   ip_addr_bytes[2], 
	   ip_addr_bytes[3]);
  
  return (0);
}

int8_t 
get_bmc_lan_conf_mac_addr (ipmi_device_t *dev, 
			   char *mac_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t mac_addr_bytes[6];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_mac_addr_rs);
  if (ipmi_cmd_lan_get_mac_addr2 (dev, 
				  get_lan_channel_number (), 
				  IPMI_GET_LAN_PARAMETER, 
				  SET_SELECTOR, 
				  BLOCK_SELECTOR, 
				  obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_lan_conf_param_mac_addr_rs, 
		     (uint8_t *)"mac_addr", 
		     mac_addr_bytes,
                     6);
  sprintf (mac_addr, 
	   "%02X:%02X:%02X:%02X:%02X:%02X", 
	   mac_addr_bytes[0], 
	   mac_addr_bytes[1], 
	   mac_addr_bytes[2], 
	   mac_addr_bytes[3], 
	   mac_addr_bytes[4], 
	   mac_addr_bytes[5]);
  
  return (0);
}

int8_t 
get_bmc_lan_conf_subnet_mask (ipmi_device_t *dev, 
			      char *subnet_mask)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t subnet_mask_bytes[4];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_subnet_mask_rs);
  if (ipmi_cmd_lan_get_subnet_mask2 (dev, 
				     get_lan_channel_number (), 
				     IPMI_GET_LAN_PARAMETER, 
				     SET_SELECTOR, 
				     BLOCK_SELECTOR, 
				     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_lan_conf_param_subnet_mask_rs, 
		     (uint8_t *)"subnet_mask", 
		     subnet_mask_bytes,
                     4);
  sprintf (subnet_mask, 
	   "%u.%u.%u.%u", 
	   subnet_mask_bytes[0], 
	   subnet_mask_bytes[1], 
	   subnet_mask_bytes[2], 
	   subnet_mask_bytes[3]);
  
  return (0);
}

int8_t 
get_bmc_lan_conf_default_gw_ip_addr (ipmi_device_t *dev, 
				     char *default_gw_ip_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t ip_addr_bytes[4];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_gw_ip_addr_rs);
  if (ipmi_cmd_lan_get_default_gw_ip_addr2 (dev, 
					    get_lan_channel_number (), 
					    IPMI_GET_LAN_PARAMETER, 
					    SET_SELECTOR, 
					    BLOCK_SELECTOR, 
					    obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_lan_conf_param_gw_ip_addr_rs, 
		     (uint8_t *)"ip_addr", 
		     ip_addr_bytes,
                     4);
  sprintf (default_gw_ip_addr, 
	   "%u.%u.%u.%u", 
	   ip_addr_bytes[0], 
	   ip_addr_bytes[1], 
	   ip_addr_bytes[2], 
	   ip_addr_bytes[3]);
  
  return (0);
}

int8_t 
get_bmc_lan_conf_default_gw_mac_addr (ipmi_device_t *dev, 
				      char *default_gw_mac_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t mac_addr_bytes[6];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_mac_addr_rs);
  if (ipmi_cmd_lan_get_default_gw_mac_addr2 (dev, 
					     get_lan_channel_number (), 
					     IPMI_GET_LAN_PARAMETER, 
					     SET_SELECTOR, 
					     BLOCK_SELECTOR, 
					     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_lan_conf_param_mac_addr_rs, 
		     (uint8_t *)"mac_addr", 
		     mac_addr_bytes,
                     6);
  sprintf (default_gw_mac_addr, 
	   "%02X:%02X:%02X:%02X:%02X:%02X", 
	   mac_addr_bytes[0], 
	   mac_addr_bytes[1], 
	   mac_addr_bytes[2], 
	   mac_addr_bytes[3], 
	   mac_addr_bytes[4], 
	   mac_addr_bytes[5]);
  
  return (0);
}

int8_t 
get_bmc_lan_conf_backup_gw_ip_addr (ipmi_device_t *dev, 
				    char *backup_gw_ip_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t ip_addr_bytes[4];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_gw_ip_addr_rs);
  if (ipmi_cmd_lan_get_backup_gw_ip_addr2 (dev, 
					   get_lan_channel_number (), 
					   IPMI_GET_LAN_PARAMETER, 
					   SET_SELECTOR, 
					   BLOCK_SELECTOR, 
					   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_lan_conf_param_gw_ip_addr_rs, 
		     (uint8_t *)"ip_addr", 
		     ip_addr_bytes,
                     4);
  sprintf (backup_gw_ip_addr, 
	   "%u.%u.%u.%u", 
	   ip_addr_bytes[0], 
	   ip_addr_bytes[1], 
	   ip_addr_bytes[2], 
	   ip_addr_bytes[3]);
  
  return (0);
}

int8_t 
get_bmc_lan_conf_backup_gw_mac_addr (ipmi_device_t *dev, 
				     char *backup_gw_mac_addr)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint8_t mac_addr_bytes[6];
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_mac_addr_rs);
  if (ipmi_cmd_lan_get_backup_gw_mac_addr2 (dev, 
					    get_lan_channel_number (), 
					    IPMI_GET_LAN_PARAMETER, 
					    SET_SELECTOR, 
					    BLOCK_SELECTOR, 
					    obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get_data (obj_cmd_rs, 
		     tmpl_get_lan_conf_param_mac_addr_rs, 
		     (uint8_t *)"mac_addr", 
		     mac_addr_bytes,
                     6);
  sprintf (backup_gw_mac_addr, 
	   "%02X:%02X:%02X:%02X:%02X:%02X", 
	   mac_addr_bytes[0], 
	   mac_addr_bytes[1], 
	   mac_addr_bytes[2], 
	   mac_addr_bytes[3], 
	   mac_addr_bytes[4], 
	   mac_addr_bytes[5]);
  
  return (0);
}

int8_t 
get_bmc_lan_conf_vlan_id (ipmi_device_t *dev, 
			  uint8_t *vlan_id_flag, 
			  uint32_t *vlan_id)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t ls_val, ms_val, val;
  uint8_t *ptr;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_vlan_id_rs);
  if (ipmi_cmd_lan_get_vlan_id2 (dev, 
				 get_lan_channel_number (), 
                                 IPMI_GET_LAN_PARAMETER, 
                                 SET_SELECTOR, 
                                 BLOCK_SELECTOR, 
                                 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_vlan_id_rs,
		(uint8_t *)"vlan_id_ls", 
		&ls_val);
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_vlan_id_rs,
		(uint8_t *)"vlan_id_ms", 
		&ms_val);
  
  ptr = (uint8_t *)vlan_id;
#if WORDS_BIGENDIAN
  ptr[3] = ls_val;
  ptr[2] = ms_val;
#else
  ptr[0] = ls_val;
  ptr[1] = ms_val;
#endif
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_vlan_id_rs,
		(uint8_t *)"vlan_id_flag", 
		&val);
  *vlan_id_flag = val;
  
  return (0);
}

int8_t 
get_bmc_lan_conf_vlan_priority (ipmi_device_t *dev, 
				uint8_t *vlan_priority)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_vlan_priority_rs);
  if (ipmi_cmd_lan_get_vlan_priority2 (dev, 
				       get_lan_channel_number (), 
                                       IPMI_GET_LAN_PARAMETER, 
                                       SET_SELECTOR, 
                                       BLOCK_SELECTOR, 
                                       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_vlan_priority_rs,
		(uint8_t *)"vlan_priority", 
		&val);
  *vlan_priority = val;
  
  return (0);
}

int8_t 
get_bmc_lan_conf_auth_type_enables (ipmi_device_t *dev, 
				    struct bmc_auth_level *bmc_auth_level)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_auth_type_enables_rs);
  if (ipmi_cmd_lan_get_auth_type_enables2 (dev, 
					   get_lan_channel_number (), 
					   IPMI_GET_LAN_PARAMETER, 
					   SET_SELECTOR, 
					   BLOCK_SELECTOR, 
					   obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_callback_level.none", 
		&val);
  bmc_auth_level->callback.type_none = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_callback_level.md2", 
		&val);
  bmc_auth_level->callback.type_md2 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_callback_level.md5", 
		&val);
  bmc_auth_level->callback.type_md5 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_callback_level.straight_password", 
		&val);
  bmc_auth_level->callback.type_straight_password = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_callback_level.oem_proprietary", 
		&val);
  bmc_auth_level->callback.type_oem_proprietary = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_user_level.none", 
		&val);
  bmc_auth_level->user.type_none = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_user_level.md2", 
		&val);
  bmc_auth_level->user.type_md2 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_user_level.md5", 
		&val);
  bmc_auth_level->user.type_md5 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_user_level.straight_password", 
		&val);
  bmc_auth_level->user.type_straight_password = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_user_level.oem_proprietary", 
		&val);
  bmc_auth_level->user.type_oem_proprietary = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_operator_level.none", 
		&val);
  bmc_auth_level->operator.type_none = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_operator_level.md2", 
		&val);
  bmc_auth_level->operator.type_md2 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_operator_level.md5", 
		&val);
  bmc_auth_level->operator.type_md5 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_operator_level.straight_password", 
		&val);
  bmc_auth_level->operator.type_straight_password = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_operator_level.oem_proprietary", 
		&val);
  bmc_auth_level->operator.type_oem_proprietary = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_admin_level.none", 
		&val);
  bmc_auth_level->admin.type_none = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_admin_level.md2", 
		&val);
  bmc_auth_level->admin.type_md2 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_admin_level.md5", 
		&val);
  bmc_auth_level->admin.type_md5 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_admin_level.straight_password", 
		&val);
  bmc_auth_level->admin.type_straight_password = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_admin_level.oem_proprietary", 
		&val);
  bmc_auth_level->admin.type_oem_proprietary = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_oem_level.none", 
		&val);
  bmc_auth_level->oem.type_none = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_oem_level.md2", 
		&val);
  bmc_auth_level->oem.type_md2 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_oem_level.md5", 
		&val);
  bmc_auth_level->oem.type_md5 = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_oem_level.straight_password", 
		&val);
  bmc_auth_level->oem.type_straight_password = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_auth_type_enables_rs, 
		(uint8_t *)"max_privilege_auth_type_oem_level.oem_proprietary", 
		&val);
  bmc_auth_level->oem.type_oem_proprietary = val;
  
  return (0);
}

int8_t 
get_bmc_lan_conf_arp_control (ipmi_device_t *dev, 
			      uint8_t *enable_gratuitous_arps, 
			      uint8_t *enable_arp_response)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_bmc_generated_arp_control_rs);
  if (ipmi_cmd_lan_get_arp2 (dev, 
			     get_lan_channel_number (), 
			     IPMI_GET_LAN_PARAMETER, 
			     SET_SELECTOR, 
			     BLOCK_SELECTOR, 
			     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_bmc_generated_arp_control_rs, 
		(uint8_t *)"bmc_generated_gratuitous_arps_flag", 
		&val);
  *enable_gratuitous_arps = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_bmc_generated_arp_control_rs, 
		(uint8_t *)"bmc_generated_arp_responses_flag", 
		&val);
  *enable_arp_response = val;
  
  return (0);
}

int8_t 
get_bmc_lan_conf_gratuitous_arp (ipmi_device_t *dev, 
				 uint8_t *gratuitous_arp_interval)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_lan_conf_param_gratuitous_arp_interval_rs);
  if (ipmi_cmd_lan_get_gratuitous_arp_interval2 (dev, 
						 get_lan_channel_number (), 
						 IPMI_GET_LAN_PARAMETER, 
						 SET_SELECTOR, 
						 BLOCK_SELECTOR, 
						 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_lan_conf_param_gratuitous_arp_interval_rs, 
		(uint8_t *)"gratuitous_arp_interval", 
		&val);
  *gratuitous_arp_interval = val;
  
  return (0);
}

int8_t 
get_bmc_serial_channel_volatile_access (ipmi_device_t *dev, 
					uint8_t *access_mode, 
					uint8_t *user_level_auth, 
					uint8_t *per_message_auth, 
					uint8_t *pef_alerting, 
					uint8_t *privilege_limit)
{
  return get_bmc_channel_access (dev, 
				 get_serial_channel_number (), 
				 1, 
				 access_mode, 
				 user_level_auth, 
				 per_message_auth, 
				 pef_alerting, 
				 privilege_limit);
}

int8_t 
get_bmc_serial_channel_non_volatile_access (ipmi_device_t *dev, 
					    uint8_t *access_mode, 
					    uint8_t *user_level_auth, 
					    uint8_t *per_message_auth, 
					    uint8_t *pef_alerting, 
					    uint8_t *privilege_limit)
{
  return get_bmc_channel_access (dev, 
				 get_serial_channel_number (), 
				 0, 
				 access_mode, 
				 user_level_auth, 
				 per_message_auth, 
				 pef_alerting, 
				 privilege_limit);
}

int8_t 
get_bmc_serial_conf_conn_mode (ipmi_device_t *dev, 
			       uint8_t *enable_basic_mode, 
			       uint8_t *enable_ppp_mode, 
			       uint8_t *enable_terminal_mode, 
			       uint8_t *connect_mode)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_serial_conf_param_connmode_rs);
  if (ipmi_cmd_get_serial_connmode2 (dev, 
				     get_serial_channel_number (), 
				     IPMI_GET_SERIAL_PARAMETER, 
				     SET_SELECTOR, 
				     BLOCK_SELECTOR, 
				     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_connmode_rs, 
		(uint8_t *)"basic_mode_enable", 
		&val);
  *enable_basic_mode = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_connmode_rs, 
		(uint8_t *)"ppp_mode_enable", 
		&val);
  *enable_ppp_mode = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_connmode_rs, 
		(uint8_t *)"terminal_mode_enable", 
		&val);
  *enable_terminal_mode = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_connmode_rs, 
		(uint8_t *)"direct", 
		&val);
  *connect_mode = val;
  
  return (0);
}

int8_t 
get_bmc_serial_conf_page_blackout_interval (ipmi_device_t *dev, 
					    uint8_t *page_blackout_interval)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_serial_conf_param_pageblackout_rs);
  if (ipmi_cmd_get_serial_page_blackout2 (dev, 
					  get_serial_channel_number (), 
					  IPMI_GET_SERIAL_PARAMETER, 
					  SET_SELECTOR, 
					  BLOCK_SELECTOR, 
					  obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_pageblackout_rs, 
		(uint8_t *)"page_blackout_interval", 
		&val);
  *page_blackout_interval = val;
  
  return (0);
}

int8_t 
get_bmc_serial_conf_call_retry_time (ipmi_device_t *dev, 
				     uint8_t *call_retry_time)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_serial_conf_param_retry_rs);
  if (ipmi_cmd_get_serial_retry_time2 (dev, 
				       get_serial_channel_number (), 
				       IPMI_GET_SERIAL_PARAMETER, 
				       SET_SELECTOR, 
				       BLOCK_SELECTOR, 
				       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_retry_rs, 
		(uint8_t *)"retry_time", 
		&val);
  *call_retry_time = val;
  
  return (0);
}

int8_t 
get_bmc_serial_conf_ipmi_msg_comm_settings (ipmi_device_t *dev, 
					    uint8_t *dtr_hangup, 
					    uint8_t *flow_control, 
					    uint8_t *bit_rate)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_serial_conf_param_commbits_rs);
  if (ipmi_cmd_get_serial_comm_bits2 (dev, 
				      get_serial_channel_number (), 
				      IPMI_GET_SERIAL_PARAMETER, 
				      SET_SELECTOR, 
				      BLOCK_SELECTOR, 
				      obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_commbits_rs, 
		(uint8_t *)"dtr_hangup", 
		&val);
  *dtr_hangup = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_commbits_rs, 
		(uint8_t *)"flow_control", 
		&val);
  *flow_control = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_serial_conf_param_commbits_rs, 
		(uint8_t *)"bit_rate", 
		&val);
  *bit_rate = val;
  
  return (0);
}

int8_t 
get_bmc_power_restore_policy (ipmi_device_t *dev, 
			      uint8_t *power_restore_policy)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_cmd_get_chassis_status_rs);
  if (ipmi_cmd_get_chassis_status2 (dev, obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_cmd_get_chassis_status_rs, 
		(uint8_t *)"power_state.power_restore_policy", 
		&val);
  *power_restore_policy = val;
  
  return (0);
}

int8_t 
get_pef_control (ipmi_device_t *dev, 
		 uint8_t *pef_enable, 
		 uint8_t *pef_event_msgs_enable, 
		 uint8_t *pef_startup_delay_enable, 
		 uint8_t *pef_alert_startup_delay_enable)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val = 0;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_pef_conf_param_pef_control_rs);
  if (ipmi_cmd_get_pef_control2 (dev, 
				 IPMI_GET_PEF_PARAMETER, 
				 SET_SELECTOR, 
				 BLOCK_SELECTOR, 
				 obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_pef_control_rs, 
		(uint8_t *)"enable_pef", 
		&val);
  *pef_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_pef_control_rs, 
		(uint8_t *)"enable_pef_event_msgs", 
		&val);
  *pef_event_msgs_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_pef_control_rs, 
		(uint8_t *)"enable_startup_delay", 
		&val);
  *pef_startup_delay_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_pef_control_rs, 
		(uint8_t *)"enable_alert_startup_delay", 
		&val);
  *pef_alert_startup_delay_enable = val;
  
  return (0);
}

int8_t 
get_pef_global_action_control (ipmi_device_t *dev, 
			       uint8_t *alert_action_enable, 
			       uint8_t *powerdown_action_enable, 
			       uint8_t *reset_action_enable, 
			       uint8_t *powercycle_action_enable, 
			       uint8_t *oem_action_enable, 
			       uint8_t *diag_interrupt_enable)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val = 0;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_pef_conf_param_global_action_control_rs);
  if (ipmi_cmd_get_pef_global_action_control2 (dev, 
					       IPMI_GET_PEF_PARAMETER, 
					       SET_SELECTOR, 
					       BLOCK_SELECTOR, 
					       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_global_action_control_rs, 
		(uint8_t *)"enable_alert_action", 
		&val);
  *alert_action_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_global_action_control_rs, 
		(uint8_t *)"enable_powerdown_action", 
		&val);
  *powerdown_action_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_global_action_control_rs, 
		(uint8_t *)"enable_reset_action", 
		&val);
  *reset_action_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_global_action_control_rs, 
		(uint8_t *)"enable_powercycle_action", 
		&val);
  *powercycle_action_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_global_action_control_rs, 
		(uint8_t *)"enable_oem_action", 
		&val);
  *oem_action_enable = val;
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_global_action_control_rs, 
		(uint8_t *)"enable_diag_interrupt", 
		&val);
  *diag_interrupt_enable = val;
  
  return (0);
}

int8_t 
get_pef_startup_delay (ipmi_device_t *dev, 
		       uint8_t *pef_startup_delay)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val = 0;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_pef_conf_param_startup_delay_rs);
  if (ipmi_cmd_get_pef_startup_delay2 (dev, 
				       IPMI_GET_PEF_PARAMETER, 
				       SET_SELECTOR, 
				       BLOCK_SELECTOR, 
				       obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_startup_delay_rs, 
		(uint8_t *)"pef_startup_delay", 
		&val);
  *pef_startup_delay = val;
  
  return (0);
}

int8_t 
get_pef_alert_startup_delay (ipmi_device_t *dev, 
			     uint8_t *pef_alert_startup_delay)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val = 0;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_get_pef_conf_param_alert_startup_delay_rs);
  if (ipmi_cmd_get_pef_alert_startup_delay2 (dev, 
					     IPMI_GET_PEF_PARAMETER, 
					     SET_SELECTOR, 
					     BLOCK_SELECTOR, 
					     obj_cmd_rs) != 0)
    {
      return (-1);
    }
  
  fiid_obj_get (obj_cmd_rs, 
		tmpl_get_pef_conf_param_alert_startup_delay_rs, 
		(uint8_t *)"pef_alert_startup_delay", 
		&val);
  *pef_alert_startup_delay = val;
  
  return (0);
}

/***********************************************************/
int8_t 
check_bmc_user_password (ipmi_device_t *dev, 
			 uint8_t userid, 
			 uint8_t *password)
{
  fiid_obj_t obj_cmd_rs = NULL;
  
  fiid_obj_alloca (obj_cmd_rs, tmpl_set_user_password_rs);
  if (ipmi_cmd_set_user_password2 (dev, 
				   userid, 
				   IPMI_PASSWORD_OPERATION_TEST_PASSWORD, 
				   (char *)password, 
				   obj_cmd_rs) != 0)
    {
      if (IPMI_COMP_CODE (obj_cmd_rs) == IPMI_PASSWORD_OPERATION_TEST_FAILED)
	return (0); /* false */
      return (-1); /* error */
    }
  
  return (1); /* true */
}
