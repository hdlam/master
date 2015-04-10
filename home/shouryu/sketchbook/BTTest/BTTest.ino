
byte received;
byte data[60];
byte count;
long currentTime;
int connectionTimeOut;
//Vector pos(0,0);
void setup()
{

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
  

  //GET DATA FROM Serial1
  while (Serial1.available() > 0)
  {
    received = (byte) Serial1.read();
    currentTime = millis();
    Serial1.println("received");
    Serial1.println(received);
    
  }

}

















