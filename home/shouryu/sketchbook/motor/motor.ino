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



byte tresh = 30;
void loop()
{
  Serial.println("new round");
  ir.getDistSensors(dists);
  for(byte i = 0; i < 8; i++){
     Serial.println(dists[i]);
  }
  if(dists[0] > tresh)
    motor.moveAtSpeeds(500,0);
  if(dists[1] > tresh)
    motor.moveAtSpeeds(0,500);
  if(dists[7] > tresh)
    motor.moveAtSpeeds(500,0);
  else
    motor.moveAtSpeeds(500,500);  
  delay(200);
}








