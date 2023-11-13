/*
 * i2c_handler.h
 *
 *  Created on: Nov 12, 2023
 *      Author: jourdanrampoldi
 */

#ifndef I2C_HANDLER_H_
#define I2C_HANDLER_H_

#include "stm32f411xe.h"
#include "stdint.h"

void i2c1_init(void);
void I2C1_write_reg(uint8_t sl_adr, uint8_t reg_adr, uint8_t data);

#endif /* I2C_HANDLER_H_ */
