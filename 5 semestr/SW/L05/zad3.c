#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define Vx 1126
#define Vxm 112640
#define LED PD2
#define LED_DDR DDRD
#define LED_PORT PORTD

// inicjalizacja UART
void uart_init()
{
  LED_DDR |= _BV(LED);
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
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

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0) |  _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  =  _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADEN) | _BV(ADATE) | _BV(ADIE); // włącz ADC
}

FILE uart_file;

volatile float sum = 0;
volatile int tab[128];
volatile int idx = 0;
ISR(ADC_vect){
//  printf("foo\r\n");
  ADCSRA |= _BV(ADIF);
  int v = ADC;
  tab[idx] = (Vxm/ADC >> 10);
  sum += tab[idx];
  idx = (idx+1)%128;
}

int main()
{
  
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  sei();
  // mierz napięcie
//  uint8_t i = 0;
  set_sleep_mode(SLEEP_MODE_IDLE);
  float wyn = 0;
  while(1) {
//    if(i <= 4)
//        LED_PORT |= _BV(LED);
//    else
//        LED_PORT &= ~_BV(LED);
//    i++;
//  
    sum = 0;
    for(uint8_t i = 0; i < 127; i++){
        ADCSRA |= _BV(ADSC); // wykonaj konwersję
        while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
        ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)LED_DDR |= _BV(LED);
        tab[i] = (Vxm/ADC >> 10);
        sum += (float) tab[i];
        
    }
    sum /= 128;
    wyn = 0;
    for(uint8_t i = 0; i < 127; i++){
        wyn += (tab[i] - sum)*(tab[i] - sum);
    }
        wyn/=128;
    printf("Odczytano pierwsza metoda: %f\r\n", wyn);
    
    sum = 0;
    for(uint8_t i = 0; i < 127; i++){
//        printf("%d", i);
        sleep_mode();
    }
    sum /= 128;
    wyn = 0;
    for(uint8_t i = 0; i < 127; i++){
        wyn += (tab[i] - sum)*(tab[i] - sum);
    }
        wyn/=128;
    printf("Odczytano druga metoda: %f\r\n", wyn);
    _delay_ms(1000);
  }
}


