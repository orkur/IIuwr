#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

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
  UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0);
}
volatile char data;
ISR(USART_RX_vect)
{
  data = UDR0;
  UCSR0B |= _BV(UDRIE0);

}
ISR(USART_UDRE_vect){
  UDR0 = data;
  UCSR0B &= ~_BV(UDRIE0);
}
ISR(USART_TX_vect){
  if(data=='\r'){
    data = '\n';
     UCSR0B |= _BV(UDRIE0);
  }
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  sei();
  // skonfiguruj strumienie wejścia/wyjścia
  set_sleep_mode(SLEEP_MODE_IDLE);
  while(1) {
    sleep_mode();
  }
}


