/*
 * megaio.c:
 *	Command-line interface to the Raspberry
 *	Pi's MegaIO board.
 *	Copyright (c) 2016-2017 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include "megaioind.h"

#define OK 1
#define ERROR -1

#define V_IN_SCALE_FACTOR	(float)1000

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)1
#define VERSION_MINOR	(int)5

#define RTC_MIN_DAY		1
#define RTC_MAX_DAY		31
#define RTC_MIN_MONTH	1
#define RTC_MAX_MONTH	12
#define RTC_MIN_YEAR  2000
#define RTC_MAX_YEAR  2255
#define RTC_MIN_HOUR	0
#define RTC_MAX_HOUR	23
#define RTC_MIN_MIN		0
#define RTC_MAX_MIN		59
#define RTC_MIN_SEC		0
#define RTC_MAX_SEC		59

enum{
	MB_TYPE_NONE = 0,
	MB_TYPE_RTU,
	MB_TYPE_ASCII
};

enum{
	MB_PARITY_NONE = 0,
	MB_PARITY_ODD,
	MB_PARITY_EVEN
};
enum{
	MB_SB_1 = 0,
	MB_SB_2,
	MB_SB_1_5,
};


int gHwAdd = MEGAIO_HW_I2C_BASE_ADD;


typedef struct	__attribute__((packed))
{
	unsigned int mbBaud :26;
	unsigned int mbType :2;
	unsigned int mbParity :2;
	unsigned int mbStopB :2;
} ModbusSetingsType;

int extractInt(char* str, int pos, int len);
int extractDate(char* str, RtcStructType* rtc);
int extractTime(char* str, RtcStructType* rtc);

const char* gMbType[3] = {"NONE", "RTU", "ASCII"};
const char* gMbParity[3] = {"NONE", "ODD", "EVEN"};
const char* gMbStopBits[3] = {"ONE", "TWO", "ONE_AND_HALF"};

			  
char *warranty ="	       Copyright (c) 2016-2019 Sequent Microsystems\n"
				"                                                             \n"
				"		This program is free software; you can redistribute it and/or modify\n"
				"		it under the terms of the GNU Leser General Public License as published\n"
				"		by the Free Software Foundation, either version 3 of the License, or\n"
				"		(at your option) any later version.\n"
				"                                    \n"
				"		This program is distributed in the hope that it will be useful,\n"
				"		but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
				"		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
				"		GNU Lesser General Public License for more details.\n"
				"			\n"
				"		You should have received a copy of the GNU Lesser General Public License\n"
				"		along with this program. If not, see <http://www.gnu.org/licenses/>.";
				


				
char *failStr = 	"    ########    ###    #### ##       #### \n"
          			"    ##         ## ##    ##  ##       #### \n"
          			"    ##        ##   ##   ##  ##       #### \n"
          			"    ######   ##     ##  ##  ##        ##  \n"
          			"    ##       #########  ##  ##            \n"
          			"    ##       ##     ##  ##  ##       #### \n"
          			"    ##       ##     ## #### ######## #### \n";
				
char *passStr = "    ########     ###     ######   ######  \n"
        				"    ##     ##   ## ##   ##    ## ##    ## \n"
        				"    ##     ##  ##   ##  ##       ##       \n"
        				"    ########  ##     ##  ######   ######  \n"
        				"    ##        #########       ##       ## \n"
        				"    ##        ##     ## ##    ## ##    ## \n"
        				"    ##        ##     ##  ######   ######  \n";
						
char *indConnStr = 	"------------------------------------------------------------\n"
"|	          Rasperry Pi Connector                    |\n"
"|           o o o o o o o o o o o o o o o o o o o o        |\n"
"|      Pin1-o o o o o o o o o o o o o o o o o o o o        |\n"
"------------------------------------------------------------\n"
"|o| +24                 |o||o||o||o||o||o|    0-10V OUT4 |o|\n"
"|o| GND                  1  G  C  C  4  4           GND  |o|\n"
"|o| 0-10V IN4            W  N  A  A  8  8     0-10V OUT3 |o|\n"
"|o| 0-10V IN3            I  D  N  N  5  5           GND  |o|\n"
"|o| 0-10V IN2            R     |  |  |  |     0-10V OUT2 |o|\n"
"|o| GND                  E     L  H  A  B           GND  |o|\n"
"|o| 0-10V IN1                                 0-10V OUT1 |o|\n"
"|o| GND                                             GND  |o|\n"
"------------------------------------------------------------\n"
"|o| 4-20mA IN4-                                      GND |o|\n"
"|o| 4-20mA IN4+                              4-20mA OUT4 |o|\n"
"|o| 4-20mA IN3-                                      GND |o|\n"
"|o| 4-20mA IN3+                              4-20mA OUT3 |o|\n"
"|o| 4-20mA IN2-                                      GND |o|\n"
"|o| 4-20mA IN2+                              4-20mA OUT2 |o|\n"
"|o| 4-20mA IN1-                                      GND |o|\n"
"|o| 4-20mA IN1+                              4-20mA OUT1 |o|\n"
"------------------------------------------------------------\n"
"------------------------------------------------------------\n"
"|o| OPTO IN4-                            OPEN DRAIN OUT4 |o|\n"
"|o| OPTO IN4+                                        GND |o|\n"
"|o| OPTO IN3-                            OPEN DRAIN OUT3 |o|\n"
"|o| OPTO IN3+                                        GND |o|\n"
"|o| OPTO IN2-                            OPEN DRAIN OUT2 |o|\n"
"|o| OPTO IN2+                                        GND |o|\n"
"|o| OPTO IN1-                            OPEN DRAIN OUT1 |o|\n"
"|o| OPTO IN1+                                        GND |o|\n"
"------------------------------------------------------------\n"
"------------------------------------------------------------\n"
"|  |O||O||O|     |O||O||O|     |O||O||O|     |O||O||O|     |\n"
"|   R  R  R       R  R  R       R  R  R       R  R  R      |\n"
"|   1  1  1       2  2  2       3  3  3       4  4  4      |\n"
"|   N  C  N       N  C  N       N  C  N       N  C  N      |\n"
"|   O  O  C       O  O  C       O  O  C       O  O  C      |\n"
"|      M             M             M             M         |\n"
"------------------------------------------------------------\n";

char *basConnStr = 	"------------------------------------------------------------\n"
"|	          Rasperry Pi Connector                    |\n"
"|           o o o o o o o o o o o o o o o o o o o o        |\n"
"|      Pin1-o o o o o o o o o o o o o o o o o o o o        |\n"
"------------------------------------------------------------\n"
"|o| +24                 |o||o||o||o||o||o|    0-10V OUT4 |o|\n"
"|o| GND                  1  G  C  C  4  4           GND  |o|\n"
"|o| 0-10V IN4            W  N  A  A  8  8     0-10V OUT3 |o|\n"
"|o| 0-10V IN3            I  D  N  N  5  5           GND  |o|\n"
"|o| 0-10V IN2            R     |  |  |  |     0-10V OUT2 |o|\n"
"|o| GND                  E     L  H  A  B           GND  |o|\n"
"|o| 0-10V IN1                                 0-10V OUT1 |o|\n"
"|o| GND                                             GND  |o|\n"
"------------------------------------------------------------\n"
"|o| 10K TH IN4-                                      GND |o|\n"
"|o| 10K TH IN4+                               0-10V OUT8 |o|\n"
"|o| 10K TH IN3-                                      GND |o|\n"
"|o| 10K TH IN3+                               0-10V OUT7 |o|\n"
"|o| 10K TH IN2-                                      GND |o|\n"
"|o| 10K TH IN2+                               0-10V OUT6 |o|\n"
"|o| 10K TH IN1-                                      GND |o|\n"
"|o| 10K TH IN1+                               0-10V OUT5 |o|\n"
"------------------------------------------------------------\n"
"------------------------------------------------------------\n"
"|o| OPTO IN4-                            OPEN DRAIN OUT4 |o|\n"
"|o| OPTO IN4+                                        GND |o|\n"
"|o| OPTO IN3-                            OPEN DRAIN OUT3 |o|\n"
"|o| OPTO IN3+                                        GND |o|\n"
"|o| OPTO IN2-                            OPEN DRAIN OUT2 |o|\n"
"|o| OPTO IN2+                                        GND |o|\n"
"|o| OPTO IN1-                            OPEN DRAIN OUT1 |o|\n"
"|o| OPTO IN1+                                        GND |o|\n"
"------------------------------------------------------------\n"
"------------------------------------------------------------\n"
"|  |O||O||O|     |O||O||O|     |O||O||O|     |O||O||O|     |\n"
"|   R  R  R       R  R  R       R  R  R       R  R  R      |\n"
"|   1  1  1       2  2  2       3  3  3       4  4  4      |\n"
"|   N  C  N       N  C  N       N  C  N       N  C  N      |\n"
"|   O  O  C       O  O  C       O  O  C       O  O  C      |\n"
"|      M             M             M             M         |\n"
"------------------------------------------------------------\n";

void printbits(int v) 
{
	int i = 0; // for C89 compatibility
	
	for(i = 17; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

int extractInt(char* str, int pos, int len)
{
  char aux[10];
  
  memset(aux, 0, 10);
  memcpy(aux, &str[pos], len);
  //printf("%s\n", aux);
  
  return atoi(aux);
}

int extractDate(char* str, RtcStructType* rtc)
{
  int i;
  
  i = extractInt(str, 3, 2);
  if(i < RTC_MIN_DAY && i > RTC_MAX_DAY)
  {
    return ERROR;
  }
  rtc->d = (uint8_t)i;
  
  i = extractInt(str, 0, 2);
  if(i < RTC_MIN_MONTH && i > RTC_MAX_MONTH)
  {
    return ERROR;  
  }
  rtc->m = (uint8_t)i;
  
  i = extractInt(str, 6, 4);
  if(i < RTC_MIN_YEAR && i > RTC_MAX_YEAR)
  {
    return ERROR;
  }
  rtc->y = (uint8_t)(i - 2000);
  
  return OK;
}

int extractTime(char* str, RtcStructType* rtc)
{
  int i;
  
  i = extractInt(str, 0, 2);
  if(i < RTC_MIN_HOUR && i > RTC_MAX_HOUR)
  {
    return ERROR;
  }
  rtc->h = (uint8_t)i;
  
  i = extractInt(str, 3, 2);
  if(i < RTC_MIN_MIN && i > RTC_MAX_MIN)
  {
    return ERROR;  
  }
  rtc->min = (uint8_t)i;
  
  i = extractInt(str, 6, 2);
  if(i < RTC_MIN_SEC && i > RTC_MAX_SEC)
  {
    return ERROR;
  }
  rtc->s = (uint8_t)i;
  
  return OK;
}

// set ON/OFF specify relay channel
int relayChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp = 0;
	
	if((channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NR_MAX))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}
	switch(state)
	{
	case OFF:
		resp = writeReg8(dev,RELAY_CLR_ADD, channel);
		break;
	case ON:
		resp = writeReg8(dev,RELAY_SET_ADD, channel);
		break;
		
	default:
		printf("Invalid relay state!\n");
		return ERROR;
		break;
	}
	if(0 <= resp)
	{
		return OK;
	}
	return FAIL;
}


/*
* doRelayWrite:
*	Write coresponding relay channel
**************************************************************************************
*/
static void doRelayWrite(int argc, char *argv[])
{
	int pin = 0;
  int val = 0;
  int dev = 0;
  int valR = 0;
  int valRmask = 0;
  int retry = 0;
  int bType = 0;

	if ((argc != 5) && (argc != 4))
	{
		printf( "Usage: megaioind <id> wrelay <relay number> <on/off> \n") ;
		printf( "Usage: megaioind <id> wrelay <relay reg value> \n") ;
		exit (1) ;
	}

	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(argc == 5)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NR_MAX))
		{
			printf( "0-10V Input channel number %d out of range, valid [%d, %d]\n", pin, 
            CHANNEL_NR_MIN, V_IN_CH_MAX);
			exit(1);
		}

		/**/ if ((strcasecmp (argv [4], "up") == 0) || (strcasecmp (argv [4], "on") == 0))
			val = ON ;
		else if ((strcasecmp (argv [4], "down") == 0) || (strcasecmp (argv [4], "off") == 0))
			val = OFF ;
		else
		{
			val = atoi (argv [4]) ;
		}
		relayChSet(dev, pin, val);
		valR = readReg8(dev, RELAY_VAL_ADD);
		
		valRmask = 0x01 & (valR >> (pin - 1));
		retry = RETRY_TIMES;

		while((retry > 0) && (valRmask != val))
		{
			relayChSet(dev, pin, val);
			valR = readReg8(dev, RELAY_VAL_ADD);
			valRmask = 0x01 & (valR >> (pin - 1));
			retry--;
		}
		#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
		#endif
		if(retry == 0)
		{
			printf( "Fail to write relay\n");
			exit(1);
		}
	}
	else
	{
		val = atoi (argv [3]) ;
		if(val < 0 || val > 255)
		{
			printf( "Invalid relay value\n");
			exit(1);
		}
		retry = RETRY_TIMES;
		valR = -1;
		while((retry > 0) && (valR != val))
		{			
			writeReg8(dev, RELAY_VAL_ADD, val);
			valR = readReg8(dev, RELAY_VAL_ADD);
		}
		if(retry == 0)
		{
			printf( "Fail to write relay\n");
			exit(1);
		}
	}

}


