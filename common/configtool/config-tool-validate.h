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

#ifndef _CONFIG_TOOL_VALIDATE_H_
#define _CONFIG_TOOL_VALIDATE_H_

#include <stdio.h>

#include "config-tool-common.h"

config_validate_t config_yes_no_validate (const char *section_name,
                                          const char *key_name,
                                          const char *value,
                                          void *arg);

config_validate_t config_check_number_range (const char *value,
                                             int min,
                                             int max);

config_validate_t config_number_range_three_bits (const char *section_name,
                                                  const char *key_name,
                                                  const char *value,
                                                  void *arg);

config_validate_t config_number_range_four_bits (const char *section_name,
                                                 const char *key_name,
                                                 const char *value,
                                                 void *arg);

config_validate_t config_number_range_seven_bits (const char *section_name,
                                                  const char *key_name,
                                                  const char *value,
                                                  void *arg);

config_validate_t config_number_range_twelve_bits (const char *section_name,
                                                   const char *key_name,
                                                   const char *value,
                                                   void *arg);

config_validate_t config_number_range_one_byte (const char *section_name,
                                                const char *key_name,
                                                const char *value,
                                                void *arg);

config_validate_t config_number_range_one_byte_non_zero (const char *section_name,
                                                         const char *key_name,
                                                         const char *value,
                                                         void *arg);

config_validate_t config_number_range_two_bytes (const char *section_name,
                                                 const char *key_name,
                                                 const char *value,
                                                 void *arg);

config_validate_t config_ip_address_validate (const char *section_name,
                                              const char *key_name,
                                              const char *value,
                                              void *arg);

config_validate_t config_mac_address_validate (const char *section_name,
                                               const char *key_name,
                                               const char *value,
                                               void *arg);

#endif /* _CONFIG_TOOL_VALIDATE_H_ */