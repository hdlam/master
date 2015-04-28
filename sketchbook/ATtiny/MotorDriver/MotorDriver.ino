//    Christian Skjetne 2012-2013
//
//    Chirp-bot motor driver
//
//    ATtiny84 chip
//
//Commands: R,L,r,l,G,S,H,D,T,0,1,A

#include <AccelStepper.h>
#include "TinyWireS84.h" 

#define I2C_SLAVE_ADDR    0x1

// Define some steppers and the pins the will use
AccelStepper stepperRight(AccelStepper::FULL4WIRE, 3, 1, 2, 0);
AccelStepper stepperLeft(AccelStepper::FULL4WIRE, 7, 9, 8, 10);

byte byteRcvd = 0;
int motorSpeed = 0;

byte i2cCommand[] = {
    0x00,0x00,0x00};

long steps = 0;
boolean go = true;
uint16_t leftSteps    = 0;
uint16_t rightSteps = 0;
int leftSpeed    = motorSpeed;
int rightSpeed = motorSpeed;
bool leftEnabled = false;
bool rightEnabled = false;

void setup()
{ 
    stepperLeft.disableOutputs();
    stepperLeft.setCurrentPosition(0);
    stepperLeft.setMaxSpeed(500);
    stepperLeft.setAcceleration(1000);
    safeSetLeftSpeed(0);

    stepperRight.disableOutputs();
    stepperRight.setCurrentPosition(0);
    stepperRight.setMaxSpeed(500);
    stepperRight.setAcceleration(1000);
    safeSetRightSpeed(0);

    TinyWireS.begin(I2C_SLAVE_ADDR);
    TinyWireS.onRequest(on_request);// callback method for i2c requests
}

void loop()
{
    //GET DATA FROM MASTER:
    int recByte = 0;
    if(TinyWireS.available() )//we expect only 3 bytes
    {
        while(recByte < 3)
        {
            i2cCommand[recByte] = TinyWireS.receive();
            recByte++;
        }
    }
    
    //HANDLE DATA FROM MASTER:
    switch (i2cCommand[0]) // Command byte
    {
    case 'L':// Left speed - set left speed
        leftSpeed = byteBufferToInt16(i2cCommand);
        break;
    case 'R':// Right speed - set right speed
        rightSpeed = byteBufferToInt16(i2cCommand);
        break;
    case 'l':// left steps - set left number of steps
        leftSteps = (uint16_t)byteBufferToInt16(i2cCommand);
        break;
    case 'r':// right steps - set right number of steps
        rightSteps = (uint16_t)byteBufferToInt16(i2cCommand);
        break;
    case 'G'://Go - start moving at constant speed
        safeSetSpeeds(leftSpeed, rightSpeed);
        go = true;
        break;
    case 'S':// Step - move to relative spot (use l and r command to set the number of steps)
        safeSetSpeeds(leftSpeed, rightSpeed);
        
        steps = rightSteps;
        if(rightSpeed < 0) steps = -1 * steps; //we need to give the steps in the right direction. Positive is clockwise 
        stepperRight.move(steps);
        
        steps = leftSteps;
        if(leftSpeed < 0) steps = -1 * steps; //we need to give the steps in the right direction. Positive is clockwise 
        stepperLeft. move(steps);
        
        // stepperRight.setSpeed(rightSpeed);
        // stepperLeft. setSpeed(leftSpeed);
        go = false;
        break;
    case 'H':// Halt - stop moving. This command does not erase the previous speed setting. just send Go command to resume.
        safeSetSpeeds(0, 0);
        break;
    case 'D':// Done? - returns number of steps left to destination
        break;
    case 'T':// Test - blinks pin 5 the given number of times
        for(int i = 0;i<(uint16_t)byteBufferToInt16(i2cCommand);i++)
        {    
            pulse(100);
        }
        break;
    case '0': // 0 - disables the output to the motors to save power
        stepperRight.disableOutputs();
        stepperLeft. disableOutputs();
        break;
    case '1': // 1 - enables the outputs to the motors
        stepperRight.enableOutputs();
        stepperLeft. enableOutputs();
        break;
    case 'A': // Acceleration - sets the acceleration value
        stepperRight.setAcceleration(byteBufferToInt16(i2cCommand));
        stepperLeft. setAcceleration(byteBufferToInt16(i2cCommand));
        break;
    }

    if(go)// run at constant speed
    {
        stepperRight.runSpeed(); 
        stepperLeft. runSpeed();
    }
    else// run only if we have not arrived at our destination
    {
        stepperRight.run();//SpeedToPosition();
        stepperLeft. run();//SpeedToPosition();
        if(stepperLeft.speed() == 0)
            stepperLeft.disableOutputs();
        if(stepperRight.speed() == 0)
            stepperRight.disableOutputs();
    }

    i2cCommand[0] = 0x00;         //flush buffer

}

void safeSetSpeeds(int lspeed, int rspeed)
{
    safeSetLeftSpeed(lspeed);
    safeSetRightSpeed(rspeed);
}

//sets the motor speed hopefully without burning up the lipo booster.
void safeSetLeftSpeed(int lspeed)
{
    if(abs(lspeed) <= 25 )
    {
        stepperLeft.setSpeed(0);
        stepperLeft.setMaxSpeed(0);
        stepperLeft.disableOutputs();
        leftEnabled = false;
        return;
    }
    if(!leftEnabled){
        stepperLeft.enableOutputs();
        leftEnabled = true;
    }
    stepperLeft.setSpeed(lspeed);
    stepperLeft.setMaxSpeed(abs(lspeed));

}

//sets the motor speed hopefully without burning up the lipo booster.
void safeSetRightSpeed(int rspeed)
{
    if(abs(rspeed) <= 25 )
    {
        stepperRight.setSpeed(0);
        stepperRight.setMaxSpeed(0);
        stepperRight.disableOutputs();
        rightEnabled = false;
        return;
    }
    if(!rightEnabled){
        stepperRight.enableOutputs();
        rightEnabled = true;
    }
    stepperRight.setSpeed(rspeed);
    stepperRight.setMaxSpeed(abs(rspeed));

}


// turn on, then of of pin 5 with the given interval. (Blocking)
void pulse(int delays)
{
    digitalWrite(5, HIGH);
    delay(delays);
    digitalWrite(5, LOW);
    delay(delays);
}


// extract the int value of the data field from the given buffer.
int byteBufferToInt16(byte b[])
{
    int i    = b[1] << 8;
    i         |= b[2];
    return i;
}

// callback method for i2c requests. 
//TODO: check to make sure that the last command received from the MASTER was a request command.
//TODO: enable requesting of more data (ex: current speed etc.)
void on_request(void)
{
    //right dist to go:
    long dtg = stepperRight.distanceToGo();
    if(dtg < 0) dtg = dtg * -1; //absolute value (direction given from wheel speed direction)
    TinyWireS.send('r');
    TinyWireS.send((dtg >> 8) & 0xFF);
    TinyWireS.send(dtg & 0xFF);

    //left dist to go:
    dtg = stepperLeft.distanceToGo();
    if(dtg < 0) dtg = dtg * -1;//absolute value (direction given from wheel speed direction)
    TinyWireS.send('l');
    TinyWireS.send((dtg >> 8) & 0xFF);
    TinyWireS.send(dtg & 0xFF);
}



