# RUMPSDUINO

## About

This project seeks out to introduce the RUMPS401 chip to the Arduino environment. It provides the plug-ins required to use and program the RUMPS401 in the Arduino IDE.

The RUMPS401 is a quad-core, low-power, ARM-M0 based SoC fully designed and owned by Universiti Tunku Abdul Rahman (UTAR), Malaysia. 

We are developing sets of libraries along with the compilation and upload toolchain necessary to make the chip itself accessible to wide range of users.



## Maintainers

Currently, this project is jointly-maintained by both the **Faculty of Engineering and Green Technology, UTAR** and the **Computer Engineering Department, Universitas Multimedia Nusantara (UMN), Indonesia**.

From 7th December 2019 onwards, all updates on this project is maintained at this repo. *The original repo at https://github.com/fanfan328/rumpsduino is **no longer maintained**.*



## Hardware & Software Specification

**Hardware**

- RUMPS401 DevBoard-01 development board
- Any USB to UART module. The PL2303 or CH340 are preferrable.

**Software**

- Windows 10
- Tested on Arduino IDE 1.8.9

*Specifications for the RUMPS401 chip and the DevBoard-01, as well as the hardware itself is not available publicly. For those interested, kindly contact the CP listed at the end of this document*



## User Guide

### Preparing the Environment

The plugin files and libraries are all located **inside** the `rumpsduino_master` folder

There are two ways of reproducing the RUMPSDUINO environment, based on the Arduino Sketchbook configuration.

**A. Assigning new sketchbook folder location**

1. Create a new folder in any location (*preferably those that need no admin privilege*), of any name.
2. Copy the content of the `rumpsduino_master` folder into the newly created folder.
3. Change the Sketchbook location on your Arduino IDE preferences (*Arduino IDE > File > Preferences*) to the newly created folder. 
4. In the Arduino IDE, select *Tools > Board*, and you should see the `RUMPS401_Alpha_1.1` listed as one of the boards.
5. Select that board. If not, you may need to restart the Arduino IDE.
6. Open the sample program (*described in the next section*), and try to verify or upload.

**B. Integrating into the existing sketchbook folder ** (*may need administrator privilege when running the Arduino IDE, depends on the location*)

1. Note your Arduino IDE sketchbook folder location from the preferences (*Arduino IDE > File > Preferences*)

2. Integrate the content of the `rumpsduino_master` folder into the Sketchbook folder location with these steps:

    - copy the content of `rumpsduino_master/hardware` to `Sketchbook_folder/hardware`
    - copy the content of `rumpsduino_master/tools` to `Sketchbook_folder/tools`
    - copy the content of `rumpsduino_master/libraries` to `Sketchbook_folder/libraries` (*currently still empty*)

3. In the Arduino IDE, select *Tools > Board*, and you should see the `RUMPS401_Alpha_1.1` listed as one of the boards.

4. Select that board. If not, you may need to restart the Arduino IDE.

5. Open the sample program (*described in the next section*), and try to verify or upload.

    

### Program structure

The RUMPS401 is equipped with four ARM M0-based cores with individual RAM and Flash memory, thus **each core is capable of / must be running its own program**.

A network-on-chip is connecting the four cores (*which details can be found on the RUMPS401 specification document*)

Programming for the RUMPS401 in the Arduino IDE is straightforward, i.e.

- You have a single `.ino` sketch file as in other Arduino boards

- The `.ino` sketch **must** follow the structure below, which is self-explanatory. The sketch defines four sets of `setup` and `loop` function for the four cores, one each. 

  ``` c
  // Program for core-0
  void setup(){
      // put your initial setup here
  }
  void loop(){
      // put your looping task here
  }
  
  // Program for core-1
  void setup1(){
      // put your initial setup here
  }
  void loop1(){
      // put your looping task here
  }
  
  // Program for core-2
  void setup2(){
      // put your initial setup here
  }
  void loop2(){
      // put your looping task here
  }
  
  // Program for core-3
  void setup3(){
      // put your initial setup here
  }
  void loop3(){
      // put your looping task here
  }
  ```



### Libraries

Currently, there are only the `Digital io`, `delay` and Serial available as Arduino-flavor functions. The library development is still very much a work on progress.

Examples regarding usage of these libraries and functions are available in the `examples` folder.



## Closing

This project is still far from complete as we are adding more libraries into the environment, while the hardware itself is still limited in terms of accessibility and number.

If you have any problem, feel free to email your questions to stefanus.krn@gmail.com or dareen.halim@umn.ac.id