
[![megaioind-rpi](res/sequent.jpg)](https://www.sequentmicrosystems.com/megaio-ind.html)

# megaioind.py

This is the python library to control [MegaIO Industrial Expansion Card For Raspberry Pi ](https://www.sequentmicrosystems.com/megaio-ind.html)

Before use you have to install python smbus library

## Functions
### set_relay(stack, relay, value)
Set one relay state.

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

relay - relay number (id) [1..4]

value - relay state 1: turn ON, 0: turn OFF[0..1]


### set_relays(stack, value)
Set all relays state.

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

value - 4 bit value of all relays (ex: 15: turn on all relays, 0: turn off all relays, 1:turn on relay #1 and off the rest)


### get_relays(stack)
Return the state of all relays.

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

return - [0..15]


### get_iin_ma(stack, ch)
Return the selected input 4-20mA channel value in mA

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in mA [0..20]


###set_iout_ma(stack, ch, value)
Set the selected output 4-20mA channel value in mA

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

value - current output value in mA [4..20]


### get_iout_ma(stack, ch)
Return the selected output 4-20mA channel value in mA

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in mA [0..20]


### get_uin_v(stack, ch)
Return the selected input 0-10V channel value in volts

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in V [0..10]


### set_uout_v(stack, ch, value)
Set the selected output 0-10V channel value in volts

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

value - voltage output value in V


### get_uout_v(stack, ch)
Get the selected output 0-10V channel value in volts  

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

ch - selected channel number [1..4]

return - value in V [0..10]


### get_opto_in(stack)
Get the state of the opto-coupled inputs

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

return - value of the inputs [0..255]


### set_oc_pin(stack, pin, val)
Set the state of the corresponding open-collector output pin

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

pin - open-collector pin # [1..4]

val - state 0: ON, 1: OFF [0..1]


### get_oc_val(stack)
Get the previously set state of all open-collector output pins

stack - stack level of the megaIO Industrial card (selectable from address jumpers [0..3])

return - state of pins ( ex: 0: all pins OFF, 15: all pins ON, 1:  pin #1 ON, all the rest: OFF)
