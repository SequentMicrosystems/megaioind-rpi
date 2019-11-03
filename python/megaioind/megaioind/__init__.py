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
U0_10_OUT_VAL5_ADD = 31
U0_10_OUT_VAL6_ADD = 33
U0_10_OUT_VAL7_ADD = 35
U0_10_OUT_VAL8_ADD = 37
U0_10_IN_VAL1_ADD = 39
U0_10_IN_VAL2_ADD = 41
U0_10_IN_VAL3_ADD = 43
U0_10_IN_VAL4_ADD = 45
R_10K_CH1 = 47
R_10K_CH2 = 49
R_10K_CH3 = 51
R_10K_CH4 = 53
TEMPERATURE_ADD = 47
CAL_CH_ADD = 55
CAL_VAL_ADD = 56

CAL_CMD_ADD = 58
MODBUS_SETINGS_ADD = 60
DIAG_TEMPERATURE_MEM_ADD = 0x72
DIAG_24V_MEM_ADD = 0x73
#DIAG_24V_MEM_ADD1
DIAG_5V_MEM_ADD = 0x75
#DIAG_5V_MEM_ADD1,
CAN_REC_MPS_MEM_ADD = 0x77
REVISION_HW_MAJOR_MEM_ADD = 0x78
REVISION_HW_MINOR_MEM_ADD = 0x79
REVISION_MAJOR_MEM_ADD = 0x7a
REVISION_MINOR_MEM_ADD = 0x7b
BUILD_DAY_MEM_ADD = 0x7c
BUILD_MOTH_MEM_ADD = 0x7d
BUILD_YEAR_MEM_ADD = 0x7e
BOARD_TYPE_MEM_ADD = 0x7f

RESET_I4_20_OUT = 100
RESET_I4_20_IN = 101
RESET_U0_10V_IN = 102
RESET_U0_10V_OUT = 103
RESET_R_IN = 104
RESET_KEY_VAL = 0xBABA

HW_ADD = 0x38
bus = smbus.SMBus(1)

def getVer(stack):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
	hw_maj = bus.read_byte_data(HW_ADD + stack, REVISION_HW_MAJOR_MEM_ADD)
	hw_min = bus.read_byte_data(HW_ADD + stack, REVISION_HW_MINOR_MEM_ADD)
	fw_maj = bus.read_byte_data(HW_ADD + stack, REVISION_MAJOR_MEM_ADD)
	fw_min = bus.read_byte_data(HW_ADD + stack, REVISION_MINOR_MEM_ADD)
	ret = "Hw "+str(hw_maj)+"."+str(hw_min) + " Fw "+str(fw_maj)+"."+str(fw_min)
	#print(" Hardware "+str(hw_maj)+"."+str(hw_min) + " Firmware "+str(fw_maj)+"."+str(fw_min))
	return ret
	
def getByte(stack, add):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
	if add > BOARD_TYPE_MEM_ADD or add < 0:
		raise ValueError("Address out of range")
		return -1;
	val = bus.read_byte_data(HW_ADD + stack, add)
	raise ValueError("Read mem["+str(add)+"] = "+str(val))
	
def setByte(stack, add, val):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
	if add > BOARD_TYPE_MEM_ADD or add < 0:
		raise ValueError("Address out of range")
		return -1;
	bus.write_byte_data(HW_ADD + stack, add, val)
	print("Write mem["+str(add)+"] = "+str(val))
	
def c2(val):
  if val > 32768:
    val = val - 65536
  return val
  
def setIOut(stack, ch, val):
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
  bus.write_word_data(HW_ADD + stack, I4_20_OUT_VAL1_ADD + (2 * ( ch - 1)), int(val * 1000))
  return 1
  
  
def getIOut(stack, ch):
  val = 0
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
  val = bus.read_word_data(HW_ADD + stack, I4_20_OUT_VAL1_ADD + (2 * (ch - 1)))
  return(c2(val) / 1000.0)
      
      
def getIIn(stack, ch):
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
  val = bus.read_word_data(HW_ADD + stack, I4_20_IN_VAL1_ADD + (2 * (ch - 1)))
  return c2(val)/1000.0
    
    
def setUOut(stack, ch, val):
  if ch < 1 or ch > 8:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
  bus.write_word_data(HW_ADD + stack, U0_10_OUT_VAL1_ADD + (2 * (ch - 1)), int(val * 1000))
  return 1
    
def getUOut(stack, ch):
  if ch < 1 or ch > 8:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
  val = bus.read_word_data(HW_ADD + stack, U0_10_OUT_VAL1_ADD + (2 * (ch - 1)))
  return(c2(val)/1000.0)
    
