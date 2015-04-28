//  Christian Skjetne, Jean-Marc Montanier 2012-2013
//
//  Chirp-bot sensor and communication driver
//
//  ATtiny84 chip

#include <TinyWireS.h>
#include <SoftwareSerialIr.h>

#define baud 9600//4800
#define SOT 1 //start
#define EOT 3 //end
#define ACK 6
#define NAK 21

#define led_pin0 7
#define led_pin1 8
#define led_pin2 9
#define led_pin3 10

#define detector_pin0 0
#define detector_pin1 1
#define detector_pin2 2
#define detector_pin3 3

#define threshold 500 //200 TESTME
#define distThreshold 700
#define sigma 100

#define i2c_address 0xB //change this
#define TWI_RX_BUFFER_SIZE 32
#define MAX_SIZE_IR_DATA 32 

byte byteRcvd = 0;

boolean gotcall = false;
boolean docall = false;
int recvPoint = -1;// this is the pin that detected the transmission. (callpoint+7 is the corresponding ir-receiver)

int preVal = 0;
int newVal = 0;

int count = 1;
int nbCall = 0; //for debug purposes

volatile byte* ptrData;
volatile int sizeData = 0;

byte receivedIRData[MAX_SIZE_IR_DATA];
int sizeReceivedIRData = 0;
boolean sizeReceivedIRDataSent = false;
int nbByteSentIRData = 0;
volatile byte transmissionStatus = 4;

byte i2cCommand = 0x00;
byte dataFromI2C[TWI_RX_BUFFER_SIZE];
int sizeDataFromI2C = 0;

struct sensor_data_t{
unsigned sensor0 : 
  10;
unsigned sensor1 : 
  10;
unsigned sensor2 : 
  10;
unsigned sensor3 : 
  10;
} 
sensor_data;

void setup()
{
  pinMode(led_pin0, OUTPUT);
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(led_pin3, OUTPUT);
  pinMode(5, OUTPUT); //flashing LED

  TinyWireS.begin(i2c_address);
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);

  randomSeed(analogRead(0));
}


//send data to anyone available in broadcast
void sendDataToIR(byte data[],int sizeData)
{
  for (int callPoint = detector_pin0 ; callPoint <= detector_pin3 ; callPoint ++ )
  {
    digitalWrite(callPoint+7, HIGH);
    //trial multiple times just in case ...
    for (int trial = 0 ; trial <3 ; trial++)
    {
      if(handshake(callPoint) == true)
      {
        startIrTransfer(callPoint,data,&sizeData); 
        transmissionStatus = 1;
        break;
      }
    }
    digitalWrite(callPoint+7, LOW);
  }
  if(transmissionStatus > 1)
  {
    transmissionStatus = 0;
  }
}

//reads the sensors and receive the data if any imcoming call
void readSensors(void)
{
  digitalWrite(detector_pin0+7, LOW);
  digitalWrite(detector_pin1+7, LOW);
  digitalWrite(detector_pin2+7, LOW);
  digitalWrite(detector_pin3+7, LOW);   
  sensor_data.sensor0 = checkDist(detector_pin0);
  sensor_data.sensor1 = checkDist(detector_pin1);
  sensor_data.sensor2 = checkDist(detector_pin2);
  sensor_data.sensor3 = checkDist(detector_pin3);

  if(gotcall)
  {
    if(handshake(recvPoint))
    {
      sizeReceivedIRData = MAX_SIZE_IR_DATA;
      startIrTransfer(recvPoint,receivedIRData,&sizeReceivedIRData);
      //pulse(receivedIRData[sizeReceivedIRData-1]);
      if(sizeReceivedIRData > 0)
      {
        digitalWrite(5,HIGH);
      }
      gotcall = false;
    }
  }
}

//Checks the distance on the given pin and sets the data.
int checkDist(int pin)
{
  digitalWrite(pin+7, LOW);
  tws_delay(1);
  preVal = analogRead(pin);
  tws_delay(3);
  int tempVal = analogRead(pin);
  digitalWrite(pin+7, HIGH);
  tws_delay(1);
  newVal = analogRead(pin);
  digitalWrite(pin+7, LOW);

  if(tempVal < preVal) preVal = tempVal; //to avoid getting blinded in the low part by other robots, check two times.

  if(checkCom(preVal, newVal, pin))//signal detected. start the handshake thing!
  {
    gotcall = true;
    recvPoint = pin;
    digitalWrite(pin+7, HIGH);
  }
  if(newVal > distThreshold) return distThreshold;
  else return abs(newVal-preVal);
}

