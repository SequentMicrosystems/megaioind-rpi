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

STACK_LEVEL_MAX = 3
CH_NR_MIN = 1
RELAY_NR_MAX = 4
RELAY_VAL_MAX = 15
CRT_IN_CH_MAX = 4
VOLT_IN_CH_MAX = 4
CRT_OUT_CH_MAX = 4
VOLT_OUT_CH_MAX = 4
OC_CH_MAX = 4
OPTO_CH_MAX = 4

SCALE_FACTOR_CRT = 1000.0
SCALE_FACTOR_VOLT = 1000.0
CRT_VAL_MIN = 4
CRT_VAL_MAX = 20
VOLT_VAL_MIN = 0
VOLT_VAL_MAX = 10

HW_ADD = 0x38
bus = smbus.SMBus(1)

# Display the lib info
def version():
  return "MegaIO Industrial python library v1.0 Sequent Microsystems"
 

# Set the selected relay on or off  
def set_relay(stack, relay, value):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
	if relay < CH_NR_MIN:
		raise ValueError('Invalid relay number')
		return
	if relay > RELAY_NR_MAX:
		raise ValueError('Invalid relay number')
		return
	if value == 0:
		bus.write_byte_data(HW_ADD + stack, RELAY_CLR_ADD, relay)
	else:
		bus.write_byte_data(HW_ADD + stack, RELAY_SET_ADD, relay)

# Set all relays value		
def set_relays(stack, value):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
	if value > RELAY_VAL_MAX :
		raise ValueError('Invalid relay value')
		return
	if value < 0:
		raise ValueError('Invalid relay value')
		return
	bus.write_byte_data(HW_ADD + stack, RELAY_VAL_ADD, value)
	
# Get all relays value	
def get_relays(stack):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
	return bus.read_byte_data(HW_ADD + stack, RELAY_VAL_ADD)

# Get the selected input 4-20mA channel value in mA
def get_iin_ma(stack, ch):
  bus = smbus.SMBus(1)
  if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
  if ch < CH_NR_MIN or ch > CRT_IN_CH_MAX:
    raise ValueError('Invalid channel number')
    return
  rawVal = bus.read_word_data(HW_ADD + stack, I4_20_IN_VAL1_ADD + 2 * (ch - 1))
  return rawVal/SCALE_FACTOR_CRT
 
# Set the selected output 4-20mA channel value in mA
def set_iout_ma(stack, ch, value):
  bus = smbus.SMBus(1)
  if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
  if ch < CH_NR_MIN or ch > CRT_IN_CH_MAX:
    raise ValueError('Invalid channel number')
    return
  if value < CRT_VAL_MIN or value > CRT_VAL_MAX:
    raise ValueError(' Value out of range')
    return   
  rawVal = int(value * SCALE_FACTOR_CRT)
  bus.write_word_data(HW_ADD + stack, I4_20_OUT_VAL1_ADD + 2 * (ch - 1), rawVal)

# Get the selected output 4-20mA channel value in mA
def get_iout_ma(stack, ch):
  bus = smbus.SMBus(1)
  if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
  if ch < CH_NR_MIN or ch > CRT_OUT_CH_MAX:
    raise ValueError('Invalid channel number')
    return
  rawVal = bus.read_word_data(HW_ADD + stack, I4_20_OUT_VAL1_ADD + 2 * (ch - 1))
  return rawVal/SCALE_FACTOR_CRT

# Get the selected input 0-10V channel value in volts  
def get_uin_v(stack, ch):
  bus = smbus.SMBus(1)
  if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
  if ch < CH_NR_MIN or ch > VOLT_IN_CH_MAX:
    raise ValueError('Invalid channel number')
    return
  rawVal = bus.read_word_data(HW_ADD + stack, U0_10_IN_VAL1_ADD + 2 * (ch - 1))
  return rawVal/SCALE_FACTOR_VOLT

# Set the selected output 0-10V channel value in volts  
def set_uout_v(stack, ch, value):
  bus = smbus.SMBus(1)
  if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
  if ch < CH_NR_MIN or ch > VOLT_OUT_CH_MAX:
    raise ValueError('Invalid channel number')
    return
  if value < VOLT_VAL_MIN or value > VOLT_VAL_MAX:
    raise ValueError(' Value out of range')
    return   
  rawVal = int(value * SCALE_FACTOR_VOLT)
  bus.write_word_data(HW_ADD + stack, U0_10_OUT_VAL1_ADD + 2 * (ch - 1), rawVal)
 
# Get the selected output 0-10V channel value in volts  
def get_uout_v(stack, ch):
  bus = smbus.SMBus(1)
  if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
  if ch < CH_NR_MIN or ch > VOLT_OUT_CH_MAX:
    raise ValueError('Invalid channel number')
    return
  rawVal = bus.read_word_data(HW_ADD + stack, U0_10_OUT_VAL1_ADD + 2 * (ch - 1))
  return rawVal/SCALE_FACTOR_CRT

# Get the optoisolated input pins state 
def get_opto_in(stack):
	outData = 0
	bus = smbus.SMBus(1)
	if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
	outData = bus.read_byte_data(HW_ADD + stack, OPTO_VAL_ADD)
	return outData
 
# Set open-collector pin state 
def set_oc_pin(stack, pin, val):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
	if pin < CH_NR_MIN or pin > OC_CH_MAX:
		raise ValueError('Invalid OC channel number')
		return
	if val != 0 and val != 1:
		raise ValueError('Invalid OC value')
		return
	if val == 1:	
		bus.write_byte_data(HW_ADD + stack, OC_SET_ADD, pin)	
	else:
		bus.write_byte_data(HW_ADD + stack, OC_CLR_ADD, pin)	
	return

# Get all open-collector pins state	
def get_oc_val(stack):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > STACK_LEVEL_MAX:
		raise ValueError('Invalid stack level')
		return
	outVal = bus.read_byte_data(HW_ADD + stack, OC_VAL_ADD)
	return outVal