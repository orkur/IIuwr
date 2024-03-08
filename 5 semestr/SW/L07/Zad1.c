
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdio.h>
#include "i2c.h"

#define BAUD 9600 //baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1) //zgodnie ze wzorem


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

const char st1[] PROGMEM ="write\0";
const char st2[] PROGMEM ="read\0";

#define i2cCheck(code, msg)\
  if((TWSR & 0xf8) != (code)){\
    printf(msg " failed, status: %.2x\r\n",(TWSR & 0xf8));\
  i2cReset();\
  continue;\
}

const uint8_t eeprom_addr = 0xa0; //id code
int main(){
  uart_init();
  fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  i2cInit();
  uint16_t addr = 0;
  uint8_t data = 0;
  char inst[]={"\0\0\0\0\0\0\0\0\0"};
  while(1){
    scanf("%s",&inst);
    printf("%s: ",inst);
    if(!(strcmp_P(inst,&st1))){ //string compare
      scanf("%"SCNx16"%"SCNx8,&addr,&data);
      printf("%.3x %"PRId8"\r\n",addr,data);
      i2cStart();
      i2cCheck(0x08,"write start"); //222 atmega
      i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
      i2cCheck(0x18,"write request");
      i2cSend(addr&0xff);
      i2cCheck(0x28,"set address");
      i2cSend(data);
      i2cCheck(0x28,"data transmitted");
      i2cStop();
      i2cCheck(0xf8,"stop");
    }else if(!(strcmp_P(inst,&st2))){
      scanf("%"SCNx16,&addr);
      printf("%.3x\n\r",addr);
      i2cStart();
      i2cCheck(0x08,"read start");
      i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
      i2cCheck(0x18,"write request");
      i2cSend(addr&0xff);
      i2cCheck(0x28,"set address");
      i2cStart();
      i2cCheck(0x10,"second start");
      i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));
      i2cCheck(0x40, "read request");
      data = i2cReadNoAck();
      i2cCheck(0x58,"read");
      i2cStop();
      i2cCheck(0xf8,"stop");
      printf("%.3x: %x\r\n",addr,data);
    }else{
      printf("Please write correct command! \r\n");
    }
  }
}
