#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */

#include "pef-config.h"
#include "pef-config-common.h"
#include "pef-config-diff.h"
#include "pef-config-map.h"
#include "pef-config-sections.h"
#include "pef-config-utils.h"
#include "pef-config-validate.h"
#include "pef-config-wrapper.h"

static pef_err_t
alert_policy_get (pef_config_state_data_t *state_data,
                  uint8_t alert_policy_entry_number,
                  uint8_t *policy_type,
                  uint8_t *policy_enabled,
                  uint8_t *policy_number,
                  uint8_t *destination_selector,
                  uint8_t *channel_number,
                  uint8_t *alert_string_set_selector,
                  uint8_t *event_specific_alert_string)
{
  uint8_t tmp_policy_type;
  uint8_t tmp_policy_enabled;
  uint8_t tmp_policy_number;
  uint8_t tmp_destination_selector;
  uint8_t tmp_channel_number;
  uint8_t tmp_alert_string_set_selector;
  uint8_t tmp_event_specific_alert_string;
  pef_err_t ret;

  if ((ret = get_bmc_pef_conf_alert_policy_table (state_data,
                                                  alert_policy_entry_number,
                                                  &tmp_policy_type,
                                                  &tmp_policy_enabled,
                                                  &tmp_policy_number,
                                                  &tmp_destination_selector,
                                                  &tmp_channel_number,
                                                  &tmp_alert_string_set_selector,
                                                  &tmp_event_specific_alert_string)) != PEF_ERR_SUCCESS)
    return ret;

  if (policy_type)
    *policy_type = tmp_policy_type;
  if (policy_enabled)
    *policy_enabled = tmp_policy_enabled;
  if (policy_number)
    *policy_number = tmp_policy_number;
  if (destination_selector)
    *destination_selector = tmp_destination_selector;
  if (channel_number)
    *channel_number = tmp_channel_number;
  if (alert_string_set_selector)
    *alert_string_set_selector = tmp_alert_string_set_selector;
  if (event_specific_alert_string)
    *event_specific_alert_string = tmp_event_specific_alert_string;

  return PEF_ERR_SUCCESS;
}

static pef_err_t
alert_policy_set (pef_config_state_data_t *state_data,
                  uint8_t alert_policy_entry_number,
                  uint8_t policy_type,
                  uint8_t policy_type_is_set,
                  uint8_t policy_enabled,
                  uint8_t policy_enabled_is_set,
                  uint8_t policy_number,
                  uint8_t policy_number_is_set,
                  uint8_t destination_selector,
                  uint8_t destination_selector_is_set,
                  uint8_t channel_number,
                  uint8_t channel_number_is_set,
                  uint8_t alert_string_set_selector,
                  uint8_t alert_string_set_selector_is_set,
                  uint8_t event_specific_alert_string,
                  uint8_t event_specific_alert_string_is_set)
{
  uint8_t tmp_policy_type;
  uint8_t tmp_policy_enabled;
  uint8_t tmp_policy_number;
  uint8_t tmp_destination_selector;
  uint8_t tmp_channel_number;
  uint8_t tmp_alert_string_set_selector;
  uint8_t tmp_event_specific_alert_string;
  pef_err_t ret;

  if ((ret = get_bmc_pef_conf_alert_policy_table (state_data,
                                                  alert_policy_entry_number,
                                                  &tmp_policy_type,
                                                  &tmp_policy_enabled,
                                                  &tmp_policy_number,
                                                  &tmp_destination_selector,
                                                  &tmp_channel_number,
                                                  &tmp_alert_string_set_selector,
                                                  &tmp_event_specific_alert_string)) != PEF_ERR_SUCCESS)
    return ret;

  if (policy_type_is_set)
    tmp_policy_type = policy_type;
  if (policy_enabled_is_set)
    tmp_policy_enabled = policy_enabled;
  if (policy_number_is_set)
    tmp_policy_number = policy_number;
  if (destination_selector_is_set)
    tmp_destination_selector = destination_selector;
  if (channel_number_is_set)
    tmp_channel_number = channel_number;
  if (alert_string_set_selector_is_set)
    tmp_alert_string_set_selector = alert_string_set_selector;
  if (event_specific_alert_string_is_set)
    tmp_event_specific_alert_string = event_specific_alert_string;

  if ((ret = set_bmc_pef_conf_alert_policy_table (state_data,
                                                  destination_selector,
                                                  policy_type,
                                                  policy_enabled,
                                                  policy_number,
                                                  destination_selector,
                                                  channel_number,
                                                  alert_string_set_selector,
                                                  event_specific_alert_string)) != PEF_ERR_SUCCESS)
    return ret;
  
  return PEF_ERR_SUCCESS;
}