/*
* doRelayRead:
*	Read relay state
******************************************************************************************
*/
static void doRelayRead(int argc, char *argv[])
{
	int pin = 0;
  int val = 0;
  int dev = 0;
  int bType = 0;

	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}

	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NR_MAX))
		{
			printf( "Relay number value out of range\n");
			exit(1);
		}
		val = readReg8(dev, RELAY_VAL_ADD);
		if(val < 0)
		{
			printf("Fail to read\n");
			exit(1);
		}

		val = val & (1 << (pin-1));
		if(val != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if(argc == 3)
	{
		val = readReg8(dev, RELAY_VAL_ADD);
		if(val < 0)
		{
			printf("Fail to read\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf( "Usage: %s read relay value\n", argv [0]) ;
		exit (1) ;
	}
}


/*
* do420InRead:
* Read 4-20mA channel
***********************************************************************************
*/
static void do420InRead(int argc, char *argv[])
{
	int pin = 0;
  int dev = 0;
  short int rd = 0;
  int bType = 0;
  float val = 0;

	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(bType != BOARD_TYPE_IND)
	{
		printf("Valid for Industrial board type only!\n");
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > CRT_IN_CH_NR_MAX))
		{
			printf( "4-20mA Input channel number %d out of range\n", pin);
			exit(1);
		}
		rd = readReg16(dev, I4_20_IN_VAL1_ADD + 2*( pin -1));
    val = rd/CRT_SCALE_FACTOR_MA;
		printf("%0.3f\n", val);
	}
	else 
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	} 
}

