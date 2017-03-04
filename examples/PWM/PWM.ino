#include <Wire.h>
#include <SN3193.h>


SN3193 sn3193;
  
void setup() {
  Serial.begin(9600);
  if (!sn3193.begin()) {
  	Serial.println("Could not find a valid SN3193 controller, check wiring!");
  	while (1) {}
  }
}

void loop() {
    
  sn3193.powerOn();  
  sn3193.mode(0);  
  sn3193.setCurrent(4);
  sn3193.turnOn(1);
  sn3193.turnOn(2);
  sn3193.turnOn(3);
  sn3193.setPWM(1,100); 
  sn3193.setPWM(2,100); 
  sn3193.setPWM(3,100);
  sn3193.pwmUpdate();
  delay(10000);
}