static pef_err_t
policy_type_checkout (pef_config_state_data_t *state_data,
                      const struct section *sect,
                      struct keyvalue *kv)
{
  uint8_t policy_type;
  pef_err_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  
  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));
  
  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((ret = alert_policy_get (state_data,
                               alert_policy_entry_number,
                               &policy_type,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL)) != PEF_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (!(kv->value = strdup (policy_type_string (policy_type))))
    {
      perror("strdup");
      return PEF_ERR_FATAL_ERROR;
    }

  return PEF_ERR_SUCCESS;
}

static pef_err_t
policy_type_commit (pef_config_state_data_t *state_data,
                    const struct section *sect,
                    const struct keyvalue *kv)
{
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  pef_err_t ret;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  return alert_policy_set (state_data,
                           alert_policy_entry_number,
                           policy_type_number (kv->value), 1,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0);
}

static pef_diff_t
policy_type_diff (pef_config_state_data_t *state_data,
                  const struct section *sect,
                  const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  pef_err_t rc;
  pef_diff_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((rc = get_number_of_alert_policy_entries (state_data,
                                                &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return rc;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((rc = alert_policy_get (state_data,
                              alert_policy_entry_number,
                              &get_val,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL)) != PEF_ERR_SUCCESS)
    {
      if (rc == PEF_ERR_NON_FATAL_ERROR)
        return PEF_DIFF_NON_FATAL_ERROR;
      return PEF_DIFF_FATAL_ERROR;
    }
  
  passed_val = policy_type_number (kv->value);
  if (passed_val == get_val)
    ret = PEF_DIFF_SAME;
  else
    {
      ret = PEF_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   policy_type_string (get_val));
    }
  return ret;
}

static pef_err_t
policy_enabled_checkout (pef_config_state_data_t *state_data,
                         const struct section *sect,
                         struct keyvalue *kv)
{
  uint8_t policy_enabled;
  pef_err_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  
  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));
  
  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((ret = alert_policy_get (state_data,
                               alert_policy_entry_number,
                               NULL,
                               &policy_enabled,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL)) != PEF_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (policy_enabled)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return PEF_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return PEF_ERR_FATAL_ERROR;
        }
    }

  return PEF_ERR_SUCCESS;
}

static pef_err_t
policy_enabled_commit (pef_config_state_data_t *state_data,
                       const struct section *sect,
                       const struct keyvalue *kv)
{
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  pef_err_t ret;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  return alert_policy_set (state_data,
                           alert_policy_entry_number,
                           0, 0,
                           same (kv->value, "yes"), 1,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0);
}

