// CANBED DUAL TEST EXAMPLE, set mask and filter
// CAN 0 Send, CAN 1 Recv

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
    
    CAN0.init(500000);          // CAN0 baudrate: 500kb/s
    CAN1.init(500000);          // CAN1 baudrate: 500kb/s
    
    CAN1.initMaskFilt(0, 0, 0x7ff, 0x01);
    CAN1.initMaskFilt(1, 0, 0x7ff, 0x03);
    CAN1.initMaskFilt(2, 0, 0x7ff, 0x05);
    CAN1.initMaskFilt(3, 0, 0x7ff, 0x07);
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
    static unsigned long id = 0;
    id++;
    if(id > 7)id = 0;
    
    unsigned char str[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    CAN0.send(id, 0, 0, 0, 8, str);
}

// ENDIF