/*
* do420OutRead:
* Read 4-20mA output channel
***********************************************************************************
*/
static void do420OutRead(int argc, char *argv[])
{
  int dev = 0;
  int bType = 0;
	int pin = 0;
  short int rd = 0;
	float val = 0;

	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(bType != BOARD_TYPE_IND)
	{
		printf("Valid for Industrial board type only!\n");
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > CRT_OUT_CH_NR_MAX))
		{
			printf( "4-20mA Output channel number %d out of range\n", pin);
			exit(1);
		}
		rd = readReg16(dev, I4_20_OUT_VAL1_ADD + 2*( pin -1));
    val = rd/CRT_SCALE_FACTOR_MA;
		printf("%0.3f\n", val);
	}
	else 
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	} 
}

/*
* do420OutWrite:
*    Write the 4-20mA out channel value
***********************************************************************************************
*/

static void do420OutWrite(int argc, char *argv[])
{
  int dev = 0;
  short int rawVal = 0;
  int pin= 0;
  int bType = 0;
  float val = 0;
  int ret = 0;
	
	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(bType != BOARD_TYPE_IND)
	{
		printf("Valid for Industrial board type only!\n");
		exit(1);
	}
	if (argc != 5)
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	}
 	pin = atoi (argv [3]) ;
	if((pin < CHANNEL_NR_MIN) || (pin > CRT_IN_CH_NR_MAX))
	{
	  printf( "4-20mA Output channel number %d out of range\n", pin);
    exit(1);
  }
	val = atof(argv[4]);
  rawVal = (int)(val * CRT_SCALE_FACTOR_MA);
	if((rawVal < CRT_OUT_VAL_MIN) || (rawVal > CRT_OUT_VAL_MAX))
	{
		printf( "4 - 20 mA  value out of range\n");
		exit(1);
	}
	ret = writeReg16(dev, I4_20_OUT_VAL1_ADD + 2 * (pin -1), rawVal );
	if(ret == ERROR)
	{
		printf("Fail to write err: %d\n", ret);
		exit(1);
	}
}


