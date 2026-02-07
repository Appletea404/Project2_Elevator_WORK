
#include <delay.h>
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "button.h"
#include "tim.h"

#ifndef INC_LEDBAR_H_
#define INC_LEDBAR_H_



#define SER_PORT_SEG GPIOA
#define SER_PIN_SEG GPIO_PIN_12

#define RCLK_PORT_SEG GPIOA
#define RCLK_PIN_SEG GPIO_PIN_11

#define SRCLK_PORT_SEG GPIOB
#define SRCLK_PIN_SEG GPIO_PIN_12

void SEVEN_SEG_Init();
void SEVEN_SEG();
void dataOut_SEG(uint8_t data);

#endif /* INC_LEDBAR_H_ */
