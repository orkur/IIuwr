#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

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

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 0000 -- normal
  // CS1   = 001  -- prescaler 1
  TCCR1B = _BV(CS10);
}
FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj licznik
  timer1_init();
  // program testowy
  while(1) {
    volatile int8_t i8 = 7, j8 = 5;
    volatile int8_t w8;
    uint16_t a = TCNT1; // wartość licznika przed czekaniem
    w8 = i8 + j8;
    uint16_t b = TCNT1; // wartość licznika po czekaniu
    printf("dane dla typu int8_t\r\n");
    printf("Zmierzony czas dodawania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w8 = i8 - j8;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas odejmowania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w8 = i8 * j8;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas mnozenia: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w8 = i8 / j8;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas dzielenia: %"PRIu16" cykli\r\n", b - a);
    
    
    _delay_ms(5000);
    volatile int16_t i16 = 7, j16 = 5;
    volatile int16_t w16;
    a = TCNT1; // wartość licznika przed czekaniem
    w16 = i16 + j16;
    b = TCNT1; // wartość licznika po czekaniu
    printf("dane dla typu int16_t\r\n");
    printf("Zmierzony czas dodawania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w16 = i16 - j16;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas odejmwania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w16 = i16 * j16;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas mnozenia: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w16 = i16 / j16;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas dzielenia: %"PRIu16" cykli\r\n", b - a);
    
    
    _delay_ms(5000);
    volatile int32_t i32 = 7, j32 = 5;
    volatile int32_t w32;
    a = TCNT1; // wartość licznika przed czekaniem
    w32 = i32 + j32;
    b = TCNT1; // wartość licznika po czekaniu
    printf("dane dla typu int32_t\r\n");
    printf("Zmierzony czas dodawania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w32 = i32 - j32;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas odejmwania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w32 = i32 * j32;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas mnozenia: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w32 = i32 / j32;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas dzielenia: %"PRIu16" cykli\r\n", b - a);
    
    
    _delay_ms(5000);
    volatile int64_t i64 = 7, j64 = 5;
    volatile int64_t w64;
    a = TCNT1; // wartość licznika przed czekaniem
    w64 = i64 + j64;
    b = TCNT1; // wartość licznika po czekaniu
    printf("dane dla typu int64_t\r\n");
    printf("Zmierzony czas dodawania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w64 = i64 - j64;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas odejmwania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w64 = i64 * j64;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas mnozenia: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w16 = i64 / j64;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas dzielenia: %"PRIu16" cykli\r\n", b - a);
    
    
    _delay_ms(5000);
    volatile float i = 7.0, j = 5.0;
    volatile float w;
    a = TCNT1; // wartość licznika przed czekaniem
    w = i + j;
    b = TCNT1; // wartość licznika po czekaniu
    printf("dane dla typu float\r\n");
    printf("Zmierzony czas dodawania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w = i - j;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas odejmwania: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w = i * j;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas mnozenia: %"PRIu16" cykli\r\n", b - a);
    
    a = TCNT1; // wartość licznika przed czekaniem
    w = i / j;
    b = TCNT1; // wartość licznika po czekaniu
    printf("Zmierzony czas dzielenia: %"PRIu16" cykli\r\n", b - a);
    
    
    _delay_ms(5000);
  }
}


