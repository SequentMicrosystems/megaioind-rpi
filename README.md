
[![megaioind-rpi](readmeres/sequent.jpg)](https://sequentmicrosystems.com)

# megaioind-rpi

This is the command to control the old Industrialand Building Automation I/O Expansion cards for Raspberry Pi. 
The user manual can be found [here](https://sequentmicrosystems.com/docs/MEGA-IO-IND-UsersGuide.pdf)

Before compiling please install [Wiring Pi](http://wiringpi.com/download-and-install/), many thanks to Gordon Henderson for the library.

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/megaioind-rpi.git
~$ cd megaioind-rpi/
~/megaioind-rpi$ sudo make install
```

Now you can access all the functions of the MegaIO Industrial board using the command "megaioind".

Type megaioind -h for command options list.

If you clone the repository any update can be made with the following commands:

```bash
~$ cd megaioind-rpi/  
~/megaioind-rpi$ git pull
~/megaioind-rpi$ sudo make install
```  
## Firmware update

In order to update the firmware, your Raspberry Pi must be connected to the Internet. We recommend disconnecting all the outputs of the board during the update, to avoind damages to your equipment.

Execute the update program, with the stack level of your board as command line parameter:

```bash
~$ cd megaioind-rpi/update/
~/megaioind-rpi/update$ chmod 777 update
~/megaioind-rpi/update$ ./update 0
```
