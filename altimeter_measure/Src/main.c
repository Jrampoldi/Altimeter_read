#include "stm32f411xe.h"
#include "i2c_handler.h"
#include "MPL3115A2_handler.h"
#include "uart_driver.h"

int main(void){
	uart1_tx_init();
	i2c1_init();
	alt_component_init();
	while(1){

	}
}
