#include <math.h>

void setup(){
  Serial.begin(9600);
}

int num = 0;
int zeros = 0;
int ones = 0;
int ran;
void loop(){
  
  if(num < 1000){
    num++;
    ran = random(0,2);
    if(ran == 1)
      ones++;
      else
      zeros++;
    
  }
  else if(num == 1000){
   Serial.print("num of 0s:");
   Serial.println(zeros); 
   Serial.print("num of 1s:");
   Serial.println(ones);
  }
  
}
