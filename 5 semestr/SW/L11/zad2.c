#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define Vx 1126

#define LED PB1
#define LED_DDR DDRB
#define LED_PORT PORTB

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
    while (!(UCSR0A & _BV(UDRE0)))
        ;
    UDR0 = data;
    return 0;
}
// odczyt jednego znaku
int uart_receive(FILE *stream)
{
    // czekaj aż znak dostępny
    while (!(UCSR0A & _BV(RXC0)))
        ;
    return UDR0;
}

FILE uart_file;


// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  ADMUX |= 1;
//  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  =  _BV(ADIE);
  ADCSRA |= _BV(ADEN); // włącz ADC
  set_sleep_mode(SLEEP_MODE_IDLE);
  
}
 volatile uint32_t open, close;

ISR(TIMER1_CAPT_vect)
{
    ADCSRA |= _BV(ADSC);
    while (!(ADCSRA & _BV(ADIF)));
    ADCSRA |= _BV(ADIF);
//    close = (5-((ADC * 5) / 1024))*1000;
    close = ADC;
}
ISR(TIMER1_OVF_vect)
{
    ADCSRA |= _BV(ADSC);
    while (!(ADCSRA & _BV(ADIF)));
    ADCSRA |= _BV(ADIF);
//    open = ((ADC * 5) / 1024)*1000;
    open = ADC;
}
void clock_init()
{
    TCCR1A = _BV(WGM10) | _BV(COM1A0);
    TCCR1B = _BV(CS11) | _BV(WGM13);
    OCR1A = 500;
    TIMSK1 = _BV(ICIE1) | _BV(TOIE1);
    // TIMSK1 |= _BV(OCIE1A);
}

int main()
{
  LED_DDR |= _BV(LED);
  clock_init();
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
    
  adc_init();
  sei();
  while(1){
    printf("open: %"PRIu32", close: %"PRIu32" \r\n", open, close);
    sleep_mode();
//    _delay_ms(1000);
  }
}


