#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"

int hd44780_transmit(char data, FILE *stream)
{
    LCD_WriteData(data);
    return 0;
}

FILE hd44780_file;

char loading[5] = {
    0x10,
    0x18,
    0x1c,
    0x1e,
    0x1f
};

void LCD_SET()
{
    for(int addr = 0; addr < 5; addr++)
    {
        for(int level = 0; level < 8; level++)
        {
            LCD_WriteCommand(HD44780_CGRAM_SET | (addr << 3) | level);
            LCD_WriteData(loading[addr]);
        }
    }
    

}
int main()
{
    // skonfiguruj wyświetlacz
    LCD_Initialize();
    LCD_SET();
    LCD_Clear();
    // skonfiguruj strumienie wyjściowe
    fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
    stdout = stderr = &hd44780_file;

    while (1)
    {
        LCD_Clear();
        LCD_GoTo(0,0);
        printf("DAJ ***** MIODU!");
        for(uint8_t i = 0; i < 16; i++)
        {
            for(char load = 0; load < 5; load++)
            {
                LCD_GoTo(i, 1);
                printf("%c", load);
                _delay_ms(5);
            }
        }
        _delay_ms(40);
    }
}