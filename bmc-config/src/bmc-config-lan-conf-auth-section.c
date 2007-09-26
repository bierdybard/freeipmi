#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <assert.h>

#include "bmc-config.h"
#include "bmc-config-wrapper.h"
#include "bmc-config-validate.h"
#include "bmc-config-utils.h"

/* convenience struct */
struct bmc_authentication_level {
  uint8_t callback_level_none;
  uint8_t callback_level_md2;
  uint8_t callback_level_md5;
  uint8_t callback_level_straight_password;
  uint8_t callback_level_oem_proprietary;
  uint8_t user_level_none;
  uint8_t user_level_md2;
  uint8_t user_level_md5;
  uint8_t user_level_straight_password;
  uint8_t user_level_oem_proprietary;
  uint8_t operator_level_none;
  uint8_t operator_level_md2;
  uint8_t operator_level_md5;
  uint8_t operator_level_straight_password;
  uint8_t operator_level_oem_proprietary;
  uint8_t admin_level_none;
  uint8_t admin_level_md2;
  uint8_t admin_level_md5;
  uint8_t admin_level_straight_password;
  uint8_t admin_level_oem_proprietary;
  uint8_t oem_level_none;
  uint8_t oem_level_md2;
  uint8_t oem_level_md5;
  uint8_t oem_level_straight_password;
  uint8_t oem_level_oem_proprietary;
};

static config_err_t 
_get_authentication_type_enables (bmc_config_state_data_t *state_data,
                                  struct bmc_authentication_level *al)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  config_err_t rv = CONFIG_ERR_FATAL_ERROR;
  config_err_t ret;
  uint8_t channel_number;
  
  assert(state_data);
  assert(al);

  if (!(obj_cmd_rs = Fiid_obj_create(tmpl_cmd_get_lan_configuration_parameters_authentication_type_enables_rs)))
    goto cleanup;
  
  if ((ret = get_lan_channel_number (state_data, &channel_number)) != CONFIG_ERR_SUCCESS)
    {
      rv = ret;
      goto cleanup;
    }

  if (ipmi_cmd_get_lan_configuration_parameters_authentication_type_enables (state_data->dev, 
									     channel_number, 
									     IPMI_GET_LAN_PARAMETER, 
									     SET_SELECTOR, 
									     BLOCK_SELECTOR, 
									     obj_cmd_rs) < 0)
    {
      if (state_data->prog_data->args->common.flags & IPMI_FLAGS_DEBUG_DUMP)
        fprintf(stderr,
                "ipmi_cmd_get_lan_configuration_parameters_authentication_type_enables: %s\n",
                ipmi_device_strerror(ipmi_device_errnum(state_data->dev)));
      rv = CONFIG_ERR_NON_FATAL_ERROR;
      goto cleanup;
    }
  
  if (Fiid_obj_get (obj_cmd_rs, "callback_level.none", &val) < 0)
    goto cleanup;
  al->callback_level_none = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "callback_level.md2", &val) < 0)
    goto cleanup;
  al->callback_level_md2 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "callback_level.md5", &val) < 0)
    goto cleanup;
  al->callback_level_md5 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "callback_level.straight_password", &val) < 0)
    goto cleanup;
  al->callback_level_straight_password = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "callback_level.oem_proprietary", &val) < 0)
    goto cleanup;
  al->callback_level_oem_proprietary = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "user_level.none", &val) < 0)
    goto cleanup;
  al->user_level_none = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "user_level.md2", &val) < 0)
    goto cleanup;
  al->user_level_md2 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "user_level.md5", &val) < 0)
    goto cleanup;
  al->user_level_md5 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "user_level.straight_password", &val) < 0)
    goto cleanup;
  al->user_level_straight_password = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "user_level.oem_proprietary", &val) < 0)
    goto cleanup;
  al->user_level_oem_proprietary = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "operator_level.none", &val) < 0)
    goto cleanup;
  al->operator_level_none = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "operator_level.md2", &val) < 0)
    goto cleanup;
  al->operator_level_md2 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "operator_level.md5", &val) < 0)
    goto cleanup;
  al->operator_level_md5 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "operator_level.straight_password", &val) < 0)
    goto cleanup;
  al->operator_level_straight_password = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "operator_level.oem_proprietary", &val) < 0)
    goto cleanup;
  al->operator_level_oem_proprietary = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "admin_level.none", &val) < 0)
    goto cleanup;
  al->admin_level_none = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "admin_level.md2", &val) < 0)
    goto cleanup;
  al->admin_level_md2 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "admin_level.md5", &val) < 0)
    goto cleanup;
  al->admin_level_md5 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "admin_level.straight_password", &val) < 0)
    goto cleanup;
  al->admin_level_straight_password = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "admin_level.oem_proprietary", &val) < 0)
    goto cleanup;
  al->admin_level_oem_proprietary = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "oem_level.none", &val) < 0)
    goto cleanup;
  al->oem_level_none = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "oem_level.md2", &val) < 0)
    goto cleanup;
  al->oem_level_md2 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "oem_level.md5", &val) < 0)
    goto cleanup;
  al->oem_level_md5 = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "oem_level.straight_password", &val) < 0)
    goto cleanup;
  al->oem_level_straight_password = val;
  
  if (Fiid_obj_get (obj_cmd_rs, "oem_level.oem_proprietary", &val) < 0)
    goto cleanup;
  al->oem_level_oem_proprietary = val;
  
  rv = CONFIG_ERR_SUCCESS;
 cleanup:
  Fiid_obj_destroy(obj_cmd_rs);
  return (rv);
}

