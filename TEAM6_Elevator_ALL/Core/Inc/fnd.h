/*
 * fnd.h
 *
 *  Created on: Jan 29, 2026
 *      Author: appletea
 */

#ifndef INC_FND_H_
#define INC_FND_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "button.h"
#include "delay.h"

#include "tim.h"



typedef struct FND_DIG
{
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIO_PinState onState;
	GPIO_PinState offState;
}FND;


#define SER_PORT_FND GPIOB
#define SER_PIN_FND GPIO_PIN_13

#define RCLK_PORT_FND GPIOC
#define RCLK_PIN_FND GPIO_PIN_4

#define SRCLK_PORT_FND GPIOA
#define SRCLK_PIN_FND GPIO_PIN_10


void dataOut_FND(uint8_t data);
void SEVEN_FND_Init();
void Dig_AllOff(void);
void Dig_On(void);
static inline void tiny_delay(void);
void SEVEN_FND();



#endif /* INC_FND_H_ */
