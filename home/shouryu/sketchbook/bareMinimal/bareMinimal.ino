#include <math.h>

void setup(){
  Serial.begin(9600);
}


void loop(){
  Serial.println(atan2(2,0));
}
