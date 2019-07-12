
[![megaioind-rpi](res/sequent.jpg)](https://www.sequentmicrosystems.com/megaio-ind.html)

# megaioind

This is the python library to control the MegaIO-IND[https://www.sequentmicrosystems.com/megaio-ind.html] and MegaIO-BAS [https://www.sequentmicrosystems.com/megaio-bas.html] expansion cards for Raspberry Pi

## Install

```bash
~$ sudo apt-get update
~$ sudo apt-get install build-essential python-pip python-dev python-smbus git
~$ git clone https://github.com/SequentMicrosystems/megaioind-rpi.git
~$ cd megaioind-rpi/python/megaioind/
~/megaioind-rpi/python/megaioind$ sudo python setup.py install
```
## Update

```bash
~$ cd megaioind-rpi/
~/megaioind-rpi$ git pull
~$ cd megaioind-rpi/python/megaioind/
~/megaioind-rpi/python/megaioind$ sudo python setup.py install
```

## Usage 

Now you can import the megaio library and use its functions. To test, read relays status from the MegaIO IND board with stack level 0:

```bash
~$ python
Python 2.7.9 (default, Sep 17 2016, 20:26:04)
[GCC 4.9.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import megaioind
>>> megaioind.getRelays(0)
0
>>>
```

## Functions
### setRelay(stack, ch, val)
Set one relay state.

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

relay - relay number (id) [1..4]

value - relay state 1: turn ON, 0: turn OFF[0..1]


### setRelays(stack, value)
Set all relays state.

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

value - 4 bit value of all relays (ex: 15: turn on all relays, 0: turn off all relays, 1:turn on relay #1 and off the rest)


### getRelays(stack)
Return the state of all relays.

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

return - [0..15]


### getIIn(stack, ch)
Return the selected input 4-20mA channel value in mA

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in mA [0..20]


### setIOut(stack, ch, value)
Set the selected output 4-20mA channel value in mA

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

value - current output value in mA [4..20]


### getIOut(stack, ch)
Return the selected output 4-20mA channel value in mA

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in mA [0..20]


### getUIn(stack, ch)
Return the selected input 0-10V channel value in volts

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in V [0..10]


### setUOut(stack, ch, value)
Set the selected output 0-10V channel value in volts

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

value - voltage output value in V


### getUOut(stack, ch)
Get the selected output 0-10V channel value in volts  

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in V [0..10]


### getOpto(stack)
Get the state of the opto-coupled inputs

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

return - value of the inputs [0..255]


### setOC(stack, pin, val)
Set the state of the corresponding open-collector output pin

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

pin - open-collector pin # [1..4]

val - state 0: ON, 1: OFF [0..1]


### getOC(stack)
Get the previously set state of all open-collector output pins

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

return - state of pins ( ex: 0: all pins OFF, 15: all pins ON, 1:  pin #1 ON, all the rest: OFF)
