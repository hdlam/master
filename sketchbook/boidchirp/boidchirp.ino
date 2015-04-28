#include <Wire.h>
#include <Vector2D.h>
#include <Boids.h>
#include <Motors.h>
#include <math.h> 
#include <IrDistCom.h>

int motor_addr = 1;
int led_front_addr = 2;
int led_back_addr = 3;
int front_addr = 0xF;
int back_addr = 0xB;

byte received;
byte data[60];
int count;
long currentTime;
Boids other[3];
Boids me;
Motors motor;
IrDistCom ir;
unsigned short dists[8];

int thresh = 40;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial1.begin(9600);  // start Serial1 for output
  Serial.begin(9600);
  count = -1;
  ir.begin(front_addr, back_addr);
  motor.begin(motor_addr);

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

void turn(float angleToTurn){
  Serial.print("trying to turn:"); 
  Serial.println(angleToTurn);
  motor.turnDegrees(angleToTurn);
  motor.turnDegrees(angleToTurn);
  delay(1000);
}

void turnToAngle(float goal){
  float angleToTurn = goal-me.angle; //this is in RADS
  //refactoring
  if(angleToTurn < -PI)
    angleToTurn += (2*PI);
  else if(angleToTurn > PI)
    angleToTurn -= (2*PI);
  angleToTurn = angleToTurn*180/PI;

  /*if(!me.boidsNearby(other)){
    if(angleToTurn > -135 && angleToTurn < -45){
      if(dists[6] > thresh && dists[0] < thresh){
        motor.moveAtSpeeds(300,300);
        motor.moveAtSpeeds(300,300); 
        delay(1000);
        return;
      }
    }
    else if(angleToTurn > 45 && angleToTurn < 135){
      if(dists[2] > thresh && dists[0] < thresh){
        motor.moveAtSpeeds(300,300);
        motor.moveAtSpeeds(300,300); 
        delay(1000);
        return;
      }
    }
  }*/
  if(angleToTurn < 1 && angleToTurn > -1){
    delay(1000);
    return;
  }
  turn(angleToTurn);

}

void sendBTCommand(){
  //BT
  Serial1.print(me.vel.x);
  Serial1.print(",");
  delay(50);
  Serial1.print(me.vel.y);
  Serial1.print(",");
  delay(50);
  Serial1.print('c');
}

boolean checkDist(){
  ir.getDistSensors(dists);
  if(dists[0] > thresh){
    int coinFlip = random(0,2);
    if(coinFlip == 0){
      turn(90);  
    }
    else{
      turn(-90); 
    }
    me.vel.reset();
    sendBTCommand();
    motor.moveAtSpeeds(0,0);
    motor.moveAtSpeeds(0,0);
    return true;
  }
  else if(dists[1] > thresh){  //maybe make it 45 deg. instead?
    turn(90);
    me.vel.reset();
    sendBTCommand();
    motor.moveAtSpeeds(0,0);
    motor.moveAtSpeeds(0,0);
    return true;
  }
  else if(dists[7] > thresh){
    turn(90);
    me.vel.reset();
    sendBTCommand();
    motor.moveAtSpeeds(0,0);
    motor.moveAtSpeeds(0,0);
    return true;
  } 
  return false;
}


void loop()
{
  if(count == -1){

    Serial1.print('r');
    delay(100);
    //    Serial.println("sending r");
  }
  else if(millis()-currentTime > 5000){
    count = -1; //if nothing is received for 5 sec. reset the count.
    motor.moveAtSpeeds(0,0);
    motor.moveAtSpeeds(0,0);
    currentTime = millis();
    Serial.println("timed out, resetting count to -1");
  }


  received = 255;

  //GET DATA FROM Serial1
  while (Serial1.available() > 0)
  {
    received = (byte) Serial1.read();
    currentTime = millis();
    if(count == -1)
    {
      if(received == 0){ //0
        Serial.println("got stop");
        motor.moveAtSpeeds(0,0);
        motor.moveAtSpeeds(0,0);
      }
      else if(received == 76){ //L rotate left
        motor.moveAtSpeeds(-300,300);
        motor.moveAtSpeeds(-300,300);
        Serial.println("got rotate 1");
      }
      else if(received == 82){ //R, rotate right
        Serial.println("got rotate 2");
        motor.moveAtSpeeds(300,-300);
        motor.moveAtSpeeds(300,-300);
      }
      else if(received == 87){ //W, forward
        motor.moveAtSpeeds(300,300);
        motor.moveAtSpeeds(300,300);
      }
      else if(received == 66){ //B, backward
        motor.moveAtSpeeds(-300,-300);
        motor.moveAtSpeeds(-300,-300);
      }
      else if(received == 100){
        count = 0;
        //initializing receiving of BT data.
      }

    }
    else{
      data[count] = received;
      count++;
      if(count == (12+16*(me.getNumOfBots()-1) )){
        count = -1;
        me.setPos(convert(data,0),convert(data, 1));
        me.setAngle(convert(data, 2));

        for(byte i = 0; i < me.getNumOfBots()-1; i++){
          byte posInArr = 4*i;
          other[i].pos.x = convert(data, posInArr+3);   
          other[i].pos.y = convert(data, posInArr+4);   
          other[i].vel.x = convert(data, posInArr+5);
          other[i].vel.y = convert(data, posInArr+6);
        }
        Serial.println("");
        Serial.println("start");
        Serial.println("posX");
        Serial.println(me.pos.x);
        Serial.println("posY");
        Serial.println(me.pos.y);
        Serial.println("Angle");
        Serial.println(me.getAngle()); 
        for(byte i = 0; i <3; i++){
          Serial.println(other[i].pos.x);
          Serial.println(other[i].pos.y);
          Serial.println(other[i].vel.x);
          Serial.println(other[i].vel.y);
        }
        me.update(other);
        Serial.println("updated");
        delay(50);         
        //MOTOR

        if(checkDist()){

        }
        else if(me.vel.length() > 0){
          turnToAngle(atan2(me.vel.y, me.vel.x));
          Serial.println("angle turned");
          if(checkDist()){
            sendBTCommand();
          }
          else{
            sendBTCommand();
            motor.moveAtSpeeds(me.vel.length()*10, me.vel.length()*10);
            motor.moveAtSpeeds(me.vel.length()*10, me.vel.length()*10);
            Serial.print("speed is set to: ");
            Serial.println(me.vel.length());
          }
        }
        else{
          delay(1000); //ensures that the turning and non turning stays synchronized to a certain degree. 
        }

        Serial.print("end");


        delay(500);
      }
    }
  }

}










