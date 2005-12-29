/* 
   $Id: bmc-info-argp.h,v 1.1 2005-12-29 01:18:53 balamurugan Exp $ 
   
   bmc-info-argp.h - displays BMC information.
   
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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
*/

#ifndef _BMC_INFO_ARGP_H
#define _BMC_INFO_ARGP_H

struct arguments
{
  struct common_cmd_args common;
};

void bmc_info_argp_parse (int argc, char **argv);
struct arguments *bmc_info_get_arguments ();

#endif
