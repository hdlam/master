#include <Wire.h>
#include <Chirp.h>
#include <Boids.h>




byte received;
byte data[60];
byte count;
long currentTime;
int connectionTimeOut;
//Vector pos(0,0);
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial1.begin(9600);  // start Serial1 for output
  Serial.begin(9600);
  count = 0;

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



boolean ini = false;
void loop()
{
  if(count == 0){
    Serial1.print('r');
    delay(100);
  }
  else if(millis()-currentTime > 2000){
    count = 0;
    Serial.println("count is reset");
  }


  received = 255;

  //GET DATA FROM Serial1
  while (Serial1.available() > 0)
  {
    received = (byte) Serial1.read();
    currentTime = millis();
    
    data[count] = received;
    count++;
    if(count == 60){
      count = 0;
      float posx = convert(data, 0);
      float posy = convert(data, 1);
      float angle = convert(data, 2);
      for(byte i = 0; i < 60; i++){
        Serial.print(data[i]);
        Serial.print(",");
      }
      Serial.println("");
      Serial.println("start");
      Serial.println("posX");
      Serial.println(posx);
      Serial.println("posY");
      Serial.println(posy);
      Serial.println("Angle");
      Serial.println(angle);

      Serial.println("");

      Serial.println(convert(data, 3));
      Serial.println(convert(data, 4));
      Serial.println(convert(data, 5));
      Serial.println(convert(data, 6));

      Serial.println(convert(data, 7));
      Serial.println(convert(data, 8));
      Serial.println(convert(data, 9));
      Serial.println(convert(data, 10));

      Serial.println(convert(data, 11));
      Serial.println(convert(data, 12));
      Serial.println(convert(data, 13));
      Serial.println(convert(data, 14));
      Serial.println("end");

      Serial1.print(20);
      Serial1.print(40);
      delay(200);
      //pos.b = ta;
      //      Serial.println(pos.f);*
    }
  }

}

