/*
* do010InRead:
* Read 0-10V in channel
***********************************************************************************
*/
static void do010InRead(int argc, char *argv[])
{
  int dev = 0;
  int bType = 0;
	int pin = 0;
  short int rd = 0;
  float val = 0;

	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > V_IN_CH_MAX))
		{
			printf( "0-10V Input channel number %d out of range, valid [%d, %d]\n", pin, 
            CHANNEL_NR_MIN, V_IN_CH_MAX);
			exit(1);
		}
		rd = readReg16(dev, U0_10_IN_VAL1_ADD + 2*( pin -1));
		val = rd/U_SCALE_FACTOR_V;
		printf("%0.3f\n", val);
	}
	else 
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	} 
}

/*
* do010OutRead:
* Read 0-10V output channel
***********************************************************************************
*/
static void do010OutRead(int argc, char *argv[])
{
	int pin = 0;
  int dev = 0;
  short int rd = 0;
  int bType = 0;
  int maxCh = V_OUT_CH_MAX;
	float val = 0;

	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(bType == BOARD_TYPE_BAS)
	{
		maxCh = BAS_V_OUT_CH_MAX;
	}
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > maxCh))
		{
			printf( "0-10V Output channel number %d out of range\n", pin);
			exit(1);
		}
		rd = readReg16(dev, U0_10_OUT_VAL1_ADD + 2*( pin -1));
		val = rd/U_SCALE_FACTOR_V;
		printf("%0.3f\n", val);
	}
	else 
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	} 
}

