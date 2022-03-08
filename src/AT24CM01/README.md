# AT24CM01
![Version](https://img.shields.io/badge/Version-v2.0.0-green.svg)

 EEPROM Library for the Atmel AT24CM01 using the Arduino platform.

### Features
---
* Supports Read / Write by variable type. Sizes are determined dynamically to support differences between devices.
* Supports Read / Write of arrays of either byte / char.
* Simple error checking of manualy entering sizes and addresses.
* Supports a TwoWire reference (begin function)

>**Read and Write by variable types of which are supported:*

|           small             |        medium       |      large           |    array     |
|           :---:             |        :---:        |      :---:           |      :---:   |
| bool                        | short               |  long                | char array   |
| char                        | word                |  unsigned long       | byte array   |
| unsigned char               | int                 |  float               |              |
| byte                        | unsigned int        |  double              |              |           


### Commands
---
See header file.

### Usage
---
The library will be found in the IDE/IO library manager. Alternatively, simply clone this library to your working library folder and include "AT24CM01.h" in your main sketch.

If you are having difficulties, please raise an issue.

### A Bit About the Sensor
---
See extras for the datasheet

### Authors
---
Myself, you can find my contact details below.

### License
---
This project is licensed under Apache 2.0 - see the LICENSE.md file for details

### Acknowledgments
----
This library took some inital formatting advice from the Arduino website.

### Feedback
---
Feel free to improve on the project and propose appropriate changes and even coding advice as I'm still new to this world.
