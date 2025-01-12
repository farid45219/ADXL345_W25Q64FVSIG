

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "adxl345.h"
#define  F_CPU 8000000UL

void setup() {
  Serial.begin(38400);
  cli();
  CLKPR = (1<<CLKPCE);
  CLKPR = (1<<CLKPS0);
  sei();
  ADXL345_Init();
  Serial.println("Init Complete");
}

void loop() {
  ADXL345_Read_Acc();
  Serial.print(ADXL345_Get_Acc_X());
  Serial.print(",");
  Serial.print(ADXL345_Get_Acc_Y());
  Serial.print(",");
  Serial.print(ADXL345_Get_Acc_Z());
  Serial.println();
  delay(20);
}
