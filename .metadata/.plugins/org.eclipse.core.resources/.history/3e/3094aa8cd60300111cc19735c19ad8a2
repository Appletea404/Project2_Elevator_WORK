#include "stepper.h"

 static uint8_t step = 0;
 static uint16_t step_count = 0;

static uint32_t prevMoveTime = 0;

static const uint8_t HALF_STEP_SEQ[8][4] =
{
    {1, 0, 0, 0}, // step 1
    {1, 1, 0, 0}, // step 2
    {0, 1, 0, 0}, // step 3
    {0, 1, 1, 0}, // step 4
    {0, 0, 1, 0}, // step 5
    {0, 0, 1, 1}, // step 6
    {0, 0, 0, 1}, // step 7
    {1, 0, 0, 1}  // step 8
};
// 1-2 상 여자

void stepMotor(uint8_t step)
{
  HAL_GPIO_WritePin(IN_PORT, IN1_PIN, HALF_STEP_SEQ[step][0]);
  HAL_GPIO_WritePin(IN_PORT, IN2_PIN, HALF_STEP_SEQ[step][1]);
  HAL_GPIO_WritePin(IN_PORT, IN3_PIN, HALF_STEP_SEQ[step][2]);
  HAL_GPIO_WritePin(IN_PORT, IN4_PIN, HALF_STEP_SEQ[step][3]);
}



void rotateSteps(uint16_t degrees, uint8_t direction)
{
  uint16_t steps = 0;
  steps = (uint16_t)((uint32_t)(degrees * STEPS_PER_REVOLATION) / 360);
  if(step_count >= steps)
  {
    return;
  }
  uint32_t now = HAL_GetTick();
  if (now - prevMoveTime >= 2)   // 2ms
  {
    prevMoveTime = now;
    // 회전 방향에 따른 스탭 설정
    if(direction == DIR_CCW)
    {
      step = (step+1) % 8; // 반시계 방향
    }
    else
    {
      step = (step+7) % 8; // 시계 방향
    }
    stepMotor(step);
    step_count++;
  }
}

void rotateInit()
{
  step_count = 0;
}

void rotateDegrees(uint16_t degrees, uint8_t direction)
{
  // 각도에 해당하는 스텝수를 계산
  uint16_t steps = (uint16_t)((uint32_t)(degrees * STEPS_PER_REVOLATION) / 360);
  // 360도를 돌리는데 필요한 스텝 = 4096, 1도를 돌리는데 필요한 스텝 = 4096/360
  rotateSteps(steps, direction); // 지정된 방향으로 회전
}
