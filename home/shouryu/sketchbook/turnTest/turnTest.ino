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
  motor.turnDegrees(1); //den blir kjørt men stopper i Wire.endTransmission();
  delay(2000);
  motor.turnDegrees(2); //den blir kjørt men stopper i Wire.endTransmission();
  delay(5000);
  motor.turnDegrees(3); //den blir kjørt men stopper i Wire.endTransmission();
  delay(2000);
  
}{
  motor.turnDegrees(1); //den blir kjørt men stopper i Wire.endTransmission();
  delay(2000);
  motor.turnDegrees(2); //den blir kjørt men stopper i Wire.endTransmission();
  delay(5000);
  motor.turnDegrees(3); //den blir kjørt men stopper i Wire.endTransmission();
  delay(2000);
  
}
