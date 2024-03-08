#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PD2
#define BTN_PIN PIND
#define BTN_PORT PORTD
#define BTN_DDR DDRD

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
volatile uint8_t read = 0;
void adc_init()
{
  ADMUX   = _BV(REFS0);
  ADMUX &= ~(0b1111);
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  =  _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  
}
void timer_init()
{
  TCCR0A |= _BV(WGM01); //ctc
  TCCR0B |= _BV(CS01) | _BV(CS00);
  TIMSK0 |= _BV(OCIE0A);
  OCR0A = 124;
}

void io_init()
{
  // ustaw pull-up na PD2 i PD3 (INT0 i INT1)
  PORTD |= _BV(BTN);
  DDRD &= ~_BV(BTN);
  // ustaw wyzwalanie przerwania na INT0 i INT1 zboczem narastającym
    EICRA |=  _BV(ISC00);
  EICRA &= ~_BV(ISC01);
  // odmaskuj przerwania dla INT0 i INT1
  EIMSK |= _BV(INT0);
}
volatile uint8_t pressed = 0;
ISR(INT0_vect) {
       if(!bit_is_set(BTN_PIN, BTN))
	pressed = 1;
       else
	pressed = 0;
        adc_init();        
        read |= 1;
	//pressed ^= 1;
//    }
}
volatile uint8_t print = 0;
volatile float ohm = 0;
ISR(TIMER0_COMPA_vect){
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    if(read)
      print = 1;
    else
      print = 0;
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
//    volatile float u = (ADC*5.0)/1023.0;
    if(pressed){    
    float u = (ADC*5.0)/1024.0;
    ohm = ((10000.0*u)/(5.0-u));
    }
}


int main()
{
  io_init();
  set_sleep_mode(SLEEP_MODE_IDLE);
  sei();
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // uruchom licznik
  timer_init();
  while(1)
  {
    
    sleep_mode();
    if(read)
        printf("%d %f \r\n", ADC, ohm);
    _delay_ms(500);
  }
}
