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
  sn3193.setCurrent(4);
  sn3193.setPWM(100,100,100); 
  sn3193.setBreathTime(1,4,4,4,4,4); 
  sn3193.setBreathTime(2,4,4,4,4,5); 
  sn3193.setBreathTime(3,4,4,6,4,6); 
  sn3193.timeUpdate();
  sn3193.mode(1);  
  delay(30000);
}
