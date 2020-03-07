[![megaioind-rpi](readmeres/sequent.jpg)](https://www.sequentmicrosystems.com/megaio-ind.html)

# Modbus

The [Industrial](https://www.sequentmicrosystems.com/megaio-ind.html) and [Building](https://www.sequentmicrosystems.com/megaio-bas.html) Automation I/O Expansion cards for Raspberry Pi can be accessed thru Modbus RTU protocol over RS-485 port.
You can set-up the RS-485 port with **megaioind** command.

Example:
'''bash
~$ megaioind 0 wcfgmb 1 9600 0 0
'''
Set Modbus RTU , Baudrate: 9600bps, parity: None. 1 Stop Bit, 
'''bash
~$ megaioind -h wcfgmb
'''
display the full set of options

## Slave Address
The slave address is set with the "stack level" jumpers, the jumpers configuration for stack level 0 (all jumpers removed) corespond to slave address 1.

## Modbus object types
All modbus RTU object type with standard addresses are implemented : Coils, Discrete Inputs, Input registers, Holding registers.

### Coils

Acces level Read/Write, Size 1 bit

| Device function | Modbus Address |
| --- | --- |
| RELAY_1 | 00011 |
| RELAY_2 | 00012 |
| RELAY_3 | 00013 |
| RELAY_4 | 00014 |
| OPEN_COLLECTOR_1 | 00015 |
| OPEN_COLLECTOR_2 | 00016 |
| OPEN_COLLECTOR_3 | 00017 |
| OPEN_COLLECTOR_4 | 00018 |

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Modbus Address |
| --- | --- |
| OPTO_1 | 10001 |
| OPTO_2 | 10002 |
| OPTO_3 | 10003 |
| OPTO_4 | 10004 |

### Input registers

Access level Read Only, Size 16 bits

| Device function | Modbus Address | Measurement Unit |
| --- | --- | --- |
| 4-20mA_IN_1 | 30001 | uA |
| 4-20mA_IN_2 | 30002 | uA |
| 4-20mA_IN_3 | 30003 | uA |
| 4-20mA_IN_4 | 30004 | uA |
| 0-10V_IN_1 | 30005 | mV |
| 0-10V_IN_2 | 30006 | mV |
| 0-10V_IN_3 | 30007 | mV |
| 0-10V_IN_4 | 30008 | mV |
| 10K_R_IN_1 | 30009 | ohm |
| 10K_R_IN_2 | 30010 | ohm |
| 10K_R_IN_3 | 30011 | ohm |
| 10K_R_IN_4 | 30012 | ohm |

### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Modbus Address | Measurement Unit |
| --- | --- | --- |
| 4-20mA_OUT_1 | 40001 | uA |
| 4-20mA_OUT_2 | 40002 | uA |
| 4-20mA_OUT_3 | 40003 | uA |
| 4-20mA_OUT_4 | 40004 | uA |
| 0-10V_OUT_1 | 40005 | mV |
| 0-10V_OUT_2 | 40006 | mV |
| 0-10V_OUT_3 | 40007 | mV |
| 0-10V_OUT_4 | 40008 | mV |
| 0-10V_OUT_5 | 40009 | mV |
| 0-10V_OUT_6 | 40010 | mV |
| 0-10V_OUT_7 | 40011 | mV |
| 0-10V_OUT_8 | 40012 | mV |
