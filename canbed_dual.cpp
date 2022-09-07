#include "canbed_dual.h"


unsigned long CANBedDual::char2long(unsigned char *str)
{
    unsigned long __t = 0;

    __t = str[0];
    __t <<= 8;
    __t |= str[1];
    __t <<= 8;
    __t |= str[2];
    __t <<= 8;
    __t |= str[3];
    return __t;
}

void CANBedDual::long2char(unsigned long __t, unsigned char *str)
{
    str[0] = (__t>>24)&0xff;
    str[1] = (__t>>16)&0xff;
    str[2] = (__t>>8)&0xff;
    str[3] = (__t>>0)&0xff;
}

void CANBedDual::makeCanConfig()
{
    unsigned long can20_baud = 500000;
    unsigned long canfd_baud = 1000000;
    
    unsigned char mf_set0   = 1;
    unsigned char mf_ext0   = 0;
    unsigned long mf_mask0  = 0;
    unsigned long mf_filt0  = 0;
    
    unsigned char mf_set1   = 0;
    unsigned char mf_ext1   = 0;
    unsigned long mf_mask1  = 0;
    unsigned long mf_filt1  = 0;
    
    unsigned char mf_set2   = 0;
    unsigned char mf_ext2   = 0;
    unsigned long mf_mask2  = 0;
    unsigned long mf_filt2  = 0;
    
    unsigned char mf_set3   = 0;
    unsigned char mf_ext3   = 0;
    unsigned long mf_mask3  = 0;
    unsigned long mf_filt3  = 0;
    
    long2char(can20_baud, &canConfig[0]);
    long2char(canfd_baud, &canConfig[4]);
    
    // set mask&filt 0
    canConfig[8] = mf_set0;
    canConfig[9] = mf_ext0;
    long2char(mf_mask0, &canConfig[10]);
    long2char(mf_filt0, &canConfig[14]);
    
    // set mask&filt 1
    canConfig[18] = mf_set1;
    canConfig[19] = mf_ext1;
    long2char(mf_mask1, &canConfig[20]);
    long2char(mf_filt1, &canConfig[24]);
    
    // set mask&filt 2
    canConfig[28] = mf_set2;
    canConfig[29] = mf_ext2;
    long2char(mf_mask2, &canConfig[30]);
    long2char(mf_filt2, &canConfig[34]);
    
    // set mask&filt 3
    canConfig[38] = mf_set3;
    canConfig[39] = mf_ext3;
    long2char(mf_mask3, &canConfig[40]);
    long2char(mf_filt3, &canConfig[44]);
}

void CANBedDual::sendConfig()
{
    CANI2C.beginTransmission(0x41);
    CANI2C.write(canNum ? 0x19 : 0x09);
    CANI2C.write(canConfig, 48);
    CANI2C.endTransmission();
    
    delay(10);
}

void CANBedDual::init(unsigned long speed)
{
    makeCanConfig();
    long2char(speed, &canConfig[0]);
    long2char(1000000, &canConfig[4]);
    
    sendConfig();
}

void CANBedDual::initFD(unsigned long speed20, unsigned long speedfd)
{

    makeCanConfig();
    long2char(speed20, &canConfig[0]);
    long2char(speedfd, &canConfig[4]);
    
    sendConfig();
}
    
void CANBedDual::initMaskFilt(unsigned char num, unsigned char ext, unsigned long mask, unsigned long filt)
{
    if(num > 3) return;
    canConfig[10*num+8] = 1;
    canConfig[10*num+9] = ext;
    long2char(mask, &canConfig[10+10*num]);
    long2char(filt, &canConfig[14+10*num]);
    
    sendConfig();
}
    
void CANBedDual::send(unsigned long id, unsigned char ext, unsigned char rtr, unsigned char fd, unsigned char len, unsigned char *dta)
{
    unsigned char dsend[100];

    dsend[0] = canNum ? 0x15 : 0X05;
    long2char(id, &dsend[1]);

    dsend[5] = rtr;       // data or remote, 1:remote, 0:data
    dsend[6] = ext;       // standard or ext, 1 ext, 0 standard

    dsend[7] = fd;
    dsend[8] = len;

    for(int i=0; i<dsend[8]; i++)
    {
        dsend[i+9] = dta[i];
    }

    CANI2C.beginTransmission(I2C_ADDR);
    CANI2C.write(dsend, dsend[8] + 9);
    CANI2C.endTransmission();
}

byte CANBedDual::checkRecv()
{
    CANI2C.beginTransmission(I2C_ADDR);
    CANI2C.write(canNum ? 0x14 : 0x04);
    CANI2C.endTransmission();

    CANI2C.requestFrom(I2C_ADDR, 1);
    int len = CANI2C.read();

    return len;
}

byte CANBedDual::read(unsigned long *id, int *ext, int *rtr, int *fd, int *len, unsigned char *str)
{
    if(checkRecv() == 0) return 0;

    CANI2C.beginTransmission(I2C_ADDR);
    CANI2C.write(canNum ? 0x16 : 0x06);
    CANI2C.endTransmission();

    CANI2C.requestFrom(I2C_ADDR, 8);

    unsigned char strg[100];

    for(int i=0; i<8; i++)
    {
        strg[i] = CANI2C.read();
    }

    unsigned long _id = 0;
    
    _id = char2long(&strg[0]);
    
    *id = _id;
    *rtr = strg[5];
    *ext = strg[4];
    *fd  = strg[6];

    int dtaLen = strg[7];
    *len = dtaLen;

    if(dtaLen <= 32)
    {
        CANI2C.beginTransmission(I2C_ADDR);
        CANI2C.write(canNum ? 0x17 : 0x07);
        CANI2C.endTransmission();

        CANI2C.requestFrom(I2C_ADDR, dtaLen);

        for(int i=0; i<dtaLen; i++)
        {
            str[i]= CANI2C.read();
        }
    }
    else
    {
        CANI2C.beginTransmission(I2C_ADDR);
        CANI2C.write(canNum ? 0x17 : 0x07);
        CANI2C.endTransmission();

        CANI2C.requestFrom(I2C_ADDR, 32);

        for(int i=0; i<32; i++)
        {
            str[i]= CANI2C.read();
        }

        CANI2C.beginTransmission(I2C_ADDR);
        CANI2C.write(canNum ? 0x18 : 0x08);
        CANI2C.endTransmission();

        CANI2C.requestFrom(I2C_ADDR, dtaLen-32);

        for(int i=0; i<(dtaLen-32); i++)
        {
            str[32+i]= CANI2C.read();
        }
    }

    return dtaLen;
}
    
    