def getUIn(stack, ch):
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
  val = bus.read_word_data(HW_ADD + stack, U0_10_IN_VAL1_ADD + (2 * (ch - 1)))
  return(c2(val)/1000.0) 
	
def getRInK(stack, ch):
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
  val = bus.read_word_data(HW_ADD + stack, R_10K_CH1 + (2 * (ch - 1)))
  return(val/1000.0) 	
  
def getRIn(stack, ch):
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
  val = bus.read_word_data(HW_ADD + stack, R_10K_CH1 + (2 * (ch - 1)))
  return(val) 	  

def getRelays(stack):
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
  val = bus.read_byte_data(HW_ADD + stack, RELAY_VAL_ADD)
  return val
  
def setRelays(stack, val):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
	try:	
		bus.write_byte_data(HW_ADD + stack, RELAY_VAL_ADD, val)
	except:
		return -1
	return 1	

		
def setRelay(stack, ch, val):
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
  if(val <> 0):
    bus.write_byte_data(HW_ADD + stack, RELAY_SET_ADD, ch)
  else:
    bus.write_byte_data(HW_ADD + stack, RELAY_CLR_ADD, ch)
  return 1

def togleRelay(stack, ch, delay, count):
  if ch < 1 or ch > 4:
    raise ValueError("Channel out of range")
    return -1
  if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1
  for i in range(count):
    setRelay(stack, ch, 1)
    time.sleep(delay)
    setRelay(stack, ch, 0)
    time.sleep(delay)

def setOC(stack, ch, val):
	if ch < 1 or ch > 4:
		raise ValueError("Channel out of range")
		return -1
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
	if(val <> 0):
		bus.write_byte_data(HW_ADD + stack, OC_SET_ADD, ch)
	else:
		bus.write_byte_data(HW_ADD + stack, OC_CLR_ADD, ch)
	return 1
	
def getOC(stack):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
	val = bus.read_byte_data(HW_ADD + stack, OC_VAL_ADD)
	return val
	
def getOpto(stack):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
	val = bus.read_byte_data(HW_ADD + stack, OPTO_VAL_ADD)
	return val

def getModbus(stack):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
	wVal = 0	
	for i in range(4):
		val = bus.read_byte_data(HW_ADD + stack, MODBUS_SETINGS_ADD + i)
		#raise ValueError(str(val))
		wVal += val << (8 * i);
		
	baud = wVal & 0x3ffffff;
	print("baud \t\t\t" + str(baud))
	type = 0x03 & (wVal >> 26);
	print("type \t\t\t" + str(type))
	parity = 0x03 & (wVal >> 28)
	print("Parity \t\t\t" + str(parity))
	stopB = 0x03 & (wVal >> 30);
	print("Stop bits code \t\t" + str(stopB))
	return 1;
	
def setModbus(stack, baud, type, parity, stopBits):
	if stack < 0 or stack > 3:
		raise ValueError('Invalid stack level')
		return -1 
	wVal = 0
	if baud < 1200 or baud > 115200:
		raise ValueError("Wrong baudrate")
		return -1
	if type < 0 or type > 1:
		raise ValueError ("Wrong type (0- disable; 1 - RTU;)")
		return -1
	if parity < 0 or parity > 2:
		raise ValueError("Wrong parity set to none")
		parity = 0
	if stopBits < 0 or stopBits > 2:
		raise ValueError("Wrong stopBits set to 1")
		stopBits = 0
	
	wVal = baud + (type << 26) + (parity << 28) + (stopBits << 30);
	for i in range(4):
		val = 0xff & (wVal >> (8 * i));
		bus.write_byte_data(HW_ADD + stack, MODBUS_SETINGS_ADD + i, val)
	return 1
	
def getCanDiag(stack):
	if stack < 0 or stack > 3:
		raise ValueError("Wrong stack level")
		return -1 		
	val = bus.read_byte_data(HW_ADD + stack, CAN_REC_MPS_MEM_ADD);
	#raise ValueError ( " CAN receive "+ str(val) + " pack's per second")
	return val;
	
def getInVolt(stack):
	if stack < 0 or stack > 3:
		raise ValueError("Wrong stack level")
		return -1
	val = bus.read_word_data(HW_ADD + stack, DIAG_24V_MEM_ADD)
	return val/1000.0
	
def getRaspVolt(stack):
	if stack < 0 or stack > 3:
		raise ValueError("Wrong stack level")
		return -1
	val = bus.read_word_data(HW_ADD + stack, DIAG_5V_MEM_ADD)
	return val/1000.0	
	
def getCpuTemp(stack):
	if stack < 0 or stack > 3:
		raise ValueError("Wrong stack level")
		return -1 		
	val = bus.read_byte_data(HW_ADD + stack, DIAG_TEMPERATURE_MEM_ADD);
	return val;	  
