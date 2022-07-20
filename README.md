# Longan Labs CANBed Dual Arduino Library

[![Actions Status](https://github.com/arduino/arduino-cli-example/workflows/test/badge.svg)](https://github.com/arduino/arduino-cli-example/actions)
[![Spell Check](https://github.com/arduino/compile-sketches/workflows/Spell%20Check/badge.svg)](https://github.com/arduino/compile-sketches/actions?workflow=Spell+Check)
[![codecov](https://codecov.io/gh/arduino/compile-sketches/branch/main/graph/badge.svg?token=Uv6f1ebMZ4)](https://codecov.io/gh/arduino/compile-sketches)

Arduino library for CANBed Dual.

With this library, you can,

1. Send a CAN2.0 frame
2. Receive a CAN2.0 frame
3. Send a CAN FD frame
4. Receive a CAN FD frame

## Installation

1. [Download the library](https://github.com/Longan-Labs/CANBedDual_Arduino_Lib/archive/refs/heads/master.zip)
2. Extract the zip file
3. In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library

## Respository Contents

* [**/examples**](./examples) - Example sketches for the library (.ino). Run these from the Arduino IDE.
* [**/src**](./src) - Source files for the library (.cpp, .h).
* [**keywords.txt**](./keywords.txt) - Keywords from this library that will be highlighted in the Arduino IDE.
* [**library.properties**](./library.properties) - General library properties for the Arduino package manager.

## Functions

- init()
- initFD()
- initMaskFilt()
- send()
- read()

## Examples

here are many examples implemented in this library. One of the examples is below. You can find other examples [here](./examples)

```Cpp
// CANBED DUAL TEST EXAMPLE
// CAN 0 Send, CAN 1 Recv
//

#include <Wire.h>
#include <stdio.h>
#include "canbed_dual.h"

CANBedDual CAN0(0);
CANBedDual CAN1(1);


void setup()
{
    Serial.begin(115200);
    pinMode(18, OUTPUT);
    
    Wire1.setSDA(6);
    Wire1.setSCL(7);
    Wire1.begin();
    
    CAN0.initFD(500000, 4000000);          // CAN0 baudrate: 500kb/s, FD baudrate, 4Mb/s
    CAN1.initFD(500000, 4000000);          // CAN1 baudrate: 500kb/s, FD baudrate, 4Mb/s
}

void loop()
{
    sendData();
    
    unsigned long id = 0;
    int ext = 0;
    int rtr = 0;
    int fd = 0;
    int len = 0;
    
    unsigned char dtaGet[100];

    if(CAN1.read(&id, &ext, &rtr, &fd, &len, dtaGet))
    {
        Serial.println("CAN1 GET DATA");
        Serial.print("id = ");
        Serial.println(id);
        Serial.print("ext = ");
        Serial.println(ext);
        Serial.print("rtr = ");
        Serial.println(rtr);
        Serial.print("fd = ");
        Serial.println(fd);
        Serial.print("len = ");
        Serial.println(len);

        for(int i=0; i<len; i++)
        {
            Serial.print(dtaGet[i]);
            Serial.print("\t");
        }
        Serial.println();
    }
}

void sendData()
{
    static unsigned int cnt = 0;
    cnt++;
    if(cnt > 99)cnt = 0;
    unsigned char str[64];
    for(int i=0; i<64; i++)str[i] = cnt;
    CAN0.send(0x01, 0, 0, 1, 64, str);
}

// ENDIF
```

## Get a Dev Board

If you need a Dev board, plese try,

- [CANBed Dual](https://www.longan-labs.cc/1030019.html)


## License

```
MIT License

Copyright (c) 2018 @ Longan Labs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Contact us

If you have any question, please feel free to contact [support@longan-labs.cc](support@longan-labs.cc)


[![Analytics](https://ga-beacon.appspot.com/UA-101965714-1/Longan_CANFD)](https://github.com/igrigorik/ga-beacon)
