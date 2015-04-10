#include <Vector2D.h>
#include <Boids.h>
#include <math.h> 

byte received;
byte data[60];
int count;
long currentTime;
Boids other[3];
Boids me;


void setup()
{


  Serial.begin(9600);
  count = -1;

}

void turnToAngle(float goal){
  Serial.println("goal goal goal");
  Serial.println(goal);
  float angleToTurn = goal-me.angle; //this is in RADS
  //refactoring
  if(angleToTurn < -PI)
    angleToTurn += (2*PI);
  else if(angleToTurn > PI)
    angleToTurn -= (2*PI);

  angleToTurn = angleToTurn*180/PI;
  Serial.print("trying to turn:"); 
  Serial.println(angleToTurn);

  delay(500);
}

float convert(byte array[], int start){
  union
  {
    byte b [4];
    float f;  
  }
  conv;
  for(byte i = 0; i < 4; i++){
    conv.b[i] = array[i+start*4]; 
  }
  return conv.f;
}
void loop()
{
  received = 255;

  //GET DATA FROM Serial1

  currentTime = millis();
  data[count] = received;
  count++;

  me.pos.x = 424.58;
  me.pos.y = 270.87;
  //  setPos(424.58,270.87);
  me.setAngle(1.22);


  /*  other[0].pos.x = 158.00;
   other[0].pos.y = 191.28;
   other[0].vel.x = 114.01;
   other[0].vel.y = 137.76;
   
   other[1].pos.x = 483.10;
   other[1].pos.y = 310.36;
   other[1].vel.x = -210.98;
   other[1].vel.y = 18.35;
   
   other[2].pos.x = 175.06;
   other[2].pos.y = 485.34;
   other[2].vel.x = 96.97;
   other[2].vel.y = -156.11;  */  //separation debug

  other[0].pos.x = 158.00;
  other[0].pos.y = 191.28;
  other[0].vel.x = 100;
  other[0].vel.y = 0;

  other[1].pos.x = 483.10;
  other[1].pos.y = 310.36;
  other[1].vel.x = 100;
  other[1].vel.y = 0;

  other[2].pos.x = 175.06;  
  other[2].pos.y = 485.34;
  other[2].vel.x = -100;
  other[2].vel.y = 30;

  /*  Serial.println("");
   Serial.println("start");
   Serial.println("posX");
   Serial.println(me.pos.x);
   Serial.println("posY");
   Serial.println(me.pos.y);
   Serial.println("Angle");
   Serial.println(me.getAngle()); 
   for(byte i = 0; i <3; i++){u
   Serial.println(other[i].pos.x);
   Serial.println(other[i].pos.y);
   Serial.println(other[i].vel.x);
   Serial.println(other[i].vel.y);
   }*/
  me.update(other);
  //Serial.println("updated;  ");
  
  
  delay(50);        
  //BT
  turnToAngle(atan2(0,0));
  Serial.print(me.vel.x);
  Serial.print(",");
  delay(50);
  Serial.print(me.vel.y);
  Serial.print(",");
  delay(50);
  Serial.println('c'); 

  /*  //MOTOR
   //turnToAngle(atan2(me.vel.y, me.vel.x));
   Serial.println("angle turned");
   //motor.moveAtSpeeds(me.vel.length(), me.vel.length());
   Serial.print("speed is set to: ");
   Serial.println(me.vel.length());
   //motor.moveAtSpeeds(me.vel.length(), me.vel.length());
   Serial.print("end");
   */
  Serial.println("");
  Serial.println("");
  delay(2000);




}









