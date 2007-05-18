#include "bmc-config.h"
#include "bmc-config-common.h"
#include "bmc-config-wrapper.h"
#include "bmc-config-diff.h"
#include "bmc-config-map.h"
#include "bmc-config-sections.h"
#include "bmc-config-validate.h"

/* volatile */

static bmc_err_t
lan_channel_volatile_access_set (bmc_config_state_data_t *state_data,
				 uint8_t access_mode,
				 uint8_t access_mode_is_set,
				 uint8_t user_level_authentication,
				 uint8_t user_level_authentication_is_set,
				 uint8_t per_message_authentication,
				 uint8_t per_message_authentication_is_set,
				 uint8_t pef_alerting,
				 uint8_t pef_alerting_is_set,
				 uint8_t channel_privilege_limit,
				 uint8_t channel_privilege_limit_is_set)
{
  uint8_t tmp_access_mode;
  uint8_t tmp_user_level_authentication;
  uint8_t tmp_per_message_authentication;
  uint8_t tmp_pef_alerting;
  uint8_t tmp_channel_privilege_limit;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_volatile_access (state_data,
                                                  &tmp_access_mode,
                                                  &tmp_user_level_authentication,
                                                  &tmp_per_message_authentication,
                                                  &tmp_pef_alerting,
                                                  &tmp_channel_privilege_limit)) != BMC_ERR_SUCCESS)
    return ret;

  if (access_mode_is_set)
    tmp_access_mode = access_mode;
  if (user_level_authentication_is_set)
    tmp_user_level_authentication = user_level_authentication;
  if (per_message_authentication_is_set)
    tmp_per_message_authentication = per_message_authentication;
  if (pef_alerting_is_set)
    tmp_pef_alerting = pef_alerting;
  if (channel_privilege_limit_is_set)
    tmp_channel_privilege_limit = channel_privilege_limit;

  if ((ret = set_bmc_lan_channel_volatile_access (state_data,
                                                  tmp_access_mode,
                                                  tmp_user_level_authentication,
                                                  tmp_per_message_authentication,
                                                  tmp_pef_alerting,
                                                  tmp_channel_privilege_limit)) != BMC_ERR_SUCCESS)
    return ret;

  return BMC_ERR_SUCCESS;
}


/* access_mode */

