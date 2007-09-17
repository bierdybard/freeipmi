#include "bmc-config.h"
#include "bmc-config-common.h"
#include "bmc-config-wrapper.h"
#include "bmc-config-diff.h"
#include "bmc-config-map.h"
#include "bmc-config-sections.h"
#include "bmc-config-validate.h"

static bmc_err_t
enable_gratuitous_arps_checkout (bmc_config_state_data_t *state_data,
				 const struct section *sect,
				 struct keyvalue *kv)
{
  uint8_t enable_arp;
  uint8_t reply_arp;
  bmc_err_t ret;

  ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
						    &enable_arp,
						    &reply_arp);
  if (ret != 0)
    return -1;

  if (kv->value)
    free (kv->value);
  
  if (enable_arp)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return -1;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return -1;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
enable_gratuitous_arps_commit (bmc_config_state_data_t *state_data,
			       const struct section *sect,
			       const struct keyvalue *kv)
{
  int ret;
  uint8_t enable_arp;
  uint8_t reply_arp;
  ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
						    &enable_arp,
						    &reply_arp);
  if (ret != 0)
    return -1;

  enable_arp = same (kv->value, "yes");

  return set_bmc_lan_conf_bmc_generated_arp_control (state_data,
						     enable_arp,
						     reply_arp);
}

static bmc_diff_t
enable_gratuitous_arps_diff (bmc_config_state_data_t *state_data,
			     const struct section *sect,
			     const struct keyvalue *kv)
{
  uint8_t enable_arp;
  uint8_t reply_arp;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                        &enable_arp,
                                                        &reply_arp)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  if (enable_arp == same (kv->value, "yes"))
    ret = BMC_DIFF_SAME;
  else
    {
      ret = BMC_DIFF_DIFFERENT; 
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   (enable_arp) ? "Yes" : "No");
    }
  return ret;
}

/* reply */

static bmc_err_t
enable_arp_response_checkout (bmc_config_state_data_t *state_data,
			      const struct section *sect,
			      struct keyvalue *kv)
{
  uint8_t enable_arp;
  uint8_t reply_arp;
  bmc_err_t ret;

  if ((ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                         &enable_arp,
                                                         &reply_arp)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (reply_arp)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return BMC_ERR_FATAL_ERROR;
        }
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
enable_arp_response_commit (bmc_config_state_data_t *state_data,
			    const struct section *sect,
			    const struct keyvalue *kv)
{
  uint8_t enable_arp;
  uint8_t reply_arp;
  bmc_err_t ret;
  
  if ((ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                         &enable_arp,
                                                         &reply_arp)) != BMC_ERR_SUCCESS)
    return ret;

  reply_arp = same (kv->value, "yes");

  return set_bmc_lan_conf_bmc_generated_arp_control (state_data,
						     enable_arp,
						     reply_arp);
}

static bmc_diff_t
enable_arp_response_diff (bmc_config_state_data_t *state_data,
			  const struct section *sect,
			  const struct keyvalue *kv)
{
  uint8_t enable_arp;
  uint8_t reply_arp;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                        &enable_arp,
                                                        &reply_arp)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  if (reply_arp == same (kv->value, "yes"))
    ret = BMC_DIFF_SAME;
  else
    {
      ret = BMC_DIFF_DIFFERENT; 
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   (reply_arp) ? "Yes" : "No");
    }

  return ret;
}

static bmc_err_t
gratuitous_arp_interval_checkout (bmc_config_state_data_t *state_data,
				  const struct section *sect,
				  struct keyvalue *kv)
{
  uint8_t interval;
  bmc_err_t ret;

  if ((ret = get_bmc_lan_conf_gratuitous_arp_interval (state_data,
                                                       &interval)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "%d", interval) < 0)
    {
      perror("asprintf");
      return BMC_ERR_FATAL_ERROR;
    }
  return BMC_ERR_SUCCESS;
}

static bmc_err_t
gratuitous_arp_interval_commit (bmc_config_state_data_t *state_data,
				const struct section *sect,
				const struct keyvalue *kv)
{
  return set_bmc_lan_conf_gratuitous_arp_interval (state_data,
                                                   atoi (kv->value));
}

static bmc_diff_t
gratuitous_arp_interval_diff (bmc_config_state_data_t *state_data,
			      const struct section *sect,
			      const struct keyvalue *kv)
{
  uint8_t interval;
  bmc_err_t rc;
  bmc_diff_t ret;
  
  if ((rc = get_bmc_lan_conf_gratuitous_arp_interval (state_data,
                                                       &interval)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  if (interval == atoi (kv->value))
    ret = BMC_DIFF_SAME;
  else
    {
      char num[32];
      ret = BMC_DIFF_DIFFERENT; 
      sprintf (num, "%d", interval);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

struct section *
bmc_lan_conf_misc_section_get (bmc_config_state_data_t *state_data)
{
  struct section *lan_conf_misc_section = NULL;
  char *section_comment = 
    "The following miscellaneous configuration options are optionally "
    "implemented by the vendor.  They may not be available your system and "
    "may not be visible below."
    "\n"
    "If set to \"Yes\", \"Enable_Gratuitous_ARPs\" will inform the BMC to "
    "regularly send out Gratuitous ARPs to allow other machines on a "
    "network resolve the BMC's MAC Address.  Many users will want to set "
    "this to \"Yes\" because it offers the easiest way to support BMC IP "
    "Address resolution.  However, it will increase traffic on your "
    "network.  The \"Gratuitous_ARP_Interval\" can be used to set the "
    "period a Gratuitous ARP is always sent."
    "\n"
    "If set to \"Yes\", \"Enable_ARP_Response\" will inform the BMC to"
    "respond to ARP requests from other machines.";

  if (!(lan_conf_misc_section = bmc_config_section_create (state_data, 
                                                           "Lan_Conf_Misc",
                                                           "Lan_Conf_Misc",
                                                           section_comment,
                                                           0)))
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_conf_misc_section,
                                       "Enable_Gratuitous_ARPs",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_gratuitous_arps_checkout,
                                       enable_gratuitous_arps_commit,
                                       enable_gratuitous_arps_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_conf_misc_section,
                                       "Enable_ARP_Response",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_arp_response_checkout,
                                       enable_arp_response_commit,
                                       enable_arp_response_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_conf_misc_section,
                                       "Gratuitous_ARP_Interval",
                                       "Give a number (x 500ms)",
                                       0,
                                       gratuitous_arp_interval_checkout,
                                       gratuitous_arp_interval_commit,
                                       gratuitous_arp_interval_diff,
                                       number_range_one_byte) < 0)
    goto cleanup;
  return lan_conf_misc_section;

 cleanup:
  if (lan_conf_misc_section)
    bmc_config_section_destroy(state_data, lan_conf_misc_section);
  return NULL;
}
