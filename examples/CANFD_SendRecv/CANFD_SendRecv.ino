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