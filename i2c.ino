
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"


typedef struct i2c_t{
  uint8_t   WriteAddr;
  uint8_t   ReadAddr;
  uint8_t   DiscardSmpl;
  uint8_t   Error;
  uint16_t  ErrorAccu;
}i2c_t;

i2c_t I2C;

void I2C_Struct_Init(void){
  I2C.WriteAddr=0x00;
  I2C.ReadAddr=0x00;
  I2C.DiscardSmpl=0x01;
  I2C.Error=0x00;
  I2C.ErrorAccu=0x00;
}

void I2C_Error_Clear(void){
  I2C.Error=0x00;
}

void I2C_Error_Accu_Clear(void){
  I2C.ErrorAccu=0x00;
}

void I2C_Error_Clear_All(void){
  I2C_Error_Clear();
  I2C_Error_Accu_Clear();
}

uint8_t I2C_Get_Error(void){
  return I2C.Error;
}

uint16_t I2C_Get_ErrorAccu(void){
  return I2C.ErrorAccu;
}

void I2C_Half_Bit_Delay(void){
  _delay_us(20);
}

void I2C_Bit_Delay(void){
  _delay_us(40);
}

void I2C_Guard_Delay(void){
  _delay_us(2);
}

void I2C_SCL_Set_DD(uint8_t state){
  if(state==1){
    I2C_SCL_DDR |=  (1<<I2C_SCL_bp);
  }else{
    I2C_SCL_DDR &=~ (1<<I2C_SCL_bp);
  }
}

void I2C_SCL_Set_Output_LL(uint8_t state){
  if(state==1){
    I2C_SCL_PORT |=  (1<<I2C_SCL_bp);
  }else{
    I2C_SCL_PORT &=~ (1<<I2C_SCL_bp);
  }
}

void I2C_SCL_Set_Output(uint8_t state){
  if(state==1){
    I2C_SCL_Set_Output_LL(1);
	I2C_SCL_Set_DD(0);
  }else{
    I2C_SCL_Set_Output_LL(0);
	I2C_SCL_Set_DD(1);
  }
}

uint8_t I2C_SCL_Get_State(void){
  if(I2C_SCL_PIN & (1<<I2C_SCL_bp)){
    return 1;
  }else{
    return 0;
  }
}

void I2C_SDA_Set_DD(uint8_t state){
  if(state==1){
    I2C_SDA_DDR |=  (1<<I2C_SDA_bp);
  }else{
    I2C_SDA_DDR &=~ (1<<I2C_SDA_bp);
  }
}

void I2C_SDA_Set_Output_LL(uint8_t state){
  if(state==1){
    I2C_SDA_PORT |=  (1<<I2C_SDA_bp);
  }else{
    I2C_SDA_PORT &=~ (1<<I2C_SDA_bp);
  }
}

void I2C_SDA_Set_Output(uint8_t state){
  if(state==1){
    I2C_SDA_Set_Output_LL(1);
	I2C_SDA_Set_DD(0);
  }else{
    I2C_SDA_Set_Output_LL(0);
	I2C_SDA_Set_DD(1);
  }
}

uint8_t I2C_SDA_Get_State(void){
  if(I2C_SDA_PIN & (1<<I2C_SDA_bp)){
    return 1;
  }else{
    return 0;
  }
}

void I2C_SCL_Reset_State(void){
  I2C_SCL_Set_DD(0);
  I2C_SCL_Set_Output(0);
}

void I2C_SDA_Reset_State(void){
  I2C_SDA_Set_DD(0);
  I2C_SDA_Set_Output(0);
}

void I2C_GPIO_Init(void){
  I2C_SCL_Set_DD(1);
  I2C_SDA_Set_DD(0);
  I2C_SCL_Set_Output(1);
  I2C_SDA_Set_Output(1);
}

void I2C_Start(void){
  I2C_SCL_Set_Output(1);
  I2C_SDA_Set_Output(1);
  I2C_Half_Bit_Delay();
  
  I2C_SDA_Set_Output(0);
  I2C_Half_Bit_Delay();
  
  I2C_SCL_Set_Output(0);
  I2C_Half_Bit_Delay();
}

void I2C_Stop(void){
  I2C_SCL_Set_Output(0);
  I2C_SDA_Set_Output(0);
  I2C_Half_Bit_Delay();
  
  I2C_SCL_Set_Output(1);
  I2C_Half_Bit_Delay();
  
  I2C_SDA_Set_Output(1);
  I2C_Half_Bit_Delay();
}

void I2C_Data_Send(uint8_t data){
  for(uint8_t i=0;i<8;i++){
    if( data & 0x80){
	  I2C_SDA_Set_Output(1);
	}else{
	  I2C_SDA_Set_Output(0);
	}
	I2C_Half_Bit_Delay();
	I2C_SCL_Set_Output(1);
	I2C_Half_Bit_Delay();
	I2C_SCL_Set_Output(0);
	I2C_Guard_Delay();
	data<<=1;
  }
}

