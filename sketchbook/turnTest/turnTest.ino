#include <Wire.h>
#include <Motors.h>

Motors motor;
void setup(){
  
  Serial.begin(9600);
  Wire.begin();
  motor.begin(1);
    
  for(byte i = 0; i < 10; i++){
     delay(1000); //slik at jeg har tid til å åpne serial monitor.
     Serial.println(i); //bare en liten timer slik at jeg vet når den faktisk prøver å starte motorer
     
  }
}

void loop(){
  motor.turnDegrees(180);
  motor.turnDegrees(180);
  delay(2000);
  motor.turnDegrees(-90);
  motor.turnDegrees(-90);
  delay(2000);
  motor.turnDegrees(-90);
  motor.turnDegrees(-90);
  delay(2000);
  
}
