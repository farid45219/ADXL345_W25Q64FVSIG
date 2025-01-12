
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "adxl345.h"


typedef struct adxl345_t{
  int16_t   ACC_X;
  int16_t   ACC_Y;
  int16_t   ACC_Z;
  int16_t   ACC_X_FIL;
  int16_t   ACC_Y_FIL;
  int16_t   ACC_Z_FIL;
}adxl345_t;

adxl345_t ADXL345;

void ADXL345_Struct_Init(void){
  ADXL345.ACC_X=0;
  ADXL345.ACC_Y=0;
  ADXL345.ACC_Z=0;
  ADXL345.ACC_X_FIL=0;
  ADXL345.ACC_Y_FIL=0;
  ADXL345.ACC_Z_FIL=0;
}

void ADXL345_Init(void){
  I2C_Init();
  ADXL345_Struct_Init();
  I2C_Write_Register(0x2D,(1<<3));
  I2C_Write_Register(0x31,(1<<3));
  I2C_Write_Register(0x2C,0x0E);
}

void ADXL345_Read_Acc(void){
  uint8_t temp[6];

  I2C_Read_Register_Multiple(0x32,6,temp);
  ADXL345.ACC_X=temp[1];
  ADXL345.ACC_X<<=8;
  ADXL345.ACC_X|=temp[0];

  ADXL345.ACC_Y=temp[3];
  ADXL345.ACC_Y<<=8;
  ADXL345.ACC_Y|=temp[2];

  ADXL345.ACC_Z=temp[5];
  ADXL345.ACC_Z<<=8;
  ADXL345.ACC_Z|=temp[4];
}

int ADXL345_Get_Acc_X(void){
  return ADXL345.ACC_X;
}

int ADXL345_Get_Acc_Y(void){
  return ADXL345.ACC_Y;
}

int ADXL345_Get_Acc_Z(void){
  return ADXL345.ACC_Z;
}