/*
* do420OutWrite:
*    Write the 0-10V out channel value
***********************************************************************************************
*/
static void do010OutWrite(int argc, char *argv[])
{
  int dev = 0;
  short int rawVal = 0;
  int pin = 0;
  int bType = 0;
  int maxCh = V_OUT_CH_MAX;
  float val = 0;
  int ret = 0;
	
	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(bType == BOARD_TYPE_BAS)
	{
		maxCh = BAS_V_OUT_CH_MAX;
	}
	if (argc != 5)
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	}
 	pin = atoi (argv [3]) ;
	if((pin < CHANNEL_NR_MIN) || (pin > maxCh))
	{
		printf( "0-10V Output channel number %d out of range\n", pin);
		exit(1);
	}
	val = atof(argv[4]);
	rawVal = (int)(val * U_SCALE_FACTOR_V);
	if((rawVal < U_OUT_VAL_MIN) || (rawVal > U_OUT_VAL_MAX))
	{
		printf( "0-10V  value out of range\n");
		exit(1);
	}
	ret = writeReg16(dev, U0_10_OUT_VAL1_ADD + 2 * (pin -1), rawVal );
	if(ret == ERROR)
	{
		printf("Fail to write, err: %d\n", ret);
		exit(1);
	}
}

/*
* doResInRead:
*	Read the input resisence (NTC 10k)
************************************************************************************************
*/

static void doResInRead(int argc, char *argv[])
{
	int dev = 0;
  short int rawVal = 0;
  int pin = 0;
  int bType = 0;
	
	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(bType != BOARD_TYPE_BAS)
	{
		printf("Valid for Building Automation board type only!\n");
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > R_CH_NR_MAX))
		{
			printf( "Res in channel number %d out of range\n", pin);
			exit(1);
		}
		rawVal = readReg16(dev, R_10K_CH1 + 2*( pin -1));
		
		printf("%d\n", rawVal);
	}
	else 
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	} 
}


/*
* doOptoInRead:
*	Read the optocupled input pins
************************************************************************************************
*/

static void doOptoInRead(int argc, char *argv[])
{
  int pin = 0;
  int val = 0;
  int dev = 0;
  int bType = 0;

	dev = doBoardInit(gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
  
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > OPTO_CH_NR_MAX))
		{
			printf( "Opto In channel out of range\n");
			exit(1);
		}

		val = readReg8(dev, OPTO_VAL_ADD);
		val = val & (1 << (pin-1));
		if(val != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if(argc == 3)
	{
		val = readReg8(dev, OPTO_VAL_ADD);
		printf("%d\n", val);
	}
	else
	{
		printf( "Usage: %s read opto pins \n", argv [0]) ;
		exit (1) ;
	} 
}

 /*
* doOcOutWrite:
*	Write coresponding Open collector output channel
**************************************************************************************
*/

static void doOcOutWrite(int argc, char *argv[])
{
	int pin = 0;
  int val = 0;
  int dev = 0;
  int valR = 0;
  int valRmask = 0;
  int retry = RETRY_TIMES;
  int bType = 0;

	if ((argc != 5) && (argc != 4))
	{
		printf( "Usage: megaioind <id>  <oc number> <on/off> \n") ;
		printf( "Usage: megaioind <id> ocwrocwriteite <oc reg value> \n") ;
    
		exit (1) ;
	}
  
	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(argc == 5)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > OC_CH_NR_MAX))
		{
			printf( "Open collector output number value out of range\n");
			exit(1);
		}
  
		if ((strcasecmp (argv [4], "up") == 0) || (strcasecmp (argv [4], "on") == 0))
			val = 1 ;
		else if ((strcasecmp (argv [4], "down") == 0) || (strcasecmp (argv [4], "off") == 0))
			val = 0 ;
		else
		{
			val = atoi (argv [4]) ;
		}
		if (val == 0)
		{
			writeReg8 (dev,OC_CLR_ADD, pin);
		}
		else
		{
			writeReg8 (dev,OC_SET_ADD, pin);
		}
		valR = readReg8(dev, OC_VAL_ADD);
		valRmask = 0x01 & (valR >> (pin - 1));
		retry = RETRY_TIMES;
	
		while((retry > 0) && (valRmask != val))
		{
			if (val == 0)
			{
				writeReg8 (dev,OC_CLR_ADD, pin);
			}
			else
			{
				writeReg8 (dev,OC_SET_ADD, pin);
			}
			valR = readReg8(dev, OC_VAL_ADD);
			valRmask = 0x01 & (valR >> (pin - 1));
			retry--;
		}
#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
#endif
		if(retry == 0)
		{
			printf( "Fail to write open collector output\n");
			exit(1);
		}
	}
	else
	{
		val = atoi (argv [3]) ;
		if(val < 0 || val > 0x0f)
		{
			printf( "Invalid open collector output value\n");
			exit(1);
		}
		retry = RETRY_TIMES;
		while((retry > 0) && (valR != val))
		{
			writeReg8 (dev,OC_VAL_ADD, val);
		
			valR = readReg8(dev, OC_VAL_ADD);
		}
		if(retry == 0)
		{
			printf( "Fail to write open collector output out\n");
			exit(1);
		}
	}
}

