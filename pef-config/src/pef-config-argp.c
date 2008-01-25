/* 
   Copyright (C) 2005 FreeIPMI Core Team
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.  
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <argp.h>
#include <ctype.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#if HAVE_FCNTL_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */
#include <errno.h>

#include "tool-cmdline-common.h"
#include "pef-config.h"
#include "pef-config-argp.h"

static error_t parse_opt (int key, char *arg, struct argp_state *state);

const char *argp_program_version = 
"IPMI PEF [pef-config-" PACKAGE_VERSION "]\n"
"Copyright (C) 2003-2005 FreeIPMI Core Team\n"
"This program is free software; you may redistribute it under the terms of\n"
"the GNU General Public License.  This program has absolutely no warranty.";

const char *argp_program_bug_address = "<freeipmi-devel@gnu.org>";

static char doc[] = "PEF-CONFIG - PEF configuration utility.";

static char args_doc[] = "";

static struct argp_option options[] = 
  {
    ARGP_COMMON_OPTIONS_DRIVER,
    ARGP_COMMON_OPTIONS_INBAND,
    ARGP_COMMON_OPTIONS_OUTOFBAND,
    ARGP_COMMON_OPTIONS_AUTHENTICATION_TYPE,
    ARGP_COMMON_OPTIONS_PRIVILEGE_LEVEL_ADMIN,
    ARGP_COMMON_OPTIONS_WORKAROUND_FLAGS,
    ARGP_COMMON_OPTIONS_DEBUG,
    {"info",       INFO_KEY,       0, 0, 
     "Show general information about PEF configuration.", 30},
    {"checkout",   CHECKOUT_KEY,   0, 0,
     "Fetch configuration information from the BMC.", 31},
    {"commit",     COMMIT_KEY,     0, 0,
     "Update configuration information to the BMC from a config file or key pairs.", 32},
    {"diff",       DIFF_KEY,       0, 0,
     "Show differences between the BMC and a config file.", 33},
    {"filename", FILENAME_KEY, "FILENAME", 0,
     "Specify a PEF config file for PEF checkout/commit/diff.", 34},
    {"key-pair", KEYPAIR_KEY, "KEY-PAIR", 0,
     "Specify KEY=VALUE pairs for checkout/commit/diff.", 35},
    {"section", SECTIONS_KEY, "SECTION", 0,
     "Specify a SECTION for checkout.", 36},
    {"listsections", LIST_SECTIONS_KEY, 0, 0,
     "List available sections for checkout.", 37},
    {"verbose", VERBOSE_KEY, 0, 0,  
     "Print additional detailed information.", 38},
    { 0 }
  };

static struct argp argp = { options, parse_opt, args_doc, doc };

