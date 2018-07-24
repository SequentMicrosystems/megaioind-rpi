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

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)0
#define VERSION_MINOR	(int)0

int gHwAdd = MEGAIO_HW_I2C_BASE_ADD;




			  
char *warranty ="	       Copyright (c) 2016-2018 Sequent Microsystems\n"
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
				


				
char *failStr = "    ########    ###    #### ##       #### \n"
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
		  
void printbits(int v) 
{
	int i; // for C89 compatibility
	
	for(i = 17; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}


// set ON/OFF specify relay channel
int relayChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp;
	
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
	if(0 < resp)
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
	int pin, val, dev, valR, valRmask, retry;

	if ((argc != 5) && (argc != 4))
	{
		printf( "Usage: megaioind <id> rwrite <relay number> <on/off> \n") ;
		printf( "Usage: megaioind <id> rwrite <relay reg value> \n") ;
		exit (1) ;
	}

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if(argc == 5)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NR_MAX))
		{
			printf( "Relay number value out of range\n");
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
	int pin, val, dev;

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}

	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < 1) || (pin > 8))
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
	int pin, dev, rd;
 float val;

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
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
	int pin, dev, rd;
 float val;

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
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
  int dev, rawVal, pin;
  float val;
  int ret;
	
	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
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
	int pin, dev, rd;
 float val;

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > V_IN_CH_MAX))
		{
			printf( "0-10V Input channel number %d out of range\n", pin);
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
	int pin, dev, rd;
 float val;

	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi (argv [3]) ;
		if((pin < CHANNEL_NR_MIN) || (pin > V_OUT_CH_MAX))
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
  int dev, rawVal, pin;
  float val;
  int ret;
	
	dev = doBoardInit (gHwAdd);
	if(dev <= 0)
	{		 
		exit(1);
	}
	if (argc != 5)
	{
		printf( "Invalid number of arguments, type: megaioind -h %s\n", argv[2]);
		exit (1) ;
	}
 	pin = atoi (argv [3]) ;
	if((pin < CHANNEL_NR_MIN) || (pin > V_OUT_CH_MAX))
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
* doOptoInRead:
*	Read the optocupled input pins
************************************************************************************************
*/

static void doOptoInRead(int argc, char *argv[])
{
  int pin, val, dev;

	dev = doBoardInit(gHwAdd);
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

		val = readReg8(dev, OPTO_IN_MEM_ADD);
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
		val = readReg8(dev, OPTO_IN_MEM_ADD);
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
	int pin, val, dev, valR, valRmask, retry;

	if ((argc != 5) && (argc != 4))
	{
		printf( "Usage: megaioind <id> ocwrite <oc number> <on/off> \n") ;
		printf( "Usage: megaioind <id> ocwrite <oc reg value> \n") ;
    
		exit (1) ;
	}
  
	dev = doBoardInit (gHwAdd);
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
			writeReg8 (dev,OC_OUT_CLR_MEM_ADD, pin);
		}
		else
		{
			writeReg8 (dev,OC_OUT_SET_MEM_ADD, pin);
		}
		valR = readReg8(dev, OC_OUT_VAL_MEM_ADD);
		valRmask = 0x01 & (valR >> (pin - 1));
		retry = RETRY_TIMES;
	
		while((retry > 0) && (valRmask != val))
		{
			if (val == 0)
			{
				writeReg8 (dev,OC_OUT_CLR_MEM_ADD, pin);
			}
			else
			{
				writeReg8 (dev,OC_OUT_SET_MEM_ADD, pin);
			}
			valR = readReg8(dev, OC_OUT_VAL_MEM_ADD);
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
			writeReg8 (dev,OC_OUT_VAL_MEM_ADD, val);
		
			valR = readReg8(dev, OC_OUT_VAL_MEM_ADD);
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
  int pin, val, dev;

  dev = doBoardInit (gHwAdd);
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

    val = readReg8(dev, OC_OUT_VAL_MEM_ADD);
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
    val = readReg8(dev, OC_OUT_VAL_MEM_ADD);
    printf("%d\n", val);
  }
  else
  {
    printf( "Usage: %s read Open collector output value\n", argv [0]) ;
    exit (1) ;
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
    "         megaioind <id> optread <channel>\n"
    "         megaioind <id> optread\n"
		"         megaioind <id> ocread\n"
		"         megaioind <id> ocwrite <ch> <on/off; 1/0>\n"
		"         megaioind <id> ocwrite <val>\n"
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
			printf("\tboard:       Check MegaIO Industrial Board Hardware and Software Version\n");
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
			printf("\triout:        Read 4 - 20 mA Output\n");
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
		else if (strcasecmp (argv [2], "optread"   ) == 0)	
		{ 
			printf("\toptread:     Read Optically Isolated Input bit or port\n");
			printf("\tUsage:       megaioind <id> optread (<channel>)\n");
			printf("\tExample:     megaio 0 optread; Read all optically isolated inputs\n"); 
			printf("\tExample:     megaio 0 optread 3; Read optically isolated input #3\n"); 
		}		
		else if(strcasecmp(argv[2], "ocread") == 0)
		{
			printf("\tocread:   Read the open-collector output's\n");
			printf("\tUsage:    megaioind <id> ocread\n");
			printf("\tExample:  megaio 0 ocread\n");
		}
		else if(strcasecmp(argv[2], "ocwrite") == 0)
		{
			printf("\tocwrite:  Write the open-collectot output's\n");
			printf("\tUsage:    megaioind <id> ocwrite <val>\n");
			printf("\tExample:  megaio 0 ocwrite 5\n");
			printf("\tUsage:    megaioind <id> ocwrite <ch> <val>\n");
			printf("\tExample:  megaio 0 ocwrite 2 on\n");
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
	int dev, hwMajor, hwMinor, minor , major;
	
	if(argc == 3)
	{
		dev = doBoardInit(gHwAdd);
		if(dev <= 0)
		{			 
			exit(1);
		}
		hwMajor = readReg8(dev, REVISION_HW_MAJOR_MEM_ADD);
		hwMinor = readReg8(dev, REVISION_HW_MINOR_MEM_ADD);
		major = readReg8(dev, REVISION_MAJOR_MEM_ADD);
		minor = readReg8(dev, REVISION_MINOR_MEM_ADD);
		printf("MegaIO Hardware version %d.%d Firmware version %d.%d\n", hwMajor, hwMinor, major, minor);
	}
	else
	{
		printf( "Usage: megaioind <id> board\n");
	}
}

static void doVersion(void)
{
	printf("MegaIO Industrial v%d.%d.%d Copyright (c) 2016 - 2018 Sequent Microsystems\n", VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: megaioind -warranty\n");

}


/*
* Main loop
************************************************************************************************
*/
int main(int argc, char *argv [])
{
  int id;
  
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
  else if (strcasecmp (argv [2], "ropto"   ) == 0)	{ doOptoInRead     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "board"   ) == 0)	{ doBoard          (argc) 		;	return 0 ; } 
  else if (strcasecmp (argv [2], "woc"     ) == 0)	{ doOcOutWrite     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [2], "roc"     ) == 0)	{ doOcOutRead      (argc, argv) ;	return 0 ; }
 
  return 0;
}