/*
* doOcOutRead:
*	Read Open collector output state
******************************************************************************************
*/
static void doOcOutRead(int argc, char *argv[])
{
  int pin = 0;
  int val = 0;
  int dev = 0;
  int bType = 0;

  dev = doBoardInit (gHwAdd, &bType);
  if(dev <= 0)
  {     
    exit(1);
  }
  
  if (argc == 4)
  {
    pin = atoi (argv [3]) ;
    if((pin < CHANNEL_NR_MIN) || (pin > OPTO_CH_NR_MAX))
    {
      printf( "Open collector output number value out of range\n");
      exit(1);
    }

    val = readReg8(dev, OC_VAL_ADD);
    val = val & (1 << (pin-1));
    if(val != 0)
    {
      printf("1\n");
    }
    else
    {
      printf("0\n");
    }
  }
  else if(argc == 3)
  {
    val = readReg8(dev, OC_VAL_ADD);
    printf("%d\n", val);
  }
  else
  {
    printf( "Usage: %s read Open collector output value\n", argv [0]) ;
    exit (1) ;
  }
  
}

static void doTimeGet(int argc, char *argv[])
{
	int dev = 0;
  int bType = 0;
  int ret = 0;
	RtcStructType rtc;
  
  UNUSED(argv);
  if(argc != 3)
  {
    printf("Invalid number of parameters type: megaioind -h time\n");
    exit(1);
  }
  
	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{  
		exit(1);
	}
	ret = readBuff(dev, (uint8_t*)&rtc, RTC_YEAR_ADD, sizeof(RtcStructType));
  if(ret > 0)
  {
    printf("%02d/%02d/%04d %02d:%02d:%02d\n", rtc.m, rtc.d, rtc.y+2000, rtc.h, rtc.min, rtc.s);
  }
  else
  {
    printf("Fail to read time\n");
    exit(1);
  }
	
}

static void doTimeSet(int argc, char *argv[])
{
	int dev = 0;
  int bType = 0;
  int ret = 0;
	RtcStructType rtc;
  
  if(argc != 5)
  {
    printf("Invalid number of parameters type: megaioind -h stime\n");
    exit(1);
  }
  
  if(OK != extractDate(argv[3], &rtc))
  {
    printf("Invalid parameter : date\n");
    exit(-1);
  }
  
  if(OK != extractTime(argv[4], &rtc))
  {
    printf("Invalid parameter : time\n");
    exit(-1);
  }
 
  //printf("%02d/%02d/%d %02d:%02d:%02d\n", rtc.d, rtc.m, rtc.y+2000, rtc.h, rtc.min, rtc.s);
	dev = doBoardInit (gHwAdd, &bType);
	if(dev <= 0)
	{  
		exit(1);
	}
	ret = writeBuff(dev, (uint8_t*)&rtc, RTC_SET_YEAR_ADD, sizeof(RtcStructType));
  if(ret < 0)
  {
    printf("Fail to set time\n");
    exit(1);
  }
  if(writeReg8(dev, RTC_CMD, SET_TIME_KEY) < 0)
  {
    printf("Fail to set time\n");
    exit(1);
  }
	
}

 char *usage = "Usage:	 megaioind -h <command>\n"
		"         megaioind -v\n"
		"         megaioind -warranty\n"
		"         megaioind <id> board\n"
    "         megaioind <id> wrelay <channel> <on/off>\n"
    "         megaioind <id> rrelay <channel>\n" 
    "         megaioind <id> rrelay\n"	
    "         megaioind <id> riin <channel>\n"
    "         megaioind <id> riout <channel>\n"
    "         megaioind <id> wiout <channel> <value>\n"
    "         megaioind <id> ruin <channel>\n"
    "         megaioind <id> ruout <channel>\n"
    "         megaioind <id> wuout <channel> <value>\n"
    "         megaioind <id> rresin <channel>\n"
    "         megaioind <id> ropto <channel>\n"
    "         megaioind <id> ropto\n"
		"         megaioind <id> roc\n"
		"         megaioind <id> woc <ch> <on/off; 1/0>\n"
		"         megaioind <id> woc <val>\n"
    "         megaioind <id> time\n"
    "         megaioind <id> stime <dd/mm/yyyy> <hh:mm:ss>\n"
		"Where: <id> = Board level id = 0..3\n"
		"Type megaioind -h <command> for more help";// No trailing newline needed here.
   