static bmc_err_t
volatile_access_mode_checkout (bmc_config_state_data_t *state_data,
			       const struct section *sect,
			       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;

  if ((ret = get_bmc_lan_channel_volatile_access (state_data,
                                                  &get_val,
                                                  &foo,
                                                  &foo,
                                                  &foo,
                                                  &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (channel_access_mode_string (get_val))))
    {
      perror("strdup");
      return BMC_ERR_FATAL_ERROR ;
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
volatile_access_mode_commit (bmc_config_state_data_t *state_data,
			     const struct section *sect,
			     const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = channel_access_mode (kv->value);
  return lan_channel_volatile_access_set (state_data,
					  commit_val, 1,
					  0, 0,
					  0, 0,
					  0, 0,
					  0, 0);
}

static bmc_diff_t
volatile_access_mode_diff (bmc_config_state_data_t *state_data,
			   const struct section *sect,
			   const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_volatile_access (state_data,
                                                 &get_val,
                                                 &foo,
                                                 &foo,
                                                 &foo,
                                                 &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_val = channel_access_mode (kv->value);

  if (passed_val == get_val)
    ret = BMC_DIFF_SAME; 
  else
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name, 
                   kv->key,
                   kv->value,
                   channel_access_mode_string (get_val));
    }

  return ret;
}

/* enable_user_level_auth */

static bmc_err_t
volatile_enable_user_level_auth_checkout (bmc_config_state_data_t *state_data,
                                          const struct section *sect,
                                          struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_volatile_access (state_data,
                                                  &foo,
                                                  &get_val,
                                                  &foo,
                                                  &foo,
                                                  &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  if (get_val)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
volatile_enable_user_level_auth_commit (bmc_config_state_data_t *state_data,
					const struct section *sect,
					const struct keyvalue *kv)
{
  uint8_t commit_val;

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  commit_val = same (kv->value, "yes");
  return lan_channel_volatile_access_set (state_data,
					  0, 0,
					  commit_val, 1,
					  0, 0,
					  0, 0,
					  0, 0);
}

static bmc_diff_t
volatile_enable_user_level_auth_diff (bmc_config_state_data_t *state_data,
				      const struct section *sect,
				      const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_volatile_access (state_data,
                                                 &foo,
                                                 &get_val,
                                                 &foo,
                                                 &foo,
                                                 &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  passed_val = same (kv->value, "yes");
  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }
  return ret;
}

/* enable_per_message_auth */

static bmc_err_t
volatile_enable_per_msg_auth_checkout (bmc_config_state_data_t *state_data,
				       const struct section *sect,
				       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;

  if ((ret = get_bmc_lan_channel_volatile_access (state_data,
                                                  &foo,
                                                  &foo,
                                                  &get_val,
                                                  &foo,
                                                  &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  if (get_val)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
volatile_enable_per_msg_auth_commit (bmc_config_state_data_t *state_data,
				     const struct section *sect,
				     const struct keyvalue *kv)
{
  uint8_t commit_val;

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  commit_val = same (kv->value, "yes");
  return lan_channel_volatile_access_set (state_data,
					  0, 0,
					  0, 0,
					  commit_val, 1,
					  0, 0,
					  0, 0);
}

static bmc_diff_t
volatile_enable_per_msg_auth_diff (bmc_config_state_data_t *state_data,
				   const struct section *sect,
				   const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_volatile_access (state_data,
                                                 &foo,
                                                 &foo,
                                                 &get_val,
                                                 &foo,
                                                 &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  passed_val = same (kv->value, "yes");
  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }

  return ret;
}

/* enable_pef_alerting */

static bmc_err_t
volatile_enable_pef_alerting_checkout (bmc_config_state_data_t *state_data,
				       const struct section *sect,
				       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_volatile_access (state_data,
                                                  &foo,
                                                  &foo,
                                                  &foo,
                                                  &get_val,
                                                  &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  if (get_val)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
volatile_enable_pef_alerting_commit (bmc_config_state_data_t *state_data,
				     const struct section *sect,
				     const struct keyvalue *kv)
{
  uint8_t commit_val;

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  commit_val = same (kv->value, "yes");
  return lan_channel_volatile_access_set (state_data,
					  0, 0,
					  0, 0,
					  0, 0,
					  commit_val, 1,
					  0, 0);
}

static bmc_diff_t
volatile_enable_pef_alerting_diff (bmc_config_state_data_t *state_data,
				   const struct section *sect,
				   const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_volatile_access (state_data,
                                                 &foo,
                                                 &foo,
                                                 &foo,
                                                 &get_val,
                                                 &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  /* achu: Backwards values in this command are handled in bmc-config-api.c */
  passed_val = same (kv->value, "yes");
  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }

  return ret;
}

/* channel_privilege_level */

static bmc_err_t
volatile_channel_priv_limit_checkout (bmc_config_state_data_t *state_data,
				      const struct section *sect,
				      struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_volatile_access (state_data,
                                                  &foo,
                                                  &foo,
                                                  &foo,
                                                  &foo,
                                                  &get_val)) != BMC_ERR_SUCCESS)
    return ret; 

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (privilege_level_string (get_val))))
    {
      perror("strdup");
      return BMC_ERR_FATAL_ERROR;
    }

  return BMC_ERR_SUCCESS;
}

static bmc_err_t
volatile_channel_priv_limit_commit (bmc_config_state_data_t *state_data,
				    const struct section *sect,
				    const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = privilege_level_number (kv->value);
  return lan_channel_volatile_access_set (state_data,
					  0, 0,
					  0, 0,
					  0, 0,
					  0, 0,
					  commit_val, 1);
}

static bmc_diff_t
volatile_channel_priv_limit_diff (bmc_config_state_data_t *state_data,
				  const struct section *sect,
				  const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_volatile_access (state_data,
                                                 &foo,
                                                 &foo,
                                                 &foo,
                                                 &foo,
                                                 &get_val)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_val = privilege_level_number (kv->value);

  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   privilege_level_string (get_val));
    }

  return ret;
}

/* non volatile */

static int
lan_channel_non_volatile_access_set (bmc_config_state_data_t *state_data,
                                     uint8_t access_mode,
                                     uint8_t access_mode_is_set,
                                     uint8_t user_level_authentication,
                                     uint8_t user_level_authentication_is_set,
                                     uint8_t per_message_authentication,
                                     uint8_t per_message_authentication_is_set,
                                     uint8_t pef_alerting,
                                     uint8_t pef_alerting_is_set,
                                     uint8_t channel_privilege_limit,
                                     uint8_t channel_privilege_limit_is_set)
{
  uint8_t tmp_access_mode;
  uint8_t tmp_user_level_authentication;
  uint8_t tmp_per_message_authentication;
  uint8_t tmp_pef_alerting;
  uint8_t tmp_channel_privilege_limit;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_non_volatile_access (state_data,
                                                      &tmp_access_mode,
                                                      &tmp_user_level_authentication,
                                                      &tmp_per_message_authentication,
                                                      &tmp_pef_alerting,
                                                      &tmp_channel_privilege_limit)) != BMC_ERR_SUCCESS)
    return ret;

  if (access_mode_is_set)
    tmp_access_mode = access_mode;
  if (user_level_authentication_is_set)
    tmp_user_level_authentication = user_level_authentication;
  if (per_message_authentication_is_set)
    tmp_per_message_authentication = per_message_authentication;
  if (pef_alerting_is_set)
    tmp_pef_alerting = pef_alerting;
  if (channel_privilege_limit_is_set)
    tmp_channel_privilege_limit = channel_privilege_limit;

  if ((ret = set_bmc_lan_channel_non_volatile_access (state_data,
                                                      tmp_access_mode,
                                                      tmp_user_level_authentication,
                                                      tmp_per_message_authentication,
                                                      tmp_pef_alerting,
                                                      tmp_channel_privilege_limit)) != BMC_ERR_SUCCESS)
    return ret;

  return BMC_ERR_SUCCESS;
}


/* access_mode */

static bmc_err_t
non_volatile_access_mode_checkout (bmc_config_state_data_t *state_data,
                                   const struct section *sect,
                                   struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;

  if ((ret = get_bmc_lan_channel_non_volatile_access (state_data,
                                                      &get_val,
                                                      &foo,
                                                      &foo,
                                                      &foo,
                                                      &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  if (!(kv->value = strdup (channel_access_mode_string (get_val))))
    {
      perror("strdup");
      return BMC_ERR_FATAL_ERROR ;
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
non_volatile_access_mode_commit (bmc_config_state_data_t *state_data,
				 const struct section *sect,
				 const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = channel_access_mode (kv->value);
  return lan_channel_non_volatile_access_set (state_data,
                                              commit_val, 1,
                                              0, 0,
                                              0, 0,
                                              0, 0,
                                              0, 0);
}

static bmc_diff_t
non_volatile_access_mode_diff (bmc_config_state_data_t *state_data,
			       const struct section *sect,
			       const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_non_volatile_access (state_data,
                                                     &get_val,
                                                     &foo,
                                                     &foo,
                                                     &foo,
                                                     &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_val = channel_access_mode (kv->value);
  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }

  return ret;
}

/* enable_user_level_auth */

static bmc_err_t
non_volatile_enable_user_level_auth_checkout (bmc_config_state_data_t *state_data,
					      const struct section *sect,
					      struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;

  if ((ret = get_bmc_lan_channel_non_volatile_access (state_data,
                                                      &foo,
                                                      &get_val,
                                                      &foo,
                                                      &foo,
                                                      &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (get_val)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
non_volatile_enable_user_level_auth_commit (bmc_config_state_data_t *state_data,
					    const struct section *sect,
					    const struct keyvalue *kv)
{
  uint8_t commit_val;
  
  commit_val = same (kv->value, "yes");
  return lan_channel_non_volatile_access_set (state_data,
					      0, 0,
					      commit_val, 1,
					      0, 0,
					      0, 0,
					      0, 0);
}

static bmc_diff_t
non_volatile_enable_user_level_auth_diff (bmc_config_state_data_t *state_data,
                                          const struct section *sect,
                                          const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_non_volatile_access (state_data,
                                                     &foo,
                                                     &get_val,
                                                     &foo,
                                                     &foo,
                                                     &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "yes");
  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }

  return ret;
}

/* enable_per_message_auth */

static bmc_err_t
non_volatile_enable_per_msg_auth_checkout (bmc_config_state_data_t *state_data,
					   const struct section *sect,
					   struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_non_volatile_access (state_data,
                                                      &foo,
                                                      &foo,
                                                      &get_val,
                                                      &foo,
                                                      &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (get_val)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
non_volatile_enable_per_msg_auth_commit (bmc_config_state_data_t *state_data,
					 const struct section *sect,
					 const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = same (kv->value, "yes");
  return lan_channel_non_volatile_access_set (state_data,
					      0, 0,
					      0, 0,
					      commit_val, 1,
					      0, 0,
					      0, 0);
}

static bmc_diff_t
non_volatile_enable_per_msg_auth_diff (bmc_config_state_data_t *state_data,
				       const struct section *sect,
				       const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_non_volatile_access (state_data,
                                                     &foo,
                                                     &foo,
                                                     &get_val,
                                                     &foo,
                                                     &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "yes");

  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }

  return ret;
}

/* enable_pef_alerting */

static bmc_err_t
non_volatile_enable_pef_alerting_checkout (bmc_config_state_data_t *state_data,
					   const struct section *sect,
					   struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_non_volatile_access (state_data,
                                                      &foo,
                                                      &foo,
                                                      &foo,
                                                      &get_val,
                                                      &foo)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (get_val)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR ;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
non_volatile_enable_pef_alerting_commit (bmc_config_state_data_t *state_data,
					 const struct section *sect,
					 const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = same (kv->value, "yes");
  return lan_channel_non_volatile_access_set (state_data,
					      0, 0,
					      0, 0,
					      0, 0,
					      commit_val, 1,
					      0, 0);
}

static bmc_diff_t
non_volatile_enable_pef_alerting_diff (bmc_config_state_data_t *state_data,
				       const struct section *sect,
				       const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_non_volatile_access (state_data,
                                                     &foo,
                                                     &foo,
                                                     &foo,
                                                     &get_val,
                                                     &foo)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "yes");

  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }

  return ret;
}

/* channel_privilege_level */

static bmc_err_t
non_volatile_channel_priv_limit_checkout (bmc_config_state_data_t *state_data,
					  const struct section *sect,
					  struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_channel_non_volatile_access (state_data,
                                                      &foo,
                                                      &foo,
                                                      &foo,
                                                      &foo,
                                                      &get_val)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (privilege_level_string (get_val))))
    {
      perror("strdup");
      return BMC_ERR_FATAL_ERROR ;
    }

  return BMC_ERR_SUCCESS;
}

static bmc_err_t
non_volatile_channel_priv_limit_commit (bmc_config_state_data_t *state_data,
					const struct section *sect,
					const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = privilege_level_number (kv->value);
  return lan_channel_non_volatile_access_set (state_data,
					      0, 0,
					      0, 0,
					      0, 0,
					      0, 0,
					      commit_val, 1);
}

static bmc_diff_t
non_volatile_channel_priv_limit_diff (bmc_config_state_data_t *state_data,
				      const struct section *sect,
				      const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_channel_non_volatile_access (state_data,
                                                     &foo,
                                                     &foo,
                                                     &foo,
                                                     &foo,
                                                     &get_val)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_val = privilege_level_number (kv->value);
  
  if (passed_val == get_val)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   privilege_level_string (get_val));
    }
  
  return ret;
}

struct section *
bmc_lan_channel_section_get (bmc_config_state_data_t *state_data)
{
  struct section * lan_channel_section = NULL;

  if (!(lan_channel_section = bmc_config_section_create (state_data, "Lan_Channel")))
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Volatile_Access_Mode",
                                       "Possible values: Disabled/Pre_Boot_Only/Always_Available/Shared",
                                       0,
                                       volatile_access_mode_checkout,
                                       volatile_access_mode_commit,
                                       volatile_access_mode_diff,
                                       channel_access_mode_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Volatile_Enable_User_Level_Auth",
                                       "Possible values: Yes/No",
                                       0,
                                       volatile_enable_user_level_auth_checkout,
                                       volatile_enable_user_level_auth_commit,
                                       volatile_enable_user_level_auth_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Volatile_Enable_Per_Message_Auth",
                                       "Possible values: Yes/No",
                                       0,
                                       volatile_enable_per_msg_auth_checkout,
                                       volatile_enable_per_msg_auth_commit,
                                       volatile_enable_per_msg_auth_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Volatile_Enable_Pef_Alerting",
                                       "Possible values: Yes/No",
                                       0,
                                       volatile_enable_pef_alerting_checkout,
                                       volatile_enable_pef_alerting_commit,
                                       volatile_enable_pef_alerting_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Volatile_Channel_Privilege_Limit",
                                       "Possible values: Callback/User/Operator/Administrator/OEM_Proprietary",
                                       0,
                                       volatile_channel_priv_limit_checkout,
                                       volatile_channel_priv_limit_commit,
                                       volatile_channel_priv_limit_diff,
                                       privilege_level_number_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Non_Volatile_Access_Mode",
                                       "Possible values: Disabled/Pre_Boot_Only/Always_Available/Shared",
                                       0,
                                       non_volatile_access_mode_checkout,
                                       non_volatile_access_mode_commit,
                                       non_volatile_access_mode_diff,
                                       channel_access_mode_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Non_Volatile_Enable_User_Level_Auth",
                                       "Possible values: Yes/No",
                                       0,
                                       non_volatile_enable_user_level_auth_checkout,
                                       non_volatile_enable_user_level_auth_commit,
                                       non_volatile_enable_user_level_auth_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Non_Volatile_Enable_Per_Message_Auth",
                                       "Possible values: Yes/No",
                                       0,
                                       non_volatile_enable_per_msg_auth_checkout,
                                       non_volatile_enable_per_msg_auth_commit,
                                       non_volatile_enable_per_msg_auth_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Non_Volatile_Enable_Pef_Alerting",
                                       "Possible values: Yes/No",
                                       0,
                                       non_volatile_enable_pef_alerting_checkout,
                                       non_volatile_enable_pef_alerting_commit,
                                       non_volatile_enable_pef_alerting_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_channel_section,
                                       "Non_Volatile_Channel_Privilege_Limit",
                                       "Possible values: Callback/User/Operator/Administrator/OEM_Proprietary",
                                       0,
                                       non_volatile_channel_priv_limit_checkout,
                                       non_volatile_channel_priv_limit_commit,
                                       non_volatile_channel_priv_limit_diff,
                                       privilege_level_number_validate) < 0)
    goto cleanup;

  return lan_channel_section;

 cleanup:
  if (lan_channel_section)
    bmc_config_section_destroy(state_data, lan_channel_section);
  return NULL;
}

