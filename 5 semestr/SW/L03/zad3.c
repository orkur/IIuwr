#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define Vx 1126

#define LED PD2
#define LED_DDR DDRD
#define LED_PORT PORTD
uint16_t tab[64] = {0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 20, 20, 20, 20, 20, 20, 20, 20, 54, 54, 54, 54, 54, 54, 54, 54, 148, 148, 148, 148, 148, 148, 148, 148, 403, 403, 403, 403, 403, 403, 403, 403, 1096, 1096, 1096, 1096, 1096, 1096, 1096, 1096};
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
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  =  _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  
}

FILE uart_file;

void delay_us(uint16_t l){
    
    while(l--) _delay_us(1);

}

int main()
{
  LED_DDR |= _BV(LED);
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  // mierz napięcie
  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)LED_DDR |= _BV(LED);
    uint16_t dl = ADC;
    dl >>= 4;
    LED_PORT = _BV(LED);
    delay_us(tab[dl]);
    LED_PORT = ~_BV(LED);
    delay_us(tab[64-dl-1]);
//    printf("%"PRIu16"\r\n",dl);
//    printf("Odczytano: %"PRIu16" Reszta: %"PRIu16"/%"PRIu16"\r\n", Vx/v, Vx%v,v);
  }
}


