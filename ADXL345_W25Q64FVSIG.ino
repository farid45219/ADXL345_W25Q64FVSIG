

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "adxl345.h"
#include "lpf.h"

#define  F_CPU 8000000UL


int acc_x = 0, acc_y = 0, acc_z = 0;

void setup() {

  LPF_Init();
  LPF_Set_Alpha(0, 10);
  LPF_Set_Alpha(1, 10);
  LPF_Set_Alpha(2, 10);

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
  acc_x = ADXL345_Get_Acc_X();
  acc_y = ADXL345_Get_Acc_Y();
  acc_z = ADXL345_Get_Acc_Z();
  acc_x = LPF_Get_Filtered_Value(0, acc_x);
  acc_y = LPF_Get_Filtered_Value(1, acc_y);
  acc_z = LPF_Get_Filtered_Value(2, acc_z);

  /*
  Serial.print(acc_x);
  Serial.print(",");
  Serial.print(acc_y);
  Serial.print(",");
  Serial.print(acc_z);
  Serial.println();
  */


  Serial.print(ADXL345_Get_Acc_X());
  Serial.print(",");
  Serial.print(acc_x);
  Serial.println();


  /*Serial.print(ADXL345_Get_Acc_X());
  Serial.print(",");
  Serial.print(ADXL345_Get_Acc_Y());
  Serial.print(",");
  Serial.print(ADXL345_Get_Acc_Z());
  Serial.println();*/
  delay(20);
}
