/*
 * i2c_lcd.c
 *
 *  Created on: 2026. 2. 3.
 *      Author: user24
 */

#include "i2c_lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

extern I2C_HandleTypeDef hi2c1;


static HAL_StatusTypeDef lcd_i2c_tx(uint8_t *buf, uint16_t len)
{
    // 너무 오래 잡아먹지 않도록: 짧은 timeout + 제한 retry
    // (I2C가 꼬이면 FND 스캔이 깨지거나 멈춘 것처럼 보이는 원인 제거)
    for (int retry = 0; retry < 3; retry++)
    {
        HAL_StatusTypeDef st = HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, buf, len, 5);
        if (st == HAL_OK) return HAL_OK;

        // BUSY 상태가 길게 가면 한번 리셋해주는 게 도움이 되는 경우가 있음
        // (필수는 아니지만 현장에선 효과 있는 편)
        if (st == HAL_BUSY)
        {
            HAL_I2C_DeInit(&hi2c1);
            HAL_I2C_Init(&hi2c1);
        }
    }
    return HAL_ERROR;
}


void lcd_command(uint8_t command)
{
	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = command & 0xf0;
	low_nibble = (command << 4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x04 | 0x08;		// en = 1, rs = 0, rw = 0, backlight = 1
	i2c_buffer[1] = high_nibble | 0x00 | 0x08;		// en = 0, rs = 0, rw = 0, backlight = 1
	i2c_buffer[2] = low_nibble 	| 0x04 | 0x08;		// en = 1, rs = 0, rw = 0, backlight = 1
	i2c_buffer[3] = low_nibble 	| 0x00 | 0x08;		// en = 0, rs = 0, rw = 0, backlight = 1

	lcd_i2c_tx(i2c_buffer, 4);



//	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, i2c_buffer, 4, 100) != HAL_OK)
//	{
//
//	}
}

void lcd_data(uint8_t data)
{
	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = data & 0xf0;
	low_nibble = (data << 4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x05 | 0x08;		// en = 1, rs = 1, rw = 0, backlight = 1
	i2c_buffer[1] = high_nibble | 0x01 | 0x08;		// en = 0, rs = 1, rw = 0, backlight = 1
	i2c_buffer[2] = low_nibble 	| 0x05 | 0x08;		// en = 1, rs = 1, rw = 0, backlight = 1
	i2c_buffer[3] = low_nibble 	| 0x01 | 0x08;		// en = 0, rs = 1, rw = 0, backlight = 1


	lcd_i2c_tx(i2c_buffer, 4);


//	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, i2c_buffer, 4, 100) != HAL_OK)
//	{
//
//	}
}

static bool Flag_lcd_init = false;

void i2c_lcd_init_check()
{
	if (Flag_lcd_init) return;
	Flag_lcd_init = true;

	HAL_Delay(50);
	lcd_command(0x33);
	HAL_Delay(5);
	lcd_command(0x32);
	HAL_Delay(5);
	lcd_command(0x28);
	HAL_Delay(5);
	lcd_command(DISPLAY_ON);
	HAL_Delay(5);
	lcd_command(0x06);
	HAL_Delay(5);
	lcd_command(CLEAR_DISPLAY);
	HAL_Delay(2);


}


void i2c_lcd_init(void)
{
	i2c_lcd_init_check();
}

void lcd_string(char *str)
{
	while(*str)lcd_data(*str++);
}

void move_cursor(uint8_t row, uint8_t col)
{
	lcd_command(0x80 | row << 6 | col);
}



/* ==============================
 * 배열(16칸) 버퍼 방식
 * ============================== */
static char lcd_line0[17];
static char lcd_line1[17];
static char prev0[17];
static char prev1[17];

static void fill_spaces_16(char *dst)
{
    for (int i = 0; i < 16; i++) dst[i] = ' ';
    dst[16] = '\0';
}

static void make_line16(char *dst16, const char *src)
{
    fill_spaces_16(dst16);
    if (!src) return;

    for (int i = 0; i < 16 && src[i] != '\0'; i++)
        dst16[i] = src[i];
}

void LCD_ClearBuffers(void)
{
    fill_spaces_16(lcd_line0);
    fill_spaces_16(lcd_line1);
    fill_spaces_16(prev0);
    fill_spaces_16(prev1);
}

void LCD_SetText2Lines(const char *line0, const char *line1)
{
    make_line16(lcd_line0, line0);
    make_line16(lcd_line1, line1);
}

// 변경된 줄만 LCD에 반영
void LCD_CommitIfChanged(void)
{
    // init이 안 됐으면 자동으로 once
    i2c_lcd_init_check();

    if (memcmp(prev0, lcd_line0, 16) != 0)
    {
        memcpy(prev0, lcd_line0, 17);
        move_cursor(0, 0);
        // 16칸 고정 출력(잔상 방지)
        for (int i = 0; i < 16; i++) lcd_data((uint8_t)prev0[i]);
    }

    if (memcmp(prev1, lcd_line1, 16) != 0)
    {
        memcpy(prev1, lcd_line1, 17);
        move_cursor(1, 0);
        for (int i = 0; i < 16; i++) lcd_data((uint8_t)prev1[i]);
    }
}
