/*
    Copyright 2017 Chiehting Huang <j.huang.1985@gmail.com>
*/
#include "Arduino.h"
#include "SN3193.h"
//#include <Wire.h>

//
// GND: 1101000 (0x68)
// VDD: 1101011 (0x6B)
// SCL: 1101001 (0x69)
// SDA: 1101010 (0x6A)
#define I2C_ADDR 0x68

// I2C commands
#define SHUTDOWN_REG           0x00
#define BREATING_CONTROL_REG   0x01
#define LED_MODE_REG           0x02
#define CURRENT_SETTING_REG    0x03
#define PWM_1_REG              0x04
#define PWM_2_REG              0x05
#define PWM_3_REG              0x06
#define PWM_UPDATE_REG         0x07
#define T0_1_REG               0x0A
#define T0_2_REG               0x0B
#define T0_3_REG               0x0C
#define T1T2_1_REG             0x10
#define T1T2_2_REG             0x11
#define T1T2_3_REG             0x12
#define T3T4_1_REG             0x16
#define T3T4_2_REG             0x17
#define T3T4_3_REG             0x18
#define TIME_UPDATE_REG        0x1C
#define LED_CONTROL_REG        0x1D
#define RESET_REG              0x2F

SN3193::SN3193(uint8_t i2cAddress) {
  _ledctrl = 0;
  _exist = false;
  address = i2cAddress;
}

SN3193::SN3193() {
  _ledctrl = 0;
  _exist = false;
  address = I2C_ADDR;
}

#if defined(ARDUINO_ARCH_ESP8266)
bool SN3193::begin(int SDA_pin, int SCL_pin) {
    Wire.begin(SDA_pin,SCL_pin);
#else
bool SN3193::begin() {
    Wire.begin();
#endif
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
        _exist = true;
    }
    return _exist;
}


bool SN3193::check() {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
        _exist = true;
    }
    return _exist;
}

bool SN3193::exist() 
{	
    return _exist;
}

void SN3193::powerOn() 
{
    write8(RESET_REG           , 0x00);
    write8(SHUTDOWN_REG        , 0x20);
    write8(BREATING_CONTROL_REG, 0x00);
}

void SN3193::powerOff() 
{
    write8(SHUTDOWN_REG, 0x20);
}

// =0 PWM mode, =1 One slot programming mode
void SN3193::mode(uint8_t mode)
{
    write8(LED_MODE_REG, mode<<5);   
}

// Ch:OUT1,OUT2,OUT3   ch= 1,2,3;
// T0(time of holding off)                  0-10 T0(D7-D4)  
// T1&T2(time of ramping up and hold on)    0-7  T1(D7-D5)|T2(D4-D1)|X
// T3&T4(time of ramping down and hold off) 0-7  T3(D7-D5)|T4(D4-D1)|X
// 0: 0.00s, 1: 0.13s, 2: 0.26s, 3: 0.52s, 4: 1.04s
// 5: 2.08s, 6: 4.16s, 7: 8.32s, 8:16.64s, 9:33.28s
//10:66.56s
void SN3193::setBreathTime(uint8_t ch, uint8_t T0, uint8_t T1, uint8_t T2, uint8_t T3, uint8_t T4)
{                              
    switch (ch)
    {
        case 1:
          write8(T0_1_REG  , (T0<<4));         // T0(time of holding off)
          write8(T1T2_1_REG, (T1<<4)|(T2<<1)); // T1&T2(time of ramping up and hold on
          write8(T3T4_1_REG, (T3<<4)|(T4<<1)); // T3&T4(time of ramping down and hold off)
          break;
        case 2:  
          write8(T0_2_REG  , (T0<<4));         // T0(time of holding off)
          write8(T1T2_2_REG, (T1<<4)|(T2<<1)); // T1&T2(time of ramping up and hold on
          write8(T3T4_2_REG, (T3<<4)|(T4<<1)); // T3&T4(time of ramping down and hold off)
          break;
        case 3:
          write8(T0_3_REG  , (T0<<4));         // T0(time of holding off)
          write8(T1T2_3_REG, (T1<<4)|(T2<<1)); // T1&T2(time of ramping up and hold on
          write8(T3T4_3_REG, (T3<<4)|(T4<<1)); // T3&T4(time of ramping down and hold off)
          break;
        break;
    } 
}

// need to write to TIME_UPDATE_REG after setting up T0_x_REG, T1T2_x_REG, T3T4_x_REG
void SN3193::timeUpdate(void)
{
  write8(TIME_UPDATE_REG,0x00);  
}

void SN3193::setPWM(uint8_t ch1,uint8_t ch2,uint8_t ch3)
{
    write8(PWM_1_REG, ch1);
    write8(PWM_2_REG, ch2);
    write8(PWM_3_REG, ch3);
    pwmUpdate();
}

void SN3193::setPWM(uint8_t ch,uint8_t value)
{
    write8(PWM_1_REG+ch-1, value);
}

// need to write to PWM_UPDATE_REG after setting up PWM_x_REG, LED_CONTROL_REG
void SN3193::pwmUpdate(void)
{
    write8(PWM_UPDATE_REG,0x00);  
}

// 0: 42mA, 1: 10mA, 2: 5mA, 3: 30mA, 4-7: 17.5mA 
void SN3193::setCurrent(uint8_t level)
{
    write8(CURRENT_SETTING_REG, level<<2);
}

void SN3193::turnOn(uint8_t ch)
{
    _ledctrl |= (1<<(ch-1));
    write8(LED_CONTROL_REG, _ledctrl);
}

void SN3193::turnOff(uint8_t ch)
{
    _ledctrl &= (~(1<<(ch-1)));
    write8(LED_CONTROL_REG, _ledctrl);
}


void SN3193::write8(uint8_t a, uint8_t d) {
    Wire.beginTransmission(address); // start transmission to device 
#if (ARDUINO >= 100)
    Wire.write(a); // sends register address to read from
    Wire.write(d);  // write data
#else
    Wire.send(a); // sends register address to read from
    Wire.send(d);  // write data
#endif
    Wire.endTransmission();
}