config_err_t 
_set_authentication_type_enables (bmc_config_state_data_t *state_data,
                                  struct bmc_authentication_level *al)
{
  fiid_obj_t obj_cmd_rs = NULL;
  config_err_t rv = CONFIG_ERR_FATAL_ERROR;
  config_err_t ret;
  uint8_t channel_number;

  assert(state_data);
  assert(al);

  if (!(obj_cmd_rs = Fiid_obj_create(tmpl_cmd_set_lan_configuration_parameters_rs)))
    goto cleanup;

  if ((ret = get_lan_channel_number (state_data, &channel_number)) != CONFIG_ERR_SUCCESS)
    {
      rv = ret;
      goto cleanup;
    }

  if (ipmi_cmd_set_lan_configuration_parameters_authentication_type_enables (state_data->dev,
                                                                             channel_number, 
                                                                             al->callback_level_none,
                                                                             al->callback_level_md2,
                                                                             al->callback_level_md5,
                                                                             al->callback_level_straight_password,
                                                                             al->callback_level_oem_proprietary,
                                                                             al->user_level_none,
                                                                             al->user_level_md2,
                                                                             al->user_level_md5,
                                                                             al->user_level_straight_password,
                                                                             al->user_level_oem_proprietary,
                                                                             al->operator_level_none,
                                                                             al->operator_level_md2,
                                                                             al->operator_level_md5,
                                                                             al->operator_level_straight_password,
                                                                             al->operator_level_oem_proprietary,
                                                                             al->admin_level_none,
                                                                             al->admin_level_md2,
                                                                             al->admin_level_md5,
                                                                             al->admin_level_straight_password,
                                                                             al->admin_level_oem_proprietary,
                                                                             al->oem_level_none,
                                                                             al->oem_level_md2,
                                                                             al->oem_level_md5,
                                                                             al->oem_level_straight_password,
                                                                             al->oem_level_oem_proprietary,
                                                                             obj_cmd_rs) < 0)
    {
      if (state_data->prog_data->args->common.flags & IPMI_FLAGS_DEBUG_DUMP)
        fprintf(stderr,
                "ipmi_cmd_set_lan_configuration_parameters_authentication_type_enables: %s\n",
                ipmi_device_strerror(ipmi_device_errnum(state_data->dev)));
      rv = CONFIG_ERR_NON_FATAL_ERROR;
      goto cleanup;
    }
  
