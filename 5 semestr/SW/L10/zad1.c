#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
    // ustaw baudrate
    UBRR0 = UBRR_VALUE;
    // wyczyść rejestr UCSR0A
    UCSR0A = 0;
    // włącz odbiornik i nadajnik
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    // ustaw format 8n1
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
    // czekaj aż znak dostępny
    while (!(UCSR0A & _BV(RXC0)));
    return UDR0;
}

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

FILE hd44780_file;

char line[16] = "                ";
void change_line()
{
  LCD_Clear();
  for(uint8_t i = 0; i < 16 && line[i] !='\r' && line[i] !='\n'; i++, LCD_GoTo(i,0))
    printf("%c", line[i]);
}

int main()
{
  uart_init();
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK);
  LCD_Clear();
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &hd44780_file;
  // program testow
  uint8_t i = 0;
  char read = '.';
  change_line();
  while(1) {
    
    if(read == '\n' || read == '\r' || i >= 16)
    {
      read = '.';
      i = 0;
      change_line();
    }else{
      scanf("%c", &read);
      line[i] = read;
      LCD_GoTo(i++, 1); //kolumna, wiersz
      printf("%c", read);
    }
    // LCD_GoTo(i%16, 1); //kolumna, wiersz
  
    // printf("|");
  }
}


