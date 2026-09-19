#include "ssd1306.h"

static I2C_HandleTypeDef *ssd1306_i2c;
static uint8_t SSD1306_Buffer[1024];

static const uint8_t font5x7[][5] =
{
    // Space
    {0x00, 0x00, 0x00, 0x00, 0x00},

    // 0-9
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9

    // :
    {0x00, 0x36, 0x36, 0x00, 0x00},

    // Letters we need
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x3E, 0x41, 0x41, 0x41, 0x22},  // C
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x46, 0x49, 0x49, 0x49, 0x31}  // S
};

void SSD1306_DrawChar(uint8_t x, uint8_t y, char c)
{
    const uint8_t *character = NULL;

    if (c == ' ')
    {
        character = font5x7[0];
    }
    else if (c >= '0' && c <= '9')
    {
        character = font5x7[1 + (c - '0')];
    }
    else
    {
        switch (c)
        {
            case ':': character = font5x7[11]; break;
            case 'F': character = font5x7[12]; break;
            case 'H': character = font5x7[13]; break;
            case 'L': character = font5x7[14]; break;
            case 'M': character = font5x7[15]; break;
            case 'N': character = font5x7[16]; break;
            case 'O': character = font5x7[17]; break;
            case 'P': character = font5x7[18]; break;
            case 'T': character = font5x7[19]; break;
            case 'E': character = font5x7[20]; break;
            case 'C': character = font5x7[21]; break;
            case 'A': character = font5x7[22]; break;
            case 'S': character = font5x7[23]; break;

            default:
                return;
        }
    }

    for (uint8_t column = 0; column < 5; column++)
    {
        uint8_t columnData = character[column];

        for (uint8_t row = 0; row < 7; row++)
        {
            if (columnData & (1U << row))
            {
                SSD1306_DrawPixel(x + column, y + row);
            }
        }
    }
}

void SSD1306_WriteString(uint8_t x, uint8_t y, const char *str)
{
    while (*str)
    {
        SSD1306_DrawChar(x, y, *str);

        x += 6;
        str++;
    }
}


void SSD1306_SendCommand(uint8_t command)
{
    uint8_t data[2];

    data[0] = 0x00;
    data[1] = command;

    HAL_I2C_Master_Transmit(
        ssd1306_i2c,
        SSD1306_I2C_ADDR,
        data,
        2,
        HAL_MAX_DELAY
    );
}

void SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
    ssd1306_i2c = hi2c;

    HAL_Delay(100);

    SSD1306_SendCommand(0xAE); // Display OFF
    SSD1306_SendCommand(0x20); // Set memory addressing mode
    SSD1306_SendCommand(0x00); // Horizontal addressing mode
    SSD1306_SendCommand(0xB0); // Set page start address
    SSD1306_SendCommand(0xC8); // COM scan direction
    SSD1306_SendCommand(0x00); // Low column address
    SSD1306_SendCommand(0x10); // High column address
    SSD1306_SendCommand(0x40); // Start line address

    SSD1306_SendCommand(0x81); // Contrast control
    SSD1306_SendCommand(0x7F);

    SSD1306_SendCommand(0xA1); // Segment remap
    SSD1306_SendCommand(0xA6); // Normal display

    SSD1306_SendCommand(0xA8); // Multiplex ratio
    SSD1306_SendCommand(0x3F);

    SSD1306_SendCommand(0xA4); // Display follows RAM
    SSD1306_SendCommand(0xD3); // Display offset
    SSD1306_SendCommand(0x02);

    SSD1306_SendCommand(0xD5); // Display clock
    SSD1306_SendCommand(0x80);

    SSD1306_SendCommand(0xD9); // Pre-charge period
    SSD1306_SendCommand(0xF1);

    SSD1306_SendCommand(0xDA); // COM pin configuration
    SSD1306_SendCommand(0x12);

    SSD1306_SendCommand(0xDB); // VCOMH deselect level
    SSD1306_SendCommand(0x40);

    SSD1306_SendCommand(0x8D); // Charge pump
    SSD1306_SendCommand(0x14);

    SSD1306_SendCommand(0xAF); // Display ON
}

void SSD1306_Clear(void)
{
    for (int i = 0; i < 1024; i++)
    {
        SSD1306_Buffer[i] = 0x00;
    }

    for (uint8_t page = 0; page < 8; page++)
    {
        SSD1306_SendCommand(0xB0 + page);
        SSD1306_SendCommand(0x00);
        SSD1306_SendCommand(0x10);

        uint8_t data[129];

        data[0] = 0x40;

        for (int x = 0; x < 128; x++)
        {
            data[x + 1] = SSD1306_Buffer[(page * 128) + x];
        }

        HAL_I2C_Master_Transmit(
            ssd1306_i2c,
            SSD1306_I2C_ADDR,
            data,
            129,
            HAL_MAX_DELAY
        );
    }
}

void SSD1306_UpdateScreen(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        SSD1306_SendCommand(0xB0 + page);
        SSD1306_SendCommand(0x00);
        SSD1306_SendCommand(0x10);

        uint8_t data[129];

        data[0] = 0x40;

        for (int x = 0; x < 128; x++)
        {
            data[x + 1] = SSD1306_Buffer[(page * 128) + x];
        }

        HAL_I2C_Master_Transmit(
            ssd1306_i2c,
            SSD1306_I2C_ADDR,
            data,
            129,
            HAL_MAX_DELAY
        );
    }
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y)
{
    if (x >= 128 || y >= 64)
    {
        return;
    }

    uint16_t index = x + (y / 8) * 128;

    SSD1306_Buffer[index] |= (1 << (y % 8));
}
