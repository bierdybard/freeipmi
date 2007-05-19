#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>
#include <assert.h>

#include "ipmi-pef.h"
#include "ipmi-pef-checkout.h"
#include "ipmi-pef-common.h"
#include "ipmi-pef-map.h"
#include "ipmi-pef-sections.h"
#include "ipmi-pef-utils.h"
#include "ipmi-pef-wrapper.h"

static pef_err_t
pef_checkout_section_common (ipmi_pef_state_data_t *state_data,
                             struct section *sect,
                             FILE *fp)
{
  struct ipmi_pef_arguments *args;
  struct keyvalue *kv;
  pef_err_t rv = PEF_ERR_FATAL_ERROR;
  pef_err_t ret = PEF_ERR_SUCCESS;

  fprintf (fp, "Section %s\n", sect->section_name);

  args = state_data->prog_data->args;
  kv = sect->keyvalues;

  while (kv)
    {
      pef_err_t this_ret;

      /* achu: Certain keys should be "hidden" and not be checked out.
       * They only linger for backwards compatability to FreeIPMI's
       * 0.2.0 bmc-config, which have several options with typoed
       * names.
       */
      if (kv->flags & PEF_DO_NOT_CHECKOUT)
        {
          kv = kv->next;
          continue;
        }

      if ((this_ret = kv->checkout (state_data,
                                    sect,
                                    kv)) == PEF_ERR_FATAL_ERROR)
        goto cleanup;

      if (this_ret == PEF_ERR_NON_FATAL_ERROR)
        {
          if (args->verbose)
            fprintf (fp, "\t## FATAL: Unable to checkout %s:%s\n",
                     sect->section_name,
                     kv->key);
          ret = PEF_ERR_NON_FATAL_ERROR;
        }
      else
        {
          int key_len = 0;

          fprintf (fp, "\t## %s\n", kv->desc);

          /* beauty comes at a cost */

          /* achu: Certain keys should have their checked out
           * value automatically commented out.  Sometimes (in the
           * case of passwords) they cannot be checked out, so the
           * default is for value to be empty.  We do not want the
           * user accidently commiting this checked out file,
           * which (in this example) clears the password.
           *
           * Some other keys may or may not have a value, depending on
           * the IPMI version or the implementation.
           */
          if (kv->flags & PEF_CHECKOUT_KEY_COMMENTED_OUT)
            key_len = fprintf(fp, "\t## %s", kv->key);
          else if (kv->flags & PEF_CHECKOUT_KEY_COMMENTED_OUT_IF_VALUE_EMPTY)
            {
              if (kv->value && strlen(kv->value))
                key_len = fprintf (fp, "\t%s", kv->key);
              else
                key_len = fprintf(fp, "\t## %s", kv->key);
            }
          else
            key_len = fprintf (fp, "\t%s", kv->key);

          while (key_len <= 45)
            {
              fprintf (fp, " ");
              key_len++;
            }

          fprintf (fp, "%s\n", kv->value);
        }

      kv = kv->next;
    }

  fprintf (fp, "EndSection\n");
  return ret;

 cleanup:
  return rv;
}

static pef_err_t
pef_checkout_section (ipmi_pef_state_data_t *state_data)
{
  struct ipmi_pef_arguments *args;
  struct sectionstr *sstr;
  FILE *fp;
  int file_opened = 0;
  pef_err_t rv = PEF_ERR_FATAL_ERROR;
  pef_err_t ret = PEF_ERR_SUCCESS;

  args = state_data->prog_data->args;

  if (args->filename && strcmp (args->filename, "-"))
    {
      if (!(fp = fopen (args->filename, "w")))
        {
          perror("fopen");
          goto cleanup;
        }
      file_opened++;
    }
  else
    fp = stdout;

  sstr = args->sectionstrs;
  while (sstr)
    {
      struct section *sect = state_data->sections;
      int found = 0;

      while (sect)
        {
          if (!strcasecmp(sect->section_name, sstr->sectionstr))
            {
              pef_err_t this_ret;

              found++;

              if ((this_ret = pef_checkout_section_common (state_data,
                                                           sect,
                                                           fp)) == PEF_ERR_FATAL_ERROR)
                goto cleanup;

              if (this_ret == PEF_ERR_NON_FATAL_ERROR)
                ret = PEF_ERR_NON_FATAL_ERROR;

              break;
            }

          sect = sect->next;
        }

      if (!found)
        {
          fprintf(stderr, "Invalid section `%s'\n", sstr->sectionstr);
          ret = 0;
        }

      sstr = sstr->next;
    }

  rv = ret;
 cleanup:
  if (file_opened)
    fclose(fp);
  return rv;
}

static pef_err_t
pef_checkout_file (ipmi_pef_state_data_t *state_data)
{
  struct ipmi_pef_arguments *args;
  struct section *sect;
  FILE *fp;
  int file_opened = 0;
  pef_err_t rv = PEF_ERR_FATAL_ERROR;
  pef_err_t ret = PEF_ERR_SUCCESS;

  args = state_data->prog_data->args;

  if (args->filename && strcmp (args->filename, "-"))
    {
      if (!(fp = fopen (args->filename, "w")))
        {
          perror("fopen");
          goto cleanup;
        }
      file_opened++;
    }
  else
    fp = stdout;

  sect = state_data->sections;
  while (sect)
    {
      pef_err_t this_ret;

      if ((this_ret = pef_checkout_section_common (state_data,
                                                   sect,
                                                   fp)) == PEF_ERR_FATAL_ERROR)
        goto cleanup;

      if (this_ret == PEF_ERR_NON_FATAL_ERROR)
        ret = PEF_ERR_NON_FATAL_ERROR;

      if (args->verbose)
        fprintf (stderr, "Completed checkout of Section: %s\n",
                 sect->section_name);

      sect = sect->next;
    }

  rv = ret;
 cleanup:
  if (file_opened)
    fclose(fp);
  return rv;
}

pef_err_t
pef_checkout (ipmi_pef_state_data_t *state_data)
{
  struct ipmi_pef_arguments *args;
  pef_err_t ret;

  args = state_data->prog_data->args;

  if (args->sectionstrs)
    ret = pef_checkout_section (state_data);
  else
    ret = pef_checkout_file (state_data);

  return ret;
}
