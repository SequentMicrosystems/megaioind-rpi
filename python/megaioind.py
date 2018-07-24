import smbus
import time
import math

RELAY_VAL_ADD = 0
RELAY_SET_ADD = 1
RELAY_CLR_ADD = 2
OC_VAL_ADD = 3
OC_SET_ADD = 4
OC_CLR_ADD = 5
OPTO_VAL_ADD = 6
I4_20_OUT_VAL1_ADD = 7
I4_20_OUT_VAL2_ADD = 9
I4_20_OUT_VAL3_ADD = 11
I4_20_OUT_VAL4_ADD = 13
I4_20_IN_VAL1_ADD = 15
I4_20_IN_VAL2_ADD = 17
I4_20_IN_VAL3_ADD  = 19
I4_20_IN_VAL4_ADD = 21
U0_10_OUT_VAL1_ADD = 23
U0_10_OUT_VAL2_ADD = 25
U0_10_OUT_VAL3_ADD = 27
U0_10_OUT_VAL4_ADD = 29
U0_10_IN_VAL1_ADD = 31
U0_10_IN_VAL2_ADD = 33
U0_10_IN_VAL3_ADD = 35
U0_10_IN_VAL4_ADD = 37
TEMPERATURE_ADD = 39
CAL_CH_ADD = 40
CAL_VAL_ADD = 41

CAL_CMD_ADD = 43


HW_ADD = 0x38
bus = smbus.SMBus(1)
