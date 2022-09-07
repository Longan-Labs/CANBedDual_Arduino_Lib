// Arduino library for CANBed Dual GD32E103CBT6

#include <Wire.h>
#include <Arduino.h>

#define I2C_ADDR    0X41
#define CANI2C      Wire1

class CANBedDual
{
  private:
    unsigned long timerDelay = 0;
    int canNum = 0;
    unsigned char canConfig[48];
    
  private:
    unsigned long char2long(unsigned char *str);
    void long2char(unsigned long __t, unsigned char *str);
    void makeCanConfig();
    void sendConfig();
    
  public:
    CANBedDual(int num)
    {
        canNum = num;
    }
    
    void init(unsigned long speed);
    void initFD(unsigned long speed20, unsigned long speedfd);
    void initMaskFilt(unsigned char num, unsigned char ext, unsigned long mask, unsigned long filt);
    // id  CAN id
    // ext 0:standard, 1:extended, 
    // rtr 0:data, 1:remote
    // fd  0:classic 1:can fd
    // len data length
    // dta data buffer
    void send(unsigned long id, unsigned char ext, unsigned char rtr, unsigned char fd, unsigned char len, unsigned char *dta);
    byte checkRecv();
    byte read(unsigned long *id, int *ext, int *rtr, int *fd, int *len, unsigned char *str);
};
