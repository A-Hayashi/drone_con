#include "SPI.h"
#include "PS_PAD.h"
#include "MCP4922.h"

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

#define A5 19
#define A4 18
#define A3 17
#define A2 16
#define A1 15
#define A0 14

#define PS2_SEL 10
PS_PAD PAD(PS2_SEL);

MCP4922 DAC1(11, 13, A2, A3); // (MOSI,SCK,CS,LDAC) define Connections for NANO_board,
MCP4922 DAC2(11, 13, A4, A5); // (MOSI,SCK,CS,LDAC) define Connections for NANO_board,

void setup() {
  SPI.begin();

  pinMode(PS2_SEL, OUTPUT);
  digitalWrite(PS2_SEL, HIGH);
  PAD.init();

  Serial.begin(9600);
}

//void loop()
//{
//  float v = 0;
//  while (true) {
//    v += 0.1;
//    if (v > 5.0) v = 0;
//    DAC1.Set(calcDAC(v), calcDAC(5.0 - v));
//    DAC2.Set(calcDAC(5.0 - v), calcDAC(v));
//    PAD.poll();
//    delay(500);
//
//    Serial.print("DAC1:");
//    Serial.print (getVolt(analogRead(A0)));
//    Serial.print("  ");
//    Serial.print (getVolt(analogRead(A1)));
//    Serial.print("\t");
//    Serial.print("DAC2:");
//    Serial.print (getVolt(analogRead(A6)));
//    Serial.print("  ");
//    Serial.print (getVolt(analogRead(A7)));
//    Serial.print("\t");
//    Serial.print("PS_CON:");
//    Serial.print(PAD.read(PS_PAD::ANALOG_LX));
//    Serial.print("  ");
//    Serial.print(PAD.read(PS_PAD::ANALOG_LY));
//    Serial.println("");
//  }
//}

int calcDAC(float v)
{
  int val = (v * 4096) / 5.0;
  val = MAX(val, 0);
  val = MIN(val, 4095);
  return val;
}

float getVolt(int ad)
{
  float val = (ad * 5.0) / 1024;
  return val;
}


float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float convJoy2Volt(int data)
{
  float val;

  data = MAX(data, -127);
  data = MIN(data, 127);
  val = fmap(data, -127, 127, 3.319, 0.001);

  return val;
}


float convJoy2Volt2(int data)
{
  float val;

  data = MAX(data, -127);
  data = MIN(data, 127);
  val = fmap(data, -127, 127, 0.001, 3.319);

  return val;
}

float convJoy2VoltThrottle(int data)
{
  float val;

  data = MAX(data, 0);
  data = MIN(data, 127);
  val = fmap(data, 0, 127, 0.001, 3.319);
  
  return val;
}


void loop() {
  PAD.poll();

  float lx = convJoy2Volt(PAD.read(PS_PAD::ANALOG_LX));
  float ly = convJoy2VoltThrottle(PAD.read(PS_PAD::ANALOG_LY));
  float rx = convJoy2Volt2(PAD.read(PS_PAD::ANALOG_RX));
  float ry = convJoy2Volt(PAD.read(PS_PAD::ANALOG_RY));

  DAC1.Set(calcDAC(lx), calcDAC(ly));
  DAC2.Set(calcDAC(rx), calcDAC(ry));

  Serial.print("LX:");
  Serial.print(lx);
  Serial.print("\t");
  Serial.print("LY:");
  Serial.print(ly);
  Serial.print("\t");
  Serial.print("RX:");
  Serial.print(rx);
  Serial.print("\t");
  Serial.print("RY:");
  Serial.print(ry);
  Serial.print("\t\t");

  Serial.print("DAC1:");
  Serial.print (getVolt(analogRead(A0)));
  Serial.print("\t");
  Serial.print (getVolt(analogRead(A1)));
  Serial.print("\t");
  Serial.print("DAC2:");
  Serial.print (getVolt(analogRead(A6)));
  Serial.print("\t");
  Serial.print (getVolt(analogRead(A7)));

  Serial.println();
}
