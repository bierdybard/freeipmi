/* 
   smbios-locate.c - SMBIOS driver to locate IPMI interfaces.

   Copyright (C) 2003, 2004, 2005 FreeIPMI Core Team

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

/* SMBIOS Reference Specification: map area between 000f0000 and
   000fffff.  The IPMI Entry Structure begins on a 16-byte boundary,
   with a 4 byte "_SM_" signature.  */

#include "freeipmi.h"

fiid_template_t tmpl_smbios_ipmi_device_info_record =
  {
    {8, "cmd"}, 
    {8, "comp_code"}, 

    {8, "type"}, /* IPMI Device Information structure indicator. value = 38
		    (Note this number is given in decimal) */
    {8, "length"}, /* Length of the structure, a minimum of 10h (for
		      full IPMI address description, this is a minimum
		      of 12h) */
    {16, "handle"}, 
    {8, "interface_type"}, /* Baseboard Management Controller (BMC)
			      interface type, see Table C1-2, Interface Type
			      field values. */
    {4, "ipmi_spec_revision.minor"}, /* Somewhat mis-named. Actually identifies the
					IPMI Specification Version, in BCD format, to
					which the BMC was designed. Bits 7:4 hold the
					most significant digit of the version, 
					while bits 3:0 hold the least significant bits,
					e.g. a value of 15h indicates version 1.5. */ 
    {4, "ipmi_spec_revision.major"},
    {8, "i2c_slave_address"}, /* The slave address on the I2C bus of this BMC. */
    {8, "nv_storage_device_address"}, /* Bus id of the NV storage device. If no storage
					 device exists for this BMC, the field is set to
					 0FFh. */
    {1, "base_addr.io_mapped_or_mem_mapped"}, 
    {15, "base_address"}, /* Identifies the base address (either memory-
			     mapped or I/O) of the BMC. If the least-
			     significant bit of the field is a 1, the address is
			     in I/O space; otherwise, the address is
			     memory-mapped. Refer to the IPMI Interface
			     Specification for usage details. */
    {1, "interrupt_trigger_mode"}, /* bit 0 - Interrupt Trigger Mode.
				      1b = level, 0b = edge. */
    {1, "interrupt_polarity"}, /* bit 1 - Interrupt Polarity.
				  1b = active high, 0b = active low. */
    {1, "base_address_modifier_interrupt_info.reserved1"}, /* bit 2 - reserved. 
							      Return as 0b. */
    {1, "interrupt_info"}, /* Interrupt Info
			      Identifies the type and polarity of the
			      interrupt associated with the IPMI system
			      interface, if any.
			      bit 3 - 1b = interrupt info specified
			      0b = interrupt info not specified */
    {1, "ls_bit_for_addresses"}, /* bit 4 - LS-bit for addresses
				    0b = Address bit 0 = 0b
				    1b = Address bit 0 = 1b */
    {1, "base_address_modifier_interrupt_info.reserved2"}, /* bit 5 - reserved. 
							      Return as 0b. */
    {2, "register_spacing"}, /* bit 7:6 - Register spacing
				00b = interface registers are on
				successive byte boundaries
				01b = interface registers are on 32-
				bit boundaries
				10b = interface registers are on 16-
				byte boundaries
				11b = reserved */
    {8, "interrupt_number"}, /* Interrupt number for IPMI System Interface.
				00h = unspecified / unsupported */
    {0, ""}
  };


int
ipmi_smbios_reg_space (u_int8_t reg_space_boundary, u_int8_t *reg_space)
{
  extern int errno;
  if (!(reg_space))
    {
      *reg_space = 0;
      errno = EINVAL;
      return (-1);
    }

  switch (reg_space_boundary)
    {
    case IPMI_SMBIOS_REG_SPACE_1BYTE_BOUND:
      *reg_space = 0x01;
      return (0);
    case IPMI_SMBIOS_REG_SPACE_4BYTE_BOUND:
      *reg_space = 0x04;
      return (0);
    case IPMI_SMBIOS_REG_SPACE_16BYTE_BOUND:
      *reg_space = 0x10;
      return (0);
    case IPMI_SMBIOS_REG_SPACE_RESERVED:
    default:
      *reg_space = 0;
      errno = EINVAL;
      return (-1);
    }
}


/* SMBIOS Reference Specification: map area between 000f0000 and
   000fffff.  The IPMI Entry Structure begins on a 16-byte boundary,
   with a 4 byte "_SM_" signature.  */

/* is_ipmi_entry
   ARGUMENTS:
   sigp = points to start of purported SMBIOS entry structure
   RETURNS:
   0 = not really a SMBIOS entry structure
   1 = yes, a real SMBIOS entry structure */ 