  rv = CONFIG_ERR_SUCCESS;
 cleanup:
  Fiid_obj_destroy(obj_cmd_rs);
  return (rv);
}

static uint8_t *
_authentication_level_ptr (const char *section_name,
                           const char *key_name,
                           struct bmc_authentication_level *al)
{
  assert(key_name);
  assert(al);

  if (!strcasecmp(key_name, "Callback_Enable_Auth_Type_None"))
    return &al->callback_level_none;
  else if (!strcasecmp(key_name, "Callback_Enable_Auth_Type_MD2"))
    return &al->callback_level_md2;
  else if (!strcasecmp(key_name, "Callback_Enable_Auth_Type_MD5"))
    return &al->callback_level_md5;
  else if (!strcasecmp(key_name, "Callback_Enable_Auth_Type_Straight_Password"))
    return &al->callback_level_straight_password;
  else if (!strcasecmp(key_name, "Callback_Enable_Auth_Type_OEM_Proprietary"))
    return &al->callback_level_oem_proprietary;
  else if (!strcasecmp(key_name, "User_Enable_Auth_Type_None"))
    return &al->user_level_none;
  else if (!strcasecmp(key_name, "User_Enable_Auth_Type_MD2"))
    return &al->user_level_md2;
  else if (!strcasecmp(key_name, "User_Enable_Auth_Type_MD5"))
    return &al->user_level_md5;
  else if (!strcasecmp(key_name, "User_Enable_Auth_Type_Straight_Password"))
    return &al->user_level_straight_password;
  else if (!strcasecmp(key_name, "User_Enable_Auth_Type_OEM_Proprietary"))
    return &al->user_level_oem_proprietary;
  else if (!strcasecmp(key_name, "Operator_Enable_Auth_Type_None"))
    return &al->operator_level_none;
  else if (!strcasecmp(key_name, "Operator_Enable_Auth_Type_MD2"))
    return &al->operator_level_md2;
  else if (!strcasecmp(key_name, "Operator_Enable_Auth_Type_MD5"))
    return &al->operator_level_md5;
  else if (!strcasecmp(key_name, "Operator_Enable_Auth_Type_Straight_Password"))
    return &al->operator_level_straight_password;
  else if (!strcasecmp(key_name, "Operator_Enable_Auth_Type_OEM_Proprietary"))
    return &al->operator_level_oem_proprietary;
  else if (!strcasecmp(key_name, "Admin_Enable_Auth_Type_None"))
    return &al->admin_level_none;
  else if (!strcasecmp(key_name, "Admin_Enable_Auth_Type_MD2"))
    return &al->admin_level_md2;
  else if (!strcasecmp(key_name, "Admin_Enable_Auth_Type_MD5"))
    return &al->admin_level_md5;
  else if (!strcasecmp(key_name, "Admin_Enable_Auth_Type_Straight_Password"))
    return &al->admin_level_straight_password;
  else if (!strcasecmp(key_name, "Admin_Enable_Auth_Type_OEM_Proprietary"))
    return &al->admin_level_oem_proprietary;
  else if (!strcasecmp(key_name, "OEM_Enable_Auth_Type_None"))
    return &al->oem_level_none;
  else if (!strcasecmp(key_name, "OEM_Enable_Auth_Type_MD2"))
    return &al->oem_level_md2;
  else if (!strcasecmp(key_name, "OEM_Enable_Auth_Type_MD5"))
    return &al->oem_level_md5;
  else if (!strcasecmp(key_name, "OEM_Enable_Auth_Type_Straight_Password"))
    return &al->oem_level_straight_password;
  else if (!strcasecmp(key_name, "OEM_Enable_Auth_Type_OEM_Proprietary"))
    return &al->oem_level_oem_proprietary;

