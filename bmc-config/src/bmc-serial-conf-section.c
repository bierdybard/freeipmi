#include "bmc-config.h"
#include "bmc-config-common.h"
#include "bmc-config-wrapper.h"
#include "bmc-config-diff.h"
#include "bmc-config-map.h"
#include "bmc-config-sections.h"
#include "bmc-config-validate.h"

static bmc_err_t
serial_conf_checkout (bmc_config_state_data_t *state_data,
		      uint8_t *basic_mode,
		      uint8_t *ppp_mode,
		      uint8_t *terminal_mode,
		      uint8_t *connect_mode)
{
  uint8_t tmp_basic_mode;
  uint8_t tmp_ppp_mode;
  uint8_t tmp_terminal_mode;
  uint8_t tmp_connect_mode;
  bmc_err_t ret;

  if ((ret = get_bmc_serial_conf_connection_mode (state_data,
                                                  &tmp_basic_mode,
                                                  &tmp_ppp_mode,
                                                  &tmp_terminal_mode,
                                                  &tmp_connect_mode)) != BMC_ERR_SUCCESS)
    return ret;

  if (basic_mode)
    *basic_mode = tmp_basic_mode;

  if (ppp_mode)
    *ppp_mode = tmp_ppp_mode;

  if (terminal_mode)
    *terminal_mode = tmp_terminal_mode;

  if (connect_mode)
    *connect_mode = tmp_connect_mode;

  return BMC_ERR_SUCCESS;
}


static bmc_err_t
serial_conf_commit (bmc_config_state_data_t *state_data,
		    uint8_t *basic_mode,
		    uint8_t *ppp_mode,
		    uint8_t *terminal_mode,
		    uint8_t *connect_mode)
{
  uint8_t tmp_basic_mode;
  uint8_t tmp_ppp_mode;
  uint8_t tmp_terminal_mode;
  uint8_t tmp_connect_mode;
  bmc_err_t ret;

  if ((ret = get_bmc_serial_conf_connection_mode (state_data,
                                                  &tmp_basic_mode,
                                                  &tmp_ppp_mode,
                                                  &tmp_terminal_mode,
                                                  &tmp_connect_mode)) != BMC_ERR_SUCCESS)
    return ret;

  if (basic_mode)
    tmp_basic_mode = *basic_mode;

  if (ppp_mode)
    tmp_ppp_mode = *ppp_mode;

  if (terminal_mode)
    tmp_terminal_mode = *terminal_mode;

  if (connect_mode)
    tmp_connect_mode = *connect_mode;

  return set_bmc_serial_conf_connection_mode (state_data,
                                              tmp_basic_mode,
                                              tmp_ppp_mode,
                                              tmp_terminal_mode,
                                              tmp_connect_mode);
}

static bmc_err_t
enable_basic_mode_checkout (bmc_config_state_data_t *state_data,
			    const struct section *sect,
			    struct keyvalue *kv)
{
  uint8_t value;
  bmc_err_t ret;