static int
is_ipmi_entry (u_int8_t* sigp)
{
  static const char smbios_entry_sig[4] = { '_', 'S', 'M', '_' };
  static const char smbios_entry_anchor[5] = { '_', 'D', 'M', 'I', '_' };
  u_int32_t csum_computed;
  u_int8_t csum_given;
  u_int8_t entry_len;
  u_int8_t* bp;

  if (memcmp (sigp, smbios_entry_sig, sizeof (smbios_entry_sig)) != 0)
    return 0;

  entry_len = sigp[IPMI_SMBIOS_ENTRY_LEN_OFFSET];

  csum_given = sigp[IPMI_SMBIOS_ENTRY_CSUM_OFFSET];
  csum_computed = 0;
  for (bp = sigp; bp < sigp + entry_len; bp++)
    csum_computed = (csum_computed + (*bp)) % (1 << CHAR_BIT);

  if (memcmp (sigp + IPMI_SMBIOS_ENTRY_ANCHOR_OFFSET, smbios_entry_anchor,
	      sizeof (smbios_entry_anchor)) != 0)
    return (0);

  csum_given = sigp[IPMI_SMBIOS_ENTRY_ANCHOR_CSUM_OFFSET];
  csum_computed = 0;
  for (bp = sigp + IPMI_SMBIOS_ENTRY_ANCHOR_CSUM_OFFSET; bp < sigp + entry_len; bp++)
    csum_computed = (csum_computed + (*bp)) % (1 << CHAR_BIT);
    
  return (1);
}


/* is_ipmi_dev_info
   ARGUMENTS:
   type = which interface (KCS, SMIC, BT)
   dev_info_p = points to start of purported IPMI device info structure
   RETURNS:
   0 = not a IPMI device info structure for TYPE
   1 = yes, IPMI device info structure for TYPE */
static int
is_ipmi_dev_info (ipmi_interface_type_t type, u_int8_t* dev_info_p)
{
  if (*dev_info_p != IPMI_SMBIOS_IPMI_DEV_INFO_SIG)
    return 0;

  if (dev_info_p[IPMI_SMBIOS_IPMI_DEV_INFO_TYPE_OFFSET] != type)
    return 0;

  return 1;
}

/* map_physmem
   ARGUMENTS:
   physaddr = physical address to access
   len = length of area to access
   startp = place to store pointer to unmap (caller responsible for unmapping)
   totallen = length of area to unmap
   RETURNS:
   pointer to area of physical memory at physmem */
static u_int8_t*
map_physmem (u_int32_t physaddr, size_t len, void** startp, size_t* totallen)
{
  u_int32_t startaddr;
  u_int32_t pad;
  int mem_fd;

  if (startp == NULL || totallen == NULL)
    return NULL;

  mem_fd = open ("/dev/mem", O_RDONLY|O_SYNC);

  if (mem_fd != -1)
    {
      pad = physaddr % sysconf (_SC_PAGESIZE);
      startaddr = physaddr - pad;
      *totallen = len + pad;
      *startp = mmap (NULL, *totallen, PROT_READ, MAP_PRIVATE, mem_fd, startaddr);
      close (mem_fd);

      if (*startp != MAP_FAILED)
	return ((u_int8_t*)(*startp) + pad);
    }
  return NULL;	  
}

/* copy_impi_dev_info
   ARGUMENTS:
   type = which interface (KCS, SMIC, BT)
   RETURNS:
   pointer to the device info structure in heap (caller responsible
   for freeing */
static u_int8_t*
copy_impi_dev_info (ipmi_interface_type_t type)
{
  int status;
  u_int8_t* result = NULL;
  void* map_entry;
  size_t map_entry_len;
  u_int8_t* pmem_entry; 

  status = 1;
  pmem_entry = map_physmem (IPMI_SMBIOS_AREA_START, IPMI_SMBIOS_AREA_LEN, &map_entry, &map_entry_len);
  if (pmem_entry != NULL)
    {
      u_int8_t* sigp;
      for (sigp = pmem_entry; sigp - pmem_entry < IPMI_SMBIOS_AREA_LEN; sigp += IPMI_SMBIOS_AREA_ALIGN)
	{
	  if (is_ipmi_entry (sigp))
	    {
	      u_int16_t s_table_len;
	      u_int8_t* pmem_table;
	      void* map_table;
	      size_t map_table_len;

	      s_table_len = *(u_int16_t*)(sigp + IPMI_SMBIOS_ENTRY_TLEN_OFFSET);
	      pmem_table = map_physmem (*(u_int32_t*)(sigp + IPMI_SMBIOS_ENTRY_PTR_OFFSET), s_table_len,
					&map_table, &map_table_len);
	      if (pmem_table != NULL)
		{
		  u_int8_t* dev_info_p;
		  size_t size;
		  u_int8_t* var_info_p;

		  dev_info_p = pmem_table;
		  size = dev_info_p[IPMI_SMBIOS_DEV_INFO_LEN_OFFSET];
		  while (dev_info_p - pmem_table < s_table_len)
		    {
		      if (is_ipmi_dev_info (type, dev_info_p))
			{
			  result = ipmi_xmalloc (size);
			  if (result != NULL)
			    {
			      status = 0;
			      memcpy (result, dev_info_p, size);
			    }
			  else
			    {
			      errno = ENOMEM;
			      status = -1;
			    }
			  break;
			}
		      var_info_p = dev_info_p + size;
		      while (var_info_p[0] != 0 || var_info_p[1] != 0)
			var_info_p++;
		      dev_info_p = var_info_p + 2;
		      size = dev_info_p[IPMI_SMBIOS_DEV_INFO_LEN_OFFSET];
		    }
		  munmap (map_table, map_table_len);
		}
	    }
	  if (status <= 0)
	    break;
	}
      munmap (map_entry, map_entry_len);
    }
  return (status == 0 ? result : NULL);
}