  fprintf(stderr, 
          "Unknown key '%s' in section '%s'\n", 
          key_name,
          section_name);
  return NULL;
}

static config_err_t
_authentication_level_checkout (const char *section_name,
                                struct config_keyvalue *kv,
                                void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  struct bmc_authentication_level al;
  config_err_t ret;
  uint8_t *flag;

  if ((ret = _get_authentication_type_enables (state_data, 
                                               &al)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (!(flag = _authentication_level_ptr(section_name, kv->key->key_name, &al)))
    return CONFIG_ERR_FATAL_ERROR;

  if (config_section_update_keyvalue_output(kv, *flag ? "Yes" : "No") < 0)
    return CONFIG_ERR_FATAL_ERROR;
  
  return CONFIG_ERR_SUCCESS;
}

static config_err_t
_authentication_level_commit (const char *section_name,
                              const struct config_keyvalue *kv,
                              void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  struct bmc_authentication_level al;
  config_err_t ret;
  uint8_t *flag;

  if ((ret = _get_authentication_type_enables (state_data, 
                                               &al)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (!(flag = _authentication_level_ptr(section_name, kv->key->key_name, &al)))
    return CONFIG_ERR_FATAL_ERROR;
  
  *flag = same (kv->value_input, "yes");

  if ((ret = _set_authentication_type_enables (state_data, 
                                               &al)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

struct config_section *
bmc_config_lan_conf_auth_section_get (bmc_config_state_data_t *state_data)
{
  struct config_section *lan_conf_auth_section = NULL;
  char *section_comment =
    "In the Lan_Conf_Auth section, allowable authentication mechanisms for "
    "IPMI 1.5 is configured.  Most users will want to set all \"MD5\" "
    "authentication to \"Yes\" and the rest to \"No\".  If you have "
    "configured a NULL username and a NULL password, you "
    "will also want to configure some of the \"None\" fields to \"Yes\" "
    "to allow \"None\" authentication to work.";

  if (!(lan_conf_auth_section = config_section_create("Lan_Conf_Auth",
                                                      "Lan_Conf_Auth",
                                                      section_comment,
                                                      0)))
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Callback_Enable_Auth_Type_None",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Callback_Enable_Auth_Type_MD2",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Callback_Enable_Auth_Type_MD5",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Callback_Enable_Auth_Type_Straight_Password",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Callback_Enable_Auth_Type_OEM_Proprietary",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "User_Enable_Auth_Type_None",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "User_Enable_Auth_Type_MD2",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "User_Enable_Auth_Type_MD5",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "User_Enable_Auth_Type_Straight_Password",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "User_Enable_Auth_Type_OEM_Proprietary",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Operator_Enable_Auth_Type_None",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Operator_Enable_Auth_Type_MD2",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Operator_Enable_Auth_Type_MD5",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Operator_Enable_Auth_Type_Straight_Password",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Operator_Enable_Auth_Type_OEM_Proprietary",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Admin_Enable_Auth_Type_None",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Admin_Enable_Auth_Type_MD2",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Admin_Enable_Auth_Type_MD5",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Admin_Enable_Auth_Type_Straight_Password",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "Admin_Enable_Auth_Type_OEM_Proprietary",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "OEM_Enable_Auth_Type_None",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "OEM_Enable_Auth_Type_MD2",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "OEM_Enable_Auth_Type_MD5",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "OEM_Enable_Auth_Type_Straight_Password",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (lan_conf_auth_section,
                              "OEM_Enable_Auth_Type_OEM_Proprietary",
                              "Possible values: Yes/No",
                              0,
                              _authentication_level_checkout,
                              _authentication_level_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  return lan_conf_auth_section;

 cleanup:
  if (lan_conf_auth_section)
    config_section_destroy(lan_conf_auth_section);
  return NULL;
}
