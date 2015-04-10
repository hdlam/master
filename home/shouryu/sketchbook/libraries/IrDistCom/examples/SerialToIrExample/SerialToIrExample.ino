/*
 *  IrDistCom example.
 *  Author: Christian Skjetne
 *
 *  You can make the robots talk to each other sending the following commands over serial:
 *  D - prints the distance sensor data.
 *  S - Sends a message to nearby robots.
 *  R - Prints any messages a robot may have received. 
 *      (Printing of messages are done automatically when detected, 'R' is only here to use as an example)
 */

#include "Wire.h"
#include <Motors.h>
#include <IrDistCom.h>

Motors motors;
IrDistCom ir;



void setup()
{
  Wire.begin();
  Serial.begin(9600);
  motors.moveAtSpeeds(0,0);// just to stop the motors
}

void loop()
{
  while(Serial.available())
  {
    Serial.println("got it, lets go!");
    char order = Serial.read();
    Serial.println(order);
    if(order == 'D')
    {
      unsigned short distSensors[8];
      ir.getDistSensors(distSensors);//reads the distance sensors and updates the 'distSensors' array with the values.
      Serial.println("Front:");
      Serial.println(distSensors[0]);//sensor 1
      Serial.println(distSensors[1]);//sensor 2
      Serial.println(distSensors[2]);//sensor 3
      Serial.println(distSensors[3]);//sensor 4
      Serial.println("\nBack:");
      Serial.println(distSensors[4]);//sensor 5
      Serial.println(distSensors[5]);//sensor 6
      Serial.println(distSensors[6]);//sensor 7
      Serial.println(distSensors[7]);//sensor 8
    }
    else if(order == 'S')//Sends a string to the robot.
    {
      byte data[] = {'C','h','r','i','s','t','i','a','n'};
      //Very short messages (about size less than 3 or 4)) are actually discurraged. 
      // If you need to send a short command you shoud send a string with
      // length of least 3 or 4 chars with the command character repeated.
      
      // Bytes 0, 1, 3, 6, 21 (NULL,SOT,EOT,ACK,NAK) are reserved. 
      // if transmitted they can in the worse case crash the ATtinies.
      byte result = ir.sendIrDataBlocking(data,9);// <- this method blocks until the message is either sent, or it times out.
      // The result byte indicates what happened with the transmission:
      // 0->front-timeout&back-timeout | no problem, probably not any robots in range.
      // 1->front-success&back-timeout | someone on ir led 0-3 got the message 
      // 2->front-timeout&back-success | someone on ir led 4-7 got the message
      // 3->both-success               | wow, you managed to send the message to two robots at the same time.
      // 5->Error                      | You have discovered a bug! Try turning the robot off and on again :/
      Serial.print("Status: ");
      Serial.println(result);
    }
    else if(order == 'R')
    {
      byte dataStatus = 0;
      if((dataStatus = ir.isIrDataAvailable()) != 0)
      {// ^^ The isIrDataAvailable can be used to check if the ATtinies 
       // have received any transmissions. The method returns a status byte that 
       // indicates which, if any, of the ATtinies that has data.
       // 0->no data available
       // 1->front has data
       // 2->back has data
       // 3->both have data
        Serial.print("tiny has data!, status: ");
        Serial.println(dataStatus);
        //lets get it from the tiny mentioned in the status:
        boolean front = true;// the front has data
        if(dataStatus == 2) front = false;// the back has data
        byte data[32]; // buffer max size is 32 bytes
        byte sizeOfData = ir.getIrData(front, data);// <- gets the data and sets it in the 'data' array. 
        // sizeOfData is the amount of bytes we got from the ATtiny.
        Serial.print("got data. size: ");
        Serial.println(sizeOfData, DEC);
        Serial.println("DATA:");
        for(int i = 0; i < sizeOfData; i++)
        {
          char c = data[i];
          Serial.print(c);// Prints out the data in the array as characters.
        }
        Serial.print("\n\n");
      }
      else
      {
        Serial.println("Tiny has no data, sorry.");
      }
    }
  }
  delay(100);
  // AUTO PRINT This method is identical to the order == 'R' above,
  // but since it runs on the main loop, it prints to serial once a new transmission has been received.
  if(ir.isIrDataAvailable() != 0)
  {
    Serial.println("AUTO READ DATA!!!");
    byte dataStatus = 0;
    if((dataStatus = ir.isIrDataAvailable()) != 0)
    {
      //lets get it:
      boolean front = true;
      if(dataStatus == 2) front = false;
      byte data[32];
      byte sizeOfData = ir.getIrData(front, data);
      Serial.print("size: ");
      Serial.println(sizeOfData, DEC);
      Serial.println("DATA:");
      for(int i = 0; i < sizeOfData; i++)
      {
        char c = data[i];
        Serial.print(c);
      }
      Serial.print("\n\n");
    }
  }
  //Start broadcast
  // You can trigger the sending of "@@@" (example of a good way to transmit an '@' command)
  // by pulling pin 13 HIGH.
  if(digitalRead(13) == HIGH)
  {
    for(int i=0;i<10;i++)
    {
      byte data[] = {'@','@','@'};
      byte result = ir.sendIrDataBlocking(data,3);
      Serial.print("Status: ");
      Serial.println(result);
      if(result > 0) break;
      else delay(50);
    }
  }
}

















