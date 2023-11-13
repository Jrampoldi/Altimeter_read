#include "i2c_handler.h"
#include "uart_driver.h"

/*Pin info:
 * PB6: SCL
 * PB7: SDA
 * */

#define SYSFREQ						16000000
#define I2C_BUS_SPEED				100000
#define MAX_RISE_TIME				17

#define GPIOBEN						(1U<<1)
#define I2C1EN						(1U<<21)

#define CR1_SWRST					(1U<<15)
#define CR1_PE						(1U<<0)
#define CR1_ACK						(1U<<10)
#define CR1_START					(1U<<8)
#define CR1_STOP					(1U<<9)

#define SR1_SB						(1U<<0)
#define SR1_TxE						(1U<<7)
#define SR1_BTF						(1U<<2)
#define SR1_ADDR					(1U<<1)

void I2C1_generate_start(void);
void I2C1_write(uint8_t data);
void I2C1_send_adr(uint8_t adr);

void i2c1_init(void){

	/*Config IO port to I2C*/
	RCC->AHB1ENR |= GPIOBEN;

	GPIOB->MODER |= (1U<<15 | 1U<<13);
	GPIOB->OTYPER |= (1U<<7 | 1U<<6);// SET OPEN DRAIN
	GPIOB->PUPDR |= (1U<<14 | 1U<<12);// ENABLE PULL UP
	GPIOB->AFR[0] |= (1U<<30 | 1U<<26); //SET AF04
    GPIOB->OSPEEDR |= (3U << 14 | 3U << 12);

	/*Config I2C*/
	RCC->APB1ENR |= I2C1EN;

	I2C1->CR1 |= CR1_SWRST; // reset I2C1
	I2C1->CR1 &= ~CR1_SWRST;

	I2C1->CR2 = (1U<<4); // SET 16MHz

	I2C1->CCR = (SYSFREQ / (2 * I2C_BUS_SPEED)); // config ccr to 100kHz

	I2C1->TRISE = MAX_RISE_TIME;

	I2C1->CR1 |= CR1_PE; //periph enable
}

void I2C1_generate_start(void){
	// Enable ACK
	I2C1->CR1 |= CR1_ACK;

	// send start condition
	I2C1->CR1 |= CR1_START;

	// wait for start to send
	while(!(I2C1->SR1 & SR1_SB));

}

void I2C1_write(uint8_t data){
	// wait for data reg empty
	while(!(I2C1->SR1 & SR1_TxE));

	// load data into register
	I2C1->DR = data;

	// wait for BTF
	while(!(I2C1->SR1 & SR1_BTF));
}

void I2C1_send_adr(uint8_t adr){

	(void)I2C1->SR1;

	// send address
	I2C1->DR = adr;

	printf("address sent\n");
	// wait for acknowledge of address
	while(!(I2C1->SR1 & SR1_ADDR));
	printf("address received\n");

	// clear addr (read to clear)
	(void)I2C1->SR1;
	(void)I2C1->SR2;
}

void I2C1_write_reg(uint8_t sl_adr, uint8_t reg_adr, uint8_t data){
	// generate start
	I2C1_generate_start();

	// send address
	I2C1_send_adr(sl_adr);
	I2C1_send_adr(reg_adr);

	// write data
	I2C1_write(data);

	// generate stop
	I2C1->CR1 |= CR1_STOP;
}