static pef_diff_t
policy_enabled_diff (pef_config_state_data_t *state_data,
                     const struct section *sect,
                     const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  pef_err_t rc;
  pef_diff_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((rc = get_number_of_alert_policy_entries (state_data,
                                                &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return rc;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((rc = alert_policy_get (state_data,
                              alert_policy_entry_number,
                              NULL,
                              &get_val,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL)) != PEF_ERR_SUCCESS)
    {
      if (rc == PEF_ERR_NON_FATAL_ERROR)
        return PEF_DIFF_NON_FATAL_ERROR;
      return PEF_DIFF_FATAL_ERROR;
    }
  
  passed_val = same (kv->value, "Yes");

  if (passed_val == get_val)
    ret = PEF_DIFF_SAME;
  else
    {
      ret = PEF_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }
  return ret;
}

static pef_err_t
policy_number_checkout (pef_config_state_data_t *state_data,
                        const struct section *sect,
                        struct keyvalue *kv)
{
  uint8_t policy_number;
  pef_err_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  
  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));
  
  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((ret = alert_policy_get (state_data,
                               alert_policy_entry_number,
                               NULL,
                               NULL,
                               &policy_number,
                               NULL,
                               NULL,
                               NULL,
                               NULL)) != PEF_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (asprintf (&kv->value, "%u", policy_number) < 0)
    {
      perror("asprintf");
      return PEF_ERR_FATAL_ERROR;
    }

  return PEF_ERR_SUCCESS;
}

static pef_err_t
policy_number_commit (pef_config_state_data_t *state_data,
                      const struct section *sect,
                      const struct keyvalue *kv)
{
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  pef_err_t ret;
  uint8_t policy_number;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  policy_number = atoi (kv->value);

  return alert_policy_set (state_data,
                           alert_policy_entry_number,
                           0, 0,
                           0, 0,
                           policy_number, 1,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0);
}

static pef_diff_t
policy_number_diff (pef_config_state_data_t *state_data,
                    const struct section *sect,
                    const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  pef_err_t rc;
  pef_diff_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((rc = get_number_of_alert_policy_entries (state_data,
                                                &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return rc;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((rc = alert_policy_get (state_data,
                              alert_policy_entry_number,
                              NULL,
                              NULL,
                              &get_val,
                              NULL,
                              NULL,
                              NULL,
                              NULL)) != PEF_ERR_SUCCESS)
    {
      if (rc == PEF_ERR_NON_FATAL_ERROR)
        return PEF_DIFF_NON_FATAL_ERROR;
      return PEF_DIFF_FATAL_ERROR;
    }
  
  passed_val = atoi (kv->value);

  if (passed_val == get_val)
    ret = PEF_DIFF_SAME;
  else
    {
      char num[32];
      ret = PEF_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static pef_err_t
destination_selector_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  uint8_t destination_selector;
  pef_err_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  
  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));
  
  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((ret = alert_policy_get (state_data,
                               alert_policy_entry_number,
                               NULL,
                               NULL,
                               NULL,
                               &destination_selector,
                               NULL,
                               NULL,
                               NULL)) != PEF_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (asprintf (&kv->value, "%u", destination_selector) < 0)
    {
      perror("asprintf");
      return PEF_ERR_FATAL_ERROR;
    }

  return PEF_ERR_SUCCESS;
}

static pef_err_t
destination_selector_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  pef_err_t ret;
  uint8_t destination_selector;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  destination_selector = atoi (kv->value);

  return alert_policy_set (state_data,
                           alert_policy_entry_number,
                           0, 0,
                           0, 0,
                           0, 0,
                           destination_selector, 1,
                           0, 0,
                           0, 0,
                           0, 0);
}

static pef_diff_t
destination_selector_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  pef_err_t rc;
  pef_diff_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((rc = get_number_of_alert_policy_entries (state_data,
                                                &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return rc;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((rc = alert_policy_get (state_data,
                              alert_policy_entry_number,
                              NULL,
                              NULL,
                              NULL,
                              &get_val,
                              NULL,
                              NULL,
                              NULL)) != PEF_ERR_SUCCESS)
    {
      if (rc == PEF_ERR_NON_FATAL_ERROR)
        return PEF_DIFF_NON_FATAL_ERROR;
      return PEF_DIFF_FATAL_ERROR;
    }
  
  passed_val = atoi (kv->value);

  if (passed_val == get_val)
    ret = PEF_DIFF_SAME;
  else
    {
      char num[32];
      ret = PEF_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static pef_err_t
channel_number_checkout (pef_config_state_data_t *state_data,
                         const struct section *sect,
                         struct keyvalue *kv)
{
  uint8_t channel_number;
  pef_err_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  
  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));
  
  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((ret = alert_policy_get (state_data,
                               alert_policy_entry_number,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               &channel_number,
                               NULL,
                               NULL)) != PEF_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (asprintf (&kv->value, "%u", channel_number) < 0)
    {
      perror("asprintf");
      return PEF_ERR_FATAL_ERROR;
    }

  return PEF_ERR_SUCCESS;
}

static pef_err_t
channel_number_commit (pef_config_state_data_t *state_data,
                       const struct section *sect,
                       const struct keyvalue *kv)
{
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  pef_err_t ret;
  uint8_t channel_number;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  channel_number = atoi (kv->value);

  return alert_policy_set (state_data,
                           alert_policy_entry_number,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           channel_number, 1,
                           0, 0,
                           0, 0);
}

static pef_diff_t
channel_number_diff (pef_config_state_data_t *state_data,
                     const struct section *sect,
                     const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  pef_err_t rc;
  pef_diff_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((rc = get_number_of_alert_policy_entries (state_data,
                                                &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return rc;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((rc = alert_policy_get (state_data,
                              alert_policy_entry_number,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              &get_val,
                              NULL,
                              NULL)) != PEF_ERR_SUCCESS)
    {
      if (rc == PEF_ERR_NON_FATAL_ERROR)
        return PEF_DIFF_NON_FATAL_ERROR;
      return PEF_DIFF_FATAL_ERROR;
    }
  
  passed_val = atoi (kv->value);

  if (passed_val == get_val)
    ret = PEF_DIFF_SAME;
  else
    {
      char num[32];
      ret = PEF_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static pef_err_t
alert_string_set_selector_checkout (pef_config_state_data_t *state_data,
                                    const struct section *sect,
                                    struct keyvalue *kv)
{
  uint8_t alert_string_set_selector;
  pef_err_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  
  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));
  
  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((ret = alert_policy_get (state_data,
                               alert_policy_entry_number,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               &alert_string_set_selector,
                               NULL)) != PEF_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (asprintf (&kv->value, "%u", alert_string_set_selector) < 0)
    {
      perror("asprintf");
      return PEF_ERR_FATAL_ERROR;
    }

  return PEF_ERR_SUCCESS;
}

static pef_err_t
alert_string_set_selector_commit (pef_config_state_data_t *state_data,
                                  const struct section *sect,
                                  const struct keyvalue *kv)
{
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  pef_err_t ret;
  uint8_t alert_string_set_selector;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  alert_string_set_selector = atoi (kv->value);

  return alert_policy_set (state_data,
                           alert_policy_entry_number,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           alert_string_set_selector, 1,
                           0, 0);
}

static pef_diff_t
alert_string_set_selector_diff (pef_config_state_data_t *state_data,
                                const struct section *sect,
                                const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  pef_err_t rc;
  pef_diff_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((rc = get_number_of_alert_policy_entries (state_data,
                                                &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return rc;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((rc = alert_policy_get (state_data,
                              alert_policy_entry_number,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              &get_val,
                              NULL)) != PEF_ERR_SUCCESS)
    {
      if (rc == PEF_ERR_NON_FATAL_ERROR)
        return PEF_DIFF_NON_FATAL_ERROR;
      return PEF_DIFF_FATAL_ERROR;
    }
  
  passed_val = atoi (kv->value);

  if (passed_val == get_val)
    ret = PEF_DIFF_SAME;
  else
    {
      char num[32];
      ret = PEF_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static pef_err_t
event_specific_alert_string_checkout (pef_config_state_data_t *state_data,
                                      const struct section *sect,
                                      struct keyvalue *kv)
{
  uint8_t event_specific_alert_string;
  pef_err_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  
  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));
  
  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((ret = alert_policy_get (state_data,
                               alert_policy_entry_number,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               &event_specific_alert_string)) != PEF_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (event_specific_alert_string)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return PEF_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return PEF_ERR_FATAL_ERROR;
        }
    }

  return PEF_ERR_SUCCESS;
}

static pef_err_t
event_specific_alert_string_commit (pef_config_state_data_t *state_data,
                                    const struct section *sect,
                                    const struct keyvalue *kv)
{
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;
  pef_err_t ret;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((ret = get_number_of_alert_policy_entries (state_data,
                                                 &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return ret;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  return alert_policy_set (state_data,
                           alert_policy_entry_number,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           0, 0,
                           same (kv->value, "yes"), 1);
}

static pef_diff_t
event_specific_alert_string_diff (pef_config_state_data_t *state_data,
                                  const struct section *sect,
                                  const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  pef_err_t rc;
  pef_diff_t ret;
  uint8_t alert_policy_entry_number;
  uint8_t number_of_alert_policy_entries;

  alert_policy_entry_number = atoi (sect->section_name + strlen ("Alert_Policy_"));

  if ((rc = get_number_of_alert_policy_entries (state_data,
                                                &number_of_alert_policy_entries)) != PEF_ERR_SUCCESS)
    return rc;

  if (alert_policy_entry_number > number_of_alert_policy_entries)
    return PEF_ERR_NON_FATAL_ERROR;

  if ((rc = alert_policy_get (state_data,
                              alert_policy_entry_number,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              &get_val)) != PEF_ERR_SUCCESS)
    {
      if (rc == PEF_ERR_NON_FATAL_ERROR)
        return PEF_DIFF_NON_FATAL_ERROR;
      return PEF_DIFF_FATAL_ERROR;
    }
  
  passed_val = same (kv->value, "Yes");

  if (passed_val == get_val)
    ret = PEF_DIFF_SAME;
  else
    {
      ret = PEF_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_val ? "Yes" : "No");
    }
  return ret;
}

struct section *
pef_config_alert_policy_table_section_get (pef_config_state_data_t *state_data, int num)
{
  struct section *sect = NULL;
  char buf[64];

  if (num <= 0)
    {
      fprintf(stderr, "Invalid Num = %d\n", num);
      return NULL;
    }

  snprintf(buf, 64, "Alert_Policy_%d", num);

  if (!(sect = pef_config_section_create (state_data, buf)))
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Policy_Type",
                                       "Possible values: Always_Send_To_This_Destination/Proceed_To_Next_Entry/Do_Not_Proceed_Any_More_Entries/Proceed_To_Next_Entry_Different_Channel/Proceed_To_Next_Entry_Different_Destination",
                                       0,
                                       policy_type_checkout,
                                       policy_type_commit,
                                       policy_type_diff,
                                       policy_type_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Policy_Enabled",
                                       "Possible values: Yes/No",
                                       0,
                                       policy_enabled_checkout,
                                       policy_enabled_commit,
                                       policy_enabled_diff,
                                       yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Policy_Number",
                                       "Give a valid number",
                                       0,
                                       policy_number_checkout,
                                       policy_number_commit,
                                       policy_number_diff,
                                       number_range_four_bits) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Destination_Selector",
                                       "Give a valid number",
                                       0,
                                       destination_selector_checkout,
                                       destination_selector_commit,
                                       destination_selector_diff,
                                       number_range_four_bits) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Channel_Number",
                                       "Give a valid number",
                                       0,
                                       channel_number_checkout,
                                       channel_number_commit,
                                       channel_number_diff,
                                       number_range_four_bits) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Alert_String_Set_Selector",
                                       "Give a valid number",
                                       0,
                                       alert_string_set_selector_checkout,
                                       alert_string_set_selector_commit,
                                       alert_string_set_selector_diff,
                                       number_range_seven_bits) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Specific_Alert_String",
                                       "Possible values: Yes/No",
                                       0,
                                       event_specific_alert_string_checkout,
                                       event_specific_alert_string_commit,
                                       event_specific_alert_string_diff,
                                       yes_no_validate) < 0) 
    goto cleanup;

  return sect;

 cleanup:
  if (sect)
    pef_config_section_destroy(state_data, sect);
  return NULL;
}

