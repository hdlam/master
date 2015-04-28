#include <Wire.h>
#include <Motors.h>
#include <IrDistCom.h>
#include <Vector2D.h>
#include <Boids.h>

int motor_addr = 1;
int led_front_addr = 2;
int led_back_addr = 3;
int front_addr = 0xF;
int back_addr = 0xB;

Motors motor;
IrDistCom ir;
unsigned short dists[8];

void setup()
{
  Wire.begin(); 

  Serial.begin(9600);
  motor.begin(motor_addr);
  ir.begin(front_addr, back_addr);
  
  for(byte i = 0; i < 10; i++){
     delay(1000);
     Serial.println(i); 
  }
  
}



byte thresh = 30;
void loop()
{
  Serial.println("new round");
  Serial.println();
  ir.getDistSensors(dists);
  for(byte i = 0; i < 8; i++){
    Serial.print(i);
    Serial.print(":");
    Serial.println(dists[i]);
  }
  /*
  if(dists[0] > thresh){
    motor.turnDegrees(180);
    motor.turnDegrees(180);ø
    Serial.println("front");
    
  }
  else if(dists[1] > thresh){
    motor.turnDegrees(-90); 
    motor.turnDegrees(-90); 
    Serial.println("turning left");
  }
  else if(dists[7] > thresh){
    motor.turnDegrees(90); 
    motor.turnDegrees(90); 
    Serial.println("turning right?");
  }
  else{
   motor.moveAtSpeeds(500,500); 
   Serial.println("go 4th");
  }
  */
  
  delay(500);
  
}








