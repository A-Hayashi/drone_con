#include "PS_PAD.h"
#include "SPI.h"

#define A5 19
#define A4 18
#define A3 17
#define A2 16
#define A1 15
#define A0 14

#define PS2_SEL 10
PS_PAD PAD(PS2_SEL);

void setup() {
  pinMode(PS2_SEL, OUTPUT);
  digitalWrite(PS2_SEL, HIGH);
  PAD.init();
  Serial.begin(9600);
}


void loop() {
  PAD.poll();
  
  int lx = PAD.read(PS_PAD::ANALOG_LX);
  int ly = PAD.read(PS_PAD::ANALOG_LY);
  int rx = PAD.read(PS_PAD::ANALOG_RX);
  int ry = PAD.read(PS_PAD::ANALOG_RY);


  Serial.print(lx);
  Serial.print("\t");
  Serial.print(ly);
  Serial.print("\t");
  Serial.print(rx);
  Serial.print("\t");
  Serial.print(ry);
  Serial.println();
}
