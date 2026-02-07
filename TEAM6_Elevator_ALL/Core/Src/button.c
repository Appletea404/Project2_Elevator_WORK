#include "button.h"
#include "stepper.h"

static uint16_t step_count = 0;
static uint32_t prevMoveTime = 0;

BUTTON_CONTROL button[4]=
    {
        {GPIOC, GPIO_PIN_9, 0},
        {GPIOB, GPIO_PIN_8, 0},
        {GPIOB, GPIO_PIN_9, 0},
        {GPIOA, GPIO_PIN_5, 0}
    };

uint8_t after_floor;
uint8_t button_flag = 0; // 눌린건지 확인
uint8_t outside = 0; // 외부에서 눌린건지
uint8_t upordown = 0; // 위 아래 판단
uint8_t openorclose = 0;

bool buttonGetPressed(uint8_t num)
{
  static uint32_t prevTime = 0;
  bool ret = false;

  if(HAL_GPIO_ReadPin(button[num].port,button[num].number) == button[num].onState)
  {
    uint32_t currTime = HAL_GetTick();
    if(currTime - prevTime > 100)
    {
      if(HAL_GPIO_ReadPin(button[num].port,button[num].number) == button[num].onState)
      {
            ret = true;
      }
      prevTime = currTime;
    }
  }
  return ret;
}

void ButtonMove(uint8_t current_floor)
{
  if(button_flag == 0)
  {
    if(buttonGetPressed(0)) // 내부 1층 버튼
        {
          after_floor = 1;
          button_flag = 1;
        }
    else if(buttonGetPressed(1)) // 내부 2층 버튼
        {
          after_floor = 2;
          button_flag = 1;
        }
    else if(buttonGetPressed(2)) // 내부 3층 버튼
        {
          after_floor = 3;
          button_flag = 1;
        }
      if(buttonGetPressed(3)) // 비상버튼
        {
          button_flag = 2;
        }
      // 외부 버튼은 일단은 내부 버튼과 동일한 기능
//      if(buttonGetPressed(4)) // 외부 1층 위로 버튼
//        {
//          after_floor = 1;
//          button_flag = 1;
//        }
//      if(buttonGetPressed(5)) // 외부 2층 위로 버튼
//        {
//          after_floor = 2;
//          button_flag = 1;
//        }
//      if(buttonGetPressed(6)) // 외부 2층 아래로 버튼
//        {
//          after_floor = 2;
//          button_flag = 1;
//        }
//      if(buttonGetPressed(7)) // 외부 3층 아래로 버튼
//        {
//          after_floor = 3;
//          button_flag = 1;
//        }
//      if(buttonGetPressed(0))
//      {
//        button_flag = 4;
//        openorclose = 1;
//      }
//      if(buttonGetPressed(1))
//      {
//        button_flag = 4;
//        openorclose = 2;
//      }
  }

  else if(button_flag == 1) // 눌림
  {
    if(buttonGetPressed(3))
    {
      button_flag = 3;
    }

    // 같은 층
    if(current_floor == after_floor) // 목표 층에 도착하면 정지하며 모든 변수 초기화
    {
      button_flag = 0;
      upordown = 0;
      rotateInit();
      rotateSteps(0, 0);
    }

    // 올라가야 함
    else if(current_floor < after_floor)
    {
      upordown = 1; // 올라가도록 설정
    }

    // 내려가야 함
    else if(current_floor > after_floor)
    {
      upordown = 2; // 내려가도록 설정
    }

    if(upordown == 1) // 올라가도록 설정되어 있다면
    {
      rotateSteps(1000, 0); // 올라가는 방향으로 아무튼 오래 돌리기
    }
    else if(upordown == 2) // 내려가도록 설정되어 있다면
    {
      rotateSteps(1000, 1); // 내려가는 방향으로 아무튼 오래 돌리기
    }
  }

  else if(button_flag == 2) // 다른 버튼 안누르고 비상만 누름
  {
    if(step_count >= 4) // 4번 동작하고 정지
      {
        button_flag = 0;
        step_count = 0;
        return;
      }
      uint32_t now = HAL_GetTick();
      if (now - prevMoveTime >= 1000)   // 1000ms
      {
        prevMoveTime = now;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // 정지 알림 용. 이후 다른 것으로 교체
        step_count++;
      }
  }

  else if(button_flag == 3) // 이동 중 비상 버튼 누름
  {
    rotateSteps(0, 0); // 일단 정지
    if(step_count >= 4) // 4번 동작하고 정지
      {
        button_flag = 1;
        step_count = 0;
        if(upordown == 1)
        {
          after_floor = current_floor + 1;
        }
        else if(upordown == 2)
        {
          after_floor = current_floor - 1;
        }
        return;
      }
      uint32_t now = HAL_GetTick();
      if (now - prevMoveTime >= 1000)   // 1000ms
      {
        prevMoveTime = now;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // 정지 알림 용. 이후 다른 것으로 교체
        step_count++;
      }
  }
  // 여닫힘 버튼 누름
  else if(button_flag == 4)
  {
    if(openorclose == 1) // 열림 버튼
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1); // 열림 대체. 추후에 변경
      if(buttonGetPressed(1)) // 열린 와중에 닫힘 버튼 누름
        {
          button_flag = 0;
          step_count = 0;
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0); // 닫힘 대체. 추후에 변경
          return;
        }
      if(step_count >= 4) // 4번 동작하고 정지
        {
          button_flag = 0;
          step_count = 0;
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0); // 닫힘 대체. 추후에 변경
          return;
        }
        uint32_t now = HAL_GetTick();
        if (now - prevMoveTime >= 1000)   // 1000ms
        {
          prevMoveTime = now;
          step_count++;
        }
    }
    else if(openorclose == 2)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0); // 닫힘 대체. 추후에 변경
      button_flag = 0;
    }
  }
}

