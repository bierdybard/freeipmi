#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */

#include "ipmi-pef.h"
#include "ipmi-pef-common.h"
#include "ipmi-pef-diff.h"
#include "ipmi-pef-parser.h"
#include "ipmi-pef-sections.h"

#if 0
/* XXX come back to this later */

static pef_diff_t
pef_diff_keypair (ipmi_pef_state_data_t *state_data,
                  struct keypair *kp)
{
  char *keypair = NULL;
  char *section_name;
  char *key_name;
  char *value;
  pef_diff_t rv = PEF_DIFF_FATAL_ERROR;
  
  if (!(keypair = strdup (kp->keypair)))
    {
      perror("strdup");
      goto cleanup;
    }

  section_name = strtok (keypair, ":");
  key_name = strtok (NULL, "=");
  value = strtok (NULL, "");

  if (!(section_name && key_name && value)) 
    {
      fprintf (stderr, "Invalid KEY-PAIR spec `%s'\n", kp->keypair);
      rv = PEF_DIFF_NON_FATAL_ERROR; 
      goto cleanup;
    }
     
  section_name = strtok (section_name, " \t");
  key_name = strtok (key_name, " \t");
  value = strtok (value, " \t");

  rv = ipmi_pef_section_diff_value (state_data,
                                    section_name, 
                                    key_name,
                                    value);
 cleanup:
  if (keypair)
    free (keypair);
  return rv;
}

static pef_diff_t
pef_diff_file (ipmi_pef_state_data_t *state_data)
{
  struct ipmi_pef_arguments *args;
  FILE *fp;
  int file_opened = 0;
  pef_diff_t rv = PEF_DIFF_FATAL_ERROR;
  pef_diff_t ret = PEF_DIFF_SAME;
  pef_diff_t this_ret;

  args = state_data->prog_data->args;

  if (args->filename && strcmp (args->filename, "-"))
    {
      if (!(fp = fopen (args->filename, "r")))
        {
          perror("fopen");
          goto cleanup;
        }
      file_opened++;
    }
  else
    fp = stdin;

  /* 1st pass */
  if ((this_ret = bmc_parser (state_data, fp)) == PEF_DIFF_FATAL_ERROR)
    goto cleanup;

  if (this_ret == PEF_DIFF_NON_FATAL_ERROR)
    ret = PEF_DIFF_NON_FATAL_ERROR;

  if (ret == PEF_DIFF_SAME) 
    {
      /* 2nd pass if 1st pass was successful */
      struct section *sect = state_data->sections;
      while (sect) 
        {
          struct keyvalue *kv = sect->keyvalues;
          while (kv) 
            {
              if (kv->value) 
                {
                  if ((this_ret = kv->diff (state_data, sect, kv)) == PEF_DIFF_FATAL_ERROR)
                    goto cleanup;

                  if (this_ret == PEF_DIFF_NON_FATAL_ERROR)
                    ret = PEF_DIFF_NON_FATAL_ERROR;
                }
              kv = kv->next;
            }
          sect = sect->next;
        }
    }

  rv = ret;
 cleanup:
  if (file_opened)
    fclose(fp);
  return rv;
}
#endif

pef_err_t
pef_diff (ipmi_pef_state_data_t *state_data)
{
  struct ipmi_pef_arguments *args;
  pef_diff_t ret;

  args = state_data->prog_data->args;

#if 0
  /* XXX */
  ret = pef_diff_file (state_data);
#endif

  if (ret == PEF_DIFF_SAME)
    return PEF_ERR_SUCCESS;
  if (ret == PEF_DIFF_DIFFERENT || ret == PEF_DIFF_NON_FATAL_ERROR)
    return PEF_ERR_NON_FATAL_ERROR;
  return PEF_ERR_FATAL_ERROR;
}

void 
report_diff (const char *section, 
	     const char *key, 
	     const char *input_value,
	     const char *actual_value)
{
  printf ("%s:%s - input=`%s':actual=`%s'\n",
	  section, key, input_value, actual_value);
}

