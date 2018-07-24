
[![megaioind-rpi](readmeres/sequent.jpg)](https://www.sequentmicrosystems.com/megaio.html)

# megaioind-rpi

This is the command to control [Mega-IO Industrial Card](https://www.sequentmicrosystems.com/megaio-ind.html)

Before compiling you have to install [Wiring Pi](http://wiringpi.com/download-and-install/), many thanks to Gordon Henderson for the library

## Usage

```bash
~$ git clone https://github.com/alexburcea2877/megaioind-rpi.git
~$ cd megaioind-rpi/
~/megaioind-rpi$ sudo make install
```

Now you can access all the functions of the MegaIO Industrial board through the command "megaioind".
Type megaioind -h for command options list.
If you clone the repository any update can be made with the following commands:

```bash
~$ cd megaioind-rpi/  
~/megaioind-rpi$ git pull
~/megaioind-rpi$ sudo make install
```  
