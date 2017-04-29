#include "ADS1115.h"

void I2c_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	//Turn on clocking for modules
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//I2C use 2 pins...
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	//Setting up I2C
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x15; // Наш адрес
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);

	//Turn on I2C
	I2C_Cmd(I2C1, ENABLE);
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}



void InitADS1115()
{
	I2c_Init();
	uint8_t conf[2];
	//Request
	I2C_StartTransmission(I2C1,I2C_Direction_Transmitter,0x90);
	I2C_Write_Data(I2C1,0x01);
	I2C_GenerateSTOP(I2C1,ENABLE);
	//Response
	I2C_StartTransmission(I2C1,I2C_Direction_Receiver,0x90);
	conf[0] = I2C_Read_ack(I2C1);
	conf[1] = I2C_Read_nack(I2C1);
	I2C_GenerateSTOP(I2C1,ENABLE);
}

//ADS1115 have 4 channel. We can select on of them in one moment.
void selectInput(uint8_t number)
{
	 uint8_t conf[2];
	 conf[1] = 227;

	 //create config for selecting channel
	 if(number == 0)
	 	 {
	 		 conf[0] = 195;
	 	 }
	 if(number == 1)
	 	 {
	 		 conf[0] = 211;
	 	 }
	 if(number == 2)
	 	 {
	 		 conf[0] = 227;
	 	 }
	 if(number == 3)
	 	 {
	 		 conf[0] = 243;
	 	 }

	//send config
	 I2C_StartTransmission(I2C1,I2C_Direction_Transmitter,0x90);
	 I2C_Write_Data(I2C1,0x01);
	 I2C_Write_Data(I2C1,conf[0]);
	 I2C_Write_Data(I2C1,conf[1]);
	 //I2C_GenerateSTOP(I2C1,ENABLE);
}


uint_fast16_t getData(uint8_t number)
{

	uint8_t i2c_data[2];
	//Request
	I2C_StartTransmission(I2C1,I2C_Direction_Transmitter,0x90);
	I2C_Write_Data(I2C1,0x00);
	I2C_GenerateSTOP(I2C1,ENABLE);
	//Response
	I2C_StartTransmission(I2C1,I2C_Direction_Receiver,0x90);
	i2c_data[0] = I2C_Read_ack(I2C1);
	i2c_data[1] = I2C_Read_nack(I2C1);
	uint_fast16_t adc = i2c_data[0] <<  8 | i2c_data[1];
	//I2C_GenerateSTOP(I2C1,ENABLE);
	return adc;
}

//From datasheet
/*
000 : AINP = AIN0 and AINN = AIN1 (default) 100 : AINP = AIN0 and AINN = GND
001 : AINP = AIN0 and AINN = AIN3 101 : AINP = AIN1 and AINN = GND
010 : AINP = AIN1 and AINN = AIN3 110 : AINP = AIN2 and AINN = GND
011 : AINP = AIN2 and AINN = AIN3 111 : AINP = AIN3 and AINN = GND
*/


