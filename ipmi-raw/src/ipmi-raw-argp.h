/* 
   $Id: ipmi-raw-argp.h,v 1.8 2007-01-22 22:12:02 chu11 Exp $ 
   
   ipmi-raw-argp.h - ipmi-raw command line argument parser.
   
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
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  
*/

#ifndef _IPMI_RAW_ARGP_H
#define _IPMI_RAW_ARGP_H

#include <limits.h>	/* ARG_MAX */

#define CMD_FILE_KEY     'f'

struct ipmi_raw_arguments
{
  struct common_cmd_args common;
  char *cmd_file;
  uint8_t cmd[ARG_MAX];
  int cmd_length;
};

void ipmi_raw_argp_parse (int argc, char **argv, struct ipmi_raw_arguments *cmd_args);

#endif
