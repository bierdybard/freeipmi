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

struct event_filter_table {
  uint8_t filter_type;
  uint8_t enable_filter;
  uint8_t event_filter_action_alert;
  uint8_t event_filter_action_power_off;
  uint8_t event_filter_action_reset;
  uint8_t event_filter_action_power_cycle;
  uint8_t event_filter_action_oem;
  uint8_t event_filter_action_diagnostic_interrupt;
  uint8_t event_filter_action_group_control_operation;
  uint8_t alert_policy_number;
  uint8_t group_control_selector;
  uint8_t event_severity;
  uint8_t generator_id_byte_1;
  uint8_t generator_id_byte_2;
  uint8_t sensor_type;
  uint8_t sensor_number;
  uint8_t event_trigger;
  uint16_t event_data1_offset_mask;
  uint8_t event_data1_and_mask;
  uint8_t event_data1_compare1;
  uint8_t event_data1_compare2;
  uint8_t event_data2_and_mask;
  uint8_t event_data2_compare1;
  uint8_t event_data2_compare2;
  uint8_t event_data3_and_mask;
  uint8_t event_data3_compare1;
  uint8_t event_data3_compare2;
};

static config_err_t
event_filter_get (pef_config_state_data_t *state_data,
                  uint8_t filter_number,
                  struct event_filter_table *eft)
{
  config_err_t ret;

