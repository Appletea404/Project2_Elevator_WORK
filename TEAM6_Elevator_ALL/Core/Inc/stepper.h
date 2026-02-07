#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "stm32f4xx_hal.h"

#define STEPS_PER_REVOLATION  4096
#define DIR_CW                0 // 시계 방향
#define DIR_CCW               1 // 반시계 방향

#define IN1_PORT              GPIOA
#define IN1_PIN               GPIO_PIN_0

#define IN2_PORT              GPIOA
#define IN2_PIN               GPIO_PIN_1

#define IN3_PORT              GPIOA
#define IN3_PIN               GPIO_PIN_4

#define IN4_PORT              GPIOB
#define IN4_PIN               GPIO_PIN_0


void stepMotor(uint8_t step);
void rotateSteps(uint16_t degrees, uint8_t direction);
void rotateDegrees(uint16_t degrees, uint8_t direction);
void rotateInit();

#endif /* INC_STEPPER_H_ */