void doHelp(int argc, char *argv[])
{
	if(argc == 3)
	{
		/**/ if (strcasecmp (argv [2], "wrelay"    ) == 0)	
		{ 
			printf("\twrelay:      Set Relay On/Off\n");
			printf("\tUsage:       megaioind <id> wrelay <channel> <on/off>\n");
			printf("\tExample:     megaioind 0 wrelay 2 On; Set Relay #2 on Board #0 On\n"); 
		}
		else if (strcasecmp (argv [2], "board"     ) == 0)	
		{ 
			printf("\tboard:       Show Hardware and Software Version, I/O Pinout & Voltages\n");
			printf("\tUsage:       megaioind <id> board\n");
			printf("\tExample:     megaioind 0 board\n"); 
		}
		else if (strcasecmp (argv [2], "rrelay"     ) == 0)	
		{ 
			printf("\trrelay:      Read Relay Status\n");
			printf("\tUsage:       megaioind <id> rrelay <channel>\n");
			printf("\tUsage:       megaioind <id> rrelay\n");
			printf("\tExample:     megaioind 0 rrelay 2; Read Status of Relay #2 on Board #0\n"); 
		}
		else if (strcasecmp (argv [2], "riin"     ) == 0)	
		{ 
			printf("\triin:        Read 4 - 20 mA Input\n");
			printf("\tUsage:       megaioind <id> riin <channel>\n");
			printf("\tExample:     megaioind 0 riin 2; Read Value of 4 -20mA input channel #2 on Board #0\n"); 
		}
    else if (strcasecmp (argv [2], "riout"     ) == 0)	
		{ 
			printf("\triout:       Read 4 - 20 mA Output\n");
			printf("\tUsage:       megaioind <id> riout <channel>\n");
			printf("\tExample:     megaioind 0 riout 2; Read Value of 4 -20mA output channel #2 on Board #0\n"); 
		}
		else if (strcasecmp (argv [2], "wiout"    ) == 0)	
		{ 
			printf("\twiout:       Write 4 - 20mA Output\n");
			printf("\tUsage:       megaioind <id> wiout <value>\n");
			printf("\tExample:     megaioind 0 wiout 1 5.33; Set 5.33 mA on output channel 1\n"); 
		}
    else if (strcasecmp (argv [2], "ruin"     ) == 0)	
		{ 
			printf("\truin:        Read 0 - 10V Input\n");
			printf("\tUsage:       megaioind <id> ruin <channel>\n");
			printf("\tExample:     megaioind 0 ruin 2; Read Value of 0 - 10V input channel #2 on Board #0\n"); 
		}
    else if (strcasecmp (argv [2], "ruout"     ) == 0)	
		{ 
			printf("\truout:       Read 0 - 10V Output\n");
			printf("\tUsage:       megaioind <id> ruout <channel>\n");
			printf("\tExample:     megaioind 0 ruout 2; Read Value of 0 - 10V output channel #2 on Board #0\n"); 
		}
		else if (strcasecmp (argv [2], "wuout"    ) == 0)	
		{ 
			printf("\twuout:       Write 0 - 10V Output\n");
			printf("\tUsage:       megaioind <id> wuout <value>\n");
			printf("\tExample:     megaioind 0 wuout 1 5.33; Set 5.33V on output channel 1\n"); 
		}
		else if (strcasecmp (argv [2], "rresin"     ) == 0)	
		{ 
			printf("\trresin:      Read resistance Input in ohm's( NTC 10K)- MegaIO-BAS only!\n");
			printf("\tUsage:       megaioind <id> rresin <channel>\n");
			printf("\tExample:     megaioind 0 rresin 2; Read Value of Resistance input channel #2 on Board #0\n"); 
		}
		else if (strcasecmp (argv [2], "ropto"   ) == 0)	
		{ 
			printf("\toptread:     Read Optically Isolated Input bit or port\n");
			printf("\tUsage:       megaioind <id> ropto (<channel>)\n");
			printf("\tExample:     megaioind 0 ropto; Read all optically isolated inputs\n"); 
			printf("\tExample:     megaioind 0 ropto 3; Read optically isolated input #3\n"); 
		}		
		else if(strcasecmp(argv[2], "roc") == 0)
		{
			printf("\tocread:      Read the open-collector output's\n");
			printf("\tUsage:       megaioind <id> roc\n");
			printf("\tExample:     megaioond 0 roc\n");
		}
		else if(strcasecmp(argv[2], "woc") == 0)
		{
			printf("\tocwrite:     Write the open-collectot output's\n");
			printf("\tUsage:       megaioind <id> woc <val>\n");
			printf("\tExample:     megaioind 0 woc 5\n");
			printf("\tUsage:       megaioind <id> woc <ch> <val>\n");
			printf("\tExample:     megaioind 0 woc 2 on\n");
		}
    else if(strcasecmp(argv[2], "time") == 0)
		{
			printf("\ttime:        Display the board time and date (mm/dd/yyyy hh:mm:ss)\n");
			printf("\tUsage:       megaioind <id> time\n");
			printf("\tExample:     megaioind 0 time\n");
		}
    else if(strcasecmp(argv[2], "stime") == 0)
		{
			printf("\ttime:        Set the time and date\n");
			printf("\tUsage:       megaioind <id> stime <mm/dd/yyyy> <hh:mm:ss>\n");
			printf("\tExample:     megaioind 0 stime 05/03/2018 01:09:14\n");
		}
		else
		{
			printf("Invalid command!\n");
			printf("%s: %s\n", argv [0], usage);
		}
	}
	else
	{
		printf("%s\n", usage);
	}
}
		
