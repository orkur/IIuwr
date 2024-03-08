#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define Vx 1126

#define LED PB1
#define LED_DDR DDRB
#define LED_PORT PORTB
// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
//  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  =  _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADATE) | _BV(ADIE); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
//  set_sleep_mode(SLEEP_MODE_IDLE);
  
}

ISR(ADC_vect)
{
    ADCSRA |= _BV(ADIF);
    OCR1A = ADC;
}
void clock_init()
{
    TCCR1A = _BV(WGM10) | _BV(WGM11) | _BV(COM1A0);
    TCCR1B = _BV(CS11) | _BV(WGM12) | _BV(WGM13);
    OCR1A = 100;
    // TIMSK1 |= _BV(OCIE1A);
}

int main()
{
  LED_DDR |= _BV(LED);
  clock_init();
  adc_init();
  sei();
  ADCSRA |= _BV(ADSC);
  while(1);
}


