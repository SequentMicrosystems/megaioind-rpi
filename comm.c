/*
 * comm.c:
 *	Communication routines "platform specific" for Raspberry Pi
 *	
 *	Copyright (c) 2016-2020 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "comm.h"
#include "megaioind.h"

#define I2C_SLAVE	0x0703
#define I2C_SMBUS	0x0720	/* SMBus-level access */

#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

// SMBus transaction types

#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */
#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages

#define I2C_SMBUS_BLOCK_MAX	512	/* As specified in SMBus standard */
#define I2C_SMBUS_I2C_BLOCK_MAX	512	/* Not specified but we use same structure */


int i2cSetup(int addr)
{
	int file;
	char filename[40];
	sprintf(filename, "/dev/i2c-1");

	if ( (file = open(filename, O_RDWR)) < 0)
	{
		printf("Failed to open the bus.");
		return -1;
	}
	if (ioctl(file, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		return -1;
	}

	return file;
}

int i2cMem8Read(int dev, int add, uint8_t* buff, int size)
{
	uint8_t intBuff[I2C_SMBUS_BLOCK_MAX];

	if (NULL == buff)
	{
		return -1;
	}

	if (size > I2C_SMBUS_BLOCK_MAX)
	{
		return -1;
	}

	intBuff[0] = 0xff & add;

	if (write(dev, intBuff, 1) != 1)
	{
		//printf("Fail to select mem add!\n");
		return -1;
	}
	if (read(dev, buff, size) != size)
	{
		//printf("Fail to read memory!\n");
		return -1;
	}
	return 0; //OK
}

int i2cMem8Write(int dev, int add, uint8_t* buff, int size)
{
	uint8_t intBuff[I2C_SMBUS_BLOCK_MAX];

	if (NULL == buff)
	{
		return -1;
	}

	if (size > I2C_SMBUS_BLOCK_MAX - 1)
	{
		return -1;
	}

	intBuff[0] = 0xff & add;
	memcpy(&intBuff[1], buff, size);

	if (write(dev, intBuff, size + 1) != size + 1)
	{
		//printf("Fail to write memory!\n");
		return -1;
	}
	return 0;
}


int readReg8(int dev, int add)
{

	int ret;
	char buf[10];

	buf[0] = 0xff & add;

	if (write(dev, buf, 1) != 1)
	{
		printf("Fail to select mem add\n");
		return -1;
	}

	if (read(dev, buf, 1) != 1)
	{
		printf("Fail to read reg\n");
		return -1;
	}
	ret = 0xff & buf[0];
	return ret;
}

int readReg16(int dev, int add)
{
	int ret = 0;
	char buf[10];
	char buf_o[2];

	buf_o[0] = 0xff & add;

	if (write(dev, buf_o, 1) != 1)
	{
		printf("Fail to select mem add\n");
		return -1;
	}

	if (read(dev, buf, 1) != 1)
	{
		printf("Fail to read reg\n");
		return -1;
	}

	buf_o[0] = 0xff & (add + 1);

	if (write(dev, buf_o, 1) != 1)
	{
		printf("Fail to select mem add\n");
		return -1;
	}

	if (read(dev, &buf[1], 1) != 1)
	{
		printf("Fail to read reg\n");
		return -1;
	}
	memcpy(&ret, buf, 2);

	return ret;
}

int readReg32(int dev, int add)
{
	uint32_t ret = 0;
	char buf[10];

	buf[0] = 0xff & add;

	if (write(dev, buf, 1) != 1)
	{
		printf("Fail to select mem add\n");
		return -1;
	}

	if (read(dev, buf, 4) != 4)
	{
		printf("Fail to read reg\n");
		return -1;
	}
	memcpy(&ret, buf, 4);

	return ret;
}

int writeReg8(int dev, int add, int val)
{
	char buf[10];

	buf[0] = 0xff & add;
	buf[1] = 0xff & val;

	if (write(dev, buf, 2) < 0)
	{
		printf("Fail to w8\n");
		return -1;
	}
	return OK;

}

int writeReg16(int dev, int add, int val)
{
	char buf[10];

	buf[0] = 0xff & add;
	memcpy(&buf[1], &val, 2);

	if (write(dev, buf, 3) < 0)
	{
		printf("Fail to w16\n");
		return FAIL;
	}
	return OK;
}

int writeReg32(int dev, int add, int val)
{
	char buf[10];

	buf[0] = 0xff & add;
	memcpy(&buf[1], &val, 4);

	if (write(dev, buf, 5) < 0)
	{
		printf("Fail to w16\n");
		return FAIL;
	}
	return OK;
}


int readReg24(int dev, int add)
{
	int ret = 0;
	char buf[10];

	buf[0] = 0xff & add;

	if (write(dev, buf, 1) != 1)
	{
		printf("Fail to select mem add\n");
		return FAIL;
	}

	if (read(dev, buf, 3) != 2)
	{
		printf("Fail to read reg\n");
		return FAIL;
	}
	memcpy(&ret, buf, 3);

#ifdef DEBUG_I
	printbits(ret);
	printf("\n");
	printf("%#08x\n", ret);
#endif
	return ret;
}

int writeReg24(int dev, int add, int val)
{
	char buf[10];

	buf[0] = 0xff & add;
	memcpy(&buf[1], &val, 3);

	if (write(dev, buf, 4) < 0)
	{
		printf("Fail to w24\n");
		return FAIL;
	}
	return OK;

}

int doBoardInit(int hwAdd, int* type)
{
	int dev, bV = -1;
	dev = i2cSetup (hwAdd);
	if(dev == -1)
	{
		return ERROR;
	}
	bV = readReg8 (dev,BOARD_TYPE_MEM_ADD);
	if(bV == -1)
	{
		printf( "MegaIO Industrial id %d not detected\n", hwAdd - MEGAIO_HW_I2C_BASE_ADD);
		return ERROR;
	}
	*type = bV;

	return dev;
}