/* get_smbios_dev_info
   ARGUMENTS:
   type = which interface (KCS, SMIC, BT)
   pinfo = pointer to information structure filled in by this function
   RETURNS:
   pinfo if successful, NULL otherwise */
ipmi_locate_info_t*
smbios_get_dev_info (ipmi_interface_type_t type, ipmi_locate_info_t* pinfo)
{
  u_int8_t* bufp;
  u_int8_t version;
  u_int64_t addr;
  u_int64_t strobed;

  bufp = copy_impi_dev_info (type);
  if (bufp == NULL)
    return (NULL);

  pinfo->locate_driver_type = IPMI_LOCATE_DRIVER_SMBIOS;

  version = bufp[IPMI_SMBIOS_IPMI_DEV_INFO_VER_OFFSET];
  pinfo->ipmi_ver_major = (version >> 4) & 0xf;
  pinfo->ipmi_ver_minor = version & 0xf;

  pinfo->interface_type = bufp[IPMI_SMBIOS_IPMI_DEV_INFO_TYPE_OFFSET];
  if (pinfo->interface_type != type)
    {
      free (bufp);
      errno = ENODEV;
      return (NULL);
    }

  strobed = addr = *(u_int64_t*)(bufp+IPMI_SMBIOS_IPMI_DEV_INFO_ADDR_OFFSET);

  if (bufp[IPMI_SMBIOS_DEV_INFO_LEN_OFFSET] > IPMI_SMBIOS_IPMI_DEV_INFO_MODIFIER_OFFSET)
    {
      u_int8_t modifier;
      u_int8_t lsb;
      int reg_space_boundary;

      modifier = bufp[IPMI_SMBIOS_IPMI_DEV_INFO_MODIFIER_OFFSET];
      lsb = (modifier >> IPMI_SMBIOS_LSB_BIT) & 1;
      strobed = (strobed & ~1) | lsb;

      reg_space_boundary = (modifier >> IPMI_SMBIOS_REGSPACING_SHIFT) & IPMI_SMBIOS_REGSPACING_MASK;
      if (ipmi_smbios_reg_space (reg_space_boundary, &pinfo->reg_space) == -1)
	{
	  free (bufp);
	  return (NULL);
	}
    }

  if (pinfo->interface_type == IPMI_INTERFACE_SSIF)
    {
      pinfo->addr_space_id  = IPMI_ADDRESS_SPACE_ID_SMBUS;
      pinfo->base_addr.bmc_smbus_slave_addr = bufp[IPMI_SMBIOS_IPMI_DEV_INFO_I2C_OFFSET];
    }
  else
    {
      if ((addr & 1) != 0)
	{
	  pinfo->addr_space_id = IPMI_ADDRESS_SPACE_ID_SYSTEM_IO;
	  pinfo->base_addr.bmc_iobase_addr = strobed;
	}
      else
	{
	  pinfo->addr_space_id = IPMI_ADDRESS_SPACE_ID_SYSTEM_MEMORY;
	  pinfo->base_addr.bmc_membase_addr = strobed;
	}
    }

  free (bufp);
  return (pinfo);
}

#ifdef IPMI_SMBIOS_MAIN
int
main (int argc, char** argv)
{
  smbios_dev_info_t idi;
  int status;

  if (smbios_get_dev_info (atoi(argv[1]), &idi, &status) == &idi)
    {
      printf ("%s", "Success!\n");
      printf ("interface type = %d\n", idi.type);
      printf ("version = %d.%d\n", idi.version.major, idi.version.minor);
      printf ("i2c slave address = %x\n", idi.i2c_slave_addr);
      printf ("nvstor address = %x\n", idi.nvstor_dev_addr);
      printf ("BMC io memory mapped = %s\n", (idi.bmc_io_mapped ? "true" : "false"));
      if (idi.bmc_io_mapped)
	printf ("BMC memory mapped registers start = %llx\n", idi.base.bmc_membase_addr);
      else
	printf ("BMC IO registers start = %llx\n", idi.base.bmc_iobase_addr);
      printf ("BMC register address spacing = %d\n", 1 << idi.reg_spacing);
      if (idi.intr.intinfo_specified)
	{
	  printf ("BMC interrupt polarity = %s\n", (idi.intr.intinfo_polarity_high ? "high" : "low"));
	  printf ("BMC interrupt trigger = %s\n", (idi.intr.intinfo_trigger_level ? "level" : "edge"));
	}
      printf ("BMC interrupt number = %x\n", idi.intr_num);
      exit (0);
    }
  else
    {
      printf ("%s", "Failure :-(\n");
      exit (1);
    }
}
#endif
