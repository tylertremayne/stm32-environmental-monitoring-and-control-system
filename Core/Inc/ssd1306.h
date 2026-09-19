#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f4xx_hal.h"

#define SSD1306_I2C_ADDR (0x3C << 1)

void SSD1306_Init(I2C_HandleTypeDef *hi2c);
void SSD1306_SendCommand(uint8_t command);
void SSD1306_Clear(void);
void SSD1306_UpdateScreen(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y);
void SSD1306_DrawChar(uint8_t x, uint8_t y, char c);
void SSD1306_WriteString(uint8_t x, uint8_t y, const char *str);

#endif