  if ((ret = get_bmc_pef_conf_event_filter_table (state_data,
                                                  filter_number,
                                                  &eft->filter_type,
                                                  &eft->enable_filter,
                                                  &eft->event_filter_action_alert,
                                                  &eft->event_filter_action_power_off,
                                                  &eft->event_filter_action_reset,
                                                  &eft->event_filter_action_power_cycle,
                                                  &eft->event_filter_action_oem,
                                                  &eft->event_filter_action_diagnostic_interrupt,
                                                  &eft->event_filter_action_group_control_operation,
                                                  &eft->alert_policy_number,
                                                  &eft->group_control_selector,
                                                  &eft->event_severity,
                                                  &eft->generator_id_byte_1,
                                                  &eft->generator_id_byte_2,
                                                  &eft->sensor_type,
                                                  &eft->sensor_number,
                                                  &eft->event_trigger,
                                                  &eft->event_data1_offset_mask,
                                                  &eft->event_data1_and_mask,
                                                  &eft->event_data1_compare1,
                                                  &eft->event_data1_compare2,
                                                  &eft->event_data2_and_mask,
                                                  &eft->event_data2_compare1,
                                                  &eft->event_data2_compare2,
                                                  &eft->event_data3_and_mask,
                                                  &eft->event_data3_compare1,
                                                  &eft->event_data3_compare2)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_set (pef_config_state_data_t *state_data,
                  uint8_t filter_number,
                  struct event_filter_table *eft)
{
  config_err_t ret;

  if ((ret = set_bmc_pef_conf_event_filter_table (state_data,
                                                  filter_number,
                                                  eft->filter_type,
                                                  eft->enable_filter,
                                                  eft->event_filter_action_alert,
                                                  eft->event_filter_action_power_off,
                                                  eft->event_filter_action_reset,
                                                  eft->event_filter_action_power_cycle,
                                                  eft->event_filter_action_oem,
                                                  eft->event_filter_action_diagnostic_interrupt,
                                                  eft->event_filter_action_group_control_operation,
                                                  eft->alert_policy_number,
                                                  eft->group_control_selector,
                                                  eft->event_severity,
                                                  eft->generator_id_byte_1,
                                                  eft->generator_id_byte_2,
                                                  eft->sensor_type,
                                                  eft->sensor_number,
                                                  eft->event_trigger,
                                                  eft->event_data1_offset_mask,
                                                  eft->event_data1_and_mask,
                                                  eft->event_data1_compare1,
                                                  eft->event_data1_compare2,
                                                  eft->event_data2_and_mask,
                                                  eft->event_data2_compare1,
                                                  eft->event_data2_compare2,
                                                  eft->event_data3_and_mask,
                                                  eft->event_data3_compare1,
                                                  eft->event_data3_compare2)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
filter_type_checkout (pef_config_state_data_t *state_data,
                      const struct section *sect,
                      struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (filter_type_string (eft.filter_type))))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
filter_type_commit (pef_config_state_data_t *state_data,
                    const struct section *sect,
                    const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.filter_type = filter_type_number (kv->value);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
filter_type_diff (pef_config_state_data_t *state_data,
                  const struct section *sect,
                  const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = filter_type_number (kv->value);
  if (passed_val == eft.filter_type)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   filter_type_string (eft.filter_type));
    }
  return ret;
}

static config_err_t
enable_filter_checkout (pef_config_state_data_t *state_data,
                        const struct section *sect,
                        struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.enable_filter)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
enable_filter_commit (pef_config_state_data_t *state_data,
                      const struct section *sect,
                      const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.enable_filter = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
enable_filter_diff (pef_config_state_data_t *state_data,
                    const struct section *sect,
                    const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.enable_filter)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.enable_filter ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
event_filter_action_alert_checkout (pef_config_state_data_t *state_data,
                                    const struct section *sect,
                                    struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.event_filter_action_alert)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_action_alert_commit (pef_config_state_data_t *state_data,
                                  const struct section *sect,
                                  const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_filter_action_alert = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_filter_action_alert_diff (pef_config_state_data_t *state_data,
                                const struct section *sect,
                                const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.event_filter_action_alert)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.event_filter_action_alert ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
event_filter_action_power_off_checkout (pef_config_state_data_t *state_data,
                                        const struct section *sect,
                                        struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.event_filter_action_power_off)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_action_power_off_commit (pef_config_state_data_t *state_data,
                                      const struct section *sect,
                                      const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_filter_action_power_off = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_filter_action_power_off_diff (pef_config_state_data_t *state_data,
                                    const struct section *sect,
                                    const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.event_filter_action_power_off)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.event_filter_action_power_off ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
event_filter_action_reset_checkout (pef_config_state_data_t *state_data,
                                    const struct section *sect,
                                    struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.event_filter_action_reset)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_action_reset_commit (pef_config_state_data_t *state_data,
                                  const struct section *sect,
                                  const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_filter_action_reset = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_filter_action_reset_diff (pef_config_state_data_t *state_data,
                                const struct section *sect,
                                const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.event_filter_action_reset)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.event_filter_action_reset ? "Yes" : "No");
    }
  return ret;
}


static config_err_t
event_filter_action_power_cycle_checkout (pef_config_state_data_t *state_data,
                                          const struct section *sect,
                                          struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.event_filter_action_power_cycle)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_action_power_cycle_commit (pef_config_state_data_t *state_data,
                                        const struct section *sect,
                                        const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_filter_action_power_cycle = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_filter_action_power_cycle_diff (pef_config_state_data_t *state_data,
                                      const struct section *sect,
                                      const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.event_filter_action_power_cycle)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.event_filter_action_power_cycle ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
event_filter_action_oem_checkout (pef_config_state_data_t *state_data,
                                  const struct section *sect,
                                  struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.event_filter_action_oem)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_action_oem_commit (pef_config_state_data_t *state_data,
                                const struct section *sect,
                                const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_filter_action_oem = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_filter_action_oem_diff (pef_config_state_data_t *state_data,
                              const struct section *sect,
                              const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.event_filter_action_oem)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.event_filter_action_oem ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
event_filter_action_diagnostic_interrupt_checkout (pef_config_state_data_t *state_data,
                                                   const struct section *sect,
                                                   struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.event_filter_action_diagnostic_interrupt)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_action_diagnostic_interrupt_commit (pef_config_state_data_t *state_data,
                                                 const struct section *sect,
                                                 const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_filter_action_diagnostic_interrupt = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_filter_action_diagnostic_interrupt_diff (pef_config_state_data_t *state_data,
                                               const struct section *sect,
                                               const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.event_filter_action_diagnostic_interrupt)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.event_filter_action_diagnostic_interrupt ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
event_filter_action_group_control_operation_checkout (pef_config_state_data_t *state_data,
                                                      const struct section *sect,
                                                      struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (eft.event_filter_action_group_control_operation)
    {
      if (!(kv->value = strdup ("Yes")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }
  else
    {
      if (!(kv->value = strdup ("No")))
        {
          perror("strdup");
          return CONFIG_ERR_FATAL_ERROR;
        }
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_filter_action_group_control_operation_commit (pef_config_state_data_t *state_data,
                                                    const struct section *sect,
                                                    const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_filter_action_group_control_operation = same (kv->value, "yes");

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_filter_action_group_control_operation_diff (pef_config_state_data_t *state_data,
                                                  const struct section *sect,
                                                  const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = same (kv->value, "Yes");

  if (passed_val == eft.event_filter_action_group_control_operation)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   eft.event_filter_action_group_control_operation ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
alert_policy_number_checkout (pef_config_state_data_t *state_data,
                              const struct section *sect,
                              struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "%u", eft.alert_policy_number) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_policy_number_commit (pef_config_state_data_t *state_data,
                            const struct section *sect,
                            const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.alert_policy_number = atoi (kv->value);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
alert_policy_number_diff (pef_config_state_data_t *state_data,
                          const struct section *sect,
                          const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = atoi (kv->value);

  if (passed_val == eft.alert_policy_number)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "%u", eft.alert_policy_number);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
group_control_selector_checkout (pef_config_state_data_t *state_data,
                                 const struct section *sect,
                                 struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "%u", eft.group_control_selector) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
group_control_selector_commit (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.group_control_selector = atoi (kv->value);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
group_control_selector_diff (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = atoi (kv->value);

  if (passed_val == eft.group_control_selector)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "%u", eft.group_control_selector);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_severity_checkout (pef_config_state_data_t *state_data,
                         const struct section *sect,
                         struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (event_severity_string (eft.event_severity))))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_severity_commit (pef_config_state_data_t *state_data,
                       const struct section *sect,
                       const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_severity = event_severity_number (kv->value);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_severity_diff (pef_config_state_data_t *state_data,
                     const struct section *sect,
                     const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = event_severity_number (kv->value);
  if (passed_val == eft.event_severity)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   event_severity_string (eft.event_severity));
    }
  return ret;
}

static config_err_t
generator_id_byte_1_checkout (pef_config_state_data_t *state_data,
                              const struct section *sect,
                              struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.generator_id_byte_1) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
generator_id_byte_1_commit (pef_config_state_data_t *state_data,
                            const struct section *sect,
                            const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.generator_id_byte_1 = strtol(kv->value, NULL, 0);
  

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
generator_id_byte_1_diff (pef_config_state_data_t *state_data,
                          const struct section *sect,
                          const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.generator_id_byte_1)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.generator_id_byte_1);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
generator_id_byte_2_checkout (pef_config_state_data_t *state_data,
                              const struct section *sect,
                              struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.generator_id_byte_2) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
generator_id_byte_2_commit (pef_config_state_data_t *state_data,
                            const struct section *sect,
                            const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.generator_id_byte_2 = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
generator_id_byte_2_diff (pef_config_state_data_t *state_data,
                          const struct section *sect,
                          const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.generator_id_byte_2)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.generator_id_byte_2);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
sensor_type_checkout (pef_config_state_data_t *state_data,
                      const struct section *sect,
                      struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);


  if (!(kv->value = strdup (sensor_type_string (eft.sensor_type))))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
sensor_type_commit (pef_config_state_data_t *state_data,
                    const struct section *sect,
                    const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.sensor_type = sensor_type_number(kv->value);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
sensor_type_diff (pef_config_state_data_t *state_data,
                  const struct section *sect,
                  const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = sensor_type_number(kv->value);

  if (passed_val == eft.sensor_type)
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
		   sensor_type_string (eft.sensor_type));
    }

  return ret;
}

static config_err_t
sensor_number_checkout (pef_config_state_data_t *state_data,
                        const struct section *sect,
                        struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.sensor_number) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
sensor_number_commit (pef_config_state_data_t *state_data,
                      const struct section *sect,
                      const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.sensor_number = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
sensor_number_diff (pef_config_state_data_t *state_data,
                    const struct section *sect,
                    const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.sensor_number)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.sensor_number);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_trigger_checkout (pef_config_state_data_t *state_data,
                        const struct section *sect,
                        struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_trigger) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_trigger_commit (pef_config_state_data_t *state_data,
                      const struct section *sect,
                      const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_trigger = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_trigger_diff (pef_config_state_data_t *state_data,
                    const struct section *sect,
                    const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_trigger)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_trigger);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data1_offset_mask_checkout (pef_config_state_data_t *state_data,
                                  const struct section *sect,
                                  struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data1_offset_mask) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data1_offset_mask_commit (pef_config_state_data_t *state_data,
                                const struct section *sect,
                                const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data1_offset_mask = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data1_offset_mask_diff (pef_config_state_data_t *state_data,
                              const struct section *sect,
                              const struct keyvalue *kv)
{
  uint16_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data1_offset_mask)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data1_offset_mask);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data1_and_mask_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data1_and_mask) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data1_and_mask_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data1_and_mask = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data1_and_mask_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data1_and_mask)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data1_and_mask);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data1_compare1_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data1_compare1) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data1_compare1_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data1_compare1 = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data1_compare1_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data1_compare1)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data1_compare1);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data1_compare2_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data1_compare2) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data1_compare2_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data1_compare2 = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data1_compare2_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data1_compare2)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data1_compare2);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data2_and_mask_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data2_and_mask) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data2_and_mask_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data2_and_mask = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data2_and_mask_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data2_and_mask)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data2_and_mask);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data2_compare1_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data2_compare1) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data2_compare1_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data2_compare1 = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data2_compare1_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data2_compare1)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data2_compare1);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data2_compare2_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data2_compare2) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data2_compare2_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data2_compare2 = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data2_compare2_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data2_compare2)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data2_compare2);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data3_and_mask_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data3_and_mask) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data3_and_mask_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data3_and_mask = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data3_and_mask_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data3_and_mask)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data3_and_mask);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data3_compare1_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data3_compare1) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data3_compare1_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data3_compare1 = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data3_compare1_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data3_compare1)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data3_compare1);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