static error_t 
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct config_arguments *cmd_args = state->input;
  struct config_keypair *kp = NULL;
  struct config_section_str *sstr = NULL;
  char *section_name = NULL;
  char *key_name = NULL;
  char *value = NULL;
  
  switch (key)
    {
    case INFO_KEY:
      if (!cmd_args->action)
        cmd_args->action = CONFIG_ACTION_INFO;
      else
        cmd_args->action = -1;
      break;
    case CHECKOUT_KEY:
      if (!cmd_args->action)
        cmd_args->action = CONFIG_ACTION_CHECKOUT;
      else
        cmd_args->action = -1;
      break;
    case COMMIT_KEY:
      if (!cmd_args->action)
        cmd_args->action = CONFIG_ACTION_COMMIT;
      else
        cmd_args->action = -1;
      break;
    case DIFF_KEY:
      if (!cmd_args->action)
        cmd_args->action = CONFIG_ACTION_DIFF;
      else
        cmd_args->action = -1;
      break;
    case FILENAME_KEY:
      if (cmd_args->filename) /* If specified more than once */
        free (cmd_args->filename);
      if (!(cmd_args->filename = strdup (arg)))
        {
          perror("strdup");
          exit(1);
        }
      break;
    case KEYPAIR_KEY:
      if (config_keypair_parse_string(arg,
                                      &section_name,
                                      &key_name,
                                      &value) < 0)
        {
          /* error printed in function call */
          exit(1);
        }
      if (!(kp = config_keypair_create(section_name,
                                       key_name,
                                       value)))
        {
          fprintf(stderr,
                  "config_keypair_create error\n");
          exit(1);
        }
      if (config_keypair_append(&(cmd_args->keypairs),
                                kp) < 0)
        {
          /* error printed in function call */
          exit(1);
        }
      if (section_name)
        free(section_name);
      section_name = NULL;
      if (key_name)
        free(key_name);
      key_name = NULL;
      if (value)
        free(value);
      value = NULL;
      kp = NULL;
      break;
    case SECTIONS_KEY:
      if (!(sstr = config_section_str_create(arg)))
        {
          fprintf(stderr,
                  "config_section_str_create error\n");
          exit(1);
        }
      if (config_section_str_append(&(cmd_args->section_strs),
                                    sstr) < 0)
        {
          /* error printed in function call */
          exit(1);
        }
      sstr = NULL;
      break;
    case LIST_SECTIONS_KEY:
      if (!cmd_args->action)
        cmd_args->action = CONFIG_ACTION_LIST_SECTIONS;
      else
        cmd_args->action = -1;
      break;
    case VERBOSE_KEY:
      cmd_args->verbose = 1;
      break;
    case ARGP_KEY_ARG:
      /* Too many arguments. */
      argp_usage (state);
      break;
    case ARGP_KEY_END:
      break;
    default:
      return common_parse_opt (key, arg, state, 
			       &(cmd_args->common));
    }
  
  return 0;
}

void 
pef_config_argp_parse (int argc, char **argv, struct config_arguments *cmd_args)
{
  init_common_cmd_args (&(cmd_args->common));
  cmd_args->action = 0;
  cmd_args->verbose = 0;
  cmd_args->filename = NULL;
  cmd_args->keypairs = NULL;
  cmd_args->section_strs = NULL;

  /* ADMIN is minimum for pef-config b/c its needed for many of the
   * ipmi cmds used
   */
  cmd_args->common.privilege_level = IPMI_PRIVILEGE_LEVEL_ADMIN;
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, NULL, cmd_args);
  verify_common_cmd_args (&(cmd_args->common));
}

int
pef_config_args_validate (struct config_arguments *cmd_args)
{
  int ret = 0;

  // action is non 0 and -1
  if (! cmd_args->action || cmd_args->action == -1)
    {
      fprintf (stderr,
               "Exactly one of --info, --checkout, --commit, --diff, or --listsections MUST be given\n");
      return -1;
    }
  
  // filename and keypair both given for checkout or diff
  if (cmd_args->filename && cmd_args->keypairs
      && cmd_args->action == CONFIG_ACTION_DIFF)
    {
      fprintf (stderr,
               "Both --filename or --keypair cannot be used\n");
      return -1;
    }

  // only one of keypairs or section can be given for checkout
  if (cmd_args->action == CONFIG_ACTION_CHECKOUT
      && (cmd_args->keypairs && cmd_args->section_strs))
    {
      fprintf (stderr,
               "Only one of --filename, --keypair, and --section can be used\n");
      return -1;
    }

  if (cmd_args->filename)
    {
      switch (cmd_args->action)
        {
        case CONFIG_ACTION_COMMIT: case CONFIG_ACTION_DIFF:
          if (access (cmd_args->filename, R_OK) != 0)
            {
              perror (cmd_args->filename);
              return -1;
            }
          break;
        case CONFIG_ACTION_CHECKOUT:
          if (access (cmd_args->filename, F_OK) == 0)
            {
              if (access (cmd_args->filename, W_OK) != 0)
                {
                  perror (cmd_args->filename);
                  return -1;
                }
            }
          else
            {
              int fd;
              fd = open (cmd_args->filename, O_CREAT, 0644);
              if (fd == -1)
                {
                  perror (cmd_args->filename);
                  return -1;
                }
              else
                {
                  close (fd);
                  unlink (cmd_args->filename);
                }
            }
          break;
        case CONFIG_ACTION_INFO:
        case CONFIG_ACTION_LIST_SECTIONS:
          /* do nothing - here to remove compile warning */
          break;
        }
    }

  return ret;
}

