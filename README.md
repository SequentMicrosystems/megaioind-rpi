
[![megaioind-rpi](readmeres/sequent.jpg)](https://www.sequentmicrosystems.com/megaio-ind.html)

# megaioind-rpi

This is the command to control the [Industrial](https://sequentmicrosystems.com/index.php?route=product/product&path=33&product_id=52) and [Building](https://sequentmicrosystems.com/index.php?route=product/product&path=33&product_id=53) Automation I/O Expansion cards for Raspberry Pi.

Before compiling please install [Wiring Pi](http://wiringpi.com/download-and-install/), many thanks to Gordon Henderson for the library.

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/megaioind-rpi.git
~$ cd megaioind-rpi/
~/megaioind-rpi$ sudo make install
```

To access the functions of the Mega
Now you can access all the functions of the MegaIO Industrial board through the command "megaioind".

Type megaioind -h for command options list.

If you clone the repository any update can be made with the following commands:

```bash
~$ cd megaioind-rpi/  
~/megaioind-rpi$ git pull
~/megaioind-rpi$ sudo make install
```  
## Firmware update

Now you can update board firmware with latest version from our website and for that Raspberry must be connected to internet. We recomend you to disconnect all outputs from board before update, during update outputs might change and can damage your external equipaments. 

For first usage you must set the execution rights for the update program.

```bash
~$ cd megaioind-rpi/update/
~/megaioind-rpi/update$ chmod 777 update
```
To update you must execute the program with stack level (jumper settings) of your board

```bash
~$ cd megaioind-rpi/update/
~/megaioind-rpi/update$ ./update 0
```
