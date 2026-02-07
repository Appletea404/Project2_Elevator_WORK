
#include <delay.h>
#include "stm32f4xx_hal.h"
#include "stdbool.h"


#ifndef INC_LEDBAR_H_
#define INC_LEDBAR_H_



#define SER_PORT_LED GPIOC
#define SER_PIN_LED GPIO_PIN_8

#define RCLK_PORT_LED GPIOC
#define RCLK_PIN_LED GPIO_PIN_6

#define SRCLK_PORT_LED GPIOC
#define SRCLK_PIN_LED GPIO_PIN_5


void dataOut_LED(uint8_t data);
void LED_BAR_UP_ing();
void LED_BAR_DOWN_ing();
#endif /* INC_LEDBAR_H_ */
