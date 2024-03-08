#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define Vx 1126

#define LEFT PD5
#define RIGHT PD6
#define LED PB1
#define STRONA_DDR DDRD
#define STRONA_PORT PORTD
// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
//  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  =  _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADATE) | _BV(ADIE); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  
}

ISR(ADC_vect)
{
    ADCSRA |= _BV(ADIF);
    if(ADC > 512)
        STRONA_PORT = _BV(RIGHT);
    else
        STRONA_PORT = _BV(LEFT);
    OCR1A = (ADC + 512 )% (1<<10);
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
  STRONA_DDR |= _BV(LEFT) | _BV(RIGHT);
  DDRB |= _BV(LED);
  clock_init();
  adc_init();
  sei();
  ADCSRA |= _BV(ADSC);
  while(1);
}