uint8_t I2C_Data_Receive(void){
  uint8_t data=0;
  I2C_SDA_Set_Output(1);
  for(uint8_t i=0;i<8;i++){
    data<<=1;
	I2C_Half_Bit_Delay();
	I2C_SCL_Set_Output(1);
	if(I2C_SDA_Get_State()){
	  data|=0x01;
	}
	I2C_Half_Bit_Delay();
	I2C_SCL_Set_Output(0);
  }
  I2C_Half_Bit_Delay();
  return data;
}

void I2C_Set_Ack(void){
  I2C_SDA_Set_Output(0);
  I2C_Half_Bit_Delay();
  I2C_SCL_Set_Output(1);
  I2C_Half_Bit_Delay();
  I2C_SCL_Set_Output(0);
}

void I2C_Set_Nack(void){
  I2C_SDA_Set_Output(1);
  I2C_Half_Bit_Delay();
  I2C_SCL_Set_Output(1);
  I2C_Half_Bit_Delay();
  I2C_SCL_Set_Output(0);
}

uint8_t I2C_Get_Ack(void){
  uint8_t ack=0;
  I2C_SDA_Set_Output(1);
  I2C_Half_Bit_Delay();
  I2C_SCL_Set_Output(1);
  ack=I2C_SDA_Get_State();
  I2C_Half_Bit_Delay();
  I2C_SCL_Set_Output(0);
  I2C_Guard_Delay();
  if(ack==0){
    return 1;
  }else{
    return 0;
  }
}

uint8_t I2C_Send_Device_Address(uint8_t address){
  uint8_t ack=0;
  I2C_Start();
  I2C_Data_Send(address);
  ack = I2C_Get_Ack();
  I2C_Stop();
  return ack;
}

uint8_t I2C_Get_Device_Address(void){
  uint8_t sts=0;
  for(uint8_t i=1;i<255;i++){
	sts = I2C_Send_Device_Address(i);
	if(sts==1){
	  sts=i;
	  break;
	}
  }
  return sts;
}

uint8_t I2C_Read_Register(uint8_t reg){
  uint8_t sts=0,data=0;
  I2C_Start();
  I2C_Data_Send(I2C.WriteAddr);
  sts |= I2C_Get_Ack()<<0;
  I2C_Data_Send(reg);
  sts |= I2C_Get_Ack()<<1;
  I2C_Stop();
  I2C_Start();
  I2C_Data_Send(I2C.ReadAddr);
  sts |= I2C_Get_Ack()<<2;
  data = I2C_Data_Receive();
  sts |= I2C_Get_Ack()<<3;
  I2C_Stop();
  if(sts == 0x07){
    sts = data;
	I2C.Error=0x00;
  }else{
    data=0;
	I2C.Error=0x01;
	I2C.ErrorAccu++;
    sts = 0xFF;
  }
  return sts;
}


void I2C_Read_Register_Multiple(uint8_t reg, uint8_t len, uint8_t *data){
  uint8_t sts=0,temp_data=0;
  uint8_t lut[8]={0,3,7,15,31,63,127,255};
  I2C_Start();
  I2C_Data_Send(I2C.WriteAddr);
  sts |= I2C_Get_Ack()<<0;
  I2C_Data_Send(reg);
  sts |= I2C_Get_Ack()<<1;
  //I2C_Stop();
  I2C_Start();
  I2C_Data_Send(I2C.ReadAddr);
  sts |= I2C_Get_Ack()<<2;
  for(uint8_t i=0;i<len; i++){
    temp_data = I2C_Data_Receive();
	  if(i==(len-1)){
	    I2C_Set_Nack();
	  }else{
	    I2C_Set_Ack();
	  }
    //sts |= (I2C_Get_Ack()<<(3+i));
	  data[i] = temp_data;
  }
  I2C_Stop();
  /*if(sts == lut[len]){
	I2C.Error=0x00;
  }else{
	I2C.Error=0x01;
	I2C.ErrorAccu++;
    sts = 0xFF;
	for(uint8_t i=0;i<len; i++){
	  data[i] = 0;
	}
  }*/
}

void I2C_Write_Register(uint8_t reg, uint8_t val){
  uint8_t sts=0,data=0;
  I2C_Start();
  I2C_Data_Send(I2C.WriteAddr);
  sts |= I2C_Get_Ack()<<0;
  I2C_Data_Send(reg);
  sts |= I2C_Get_Ack()<<1;
  I2C_Data_Send(val);
  sts |= I2C_Get_Ack()<<2;
  I2C_Stop();
  if(sts == 0x07){
    sts = data;
	I2C.Error=0x00;
  }else{
	I2C.Error=0x02;
	I2C.ErrorAccu++;
    sts = 0xFF;
  }
}
 
void I2C_Init(void){
  I2C_Struct_Init();
  I2C_GPIO_Init();
  I2C.WriteAddr = I2C_Get_Device_Address();
  I2C.ReadAddr = I2C.WriteAddr+1;
}


uint8_t I2C_Get_Write_Addr(void){
  return I2C.WriteAddr;
}

uint8_t I2C_Get_Read_Addr(void){
  return I2C.ReadAddr;
}




