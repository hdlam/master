////Christian Skjetne


#include <Wire.h>

//#define I2C_SLAVE_ADDR  0x1            // i2c slave address (38)
byte test;
byte ta[4] = {
  255,255,255,255};
byte count = 0;
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial1.begin(9600);  // start Serial1 for output
  Serial.begin(9600);

}

float convert(byte array[]){
  union
  {
    byte b [4];
    float f;  
  }
  conv;
  for(byte i = 0; i < 4; i++){
    conv.b[3-i] = array[i]; 
  }
  return conv.f;
}
union
{
  byte b [4];
  float f;  
}
pos;

void loop()
{
  test = 255;
  //GET DATA FROM Serial1
  while (Serial1.available() > 0)//we expect only 5 bytes
  {
    test = (byte) Serial1.read();
  }

  if(test != 255){
    pos.b[3-count] = test;
    count++;
    if(count == 4){
      count = 0; 
      for(byte i = 0; i < 4; i++)
      {
        Serial.print((String)pos.b[i] + " "); // prints the byte in hex
        //    speedLong = (speedLong << 8) | speedArray[i];
      }
      //pos.b = ta;
      Serial.println(pos.f);
    }
  }






}