void doBoard(int argc)
{
	int dev = 0;
  int hwMajor = 0;
  int hwMinor = 0;
  int minor = 0;
  int major = 0;
	int bType = 0;
	int val = 0;
	float vIn = 0;
  float vRasp = 0;
	
	
	if(argc == 3)
	{
		dev = doBoardInit(gHwAdd, &bType);
		if(dev <= 0)
		{			 
			exit(1);
		}
		hwMajor = readReg8(dev, REVISION_HW_MAJOR_MEM_ADD);
		hwMinor = readReg8(dev, REVISION_HW_MINOR_MEM_ADD);
		major = readReg8(dev, REVISION_MAJOR_MEM_ADD);
		minor = readReg8(dev, REVISION_MINOR_MEM_ADD);
		val = readReg16(dev, DIAG_24V_MEM_ADD);
		vIn = (float)val/V_IN_SCALE_FACTOR;
		val = readReg16(dev, DIAG_5V_MEM_ADD);
		vRasp = (float)val/V_IN_SCALE_FACTOR;
		val = readReg8(dev, DIAG_TEMPERATURE_MEM_ADD);
		
		switch (bType)
		{
		case BOARD_TYPE_IND:
			printf("MegaIO IND Board Version %d.%d\tFirmware Version %d.%d\n", hwMajor, hwMinor, major, minor);
      printf("VIN %.02fV\tVCC Raspberry %.02fV\t CPU Temperature %dºC\n", vIn, vRasp, val);
      printf("%s", indConnStr);
			break;
		case BOARD_TYPE_BAS:
			printf("MegaIO BAS Board Version %d.%d\tFirmware Version %d.%d\n", hwMajor, hwMinor, major, minor);
			printf("VIN %.02fV\tVCC Raspberry %.02fV\t CPU Temperature %dºC\n", vIn, vRasp, val);
      printf("%s", basConnStr);
			break;
		default:
			printf("Invalid board Type\n");
			break;
		}
		
	}
	else
	{
		printf( "Usage: megaioind <id> board\n");
	}
}

static void doVersion(void)
{
	printf("MegaIO Industrial v%d.%d.%d Copyright (c) 2016 - 2019 Sequent Microsystems\n", VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: megaioind -warranty\n");

}


/*
* Main loop
************************************************************************************************
*/
int main(int argc, char *argv [])
{
  int id = 0;
  
  if (argc == 1)
  {
    printf( "%s\n", usage) ;
    return 1 ;
  }

  
// Help

  if (strcasecmp (argv [1], "-h") == 0)
  {
    doHelp(argc, argv);
    return 0;
  }
  
  // Warranty

  if (strcasecmp (argv [1], "-warranty") == 0)
  {
    printf("%s\n", warranty);
    return 0;
  }
  
  
// Version

  if (strcasecmp (argv [1], "-v") == 0)
  {
    doVersion();
    return 0;
  }


  
  if(argc < 3)
  {
	printf( "Invalid number of arguments!\n %s\n", usage) ;
    return 1 ;
  }
  id = atoi(argv[1]);
  if((id < 0) || (id > 3))
  {
    printf( "invalid boartd id\n");
	return 1;
  }
  gHwAdd = MEGAIO_HW_I2C_BASE_ADD + id;
  
  
  /**/ if (strcasecmp (argv [2], "wrelay"  ) == 0)	{ doRelayWrite     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "rrelay"  ) == 0)	{ doRelayRead      (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "riin"    ) == 0)	{ do420InRead      (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "riout"   ) == 0)	{ do420OutRead     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "wiout"   ) == 0)	{ do420OutWrite    (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "ruin"    ) == 0)	{ do010InRead      (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "ruout"   ) == 0)	{ do010OutRead     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "wuout"   ) == 0)	{ do010OutWrite    (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "rresin"  ) == 0)	{ doResInRead	   (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "ropto"   ) == 0)	{ doOptoInRead     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "board"   ) == 0)	{ doBoard          (argc) 		;	return 0 ; } 
  else if (strcasecmp (argv [2], "time"    ) == 0)	{ doTimeGet        (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "stime"    ) == 0)	{ doTimeSet        (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "woc"     ) == 0)	{ doOcOutWrite     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "roc"     ) == 0)	{ doOcOutRead      (argc, argv) ;	return 0 ; }
  else 
    printf("Invalid argument \n");
  return 0;
}