event_data3_compare2_checkout (pef_config_state_data_t *state_data,
                               const struct section *sect,
                               struct keyvalue *kv)
{
  config_err_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "0x%02X", eft.event_data3_compare2) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
event_data3_compare2_commit (pef_config_state_data_t *state_data,
                             const struct section *sect,
                             const struct keyvalue *kv)
{
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  config_err_t ret;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((ret = get_number_of_event_filters (state_data,
                                          &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = event_filter_get (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  eft.event_data3_compare2 = strtol(kv->value, NULL, 0);

  if ((ret = event_filter_set (state_data,
                               event_filter_number,
                               &eft)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_diff_t
event_data3_compare2_diff (pef_config_state_data_t *state_data,
                           const struct section *sect,
                           const struct keyvalue *kv)
{
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t event_filter_number;
  uint8_t number_of_event_filters;
  struct event_filter_table eft;

  event_filter_number = atoi (sect->section_name + strlen ("Event_Filter_"));

  if ((rc = get_number_of_event_filters (state_data,
                                         &number_of_event_filters)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (event_filter_number > number_of_event_filters)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = event_filter_get (state_data,
                              event_filter_number,
                              &eft)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  passed_val = strtol(kv->value, NULL, 0);

  if (passed_val == eft.event_data3_compare2)
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "0x%02X", eft.event_data3_compare2);
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   num);
    }
  return ret;
}

struct section *
pef_config_event_filter_table_section_get (pef_config_state_data_t *state_data, int num)
{
  struct section *sect = NULL;
  char buf[64];

  if (num <= 0)
    {
      fprintf(stderr, "Invalid Num = %d\n", num);
      return NULL;
    }

  snprintf(buf, 64, "Event_Filter_%d", num);

  if (!(sect = pef_config_section_create (state_data, 
                                          buf, 
                                          NULL, 
                                          NULL, 
                                          0)))
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Filter_Type",
                                       "Possible values: Manufacturer_Pre_Configured/Software_Configurable/Reserved1/Reserved3",
                                       0,
                                       filter_type_checkout,
                                       filter_type_commit,
                                       filter_type_diff,
                                       filter_type_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Enable_Filter",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_filter_checkout,
                                       enable_filter_commit,
                                       enable_filter_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Filter_Action_Alert",
                                       "Possible values: Yes/No",
                                       0,
                                       event_filter_action_alert_checkout,
                                       event_filter_action_alert_commit,
                                       event_filter_action_alert_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Filter_Action_Power_Off",
                                       "Possible values: Yes/No",
                                       0,
                                       event_filter_action_power_off_checkout,
                                       event_filter_action_power_off_commit,
                                       event_filter_action_power_off_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Filter_Action_Reset",
                                       "Possible values: Yes/No",
                                       0,
                                       event_filter_action_reset_checkout,
                                       event_filter_action_reset_commit,
                                       event_filter_action_reset_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Filter_Action_Power_Cycle",
                                       "Possible values: Yes/No",
                                       0,
                                       event_filter_action_power_cycle_checkout,
                                       event_filter_action_power_cycle_commit,
                                       event_filter_action_power_cycle_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Filter_Action_Oem",
                                       "Possible values: Yes/No",
                                       0,
                                       event_filter_action_oem_checkout,
                                       event_filter_action_oem_commit,
                                       event_filter_action_oem_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Filter_Action_Diagnostic_Interrupt",
                                       "Possible values: Yes/No",
                                       0,
                                       event_filter_action_diagnostic_interrupt_checkout,
                                       event_filter_action_diagnostic_interrupt_commit,
                                       event_filter_action_diagnostic_interrupt_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Filter_Action_Group_Control_Operation",
                                       "Possible values: Yes/No",
                                       0,
                                       event_filter_action_group_control_operation_checkout,
                                       event_filter_action_group_control_operation_commit,
                                       event_filter_action_group_control_operation_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Alert_Policy_Number",
                                       "Give a valid number",
                                       0,
                                       alert_policy_number_checkout,
                                       alert_policy_number_commit,
                                       alert_policy_number_diff,
                                       config_number_range_four_bits) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Group_Control_Selector",
                                       "Give a valid number",
                                       0,
                                       group_control_selector_checkout,
                                       group_control_selector_commit,
                                       group_control_selector_diff,
                                       config_number_range_three_bits) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Severity",
                                       "Possible values: Unspecified/Monitor/Information/OK/Non_Critical/Critical/Non_Recoverable",
                                       0,
                                       event_severity_checkout,
                                       event_severity_commit,
                                       event_severity_diff,
                                       event_severity_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Generator_Id_Byte_1",
                                       "Specify a hex Slave Address or Software ID from Event Message or 0xFF to Match Any",
                                       0,
                                       generator_id_byte_1_checkout,
                                       generator_id_byte_1_commit,
                                       generator_id_byte_1_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Generator_Id_Byte_1",
                                       "Specify a hex Channel Number or LUN to match or 0xFF to Match Any",
                                       0,
                                       generator_id_byte_2_checkout,
                                       generator_id_byte_2_commit,
                                       generator_id_byte_2_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Sensor_Type",
                                       "Specify a Sensor Type, For options see the MAN page",
                                       0,
                                       sensor_type_checkout,
                                       sensor_type_commit,
                                       sensor_type_diff,
                                       sensor_type_validate) < 0) 
    goto cleanup;
  
  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Sensor_Number",
                                       "Specify a Sensor Number or 0xFF to Match Any",
                                       0,
                                       sensor_number_checkout,
                                       sensor_number_commit,
                                       sensor_number_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Trigger",
                                       "Specify a Event/Reading Type Number or 0xFF to Match Any",
                                       0,
                                       event_trigger_checkout,
                                       event_trigger_commit,
                                       event_trigger_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data1_Offset_Mask",
                                       "Give a valid number",
                                       0,
                                       event_data1_offset_mask_checkout,
                                       event_data1_offset_mask_commit,
                                       event_data1_offset_mask_diff,
                                       config_number_range_two_bytes) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data1_AND_Mask",
                                       "Give a valid number",
                                       0,
                                       event_data1_and_mask_checkout,
                                       event_data1_and_mask_commit,
                                       event_data1_and_mask_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data1_Compare1",
                                       "Give a valid number",
                                       0,
                                       event_data1_compare1_checkout,
                                       event_data1_compare1_commit,
                                       event_data1_compare1_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data1_Compare2",
                                       "Give a valid number",
                                       0,
                                       event_data1_compare2_checkout,
                                       event_data1_compare2_commit,
                                       event_data1_compare2_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;
  
  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data2_AND_Mask",
                                       "Give a valid number",
                                       0,
                                       event_data2_and_mask_checkout,
                                       event_data2_and_mask_commit,
                                       event_data2_and_mask_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data2_Compare1",
                                       "Give a valid number",
                                       0,
                                       event_data2_compare1_checkout,
                                       event_data2_compare1_commit,
                                       event_data2_compare1_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data2_Compare2",
                                       "Give a valid number",
                                       0,
                                       event_data2_compare2_checkout,
                                       event_data2_compare2_commit,
                                       event_data2_compare2_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data3_AND_Mask",
                                       "Give a valid number",
                                       0,
                                       event_data3_and_mask_checkout,
                                       event_data3_and_mask_commit,
                                       event_data3_and_mask_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data3_Compare1",
                                       "Give a valid number",
                                       0,
                                       event_data3_compare1_checkout,
                                       event_data3_compare1_commit,
                                       event_data3_compare1_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       sect,
                                       "Event_Data3_Compare2",
                                       "Give a valid number",
                                       0,
                                       event_data3_compare2_checkout,
                                       event_data3_compare2_commit,
                                       event_data3_compare2_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  return sect;

 cleanup:
  if (sect)
    pef_config_section_destroy(state_data, sect);
  return NULL;
}
