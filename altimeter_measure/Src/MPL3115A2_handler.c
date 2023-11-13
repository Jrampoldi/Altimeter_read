#include "MPL3115A2_handler.h"



#define SLAVE_ADDRESS_WRITE					0xC0
#define SLAVE_ADDRESS_READ					0xC1

#define SLAVE_CR1							0x26
#define SLAVE_DATA_CFG						0X13

#define P_MSB								0X01
#define P_CSB								0X02
#define P_LSB								0x03

#define T_MSB								0x04
#define T_LSB								0x05

#define DATA_CHECK							0X08


void alt_component_init(void){
	I2C1_write_reg(SLAVE_ADDRESS_WRITE, SLAVE_CR1, 0XB8);
}
