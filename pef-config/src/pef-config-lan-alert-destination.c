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

#include "freeipmi-portability.h"

#define PEF_CONFIG_MAXIPADDRLEN 16
#define PEF_CONFIG_MAXMACADDRLEN 24

static config_err_t
destination_type_get (pef_config_state_data_t *state_data,
                      uint8_t destination_selector,
                      uint8_t *alert_destination_type,
                      uint8_t *alert_acknowledge,
                      uint8_t *alert_acknowledge_timeout,
                      uint8_t *alert_retries)
{
  uint8_t tmp_alert_destination_type;
  uint8_t tmp_alert_acknowledge;
  uint8_t tmp_alert_acknowledge_timeout;
  uint8_t tmp_alert_retries;
  config_err_t ret;

  if ((ret = get_bmc_destination_type (state_data,
                                       destination_selector,
                                       &tmp_alert_destination_type,
                                       &tmp_alert_acknowledge,
                                       &tmp_alert_acknowledge_timeout,
                                       &tmp_alert_retries)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (alert_destination_type)
    *alert_destination_type = tmp_alert_destination_type;
  if (alert_acknowledge)
    *alert_acknowledge = tmp_alert_acknowledge;
  if (alert_acknowledge_timeout)
    *alert_acknowledge_timeout = tmp_alert_acknowledge_timeout;
  if (alert_retries)
    *alert_retries = tmp_alert_retries;

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
destination_type_set (pef_config_state_data_t *state_data,
                      uint8_t destination_selector,
                      uint8_t alert_destination_type,
                      uint8_t alert_destination_type_is_set,
                      uint8_t alert_acknowledge,
                      uint8_t alert_acknowledge_is_set,
                      uint8_t alert_acknowledge_timeout,
                      uint8_t alert_acknowledge_timeout_is_set,
                      uint8_t alert_retries,
                      uint8_t alert_retries_is_set)
{
  uint8_t tmp_alert_destination_type;
  uint8_t tmp_alert_acknowledge;
  uint8_t tmp_alert_acknowledge_timeout;
  uint8_t tmp_alert_retries;
  config_err_t ret;

  if ((ret = get_bmc_destination_type(state_data,
                                      destination_selector,
                                      &tmp_alert_destination_type,
                                      &tmp_alert_acknowledge,
                                      &tmp_alert_acknowledge_timeout,
                                      &tmp_alert_retries)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (alert_destination_type_is_set)
    tmp_alert_destination_type = alert_destination_type;
  if (alert_acknowledge_is_set)
    tmp_alert_acknowledge = alert_acknowledge;
  if (alert_acknowledge_timeout_is_set)
    tmp_alert_acknowledge_timeout = alert_acknowledge_timeout;
  if (alert_retries_is_set)
    tmp_alert_retries = alert_retries;

  if ((ret = set_bmc_destination_type(state_data,
                                      destination_selector,
                                      tmp_alert_destination_type,
                                      tmp_alert_acknowledge,
                                      tmp_alert_acknowledge_timeout,
                                      tmp_alert_retries)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_destination_type_checkout (pef_config_state_data_t *state_data,
                                 const struct config_section *section,
                                 struct config_keyvalue *kv)
{
  uint8_t destination_type;
  config_err_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = destination_type_get (state_data,
                                   destination_selector,
                                   &destination_type,
                                   NULL,
                                   NULL,
                                   NULL)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (!(kv->value = strdup (alert_destination_type_string (destination_type))))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }
  
  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_destination_type_commit (pef_config_state_data_t *state_data,
                               const struct config_section *section,
                               const struct config_keyvalue *kv)
{
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  config_err_t ret;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  return destination_type_set (state_data,
                               destination_selector,
                               alert_destination_type_number (kv->value), 1,
                               0, 0,
                               0, 0,
                               0, 0);
}

static config_diff_t
alert_destination_type_diff (pef_config_state_data_t *state_data,
                             const struct config_section *section,
                             const struct config_keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((rc = get_number_of_lan_alert_destinations (state_data, 
                                                  &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = destination_type_get (state_data,
                                  destination_selector,
                                  &get_val,
                                  0,
                                  0,
                                  0)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  passed_val = alert_destination_type_number (kv->value);
  if (passed_val == get_val)
    ret = CONFIG_DIFF_SAME;
  else 
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   alert_destination_type_string (get_val));
    }
  return ret;
}

static config_err_t
alert_acknowledge_checkout (pef_config_state_data_t *state_data,
                            const struct config_section *section,
                            struct config_keyvalue *kv)
{
  uint8_t alert_acknowledge;
  config_err_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((ret = destination_type_get (state_data,
                                   destination_selector,
                                   NULL,
                                   &alert_acknowledge,
                                   NULL,
                                   NULL)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);
  
  if (alert_acknowledge)
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
alert_acknowledge_commit (pef_config_state_data_t *state_data,
                          const struct config_section *section,
                          const struct config_keyvalue *kv)
{
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  config_err_t ret;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  return destination_type_set (state_data,
                               destination_selector,
                               0, 0,
                               same (kv->value, "yes"), 1,
                               0, 0,
                               0, 0);
}

static config_diff_t
alert_acknowledge_diff (pef_config_state_data_t *state_data,
                        const struct config_section *section,
                        const struct config_keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));
  
  if ((rc = get_number_of_lan_alert_destinations (state_data, 
                                                  &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return rc;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((rc = destination_type_get (state_data,
                                  destination_selector,
                                  0,
                                  &get_val,
                                  0,
                                  0)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  passed_val = same (kv->value, "Yes");

  if (passed_val == get_val)
    ret = CONFIG_DIFF_SAME;
  else 
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   get_val ? "Yes" : "No");
    }
  return ret;
}

static config_err_t
alert_acknowledge_timeout_checkout (pef_config_state_data_t *state_data,
                                    const struct config_section *section,
                                    struct config_keyvalue *kv)
{
  uint8_t alert_acknowledge_timeout;
  config_err_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  
  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((ret = destination_type_get (state_data,
                                   destination_selector,
                                   NULL,
                                   NULL,
                                   &alert_acknowledge_timeout,
                                   NULL)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (kv->value)
    free (kv->value);

  if (asprintf (&kv->value, "%u", alert_acknowledge_timeout) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_acknowledge_timeout_commit (pef_config_state_data_t *state_data,
                                  const struct config_section *section,
                                  const struct config_keyvalue *kv)
{
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  config_err_t ret;
  uint8_t alert_acknowledge_timeout;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  alert_acknowledge_timeout = atoi (kv->value);

  return destination_type_set (state_data,
                               destination_selector,
                               0, 0,
                               0, 0,
                               alert_acknowledge_timeout, 1,
                               0, 0);
}

static config_diff_t
alert_acknowledge_timeout_diff (pef_config_state_data_t *state_data,
                                const struct config_section *section,
                                const struct config_keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));
  
  if ((rc = get_number_of_lan_alert_destinations (state_data, 
                                                  &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return rc;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((rc = destination_type_get (state_data,
                                  destination_selector,
                                  0,
                                  0,
                                  &get_val,
                                  0)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  passed_val = atoi (kv->value);

  if (passed_val == get_val)
    ret = CONFIG_DIFF_SAME;
  else 
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "%u", get_val);
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   num);
    }
  return ret;
}

static config_err_t
alert_retries_checkout (pef_config_state_data_t *state_data,
                        const struct config_section *section,
                        struct config_keyvalue *kv)
{
  uint8_t alert_retries;
  config_err_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  
  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((ret = destination_type_get (state_data,
                                   destination_selector,
                                   NULL,
                                   NULL,
                                   NULL,
                                   &alert_retries)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (kv->value)
    free (kv->value);
  
  if (asprintf (&kv->value, "%u", alert_retries) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_retries_commit (pef_config_state_data_t *state_data,
                      const struct config_section *section,
                      const struct config_keyvalue *kv)
{
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  config_err_t ret;
  uint8_t alert_retries;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  alert_retries = atoi (kv->value);

  return destination_type_set (state_data,
                               destination_selector,
                               0, 0,
                               0, 0,
                               0, 0,
                               alert_retries, 1);
}

static config_diff_t
alert_retries_diff (pef_config_state_data_t *state_data,
                    const struct config_section *section,
                    const struct config_keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));
  
  if ((rc = get_number_of_lan_alert_destinations (state_data, 
                                                  &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return rc;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((rc = destination_type_get (state_data,
                                  destination_selector,
                                  0,
                                  0,
                                  0,
                                  &get_val)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  passed_val = atoi (kv->value);

  if (passed_val == get_val)
    ret = CONFIG_DIFF_SAME;
  else 
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT;
      sprintf (num, "%u", get_val);
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   num);
    }
  return ret;
}

config_validate_t
alert_retries_validate (const char *section_name,
                        const char *key_name,
                        const char *value)
{
  return config_check_number_range(value, 0, IPMI_ALERT_RETRIES_MAX);
}

static config_err_t
destination_addresses_get (pef_config_state_data_t *state_data,
                           uint8_t destination_selector,
                           uint8_t *alert_gateway,
                           char *alert_ip_address,
                           unsigned int alert_ip_address_len,
                           char *alert_mac_address,
                           unsigned int alert_mac_address_len)
{
  uint8_t tmp_alert_gateway;
  char tmp_ip[PEF_CONFIG_MAXIPADDRLEN + 1];
  char tmp_mac[PEF_CONFIG_MAXMACADDRLEN+1];
  config_err_t ret;

  memset(tmp_ip, '\0', PEF_CONFIG_MAXIPADDRLEN + 1);
  memset(tmp_mac, '\0', PEF_CONFIG_MAXMACADDRLEN+1);

  if ((ret = get_bmc_destination_addresses (state_data,
                                            destination_selector,
                                            &tmp_alert_gateway,
                                            tmp_ip,
                                            PEF_CONFIG_MAXIPADDRLEN + 1,
                                            tmp_mac,
                                            PEF_CONFIG_MAXMACADDRLEN+1)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (alert_gateway)
    *alert_gateway = tmp_alert_gateway;
  if (alert_ip_address)
    { 
      if (alert_ip_address_len >= PEF_CONFIG_MAXIPADDRLEN + 1)
        memcpy(alert_ip_address, tmp_ip, PEF_CONFIG_MAXIPADDRLEN + 1);
      else
        return CONFIG_ERR_FATAL_ERROR;
    }
  if (alert_mac_address)
    { 
      if (alert_mac_address_len >= PEF_CONFIG_MAXMACADDRLEN + 1)
        memcpy(alert_mac_address, tmp_mac, PEF_CONFIG_MAXMACADDRLEN + 1);
      else
        return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
destination_addresses_set (pef_config_state_data_t *state_data,
                           uint8_t destination_selector,
                           uint8_t alert_gateway,
                           int alert_gateway_is_set,
                           char *alert_ip_address,
                           int alert_ip_address_is_set,
                           char *alert_mac_address,
                           int alert_mac_address_is_set)
{
  uint8_t tmp_alert_gateway;
  char tmp_ip[PEF_CONFIG_MAXIPADDRLEN + 1];
  char tmp_mac[PEF_CONFIG_MAXMACADDRLEN+1];
  char *tmp_ip_ptr;
  char *tmp_mac_ptr;
  config_err_t ret;

  memset(tmp_ip, '\0', PEF_CONFIG_MAXIPADDRLEN + 1);
  memset(tmp_mac, '\0', PEF_CONFIG_MAXMACADDRLEN+1);

  tmp_ip_ptr = tmp_ip;
  tmp_mac_ptr = tmp_mac;

  if ((ret = get_bmc_destination_addresses (state_data,
                                            destination_selector,
                                            &tmp_alert_gateway,
                                            tmp_ip_ptr,
                                            PEF_CONFIG_MAXIPADDRLEN+1,
                                            tmp_mac_ptr,
                                            PEF_CONFIG_MAXMACADDRLEN+1)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (alert_gateway_is_set)
    tmp_alert_gateway = alert_gateway;
  if (alert_ip_address && alert_ip_address_is_set)
    tmp_ip_ptr = alert_ip_address;
  if (alert_mac_address && alert_mac_address_is_set)
    tmp_mac_ptr = alert_mac_address;

  if ((ret = set_bmc_destination_addresses(state_data,
                                           destination_selector,
                                           tmp_alert_gateway,
                                           tmp_ip_ptr,
                                           tmp_mac_ptr)) != CONFIG_ERR_SUCCESS)
    return ret;

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_gateway_checkout (pef_config_state_data_t *state_data,
                        const struct config_section *section,
                        struct config_keyvalue *kv)
{
  uint8_t gateway;
  config_err_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  
  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((ret = destination_addresses_get (state_data,
                                        destination_selector,
                                        &gateway,
                                        NULL,
                                        0,
                                        NULL,
                                        0)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (kv->value)
    free (kv->value);
  
  if (!(kv->value = strdup (alert_gateway_string (gateway))))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }
  
  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_gateway_commit (pef_config_state_data_t *state_data,
                      const struct config_section *section,
                      const struct config_keyvalue *kv)
{
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  config_err_t ret;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  return destination_addresses_set (state_data,
                                    destination_selector,
                                    alert_gateway_number (kv->value), 1,
                                    NULL, 0,
                                    NULL, 0);
}

static config_diff_t
alert_gateway_diff (pef_config_state_data_t *state_data,
                    const struct config_section *section,
                    const struct config_keyvalue *kv)
{
  uint8_t get_val;
  uint8_t passed_val;
  config_err_t rc;
  config_diff_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((rc = get_number_of_lan_alert_destinations (state_data, 
                                                  &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = destination_addresses_get (state_data,
                                       destination_selector,
                                       &get_val,
                                       NULL,
                                       0,
                                       NULL,
                                       0)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  passed_val = alert_gateway_number (kv->value);
  if (passed_val == get_val)
    ret = CONFIG_DIFF_SAME;
  else 
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   alert_gateway_string (get_val));
    }
  return ret;
}

static config_err_t
alert_ip_address_checkout (pef_config_state_data_t *state_data,
                           const struct config_section *section,
                           struct config_keyvalue *kv)
{
  config_err_t ret;
  char alert_ip[PEF_CONFIG_MAXIPADDRLEN + 1];
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  
  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));
  
  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((ret = destination_addresses_get (state_data,
                                        destination_selector,
                                        NULL,
                                        alert_ip,
                                        PEF_CONFIG_MAXIPADDRLEN + 1,
                                        NULL,
                                        0)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (kv->value)
    free (kv->value);
  
  if (!(kv->value = strdup (alert_ip)))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }
  
  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_ip_address_commit (pef_config_state_data_t *state_data,
                         const struct config_section *section,
                         const struct config_keyvalue *kv)
{
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  config_err_t ret;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  return destination_addresses_set (state_data,
                                    destination_selector,
                                    0, 0,
                                    kv->value, 1,
                                    NULL, 0);
}

static config_diff_t
alert_ip_address_diff (pef_config_state_data_t *state_data,
                       const struct config_section *section,
                       const struct config_keyvalue *kv)
{
  char alert_ip[PEF_CONFIG_MAXIPADDRLEN + 1];
  config_err_t rc;
  config_diff_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((rc = get_number_of_lan_alert_destinations (state_data, 
                                                  &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = destination_addresses_get (state_data,
                                       destination_selector,
                                       NULL,
                                       alert_ip,
                                       PEF_CONFIG_MAXIPADDRLEN + 1,
                                       NULL,
                                       0)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  if (same (alert_ip, kv->value))
    ret = CONFIG_DIFF_SAME;
  else 
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   alert_ip);
    }
  return ret;
}

static config_err_t
alert_mac_address_checkout (pef_config_state_data_t *state_data,
                            const struct config_section *section,
                            struct config_keyvalue *kv)
{
  config_err_t ret;
  char alert_mac[PEF_CONFIG_MAXMACADDRLEN + 1];
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  
  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));
  
  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;
  
  if ((ret = destination_addresses_get (state_data,
                                        destination_selector,
                                        NULL,
                                        NULL,
                                        0,
                                        alert_mac,
                                        PEF_CONFIG_MAXMACADDRLEN + 1)) != CONFIG_ERR_SUCCESS)
    return ret;
  
  if (kv->value)
    free (kv->value);
  
  if (!(kv->value = strdup (alert_mac)))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }
  
  return CONFIG_ERR_SUCCESS;
}

static config_err_t
alert_mac_address_commit (pef_config_state_data_t *state_data,
                          const struct config_section *section,
                          const struct config_keyvalue *kv)
{
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;
  config_err_t ret;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((ret = get_number_of_lan_alert_destinations (state_data, 
                                                   &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  return destination_addresses_set (state_data,
                                    destination_selector,
                                    0, 0,
                                    NULL, 0,
                                    kv->value, 1);
}

static config_diff_t
alert_mac_address_diff (pef_config_state_data_t *state_data,
                        const struct config_section *section,
                        const struct config_keyvalue *kv)
{
  char alert_mac[PEF_CONFIG_MAXMACADDRLEN + 1];
  config_err_t rc;
  config_diff_t ret;
  uint8_t destination_selector;
  uint8_t number_of_lan_alert_destinations;

  destination_selector = atoi (section->section_name + strlen ("Lan_Alert_Destination_"));

  if ((rc = get_number_of_lan_alert_destinations (state_data, 
                                                  &number_of_lan_alert_destinations)) != CONFIG_ERR_SUCCESS)
    return rc;

  if (destination_selector > number_of_lan_alert_destinations)
    return CONFIG_ERR_NON_FATAL_ERROR;

  if ((rc = destination_addresses_get (state_data,
                                       destination_selector,
                                       NULL,
                                       NULL,
                                       0,
                                       alert_mac,
                                       PEF_CONFIG_MAXMACADDRLEN + 1)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }
  
  if (same (alert_mac, kv->value))
    ret = CONFIG_DIFF_SAME;
  else 
    {
      ret = CONFIG_DIFF_DIFFERENT;
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   alert_mac);
    }
  return ret;
}

struct config_section *
pef_config_lan_alert_destination_section_get (pef_config_state_data_t *state_data, int num)
{
  struct config_section *section = NULL;
  char buf[64];

  if (num <= 0)
    {
      fprintf(stderr, "Invalid Num = %d\n", num);
      return NULL;
    }

  snprintf(buf, 64, "Lan_Alert_Destination_%d", num);

  if (!(section = pef_config_section_create (state_data, 
                                          buf, 
                                          NULL, 
                                          NULL, 
                                          0)))
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       section,
                                       "Alert_Destination_Type",
                                       "Possible values: PET_Trap/OEM1/OEM2",
                                       0,
                                       alert_destination_type_checkout,
                                       alert_destination_type_commit,
                                       alert_destination_type_diff,
                                       alert_destination_type_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       section,
                                       "Alert_Acknowledge",
                                       "Possible values: Yes/No",
                                       0,
                                       alert_acknowledge_checkout,
                                       alert_acknowledge_commit,
                                       alert_acknowledge_diff,
                                       config_yes_no_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       section,
                                       "Alert_Acknowledge_Timeout",
                                       "Give valid unsigned number in seconds",
                                       0,
                                       alert_acknowledge_timeout_checkout,
                                       alert_acknowledge_timeout_commit,
                                       alert_acknowledge_timeout_diff,
                                       config_number_range_one_byte) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       section,
                                       "Alert_Retries",
                                       "Give valid unsigned number",
                                       0,
                                       alert_retries_checkout,
                                       alert_retries_commit,
                                       alert_retries_diff,
                                       alert_retries_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       section,
                                       "Alert_Gateway",
                                       "Possible values: Default/Backup",
                                       0,
                                       alert_gateway_checkout,
                                       alert_gateway_commit,
                                       alert_gateway_diff,
                                       alert_gateway_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       section,
                                       "Alert_IP_Address",
                                       "Give valid IP address",
                                       0,
                                       alert_ip_address_checkout,
                                       alert_ip_address_commit,
                                       alert_ip_address_diff,
                                       config_ip_address_validate) < 0) 
    goto cleanup;

  if (pef_config_section_add_keyvalue (state_data,
                                       section,
                                       "Alert_MAC_Address",
                                       "Give valid MAC address",
                                       0,
                                       alert_mac_address_checkout,
                                       alert_mac_address_commit,
                                       alert_mac_address_diff,
                                       config_mac_address_validate) < 0) 
    goto cleanup;

  return section;

 cleanup:
  if (section)
    pef_config_section_destroy(state_data, section);
  return NULL;
}

