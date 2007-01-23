/* 

   bmc-checkout.h - function prototypes

   Copyright (C) 2006 FreeIPMI Core Team

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


#ifndef _BMC_CHECKOUT_H_
#define _BMC_CHECKOUT_H_

#include "bmc-config.h"
#include "bmc-sections.h"

int bmc_checkout (struct bmc_config_arguments *args, struct section *sections);

#endif /* _BMC_CHECKOUT_H_ */
