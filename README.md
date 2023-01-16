# DDS generator with TFT touch screen 

This repo contains source code of the signal generator I made during my bachelor's thesis.

## Features

* 0.1-4 MHz range DDS signal generation (sine, triangle, square) with frequency and amplitude control
* 5 Vp-p PWM signal with frequency and duty cycle control 
* LCD touch screen with fully operational GUI (signal on-off, wave select, frequency/amplitude/duty control, virtual numpad
* Rotary encoder for fine-tuning
* USB connector for remote UART connection with PC.

## Hardware

There is ATMega2560 MCU inside and it controls all of equipement inside:
* AD9833 DDS generator
* MCP41010 digital potentiometer
* ILI9341 TFT screen (320x240) with resistive touch panel
* Rotary encoder

Moreover, there are two op-amps (AD8675) as amplifier of DDS (sin/squ/tri) and PWM outputs. Both outputs can be cut off by relay.

## Software

Source code was written in VS Code with Arduino IDE support. Most of code was included in `dds.ino` file. 

The `ILI9341.h` file contains the vast majority of functions to control LCD screen and touch panel. It was based on https://github.com/PaulStoffregen/ILI9341_t3, but I have included some more functions, such as rectangles with round corners (fillets), Rect struct, Touch struct and easier virtual buttons control.

The project utilizes libraries:
* AD9833.h (https://github.com/Billwilliams1952/AD9833-Library-Arduino) - DDS driver
* Timers.h (https://github.com/nettigo/Timers) - some timing

These libs need to be added to Arduino by Library manager (by downloading .zip from GitHub and option "add .zip library").

Files included with description:

* **dds.ino** - main file with most of logic
* **ILI9341.h** - LCD Touch screen driver (generating shapes, sprites, decoding and printing of fonts, touch panel)
* **pwm.h** - PWM generation using Timer4
* **BcdFloat.h* - BCDFloat class. It defines new class of object which contains floating-point number as set of digits with decimal point. Provides basic functionality (adding, substracting, approxing, changing of specified digits, removing residual zeros). It has been set up to provide better management of floating-point values by GUI and avoid floating-point computing errors (e.g. 20.0000 + 10.00000 = 29.99999).
* **symbols.h** - contains memory-programmable graphics

* **Fonts** - directory contains pre-rendered font graphics.
