
#ifndef INC_ELEVATOR_H_
#define INC_ELEVATOR_H_


#include "button.h"   // buttonGetPressed(), current_floor
#include "stepper.h"  // rotateSteps(), rotateInit()
#include "fnd.h"      // FND_UP(), FND_DOWN(), FND_ScanOnce()
#include "7seg.h"     // SEVEN_SEG()
#include "ledbar.h"   // LED_BAR_UP_ing(), LED_BAR_DOWN_ing()
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "motor.h"
#include "i2c_lcd.h"
#include "i2c.h"

#define DIR_CW                0 // 시계 방향
#define DIR_CCW               1

void ButtonMove(uint8_t current_floor);



extern uint8_t current_floor;




#endif /* INC_ELEVATOR_H_ */
