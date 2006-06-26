/* 
   ipmi-ssif-api.h: IPMI - SMBus System Interface - SMS Api

   Copyright (C) 2005 FreeIPMI Core Team

   Based on ipmitool.c provided by Amitoj Singh <amitoj@fnal.gov> and 
   Don Holmgren <djholm@fnal.gov>

   Under GNU/Linux, requires i2c-dev, i2c-i801, i2c-core drivers version >= 2.8.7

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

#ifndef IPMI_SSIF_API_H
#define IPMI_SSIF_API_H

#include <stdint.h>

#define IPMI_DEFAULT_I2C_DEVICE        "/dev/i2c-0"
#define IPMI_DEFAULT_SSIF_IPMB_ADDR    0x42

#define IPMI_SSIF_SINGLE_PART_WRITE_SMBUS_CMD          0x02
#define IPMI_SSIF_MULTI_PART_WRITE_START_SMBUS_CMD     0x06
#define IPMI_SSIF_MULTI_PART_WRITE_MIDDLE_SMBUS_CMD    0x07
#define IPMI_SSIF_MULTI_PART_WRITE_END_SMBUS_CMD       0x07

#define IPMI_SSIF_SINGLE_PART_READ_SMBUS_CMD           0x03
#define IPMI_SSIF_MULTI_PART_READ_START_SMBUS_CMD      0x03
#define IPMI_SSIF_MULTI_PART_READ_MIDDLE_SMBUS_CMD     0x09
#define IPMI_SSIF_MULTI_PART_READ_END_SMBUS_CMD        0x09
#define IPMI_SSIF_MULTI_PART_READ_RETRY_SMBUS_CMD      0x0A

#define IPMI_SSIF_MULTI_PART_READ_START_SIZE        30
#define IPMI_SSIF_MULTI_PART_READ_START_PATTERN1    0x0
#define IPMI_SSIF_MULTI_PART_READ_START_PATTERN2    0x1
#define IPMI_SSIF_MULTI_PART_READ_END_PATTERN       0xFF

/* START: copied from <linux/i2c.h> and <linux/i2c-dev.h>, */
/*        and prefixed IPMI.                               */
#define IPMI_I2C_SLAVE               0x0703
#define IPMI_I2C_SMBUS               0x0720
#define IPMI_I2C_SMBUS_BLOCK_DATA    5
#define IPMI_I2C_SMBUS_BLOCK_MAX     32
#define IPMI_I2C_SMBUS_READ          1
#define IPMI_I2C_SMBUS_WRITE         0

union ipmi_i2c_smbus_data
{
  uint8_t  byte;
  uint16_t word;
  uint8_t  block[IPMI_I2C_SMBUS_BLOCK_MAX + 3];
};

struct ipmi_i2c_smbus_ioctl_data
{
  uint8_t  read_write;
  uint8_t  command;
  uint32_t size;
  union ipmi_i2c_smbus_data *data;
};
/* END: copied from <linux/i2c.h> and <linux/i2c-dev.h>, */
/*      and prefixed IPMI.                               */

#define IPMI_SSIF_CTX_ERR_SUCCESS         0
#define IPMI_SSIF_CTX_ERR_NULL            1
#define IPMI_SSIF_CTX_ERR_INVALID         2
#define IPMI_SSIF_CTX_ERR_PARAMETERS      3
#define IPMI_SSIF_CTX_ERR_PERMISSION      4
#define IPMI_SSIF_CTX_ERR_IO_PARAMETERS   5
#define IPMI_SSIF_CTX_ERR_IO_INIT         6
#define IPMI_SSIF_CTX_ERR_OVERFLOW        7
#define IPMI_SSIF_CTX_ERR_BUSY            8
#define IPMI_SSIF_CTX_ERR_OUTMEM          9
#define IPMI_SSIF_CTX_ERR_INTERNAL        10
#define IPMI_SSIF_CTX_ERR_ERRNUMRANGE     11

#define IPMI_SSIF_MODE_BLOCKING    0
#define IPMI_SSIF_MODE_NONBLOCKING 1
#define IPMI_SSIF_MODE_DEFAULT     IPMI_SSIF_MODE_BLOCKING

typedef struct ipmi_ssif_ctx *ipmi_ssif_ctx_t;

/* Notes:
 *
 * IPMBAddress - slave address of the BMC on the SMBus (usually 0x42)
 *
 */
ipmi_ssif_ctx_t ipmi_ssif_ctx_create(void);
int8_t ipmi_ssif_ctx_destroy(ipmi_ssif_ctx_t ctx);
int32_t ipmi_ssif_ctx_errnum(ipmi_ssif_ctx_t ctx);
char *ipmi_ssif_ctx_strerror(int32_t errnum);

int8_t ipmi_ssif_ctx_get_i2c_device(ipmi_ssif_ctx_t ctx, char **i2c_device);
int8_t ipmi_ssif_ctx_get_ipmb_address(ipmi_ssif_ctx_t ctx, uint8_t *ipmb_address);
int8_t ipmi_ssif_ctx_get_mode(ipmi_ssif_ctx_t ctx, uint8_t *mode);

int8_t ipmi_ssif_ctx_set_i2c_device(ipmi_ssif_ctx_t ctx, char *i2c_device);
int8_t ipmi_ssif_ctx_set_ipmb_address(ipmi_ssif_ctx_t ctx, uint8_t ipmb_address);
int8_t ipmi_ssif_ctx_set_mode(ipmi_ssif_ctx_t ctx, uint8_t mode);

int8_t ipmi_ssif_ctx_io_init(ipmi_ssif_ctx_t ctx);

int32_t ipmi_ssif_write (ipmi_ssif_ctx_t ctx,
			 uint8_t *buf,
			 uint32_t buf_len);

int32_t ipmi_ssif_read (ipmi_ssif_ctx_t ctx,
			uint8_t* buf,
			uint32_t buf_len);

#endif /* IPMI_SSIF_API_H */
