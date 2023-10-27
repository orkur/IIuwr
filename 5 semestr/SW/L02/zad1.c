#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PD2
#define BTN_PIN PIND
#define BTN_PORT PORTD
#define BTN_DDR DDRD
#define BUF_SIZE 1000
uint8_t buffor[BUF_SIZE/8];
void set(uint16_t i)
{
  if(i == 0) i = BUF_SIZE;
  i--;
  buffor[i/8] |= _BV(7 - (i % 8));
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

int main() {
  BTN_DDR &= ~_BV(BTN);
  BTN_PORT |= _BV(BTN);
  LED_DDR |= _BV(LED);
  uint16_t i = 0;
  while (1) {
    if (!(BTN_PIN & _BV(BTN))){
      set(i);
    }
    light(i);
    i = (i+1)%BUF_SIZE;
    _delay_ms(1);
  }
}
