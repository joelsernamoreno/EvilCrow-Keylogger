# EvilCrow-Keylogger
WiFi keylogger with Micro SD slot, based on the Atmega32U4 microcontroller and the ESP32-PICO module

![EvilCrow](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/Logo1.jpg)



**Idea, development and implementation:** Joel Serna (@JoelSernaMoreno) & Ernesto Sánchez (@ernesto_xload)

**Collaborators:**  Ignacio Díaz (@Nacon_96) / Forensic&Security (@ForensicSec)

**PCB design, manufacturer and distributor:** April Brother (@aprbrother)

The developers and collaborators of this project do not earn money with this. 
You can invite me for a coffee to further develop Low-Cost hacking devices. If you don't invite me for a coffee, nothing happens, I will continue developing devices.

[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/E1E614OA5)

**For Sale at:**

* Aliexpress: Coming soon...
* Tindie: Coming soon...

# Disclaimer

Evil Crow Keylogger is a physical keylogger device for professionals and cybersecurity enthusiasts.

AprilBrother and the collaborators of this project are not responsible for the incorrect use of Evil Crow Keylogger.

We recommend using this device for testing, learning and fun :D

# Introduction

Evil Crow Keylogger is a physical keylogger with the following hardware:

* Atmega32U4 with Arduino Lilypad USB bootloader
* ESP32-PICO module for communication via Wi-Fi
* Slot MicroSD
* USB Host MAX3421
* Hall sensor for unbrick device

**NOTE:** Some keys or modifiers have not been implemented, this is a PoC. I don't have time or material to test all the keyboards. If you have any errors, you can contact me by Twitter: @JoelSernaMoreno

**Layouts:**

* BE_BE layout support.
* DE_DE layout support.
* ES_ES layout support.
* EN_US layout support.
* FI_FI layout support.
* FR_FR layout support.
* IT_IT layout support.
* PT_PT layout support.
* TR_TR layout support.

**TODO:**

* MORE LAYOUTS
* MORE KEYS
* TEST ALL LAYOUTS

**NOTE:** Please do not ask me to implement new functions in this code. You can develop code for Evil Crow Keylogger and send me PR with your new code.

# Installation

## Software requirements

* 0.- Add your user to the dialout group: sudo usermod -a -G dialout USER

* 1.- Install esptool: sudo apt install esptool

* 2.- Install pyserial: sudo pip install pyserial

* 3.- Download and Install the Arduino IDE: https://www.arduino.cc/en/main/software

* 4.- Open Arduino IDE.

* 5.- Go to File - Preferences. Locate the field "Additional Board Manager URLs:" Add "https://dl.espressif.com/dl/package_esp32_index.json" without quotes. Click "Ok"

* 6.- Select Tools - Board - Boards Manager. Search for "esp32". Install "esp32 by Espressif system version 1.0.3". Click "Close".

* 7.- Download/extract EvilCrow-Keylogger repository.

* 8.- Copy the Keyboard and USB Host Shield libraries included in this repository to your Arduino library directory. 

**NOTE:** The Keyboard library included in this repository has been modified, EvilCrow Keylogger needs this library to work.

## Upload the ESP32 code

To upload the ESP32 code into the keylogger, you can do this in different ways: You can use an Arduino, an FTDI or an ESP Flasher from April Brother.
On this way I will use an Arduino to upload the ESP32 code.

Here you can see all the pins corresponding to ESP32:

![ESP32](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/esp32.PNG)

* 0.- Wire the Keylogger with Arduino using the following pinout:

![Flash](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/esp32-flash.PNG)

![Wire](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/wire.jpg)

* 1.- Open Arduino IDE.

* 2.- Open the ESP32.ino sketch.

* 3.- Select Tools - Board - "ESP32 Dev Module".

* 4.- Connecting the Arduino device to the computer.

* 5.- Upload the code to the board.

## Upload atmega32u4 code

* 0.- Connect Evil Crow Keylogger via USB port.

![usb](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/usb.jpg)

* 1.- Open Arduino IDE.

* 2.- Open the ATMEGA32U4.ino sketch.

* 3.- Select Tools - Board – "Arduino Lilypad USB".

* 4.- Upload the code to the board.

* Done!

# First steps with Evil Crow Keylogger

* 0.- Connect a keyboard to the Evil Crow Keylogger USB host port.

* 1.- Connect Evil Crow Keylogger to your laptop.

* 2.- Open a notepad and type Hello World with the keyboard connected to the keylogger

* 3.- Visualize the wifi networks around you and connect to the Keylogger network.

* 4.- Enter the password for the wifi network (default password: 123456789).

* 5.- Open a browser and access the web panel (default IP: 192.168.4.1).

* 6.- Click on the View Log option

![Log](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/log.jpg)

# Use the Micro SD Slot

Evil Crow Keylogger has a slot to use an SD card, but this is not implemented in this code.

To use the SD card you will have to program the necessary code for this. 

To test the SD slot... you can use some basic example of SD included in the Arduino IDE.

You will only have to change the default CS pin to D5. This is the example:

![SD](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/sd.png)

# Unbrick Evil Crow Keylogger with Hall Sensor

First, you’ll need to set the serial port to the bootloader. But that port is only visible when the board is in bootloader mode, so pull the reset line low twice quickly to invoke the bootloader reset feature. You can quickly press the reset button** twice. While the Evil Crow Keylogger is in the bootloader mode, change the ‘Tools > Serial Port’ menu to the bootloader COM port. Quick! You’ve only got eight seconds.

**The reset button is a HALL sensor, that means you need to place a magnet close that side of the PCB, in order to simulate the "button pressure".**

![Sensor](https://github.com/joelsernamoreno/EvilCrow-Keylogger/blob/master/images/sensor.PNG)

* 1.- Open Arduino IDE and open ATMEGA32U4.ino sketch

* 2.- Connect Evil Crow Keylogger via USB port.

* 3.- Press Upload sketch

* 4.- Start the unbrick phase with a magnet by placing it close that side of the PCB where the hall sensor is located (do it two times). Close-away-close-away
