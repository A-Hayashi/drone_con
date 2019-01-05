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

void loop()
{
  float v = 0;
  while (true) {
    v += 0.1;
    if (v > 5.0) v = 0;
    DAC1.Set(calcDAC(v), calcDAC(5.0 - v));
    DAC2.Set(calcDAC(5.0 - v), calcDAC(v));
    PAD.poll();
    delay(500);

    Serial.print("DAC1:");
    Serial.print (getVoltage(analogRead(A0)));
    Serial.print("  ");
    Serial.print (getVoltage(analogRead(A1)));
    Serial.print("\t");
    Serial.print("DAC2:");
    Serial.print (getVoltage(analogRead(A6)));
    Serial.print("  ");
    Serial.print (getVoltage(analogRead(A7)));
    Serial.print("\t");
    Serial.print("PS_CON:");
    Serial.print(PAD.read(PS_PAD::ANALOG_LX));
    Serial.print("  ");
    Serial.print(PAD.read(PS_PAD::ANALOG_LY));
    Serial.println("");
  }
}


int calcDAC(float v)
{
  int val = (v * 4096) / 5.0;
  val = MAX(val, 0);
  val = MIN(val, 4095);
  return val;
}

float getVoltage(int ad)
{
  float val = (ad * 5.0) / 1024;
  return val;
}


//void loop() {
//  PAD.poll();
//
//  int lx = PAD.read(PS_PAD::ANALOG_LX);
//  int ly = PAD.read(PS_PAD::ANALOG_LY);
//  int rx = PAD.read(PS_PAD::ANALOG_RX);
//  int ry = PAD.read(PS_PAD::ANALOG_RY);
//
//
//  Serial.print(lx);
//  Serial.print("\t");
//  Serial.print(ly);
//  Serial.print("\t");
//  Serial.print(rx);
//  Serial.print("\t");
//  Serial.print(ry);
//  Serial.println();
//}