  if ((ret = serial_conf_checkout (state_data,
                                   &value,
                                   NULL,
                                   NULL,
                                   NULL)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (value)
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
enable_basic_mode_commit (bmc_config_state_data_t *state_data,
			  const struct section *sect,
			  const struct keyvalue *kv)
{
  uint8_t value;
  value = (same (kv->value, "yes") ? 1 : 0);

  return serial_conf_commit (state_data,
			     &value, NULL, NULL, NULL);
}

static bmc_diff_t
enable_basic_mode_diff (bmc_config_state_data_t *state_data,
			const struct section *sect,
			const struct keyvalue *kv)
{
  uint8_t get_value;
  uint8_t passed_value;
  bmc_err_t rc;
  bmc_diff_t ret;
  
  if ((rc = serial_conf_checkout (state_data,
                                   &get_value,
                                   NULL,
                                   NULL,
                                   NULL)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_value = same (kv->value, "yes");

  if (passed_value == get_value)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_value ? "Yes" : "No");
    }
  return ret;
}

/* ppp */

static bmc_err_t
enable_ppp_mode_checkout (bmc_config_state_data_t *state_data,
			  const struct section *sect,
			  struct keyvalue *kv)
{
  uint8_t value;
  bmc_err_t ret;

  if ((ret = serial_conf_checkout (state_data,
                                   NULL,
                                   &value,
                                   NULL,
                                   NULL)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (value)
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
enable_ppp_mode_commit (bmc_config_state_data_t *state_data,
			const struct section *sect,
			const struct keyvalue *kv)
{
  uint8_t value;
  value = (same (kv->value, "yes") ? 1 : 0);

  return serial_conf_commit (state_data,
			     NULL, &value, NULL, NULL);
}

static bmc_diff_t
enable_ppp_mode_diff (bmc_config_state_data_t *state_data,
		      const struct section *sect,
		      const struct keyvalue *kv)
{
  uint8_t get_value;
  uint8_t passed_value;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = serial_conf_checkout (state_data,
                                  NULL,
                                  &get_value,
                                  NULL,
                                  NULL)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_value = same (kv->value, "yes");

  if (passed_value == get_value)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_value ? "Yes" : "No");
    }
  return ret;
}

/* terminal */

static bmc_err_t
enable_terminal_mode_checkout (bmc_config_state_data_t *state_data,
			       const struct section *sect,
			       struct keyvalue *kv)
{
  uint8_t value;
  bmc_err_t ret;

  if ((ret = serial_conf_checkout (state_data,
                                   NULL,
                                   NULL,
                                   &value,
                                   NULL)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (value)
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
enable_terminal_mode_commit (bmc_config_state_data_t *state_data,
			     const struct section *sect,
			     const struct keyvalue *kv)
{
  uint8_t value;
  value = (same (kv->value, "yes") ? 1 : 0);

  return serial_conf_commit (state_data,
			     NULL, NULL, &value, NULL);
}

static bmc_diff_t
enable_terminal_mode_diff (bmc_config_state_data_t *state_data,
			   const struct section *sect,
			   const struct keyvalue *kv)
{
  uint8_t get_value;
  uint8_t passed_value;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = serial_conf_checkout (state_data,
                                  NULL,
                                  NULL,
                                  &get_value,
                                  NULL)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_value = same (kv->value, "yes");

  if (passed_value == get_value)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   get_value ? "Yes" : "No");
    }
  return ret;
}

static bmc_err_t
connect_mode_checkout (bmc_config_state_data_t *state_data,
		       const struct section *sect,
		       struct keyvalue *kv)
{
  uint8_t value;
  bmc_err_t ret;

  if ((ret = serial_conf_checkout (state_data,
                                   NULL,
                                   NULL,
                                   NULL,
                                   &value)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (connect_mode_string (value))))
    {
      perror("strdup");
      return BMC_ERR_FATAL_ERROR;
    }

  return BMC_ERR_SUCCESS;
}

static bmc_err_t
connect_mode_commit (bmc_config_state_data_t *state_data,
		     const struct section *sect,
		     const struct keyvalue *kv)
{
  uint8_t value;
  value = connect_mode_number (kv->value);

  return serial_conf_commit (state_data,
			     NULL, NULL, NULL, &value);
}

static bmc_diff_t
connect_mode_diff (bmc_config_state_data_t *state_data,
		   const struct section *sect,
		   const struct keyvalue *kv)
{
  uint8_t get_value;
  uint8_t passed_value;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = serial_conf_checkout (state_data,
                                  NULL,
                                  NULL,
                                  NULL,
                                  &get_value)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_value = connect_mode_number (kv->value);
  if (passed_value == get_value)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   connect_mode_string (get_value));
    }
  return ret;
}

static bmc_err_t
page_blackout_interval_checkout (bmc_config_state_data_t *state_data,
				 const struct section *sect,
				 struct keyvalue *kv)
{
  uint8_t interval;
  bmc_err_t ret;

  if ((ret = get_bmc_serial_conf_page_blackout_interval (state_data,
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
page_blackout_interval_commit (bmc_config_state_data_t *state_data,
			       const struct section *sect,
			       const struct keyvalue *kv)
{
  return set_bmc_serial_conf_page_blackout_interval (state_data,
						     atoi (kv->value));
}

static bmc_diff_t
page_blackout_interval_diff (bmc_config_state_data_t *state_data,
			     const struct section *sect,
			     const struct keyvalue *kv)
{
  uint8_t interval;
  int passed_interval;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_serial_conf_page_blackout_interval (state_data,
                                                        &interval)) != BMC_ERR_SUCCESS) 
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_interval = atoi (kv->value);

  if (passed_interval == interval)
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

/* retry time */

static bmc_err_t
call_retry_interval_checkout (bmc_config_state_data_t *state_data,
			      const struct section *sect,
			      struct keyvalue *kv)
{
  uint8_t interval;
  bmc_err_t ret;

  if ((ret = get_bmc_serial_conf_call_retry_interval (state_data,
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
call_retry_interval_commit (bmc_config_state_data_t *state_data,
			    const struct section *sect,
			    const struct keyvalue *kv)
{
  return set_bmc_serial_conf_call_retry_interval (state_data,
                                                  atoi (kv->value));
}

static bmc_diff_t
call_retry_interval_diff (bmc_config_state_data_t *state_data,
			  const struct section *sect,
			  const struct keyvalue *kv)
{
  uint8_t interval;
  int passed_interval;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = get_bmc_serial_conf_call_retry_interval (state_data,
                                                     &interval)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_interval = atoi (kv->value);

  if (passed_interval == interval)
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

static bmc_err_t
serial_conf_comm_checkout (bmc_config_state_data_t *state_data,
			   uint8_t *dtr_hangup,
			   uint8_t *flow_control,
			   uint8_t *bit_rate)
{
  uint8_t tmp_dtr_hangup;
  uint8_t tmp_flow_control;
  uint8_t tmp_bit_rate;
  bmc_err_t ret;

  if ((ret = get_bmc_serial_conf_ipmi_messaging_comm_settings (state_data,
                                                               &tmp_dtr_hangup,
                                                               &tmp_flow_control,
                                                               &tmp_bit_rate)) != BMC_ERR_SUCCESS)
    return ret;

  if (dtr_hangup)
    *dtr_hangup = tmp_dtr_hangup;

  if (flow_control)
    *flow_control = tmp_flow_control;

  if (bit_rate)
    *bit_rate = tmp_bit_rate;

  return BMC_ERR_SUCCESS;
}

static bmc_err_t
serial_conf_comm_commit (bmc_config_state_data_t *state_data,
			 uint8_t *dtr_hangup,
			 uint8_t *flow_control,
			 uint8_t *bit_rate)
{
  uint8_t tmp_dtr_hangup;
  uint8_t tmp_flow_control;
  uint8_t tmp_bit_rate;
  bmc_err_t ret;

  if ((ret = get_bmc_serial_conf_ipmi_messaging_comm_settings (state_data,
                                                               &tmp_dtr_hangup,
                                                               &tmp_flow_control,
                                                               &tmp_bit_rate)) != BMC_ERR_SUCCESS)
    return ret;

  if (dtr_hangup)
    tmp_dtr_hangup = *dtr_hangup;
  if (flow_control)
    tmp_flow_control = *flow_control;
  if (bit_rate)
    tmp_bit_rate = *bit_rate;

  return set_bmc_serial_conf_ipmi_messaging_comm_settings (state_data,
							   tmp_dtr_hangup,
							   tmp_flow_control,
							   tmp_bit_rate);
}

static bmc_err_t
enable_dtr_hangup_checkout (bmc_config_state_data_t *state_data,
			    const struct section *sect,
			    struct keyvalue *kv)
{
  uint8_t value;
  bmc_err_t ret;
  
  if ((ret = serial_conf_comm_checkout (state_data,
                                        &value,
                                        NULL,
                                        NULL)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (value)
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
enable_dtr_hangup_commit (bmc_config_state_data_t *state_data,
			  const struct section *sect,
			  const struct keyvalue *kv)
{
  uint8_t value = same (kv->value, "yes");

  return serial_conf_comm_commit (state_data,
				  &value,
				  NULL,
				  NULL);
}

static bmc_diff_t
enable_dtr_hangup_diff (bmc_config_state_data_t *state_data,
			const struct section *sect,
			const struct keyvalue *kv)
{
  uint8_t passed_value;
  uint8_t got_value;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = serial_conf_comm_checkout (state_data,
                                       &got_value,
                                       NULL,
                                       NULL)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_value = same (kv->value, "yes") ? 1 : 0;

  if (passed_value == got_value)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   got_value ? "Yes" : "No");
    }
  return ret;
}

static bmc_err_t
flow_control_checkout (bmc_config_state_data_t *state_data,
		       const struct section *sect,
		       struct keyvalue *kv)
{
  uint8_t value;
  bmc_err_t ret;
  
  if ((ret = serial_conf_comm_checkout (state_data,
                                        NULL,
                                        &value,
                                        NULL)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (flow_control_string (value))))
    {
      perror("strdup");
      return BMC_ERR_FATAL_ERROR;
    }

  return BMC_ERR_SUCCESS;
}

static bmc_err_t
flow_control_commit (bmc_config_state_data_t *state_data,
		     const struct section *sect,
		     const struct keyvalue *kv)
{
  uint8_t value = flow_control_number (kv->value);
  return serial_conf_comm_commit (state_data,
				  NULL,
				  &value,
				  NULL);
}

static bmc_diff_t
flow_control_diff (bmc_config_state_data_t *state_data,
		   const struct section *sect,
		   const struct keyvalue *kv)
{
  uint8_t passed_value;
  uint8_t got_value;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = serial_conf_comm_checkout (state_data,
                                       NULL,
                                       &got_value,
                                       NULL)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_value = flow_control_number (kv->value);

  if (passed_value == got_value)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   flow_control_string (got_value));
    }
  return ret;
}

static bmc_err_t
bit_rate_checkout (bmc_config_state_data_t *state_data,
		   const struct section *sect,
		   struct keyvalue *kv)
{
  uint8_t value;
  bmc_err_t ret;
  
  if ((ret = serial_conf_comm_checkout (state_data,
                                        NULL,
                                        NULL,
                                        &value)) != BMC_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (!(kv->value = strdup (bit_rate_string (value))))
    {
      perror("strdup");
      return BMC_ERR_FATAL_ERROR;
    }

  return BMC_ERR_SUCCESS;
}

static bmc_err_t
bit_rate_commit (bmc_config_state_data_t *state_data,
		 const struct section *sect,
		 const struct keyvalue *kv)
{
  uint8_t value = bit_rate_number (kv->value);
  return serial_conf_comm_commit (state_data,
				  NULL,
				  NULL,
				  &value);
}

static bmc_diff_t
bit_rate_diff (bmc_config_state_data_t *state_data,
	       const struct section *sect,
	       const struct keyvalue *kv)
{
  uint8_t passed_value;
  uint8_t got_value;
  bmc_err_t rc;
  bmc_diff_t ret;

  if ((rc = serial_conf_comm_checkout (state_data,
                                       NULL,
                                       NULL,
                                       &got_value)) != BMC_ERR_SUCCESS)
    {
      if (rc == BMC_ERR_NON_FATAL_ERROR)
        return BMC_DIFF_NON_FATAL_ERROR;
      return BMC_DIFF_FATAL_ERROR;
    }

  passed_value = bit_rate_number (kv->value);

  if (passed_value == got_value)
    ret = BMC_DIFF_SAME;
  else 
    {
      ret = BMC_DIFF_DIFFERENT;
      report_diff (sect->section_name,
                   kv->key,
                   kv->value,
                   bit_rate_string (got_value));
    }
  return ret;
}

struct section *
bmc_serial_conf_section_get (bmc_config_state_data_t *state_data)
{
  struct section *bmc_serial_conf_section = NULL;

  if (!(bmc_serial_conf_section = bmc_config_section_create(state_data, "Serial_Conf")))
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Enable_Basic_Mode",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_basic_mode_checkout,
                                       enable_basic_mode_commit,
                                       enable_basic_mode_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Enable_PPP_Mode",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_ppp_mode_checkout,
                                       enable_ppp_mode_commit,
                                       enable_ppp_mode_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Enable_Terminal_Mode",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_terminal_mode_checkout,
                                       enable_terminal_mode_commit,
                                       enable_terminal_mode_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Connect_Mode",
                                       "Possible values: Modem_Connect/Direct_Mode",
                                       0,
                                       connect_mode_checkout,
                                       connect_mode_commit,
                                       connect_mode_diff,
                                       connect_mode_number_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Page_Blackout_Interval",
                                       "Give a valid number",
                                       0,
                                       page_blackout_interval_checkout,
                                       page_blackout_interval_commit,
                                       page_blackout_interval_diff,
                                       number_range_one_byte) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Call_Retry_Interval",
                                       "Give a valid number",
                                       0,
                                       call_retry_interval_checkout,
                                       call_retry_interval_commit,
                                       call_retry_interval_diff,
                                       number_range_one_byte) < 0)
    goto cleanup;

  /* achu: For backwards compatability to bmc-config in 0.2.0 */
  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Call_Retry_Time",
                                       "Give a valid number",
                                       BMC_DO_NOT_CHECKOUT,
                                       call_retry_interval_checkout,
                                       call_retry_interval_commit,
                                       call_retry_interval_diff,
                                       number_range_one_byte) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Enable_DTR_Hangup",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_dtr_hangup_checkout,
                                       enable_dtr_hangup_commit,
                                       enable_dtr_hangup_diff,
                                       yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Flow_Control",
                                       "Possible values: No_Flow_Control/RTS_CTS/XON_XOFF",
                                       0,
                                       flow_control_checkout,
                                       flow_control_commit,
                                       flow_control_diff,
                                       flow_control_number_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       bmc_serial_conf_section,
                                       "Bit_Rate",
                                       "Possible values: 9600/19200/38400/57600/115200",
                                       0,
                                       bit_rate_checkout,
                                       bit_rate_commit,
                                       bit_rate_diff,
                                       bit_rate_number_validate) < 0)
    goto cleanup;

  return bmc_serial_conf_section;

 cleanup:
  if (bmc_serial_conf_section)
    bmc_config_section_destroy(state_data, bmc_serial_conf_section);
  return NULL;
}
