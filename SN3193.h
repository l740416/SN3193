/*
  Copyright 2017 Chiehting Huang <j.huang.1985@gmail.com>

*/
#ifndef SN3193_h
#define SN3193_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
 #include "TinyWireM.h"
 #define Wire TinyWireM
#elif defined(ARDUINO_ARCH_ESP8266)
 #include <Wire.h>
#else
 #include <avr/pgmspace.h>
 #include <Wire.h>
#endif

class SN3193
{
  public:
    SN3193();
    SN3193(uint8_t i2cAddress);
#if defined(ARDUINO_ARCH_ESP8266)
    bool begin(int SDA_pin, int SCL_pin);
#else
    bool begin();
#endif
		bool check();
    bool exist();
    void powerOn();
    void powerOff();
    void mode(uint8_t mode);
    void setBreathTime(uint8_t Ch, uint8_t T0, uint8_t T1, uint8_t T2, uint8_t T3, uint8_t T4);
    void timeUpdate(void);
    void setPWM(uint8_t ch1,uint8_t ch2,uint8_t ch3);
    void setPWM(uint8_t ch,uint8_t value);
    void pwmUpdate(void);
    void setCurrent(uint8_t level);
    void turnOn(uint8_t ch);
    void turnOff(uint8_t ch);
    
  private:
    void    write8(uint8_t a, uint8_t d);
    bool    _exist;
    uint8_t _ledctrl;
    uint8_t  address;
};

#endif

