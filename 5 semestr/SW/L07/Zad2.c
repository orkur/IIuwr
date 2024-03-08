
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
uint16_t read_from_string(char a, char b)
{
  if(a >= '0' && a <='9') a = (a - '0');
  else a = (a - 'A') + 10;
  if(b >= '0' && b <='9') b = (b - '0');
  else b = (b - 'A') + 10;
  return a*16  + b;
}
int main(){
  uart_init();
  fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  i2cInit();
  uint16_t addr = 0;
  uint8_t data = 0;
  uint8_t length = 0;
  char inst[]={"\0\0\0\0\0\0\0\0\0"};
  char wej[32];
  while(1){
    scanf("%s",&inst);
    printf("%s: ",inst);
    if(!(strcmp_P(inst,&st1))){ //string compare
      scanf("%s",&wej);
      length = read_from_string(wej[0], wej[1]);
      printf("%s\r\n", wej);
      printf("dlugosc->%.2x \r\n",length);
      addr = read_from_string(wej[2], wej[3]) * 16*16 + read_from_string(wej[4], wej[5]);
      printf("adres->%.4x \r\n",addr);
      i2cStart();
      i2cCheck(0x08,"write start"); //222 atmega
      i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
      i2cCheck(0x18,"write request");
      uint16_t i = 6;
      i2cSend(addr&0xff);
      i2cCheck(0x28,"set address");
      while(length--){
        data = read_from_string(wej[i], wej[i+1]);
        i+=2;
        printf("adres->%.4x liczba->%x\r\n",addr, data);
        i2cSend(data);
        i2cCheck(0x28,"data transmitted");
      }
      i2cStop();
      i2cCheck(0xf8,"stop");
    }else if(!(strcmp_P(inst,&st2))){ //read
      scanf("%"SCNx16"",&addr);
      printf("%.4x ",addr);
      scanf("%"SCNx8"",&length);
      printf("%"PRId16"\n\r", length);
      i2cStart();
      i2cCheck(0x08,"read start");
      i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
      i2cCheck(0x18,"write request");
      i2cSend(addr&0xff);
      i2cCheck(0x28,"set address");
      i2cStart();
      i2cCheck(0x10,"second start");
      uint8_t i = 1;
      i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));
      i2cCheck(0x40, "read request");  
      printf("%.2x%.4x", length, addr);
      uint8_t checksum = length + (addr&0xFF) + ((addr >> 8)&0xFF);
      while(i <= length){
        if(i == length){
          data = i2cReadNoAck();
          i2cCheck(0x58,"read not ack");
        }else{
          data = i2cReadAck();
          i2cCheck(0x50,"read ack");
        
        }
        i++;
        printf("%.2x", data);
        checksum += data;
      }
      printf("%.2x\r\n", checksum);
      i2cStop();
      i2cCheck(0xf8,"stop");
    }else{
      printf("Please write correct command! \r\n");
    }
  }
}

