[![megaioind-rpi](readmeres/sequent.jpg)](https://www.sequentmicrosystems.com/megaio-ind.html)

# Modbus

The [Industrial](https://sequentmicrosystems.com/index.php?route=product/product&path=20&product_id=52) and [Building](https://sequentmicrosystems.com/index.php?route=product/product&path=33&product_id=53) Automation I/O Expansion cards for Raspberry Pi can be accessed thru Modbus RTU protocol over RS-485 port.
You can set-up the RS-485 port with **megaioind** command.

Example:
```bash
~$ megaioind 0 wcfgmb 1 9600 0 0
```
Set Modbus RTU , Baudrate: 9600bps, parity: None. 1 Stop Bit, 
```bash
~$ megaioind -h wcfgmb
```
display the full set of options

## Slave Address
The slave address is set with the "stack level" jumpers. For example the jumpers configuration for stack level 0 (all jumpers removed) corespond to slave address 1.

## Modbus object types
All modbus RTU object type with standard addresses are implemented : Coils, Discrete Inputs, Input registers, Holding registers.

### Coils

Acces level Read/Write, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| RELAY_1 | 0001 | 0x00 |
| RELAY_2 | 0002 | 0x01 |
| RELAY_3 | 0003 | 0x02 |
| RELAY_4 | 0004 | 0x03 |
| OPEN_COLLECTOR_1 | 0005 | 0x04 |
| OPEN_COLLECTOR_2 | 0006 | 0x05 |
| OPEN_COLLECTOR_3 | 0007 | 0x06 |
| OPEN_COLLECTOR_4 | 0008 | 0x07 |

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| OPTO_1 | 10001 | 0x00 |
| OPTO_2 | 10002 | 0x01 |
| OPTO_3 | 10003 | 0x02 |
| OPTO_4 | 10004 | 0x03 |

### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit |
| --- | --- | --- | --- |
| 4-20mA_IN_1 | 30001 | 0x00 | uA |
| 4-20mA_IN_2 | 30002 | 0x01 | uA |
| 4-20mA_IN_3 | 30003 | 0x02 | uA |
| 4-20mA_IN_4 | 30004 | 0x03 | uA |
| 0-10V_IN_1 | 30005 | 0x04 | mV |
| 0-10V_IN_2 | 30006 | 0x05 | mV |
| 0-10V_IN_3 | 30007 | 0x06 | mV |
| 0-10V_IN_4 | 30008 | 0x07 | mV |
| 10K_R_IN_1 | 30009 | 0x08 | ohm |
| 10K_R_IN_2 | 30010 | 0x09 | ohm |
| 10K_R_IN_3 | 30011 | 0x0a | ohm |
| 10K_R_IN_4 | 30012 | 0x0b | ohm |

### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit |
| --- | --- | --- | --- |
| 4-20mA_OUT_1 | 40001 | 0x00 | uA |
| 4-20mA_OUT_2 | 40002 | 0x01 | uA |
| 4-20mA_OUT_3 | 40003 | 0x02 | uA |
| 4-20mA_OUT_4 | 40004 | 0x03 | uA |
| 0-10V_OUT_1 | 40005 | 0x04 | mV |
| 0-10V_OUT_2 | 40006 | 0x05 | mV |
| 0-10V_OUT_3 | 40007 | 0x06 | mV |
| 0-10V_OUT_4 | 40008 | 0x07 | mV |
| 0-10V_OUT_5 | 40009 | 0x08 | mV |
| 0-10V_OUT_6 | 40010 | 0x09 | mV |
| 0-10V_OUT_7 | 40011 | 0x0a | mV |
| 0-10V_OUT_8 | 40012 | 0x0b | mV |


## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)

