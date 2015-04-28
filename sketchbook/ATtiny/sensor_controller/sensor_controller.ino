#include <TinyWireS84.h>

#define led_pin0 7
#define led_pin1 8
#define led_pin2 9
#define led_pin3 10

#define detector_pin0 0
#define detector_pin1 1
#define detector_pin2 2
#define detector_pin3 3

#define i2c_address 0xB

struct sensor_data_t{
  unsigned sensor0 : 10;
  unsigned sensor1 : 10;
  unsigned sensor2 : 10;
  unsigned sensor3 : 10;
} sensor_data, ir_data;

void setup()
{
  pinMode(led_pin0, OUTPUT);
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(led_pin3, OUTPUT);
  
  TinyWireS.begin(i2c_address);
  TinyWireS.onRequest(on_request);
}
const short default_num_ir_readings = 10;
const short default_num_distance_readings = 10;
short num_ir_readings = 10;
short num_distance_readings = 10;
short ir_index = 0;
short distance_index = 0;
sensor_data_t recent_ir_readings[default_num_ir_readings];
sensor_data_t recent_distance_readings[default_num_distance_readings];
short ir_totals[4];
short distance_totals[4];

void readSensors(void)
{
  
  int preVal = analogRead(detector_pin0);
  ir_data.sensor0 = preVal;
  digitalWrite(led_pin0, HIGH);
  delay(1);
  int newVal = analogRead(detector_pin0);
  if(preVal < newVal) 
    sensor_data.sensor0 = newVal-preVal;
  digitalWrite(led_pin0, LOW);

  
  preVal = analogRead(detector_pin1);
  ir_data.sensor1 = preVal;
  digitalWrite(led_pin1, HIGH);
  delay(1);
  newVal = analogRead(detector_pin1);
  if(preVal < newVal) 
    sensor_data.sensor1 = newVal-preVal;
  digitalWrite(led_pin1, LOW);
  
  preVal = analogRead(detector_pin2);
  ir_data.sensor2 = preVal;
  digitalWrite(led_pin2, HIGH);
  delay(1);
  newVal = analogRead(detector_pin2);
  if(preVal < newVal) 
    sensor_data.sensor2 = newVal-preVal;
  digitalWrite(led_pin2, LOW);
  
  preVal = analogRead(detector_pin3);
  ir_data.sensor3 = preVal;
  digitalWrite(led_pin3, HIGH);
  delay(1);
  newVal = analogRead(detector_pin3);
  if(preVal < newVal) 
    sensor_data.sensor3 = newVal-preVal;
  digitalWrite(led_pin3, LOW);

  ir_totals[0] -= recent_ir_readings[ir_index].sensor0;
  ir_totals[1] -= recent_ir_readings[ir_index].sensor1;
  ir_totals[2] -= recent_ir_readings[ir_index].sensor2;
  ir_totals[3] -= recent_ir_readings[ir_index].sensor3;

  distance_totals[0] -= recent_distance_readings[distance_index].sensor0;
  distance_totals[1] -= recent_distance_readings[distance_index].sensor1;
  distance_totals[2] -= recent_distance_readings[distance_index].sensor2;
  distance_totals[3] -= recent_distance_readings[distance_index].sensor3;

  recent_ir_readings[ir_index] = ir_data;
  recent_distance_readings[distance_index] = sensor_data;

  ir_totals[0] += recent_ir_readings[ir_index].sensor0;
  ir_totals[1] += recent_ir_readings[ir_index].sensor1;
  ir_totals[2] += recent_ir_readings[ir_index].sensor2;
  ir_totals[3] += recent_ir_readings[ir_index].sensor3;

  distance_totals[0] += recent_distance_readings[distance_index].sensor0;
  distance_totals[1] += recent_distance_readings[distance_index].sensor1;
  distance_totals[2] += recent_distance_readings[distance_index].sensor2;
  distance_totals[3] += recent_distance_readings[distance_index].sensor3;

  ir_index = (ir_index + 1) % num_ir_readings;
  distance_index = (distance_index + 1) % num_distance_readings;
}

void sendData(void)
{
  sensor_data_t ir_temp;
  ir_temp.sensor0 = ir_totals[0] / num_ir_readings;
  ir_temp.sensor1 = ir_totals[1] / num_ir_readings;
  ir_temp.sensor2 = ir_totals[2] / num_ir_readings;
  ir_temp.sensor3 = ir_totals[3] / num_ir_readings;

  sensor_data_t distance_temp;
  distance_temp.sensor0 = distance_totals[0] / num_distance_readings;
  distance_temp.sensor1 = distance_totals[1] / num_distance_readings;
  distance_temp.sensor2 = distance_totals[2] / num_distance_readings;
  distance_temp.sensor3 = distance_totals[3] / num_distance_readings;

  uint8_t* dist = (uint8_t*)&distance_temp;
  uint8_t* ir = (uint8_t*)&ir_temp;

  TinyWireS.send(dist[0]);
  TinyWireS.send(dist[1]);
  TinyWireS.send(dist[2]);
  TinyWireS.send(dist[3]);
  TinyWireS.send(dist[4]);

  TinyWireS.send(ir[0]);
  TinyWireS.send(ir[1]);
  TinyWireS.send(ir[2]);
  TinyWireS.send(ir[3]);
  TinyWireS.send(ir[4]);
}

void pulse(int delays)
{
  digitalWrite(5, HIGH);
  delay(delays);
  digitalWrite(5, LOW);
  delay(delays);
}

void loop()
{
  // if(TinyWireS.available())
  // {
  //   uint8_t rec = TinyWireS.receive();
  //   if(rec > 20)
  //   {
  //     rec = 20;
  //   }
  //   num_distance_readings = rec;
  // }
  // delay(5);
  ;
}

void on_request(void)
{
  readSensors();
  digitalWrite(5, HIGH);
  sendData();
}



