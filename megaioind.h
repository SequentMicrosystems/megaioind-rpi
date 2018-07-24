
#ifndef MEGAIO_H_
#define MEGAIO_H_

#define RETRY_TIMES	10

enum{
	RELAY_VAL_ADD = 0,
	RELAY_SET_ADD,
	RELAY_CLR_ADD,
	OC_OUT_VAL_MEM_ADD,
	OC_OUT_SET_MEM_ADD,
	OC_OUT_CLR_MEM_ADD,
	OPTO_IN_MEM_ADD,
	I4_20_OUT_VAL1_ADD,
	SP1,
	I4_20_OUT_VAL2_ADD,
	SP2,
	I4_20_OUT_VAL3_ADD,
	SP3,
	I4_20_OUT_VAL4_ADD,
	SP4,
	I4_20_IN_VAL1_ADD,
	SP5,
	I4_20_IN_VAL2_ADD,
	SP6,
	I4_20_IN_VAL3_ADD,
	SP7,
	I4_20_IN_VAL4_ADD,
	SP8,
	U0_10_OUT_VAL1_ADD,
	SP9,
	U0_10_OUT_VAL2_ADD,
	SP10,
	U0_10_OUT_VAL3_ADD,
	SP11,
	U0_10_OUT_VAL4_ADD,
	SP12,
	U0_10_IN_VAL1_ADD,
	SP13,
	U0_10_IN_VAL2_ADD,
	SP14,
	U0_10_IN_VAL3_ADD,
	SP15,
	U0_10_IN_VAL4_ADD,
	SP16,
	TEMPERATURE_ADD,
	CAL_CH_ADD,
	CAL_VAL_ADD,
	SP17,
	CAL_CMD_ADD,
 
  REVISION_HW_MAJOR_MEM_ADD = 0x3c,
  REVISION_HW_MINOR_MEM_ADD,	
  REVISION_MAJOR_MEM_ADD,
  REVISION_MINOR_MEM_ADD
  
};


#define GPIO_PIN_NUMBER					(u8)6


#define ERROR	-1
#define FAIL	0
#define OK		1

#define CHANNEL_NR_MIN		1
#define RELAY_CH_NR_MAX		4
#define CRT_IN_CH_NR_MAX		4
#define CRT_OUT_CH_NR_MAX  4
#define V_IN_CH_MAX        4
#define V_OUT_CH_MAX        4
#define OPTO_CH_NR_MAX		4
#define OC_CH_NR_MAX		4


#define CRT_OUT_VAL_MIN  4000
#define CRT_OUT_VAL_MAX    20000
#define CRT_SCALE_FACTOR_MA  (float)1000

#define U_OUT_VAL_MIN  0
#define U_OUT_VAL_MAX    10000
#define U_SCALE_FACTOR_V    (float)1000


#define ANALOG_VAL_MIN		0
#define ANALOG_VAL_MAX		4095
#define ANALOG_ERR_THRESHOLD	100

#define	COUNT_KEY	0
#define YES		1
#define NO		2

#define MEGAIO_HW_I2C_BASE_ADD	0x38


typedef uint8_t u8;
typedef uint16_t u16;

typedef enum
{
	OFF = 0,
	ON
} OutStateEnumType;

enum
{
	INT_DISABLE = 0,
	INT_RISING,
	INT_FALLING,
	INT_BOTH
};
	

void startThread(void);
int checkThreadResult(void);
int readReg16(int dev, int add);
int readReg24(int dev, int add);
int writeReg16(int dev, int add, int val);
int writeReg24(int dev, int add, int val);
void printbits(int v);
int writeReg8(int dev, int add, int val);
int readReg8(int dev, int add);
int doBoardInit(int hwAdd);
int getLedVal(int chip);
int setLedVal(int chip, int val);
void busyWait(int ms);

#endif //MEGAIO_H_
