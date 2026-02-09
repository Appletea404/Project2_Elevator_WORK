#include "fnd.h"


void dataOut_FND(uint8_t data)
{
    /*
     * i = 7 → 0
     * MSB부터 LSB까지 순차적으로 전송
     */
	for(int i = 7; i >= 0; i--)
	{
        /*
         * 현재 전송할 비트를 SER 핀에 출력
         * data의 i번째 비트가 1이면 HIGH, 0이면 LOW
         *
         * 이 시점에서는 아직 74HC595 내부로 들어가지 않고
         * 단순히 입력 핀에 값만 세팅된 상태
         */
		if(data & (1 << i))
		{

			HAL_GPIO_WritePin(SER_PORT_FND, SER_PIN_FND, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(SER_PORT_FND, SER_PIN_FND, GPIO_PIN_RESET);
		}

        /*
         * SRCLK(Shift Clock) 펄스 생성
         *
         * - SRCLK의 상승엣지에서 SER 값이
         *   내부 쉬프트 레지스터로 이동
         * - 이후 비트들은 한 칸씩 밀려남
         *
         * delay_us()는 setup/hold time 확보용 여유
         */
		HAL_GPIO_WritePin(SRCLK_PORT_FND, SRCLK_PIN_FND, GPIO_PIN_SET);
		delay_us(5);

		HAL_GPIO_WritePin(SRCLK_PORT_FND, SRCLK_PIN_FND, GPIO_PIN_RESET);
		delay_us(5);
	}

    /*
     * RCLK(Latch Clock) 펄스 생성
     *
     * - 지금까지 쉬프트 레지스터에 쌓인 8비트 데이터를
     *   출력 래치로 복사
     * - QA~QH 핀이 이 순간에만 갱신됨
     *
     * 장점:
     * - 데이터 전송 중에는 출력이 바뀌지 않음
     * - LED/FND 사용 시 중간 상태가 보이지 않음
     */
	HAL_GPIO_WritePin(RCLK_PORT_FND, RCLK_PIN_FND, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(RCLK_PORT_FND, RCLK_PIN_FND, GPIO_PIN_RESET);
}



FND FND_SET[4] = {
		{GPIOB, GPIO_PIN_2, GPIO_PIN_SET, GPIO_PIN_RESET},		//DIG1
		{GPIOB, GPIO_PIN_1, GPIO_PIN_SET, GPIO_PIN_RESET},		//DIG2
		{GPIOB, GPIO_PIN_15, GPIO_PIN_SET, GPIO_PIN_RESET},		//DIG3
		{GPIOB, GPIO_PIN_14, GPIO_PIN_SET, GPIO_PIN_RESET},		//DIG4
};

static const uint8_t FND_IDX[4] = {0,1,2,3};

#define DIG_ON   GPIO_PIN_RESET
#define DIG_OFF  GPIO_PIN_SET


#define SEG_1 0x06
#define SEG_2 0x5B
#define SEG_3 0x4F

// 문자
#define SEG_U 0x3E  // U : b c d e f
#define SEG_P 0x73  // P : a b e f g
#define SEG_d 0x5E  // d : b c d e g
#define SEG_o 0x5C  // o : c d e g  (소문자 o는 0과 헷갈려서 이렇게 많이 씀)
#define SEG_F 0x71  // F : a e f g



void SEVEN_FND_Init()
{
	dataOut_FND(0x00);
}

void Dig_AllOff(void)
{
	for(uint8_t i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(FND_SET[FND_IDX[i]].port, FND_SET[FND_IDX[i]].pin, DIG_OFF);
	}


}

void Dig_SINGLE_ON(uint8_t i) // 0~3
{
		Dig_AllOff();
		HAL_GPIO_WritePin(FND_SET[FND_IDX[i]].port, FND_SET[FND_IDX[i]].pin, DIG_ON);


}

static uint8_t display[4] = {0,0,0,0}; // DIG1~DIG4에 나갈 세그 패턴
static uint8_t scan = 0;

void FND_ScanOnce(void)
{
    Dig_AllOff();
    tiny_delay();                 // 고스트 줄이기(선택)

    dataOut_FND(display[scan]);   // 세그 패턴 먼저 출력
    Dig_SINGLE_ON(scan);             // 해당 자리만 ON

    scan++;
    if (scan >= 4) scan = 0;
}

/* tiny delay to reduce ghosting */
static inline void tiny_delay(void)
{
    for (volatile int i = 0; i < 80; i++) { __NOP(); }
}

static inline uint8_t floor_to_seg(uint8_t floor)
{
    switch(floor){
        case 1: return SEG_1;
        case 2: return SEG_2;
        case 3: return SEG_3;
        default: return SEG_1;
    }
}

void FND_Init()
{
    display[0] = SEG_U;
    display[1] = SEG_P;
    display[2] = SEG_1;
    display[3] = SEG_F;
}

void FND_SetUp(uint8_t floor)
{
    display[0] = SEG_U;                 // DIG1
    display[1] = SEG_P;                 // DIG2
    display[2] = floor_to_seg(floor);   // DIG3
    display[3] = SEG_F;                 // DIG4
}

void FND_SetDown(uint8_t floor)
{
    display[0] = SEG_d;
    display[1] = SEG_o;
    display[2] = floor_to_seg(floor);
    display[3] = SEG_F;
}

static uint32_t prevTick = 0;
const static uint32_t waitTick = 200;



void FND_UP(uint8_t floor)
{
	uint32_t currentTick = HAL_GetTick();		//Non-blocking
	if ((currentTick - prevTick) < waitTick) return; // 200ms 아직 안 됨
	prevTick = currentTick;
	FND_SetUp(floor);
}

void FND_DOWN(uint8_t floor)
{
	uint32_t currentTick = HAL_GetTick();		//Non-blocking
	if ((currentTick - prevTick) < waitTick) return; // 200ms 아직 안 됨
	prevTick = currentTick;
	FND_SetDown(floor);
}








