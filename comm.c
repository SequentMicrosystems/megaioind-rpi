/*
 * comm.c:
 *	Communication routines "platform specific" for Raspberry Pi
 *	
 *	Copyright (c) 2016-2018 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include "megaioind.h"
#include <string.h>

static volatile int globalResponse = 0;


PI_THREAD (waitForKey)
{
 char resp;
 int respI = NO;

 
	struct termios info;
	tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
	info.c_lflag &= ~ICANON;      /* disable canonical mode */
	info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
	info.c_cc[VTIME] = 0;         /* no timeout */
	tcsetattr(0, TCSANOW, &info); /* set i */

	(void)piHiPri (10) ;	// Set this thread to be high priority
	resp = getchar();
	if((resp == 'y') || (resp == 'Y'))
		respI = YES;
	
    piLock (COUNT_KEY) ;
	globalResponse = respI ;
    piUnlock (COUNT_KEY) ;
	
	info.c_lflag |= ICANON;      /* disable canonical mode */
	info.c_cc[VMIN] = 0;          /* wait until at least one keystroke available */
	info.c_cc[VTIME] = 0;         /* no timeout */
	tcsetattr(0, TCSANOW, &info); /* set i */
	printf("\n");
	return &waitForKey;
}

void startThread(void)
{
	wiringPiSetupSys ();
	piThreadCreate (waitForKey);
}

int checkThreadResult(void)
{
	int res;
	piLock (COUNT_KEY) ;
	res = globalResponse;
	piUnlock(COUNT_KEY);
	return res;
}
	
int readReg16(int dev, int add)
{
	int val;
	
	val = wiringPiI2CReadReg16(dev, add);
	
	return val;
}

int writeReg16(int dev, int add, int val)
{
	int ret;
 
	ret = wiringPiI2CWriteReg16(dev,add, val);
	return ret;
}


int writeReg8(int dev, int add, int val)
{
  int ret;
  
	ret = wiringPiI2CWriteReg8(dev, add, val);
	
	return ret;
}

int readReg8(int dev, int add)
{
	return wiringPiI2CReadReg8(dev, add);
}


int readReg24(int dev, int add)
{
	int val, aux8;
	
	aux8 = readReg8(dev, add + 2);
	val = aux8;
	/*val = 0xffff00 & (val << 8);*/
	aux8 = readReg8(dev, add + 1);
	val += 0xff00 & (aux8 << 8);
	
	aux8 = readReg8(dev, add );
	val += 0xff0000 & (aux8 << 16);
#ifdef DEBUG_I	
	printbits(val);
	printf("\n");
	printf("%#08x\n", val);
#endif
	return val;
}



int writeReg24(int dev, int add, int val)
{
	int wVal;//, aux8;
	
	wVal = 0xff & (val >> 8);
	writeReg8(dev,add+1, wVal);
	
	wVal = 0xff & ( val >> 16);
	writeReg8(dev,add, wVal);
	
	wVal = 0xff & val;
	writeReg8(dev,add+2, wVal);
	
	return 0;
}

	
int doBoardInit(int hwAdd)
{
	int dev, bV = -1;
	dev = wiringPiI2CSetup (hwAdd);
	if(dev == -1)
	{
		return ERROR;
	}
	bV = wiringPiI2CReadReg8 (dev,REVISION_HW_MAJOR_MEM_ADD);
	if(bV == -1)
	{
		printf( "MegaIO Industrial id %d not detected\n", hwAdd - MEGAIO_HW_I2C_BASE_ADD);
		return ERROR;
	}
	return dev;
}



void busyWait(int ms)
{
	delay(ms);
}
	