//This method is triggered when we have a suspected transmission coming in.
boolean checkCom(int v1, int v2, int apin)
{
  if(v1 > threshold && v2 > threshold)//possible detection! preval should be lower. (we can set the threshold to change the sensitivity. (should match the communication range more or less, but is now way too low (too sensitive)))
  {
    for(byte i=0;i<4;i++)//check for high signal 4 times in 4ms
    {
      tws_delay(1);
      if(analogRead(apin) <= threshold) //(if the signal goes <= the threshold during the detection, we ignore the call)
      {  
        return false; 
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}



//simple method to blink an led. Poor mans display.
void pulse(int number)
{
  if(number > 10) return;
  for(byte i=0;i<number;i++)
  {
    digitalWrite(5, HIGH);
    tws_delay(100);
    digitalWrite(5, LOW);
    tws_delay(500);  
  } 
}


boolean handshake(int apin)
{
  if(gotcall) //wait for the sender to put his led low
  {
    //pulse(2);
    tws_delay(20);
    for(int i = 0 ; i < 10 ; i++ )
    {
      if(digitalRead(apin))//analogRead(apin) <= threshold) TESTME
      {  
        digitalWrite(apin+7,LOW); 
        return true; 
      }
      tws_delay(1);
    }
  }
  else if (docall)//wait for a receiver to put his led high
  {  
    //pulse(1);
    tws_delay(20);
    for(int i = 0 ; i < 4 ; i++ )
    {
      int readValue = analogRead(apin);
      if (checkCom(readValue,readValue,apin) == true)
      {
        digitalWrite(apin+7,LOW); 
        return true;
      }
    }
  }
  return false; 
}

//This method triggers once the handshake has been detected. Sends or receives data.
void startIrTransfer(int apin, byte data[],int* sizeData)
{
  SoftwareSerialIr mySerial(apin, apin+7, true, true); // RX, TX
  mySerial.begin(baud);
  mySerial.flush();

  if(docall)//send data!
  {
    int i =0;
    while(1)
    {
      mySerial.write(SOT);//start of transfer.
      for(int byteNum = 0; byteNum < *sizeData; byteNum++)
        mySerial.write(data[byteNum]); //the byte of data being transmitted.
      mySerial.write(EOT);//end of transfer.
      char c = 0;
      while(mySerial.available())
      {
        c = mySerial.read();
        if(c == ACK)//check to see if we got an ack back.
        {
          //got ack
          break;
        }
        else if(c == NAK)//check to see if we got a nack back.
        {
          //got nak
          break;
        }
        else if(i>20)
        {
          break;
        }
        else
        {
          i++;
        }
      }
      if(c == ACK || i>20)//Transmit five 0x00's (this ends the transmission.)
      {
        for(byte j=0;j<5;j++) 
        {
          mySerial.write((byte)0);
        }
        break;
      }
      i++;
      tws_delay(50);
    }
  }
  else// receive data!
  {
    memset(data, 'f', (*sizeData));
    int i =0;
    while(1)
    {
      char c = 0;
      while(mySerial.available())
      {
        c = mySerial.read();
        if(c==SOT)
        {
          *sizeData = 0; //reset buffer if the we got a SOT.
        }
        else if(c==EOT) break;     //if we got a EOT, we end the transmission.
        else 
        {
          data[(*sizeData)++] = c; //put the data in the buffer.
          if((*sizeData) >= MAX_SIZE_IR_DATA) //buffer overflow
            break;
        }
      }
      if(c==EOT) //we got an EOT, send ack.
      {
        mySerial.write(ACK);
        break;
      }
      if((*sizeData)>=MAX_SIZE_IR_DATA) 
      {
        *sizeData = 0;
        mySerial.write(NAK);//buffer overflow before EOF, send nack.
      }
      if(c==0)i++;//we count the 0x00 bytes, terminate if more than 30.
      else i=0;
      if(i>=30)//30 reads is a timeout.
      {
        *sizeData = 0;//TODO: we have garbage 0 data, this is to to confirm that
        break;
      }
      i++;
      tws_delay(50);
    }
  }
  gotcall = false;
  docall = false;
}

//sends the distance data back over i2c.
void sendDataToI2C(uint8_t* data,int length)
{
  for (int i=0 ; i < length ; i++)
  {
    TinyWireS.send(data[i]);
  }
}


//void on_request(void)//used to poll data from the sensors over i2c.
//{
//  /*
//  readSensors();
//   digitalWrite(5, HIGH); //debug LED
//   sendData();
//   */
//  char str[4]= {
//    0,0,0,0      };
//  sprintf(str, "%d", newVal);
//  TinyWireS.send((unsigned) str[0]);
//  TinyWireS.send((unsigned) str[1]);
//  TinyWireS.send((unsigned) str[2]);
//  TinyWireS.send((unsigned) str[3]);
//}

void loop()//main loop.
{
  /*
  if(digitalRead(5) == HIGH)
   {
   docall = true;
   byte data[] = {
   4,2,5    };
   sendDataToIR(data,3);
   docall = false;
   }
   */
  //readSensors();


  if (i2cCommand == 'S' && transmissionStatus > 1)
  {
    docall = true;
    byte data[MAX_SIZE_IR_DATA];
    int localSize = sizeDataFromI2C;
    memcpy(data,dataFromI2C,MAX_SIZE_IR_DATA);
    sendDataToIR(data,localSize);
    digitalWrite(5, LOW);
  }
  else
  { 
    readSensors();
    ptrData = (byte*) &sensor_data;
    sizeData = sizeof(sensor_data);
  }

}


void receiveEvent(uint8_t howMany)
{
  i2cCommand = TinyWireS.receive();
  if(i2cCommand == 'S')  digitalWrite(5, HIGH);
  
  /*howMany--;
   if (!howMany)
   {
   // This write was only to set the buffer for next read
   return;
   }
   sizeDataFromI2C = 0;
   while(howMany--)
   {
   dataFromI2C[sizeDataFromI2C] = TinyWireS.receive();
   sizeDataFromI2C++;
   }*/
  sizeDataFromI2C = TinyWireS.receive();
  for (int nbByteDataRead = 0 ; nbByteDataRead < sizeDataFromI2C ; nbByteDataRead ++)
  {
    //sizeReceivedIRData = TinyWireS.receive();
    dataFromI2C[nbByteDataRead] = TinyWireS.receive();
  }
  //  sizeReceivedIRData = sizeDataFromI2C;
}



void requestEvent(void)
{
  if(i2cCommand == 'D') 
  {
    if(sizeData > 0) 
    {
      TinyWireS.send(*ptrData);
      ptrData ++;
      sizeData --;
    }
    else if (sizeData == 0)
    {
      TinyWireS.send(*ptrData);
      i2cCommand = 0x00;
    }
    /*
    else
     {
     TinyWireS.send(0x00);
     }
     */
  }
  else if(i2cCommand == 'R')
  {
    if (sizeReceivedIRDataSent == false)
    {
      digitalWrite(5,LOW);
      TinyWireS.send(sizeReceivedIRData);
      sizeReceivedIRDataSent = true;
      nbByteSentIRData = 0;
      if(sizeReceivedIRData == 0)
      {
        i2cCommand = 0x00;
        sizeReceivedIRDataSent = false;
        sizeReceivedIRData = 0;
        return;
      }
    }
    else
    {
      if (nbByteSentIRData < sizeReceivedIRData )
      {
        TinyWireS.send(receivedIRData[nbByteSentIRData++]);
      }

      if ( nbByteSentIRData >= sizeReceivedIRData )
      {
        i2cCommand = 0x00;
        sizeReceivedIRDataSent = false;
        sizeReceivedIRData = 0;
      }
    }
  }
  else if(i2cCommand == 'S')
  {
    if(transmissionStatus > 1) return;
    TinyWireS.send(transmissionStatus);
    i2cCommand = 0x00;
    transmissionStatus = 3;
  }
  else
  {
    TinyWireS.send(0x00);
  }
}















