#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define LED PB4
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PD2
#define BTN_PIN PIND
#define BTN_PORT PORTD
#define BTN_DDR DDRD
#define BUF_SIZE 1000
#define UNIT 100
#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

int main() {
	// zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  BTN_DDR &= ~_BV(BTN);
  BTN_PORT |= _BV(BTN);
  LED_DDR |= _BV(LED);
  char t[] = {'!', '!', 'T', 'E', 'M', 'N', 'A', 'I', 'O', 'G', 'K','D', 'W', 'R','U', 'S', '!', '!', 'Q', 'Z', 'Y', 'C', 'X', 'B', 'J', 'P', '!', 'L', '!', 'F', 'V', 'H'}; 
  uint16_t i = 1;
  while (1) {
    //printf("foo\r\n");
    uint32_t ms = 0;
    while (!(BTN_PIN & _BV(BTN))){
      ms++;
      _delay_ms(1);
      if(ms>3*UNIT) LED_PORT |= _BV(LED);
    }
    LED_PORT &= ~_BV(LED);
    if(ms > 3*UNIT) i = i*2;
    else i = i*2 + 1;
    _delay_ms(1);
    ms = 0; 
    while (BTN_PIN & _BV(BTN)){
      ms++;
      _delay_ms(1);
      if(ms>3*UNIT) LED_PORT |= _BV(LED);
      if(ms>7*UNIT) break;
    }
    LED_PORT &= ~_BV(LED);
    if(ms > 7*UNIT){
      printf("%c ", t[i]);
      i = 1;
    }else if(ms > 3*UNIT){
      printf("%c", t[i]);
      i = 1;
    }
  }
}
