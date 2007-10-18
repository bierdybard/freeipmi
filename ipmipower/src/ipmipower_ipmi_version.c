/*****************************************************************************\
 *  $Id: ipmipower_ipmi_version.c,v 1.6 2007-10-18 16:18:52 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2003-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-155698
 *  
 *  This file is part of Ipmipower, a remote power control utility.
 *  For details, see http://www.llnl.gov/linux/.
 *  
 *  Ipmipower is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by the 
 *  Free Software Foundation; either version 2 of the License, or (at your 
 *  option) any later version.
 *  
 *  Ipmipower is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 *  for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmipower.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <assert.h>

#include "ipmipower_ipmi_version.h"
#include "ipmipower_wrappers.h"

ipmi_version_t 
ipmipower_ipmi_version_index(char *str) 
{
  assert(str != NULL);

  if (!strcasecmp(str, "auto"))
    return IPMI_VERSION_AUTO;
  else if (!strcasecmp(str, "1.5"))
    return IPMI_VERSION_1_5;
  else if (!strcasecmp(str, "2.0"))
    return IPMI_VERSION_2_0;
  else 
    return IPMI_VERSION_INVALID;
}

char *
ipmipower_ipmi_version_string(ipmi_version_t ipmi_version) 
{
  assert(IPMI_VERSION_VALID_OR_AUTO(ipmi_version));

  switch(ipmi_version) 
    {
    case IPMI_VERSION_AUTO:
      return "auto";
      break;
    case IPMI_VERSION_1_5:
      return "1.5";
      break;
    case IPMI_VERSION_2_0:
      return "2.0";
      break;
    default:
      err_exit("ipmipower_ipmi_version_string: Invalid Ipmi Version Type: %d\n", ipmi_version);
    }
  
  return NULL;                  /* NOT_REACHED */
}

char *
ipmipower_ipmi_version_list(void) 
{
  return "auto, 1.5, 2.0";
}
