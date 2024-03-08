#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define LED PB2
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PA7
#define BTN_PIN PINA
#define BTN_PORT PORTA
#define BTN_DDR DDRA
#define BUF_SIZE 1000

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

uint8_t buffor[BUF_SIZE/8];
void set(uint16_t i, uint16_t l)
{

      	if(i == 0) i = BUF_SIZE;
  i--;
  if(l)
      buffor[i/8] |= _BV(7 - (i % 8));
  else
      buffor[i/8] &= ~_BV(7 - (i % 8));
      
}
void light(uint16_t i)
{
  if (buffor[i/8] & _BV(7 - (i % 8))){
      LED_PORT |= _BV(LED);
      buffor[i/8] &= ~_BV(7-(i % 8));
  }else{
      LED_PORT &= ~_BV(LED);
  }
}

void io_init()
{
  // ustaw pull-up na PD2 i PD3 (INT0 i INT1)
  BTN_PORT |= _BV(BTN);
  BTN_DDR &= ~_BV(BTN);
  // ustaw wyjście na PB5
  DDRB |= _BV(LED);
  // ustaw wyzwalanie przerwania na INT0 i INT1 zboczem narastającym
  MCUCR |=  _BV(ISC00);
  GIMSK |= _BV(PCIE0);
  // odmaskuj przerwania dla INT0 i INT1
  PCMSK0 |= _BV(PCINT7);
}
void timer_init(){
  TCCR0A |= _BV(WGM01); //ctc
  TCCR0B |= _BV(CS01);
  TIMSK0 |= _BV(OCIE0A);
  OCR0A = 124;
}

volatile uint16_t i = 1; 
volatile uint8_t bit = 0;

ISR(PCINT0_vect) {
    if(!bit_is_set(BTN_PIN, BTN))
        bit = 1;
    else
	bit = 0;
}
ISR(TIM0_COMPA_vect){
    if(bit)
	set(i,1);
    i = (i + 1)%BUF_SIZE;
    light(i);
}


int main() {
  io_init();
  set_sleep_mode(SLEEP_MODE_IDLE);
  sei();
  timer_init();
  while (1) {
    sleep_mode();
  }
}
