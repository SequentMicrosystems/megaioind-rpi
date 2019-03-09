
#ifndef MEGAIO_H_
#define MEGAIO_H_

#define RETRY_TIMES	10

enum
{
	RELAY_VAL_ADD = 0,
	RELAY_SET_ADD,
	RELAY_CLR_ADD,
	OC_VAL_ADD,
	OC_SET_ADD,
	OC_CLR_ADD,
	OPTO_VAL_ADD,
	I4_20_OUT_VAL1_ADD,
	SPARE1,
	I4_20_OUT_VAL2_ADD,
	SPARE2,
	I4_20_OUT_VAL3_ADD,
	SPARE3,
	I4_20_OUT_VAL4_ADD,
	SPARE4,
	I4_20_IN_VAL1_ADD,
	SPARE5,
	I4_20_IN_VAL2_ADD,
	SPARE6,
	I4_20_IN_VAL3_ADD,
	SPARE7,
	I4_20_IN_VAL4_ADD,
	SPARE8,
	U0_10_OUT_VAL1_ADD,
	SPARE9,
	U0_10_OUT_VAL2_ADD,
	SPARE10,
	U0_10_OUT_VAL3_ADD,
	SPARE11,
	U0_10_OUT_VAL4_ADD,
	SPARE12,
	U0_10_OUT_VAL5_ADD,
	SPARE13,
	U0_10_OUT_VAL6_ADD,
	SPARE14,
	U0_10_OUT_VAL7_ADD,
	SPARE15,
	U0_10_OUT_VAL8_ADD,
	SPARE16,
	U0_10_IN_VAL1_ADD,
	SPARE17,
	U0_10_IN_VAL2_ADD,
	SPARE18,
	U0_10_IN_VAL3_ADD,
	SPARE19,
	U0_10_IN_VAL4_ADD,
	SPARE20,
	R_10K_CH1,
	SP21,
	R_10K_CH2,
	SP22,
	R_10K_CH3,
	SP23,
	R_10K_CH4,
	SP24,
	CAL_CH_ADD = 55,
	CAL_VAL_ADD,
	SPARE24,
	CAL_CMD_ADD,
	TEMPERATURE_ADD,
	MODBUS_SETINGS_ADD = 60,
	NBS1,
	MBS2,
	MBS3,
	MODBUS_ID_OFFSET_ADD,

	DIAG_TEMPERATURE_MEM_ADD = 0x72,
	DIAG_24V_MEM_ADD,
	DIAG_24V_MEM_ADD1,
	DIAG_5V_MEM_ADD,
	DIAG_5V_MEM_ADD1,
	CAN_REC_MPS_MEM_ADD,
	REVISION_HW_MAJOR_MEM_ADD = 0x78,
	REVISION_HW_MINOR_MEM_ADD,
	REVISION_MAJOR_MEM_ADD,
	REVISION_MINOR_MEM_ADD,
	BUILD_DAY_MEM_ADD,
	BUILD_MOTH_MEM_ADD,
	BUILD_YEAR_MEM_ADD,
	BOARD_TYPE_MEM_ADD,

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
	
enum
{
	BOARD_TYPE_NONE = 0,
	BOARD_TYPE_IND,
	BOARD_TYPE_BAS
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
