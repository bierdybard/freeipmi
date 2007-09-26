#ifndef _CONFIG_COMMON_H_
#define _CONFIG_COMMON_H_

#include <stdio.h>

#include "cmdline-parse-common.h"

#define CONFIG_CHECKOUT_KEY_COMMENTED_OUT                  0x01
#define CONFIG_CHECKOUT_KEY_COMMENTED_OUT_IF_VALUE_EMPTY   0x02
#define CONFIG_DO_NOT_CHECKOUT                             0x04

#define CONFIG_CHECKOUT_LINE_LEN                           45

#define CONFIG_PARSE_BUFLEN                                4096

#define SET_SELECTOR      0x0
#define BLOCK_SELECTOR    0x0

#define same(a,b) (strcasecmp(a,b) == 0)

typedef enum
  {
    CONFIG_ACTION_INFO = 1,
    CONFIG_ACTION_CHECKOUT,
    CONFIG_ACTION_COMMIT,
    CONFIG_ACTION_DIFF,
    CONFIG_ACTION_LIST_SECTIONS,
  } config_action_t;

typedef enum
  {
    CONFIG_ERR_FATAL_ERROR = -2,
    CONFIG_ERR_NON_FATAL_ERROR = -1,
    CONFIG_ERR_SUCCESS = 0,
  } config_err_t;

typedef enum
  {
    CONFIG_VALIDATE_FATAL_ERROR = -2,
    CONFIG_VALIDATE_INVALID_VALUE = -1,
    CONFIG_VALIDATE_VALID_VALUE = 0,
  } config_validate_t;

struct config_keypair
{
  char *section_name;
  char *key_name;
  char *value_input;
  struct config_keypair *next;
};

struct config_section_str
{
  char *section_name;
  struct config_section_str *next;
};

struct config_arguments
{
  struct common_cmd_args common;

  config_action_t action;

  int verbose;
  char *filename;
  struct config_keypair *keypairs;
  struct config_section_str *section_strs;
};

struct config_keyvalue {
  struct config_key *key;
  char *value_input;
  char *value_output;
  struct config_keyvalue *next;
};

/* Fills in kv->value_output as a printable string  */
typedef config_err_t (*Key_Checkout) (const char *section_name,
                                      struct config_keyvalue *kv,
                                      void *arg);

/* Takes kv->value_input and commits it */
typedef config_err_t (*Key_Commit) (const char *section_name,
                                    const struct config_keyvalue *kv,
                                    void *arg);

/* Determines if an inputted value is valid */
typedef config_validate_t (*Key_Validate) (const char *section_name,
                                           const char *key_name,
                                           const char *value);

struct config_key {
  char *key_name;
  char *description;
  unsigned int flags;
  Key_Checkout checkout;
  Key_Commit commit;
  Key_Validate validate;
  struct config_key *next;
};

struct config_section {
  char *section_name;
  char *section_comment_section_name;
  char *section_comment;
  unsigned int flags;
  /* keys in this section */
  struct config_key *keys;
  /* key and values for checkout/commit/diff */
  struct config_keyvalue *keyvalues;
  struct config_section *next;
};

#endif /* _CONFIG_COMMON_H_ */
