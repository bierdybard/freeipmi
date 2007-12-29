#ifndef _IPMI_SEL_ENTRY_H
#define _IPMI_SEL_ENTRY_H

#include "ipmi-sel.h"

#define IPMI_SEL_RECORD_SIZE 16

int ipmi_sel_get_entry (ipmi_sel_state_data_t *state_data,
                        uint16_t record_id, 
                        uint16_t *next_record_id,
                        uint16_t *stored_record_id,
                        char **timestamp,
                        char **sensor_info,
                        char **event_message,
                        char **event_data2_message,
                        char **event_data3_message);

#endif
