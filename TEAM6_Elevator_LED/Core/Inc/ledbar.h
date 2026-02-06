
#include "stm32f4xx_hal.h"
#include "delay.h"
#include "stdbool.h"


#ifndef INC_LEDBAR_H_
#define INC_LEDBAR_H_

#define SER_PIN GPIO_PIN_8
#define RCLK_PIN GPIO_PIN_6
#define SRCLK_PIN GPIO_PIN_5
#define GPIO_PORT GPIOC

void LED_BAR_UP_ing();
void LED_BAR_DOWN_ing();
#endif /* INC_LEDBAR_H_ */
