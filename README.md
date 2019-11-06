LED Cube 8x8x8
==============

![](https://i.imgur.com/PlWxtu5.jpg)

## The Cube in action

[![](https://img.youtube.com/vi/Bjb18RBypwo/0.jpg)](http://www.youtube.com/watch?v=Bjb18RBypwo)

Author : Jose Javier Gonzalez Ortiz
Copyright ©2013-2014, Jose Javier Gonzalez Ortiz. All rights reserved.

Code for controlling a 8x8x8 LED Cube via SPI.

The code is designed for a Microchip dSPIC33FJ32MC202 microcontroller. 

The libraries work together to enable the LED cube to play effects and games interacting
with the user via serial communication.



    - cubo -        General library has the main initialization and the SPI configuration.
                    It also is the responsible for loading the cube configuration periodically in the SPI module

    - function -    Library designed as a general purpose interface to configure the different parts of the cube.
                    Supports the treatment with integer and voxel structures.

    - effect -      A completely functional effect launcher with the capability of secondary effect and variable period.
                    A set of effect launcher related functions are defined.
                    A huge amount of effects are also included divided in different types and categories.

    - font -        Library used to define a 8x8 bitmap font to be used by the cube. 
                    Ascii, greek, hiragana and other fonts are included. 
                    Font effects are also defined as well as a few helper functions to interact with the constants.

    - snake -       Library used to define a Snake-like game in three dimensions which is executed by the cube.

    - life -        Library to display a variable-ruled three dimensional Conway's Game of Life in the cube.
                    A few glider based effect are predefined for convinience.

    - serial -      Library to interact with the uart module and parse the commands and parameters received by it.
                    Echo is managed. 

External libraries (other people's work):

    - config -      General library for configuring the PLL oscillator and uart mapping.

    - uart -        Library which include functions to easily interact with the uart module.
