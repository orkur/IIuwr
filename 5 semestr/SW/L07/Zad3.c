#define __AVR_ATmega328P__

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

const uint8_t RTC_ADDR = 0xd0;
uint8_t ACTUAL_CENTURY = 20;

const char date[] PROGMEM ="date\0";
const char time[] PROGMEM ="time\0";
const char set[] PROGMEM ="set\0";
char input[12];

#define i2cCheck(code, msg)\
    if ((TWSR & 0xf8) != (code))\
    {\
        printf(msg " failed, status: %.2x\r\n", (TWSR & 0xf8)); \
        i2cReset();\
    }

const uint8_t eeprom_addr = 0xa0; //id code
uint16_t read_from_string(char a, char b)
{
  a = (a - '0');
  b = (b - '0');
  return a*16  + b;
}

void write_byte(uint8_t addr, uint8_t data){
    i2cStart();
    i2cCheck(0x08,"write start");

    i2cSend(RTC_ADDR);
    i2cCheck(0x18,"write request");

    i2cSend(addr);
    i2cCheck(0x28,"set address");

    i2cSend(data);
    i2cCheck(0x28,"data transmitted");

    i2cStop();
    i2cCheck(0xf8,"stop");
}

uint8_t read_byte(uint8_t addr) {

    i2cStart();
    i2cCheck(0x08,"write start");

    i2cSend(0xd0);
    i2cCheck(0x18,"write request");

    i2cSend(addr);
    i2cCheck(0x28,"set address");

    i2cStart();
    i2cCheck(0x10,"read start");

    i2cSend(0xd0 | 1);
    i2cCheck(0x40,"read request");

    uint8_t byte = i2cReadNoAck();
    i2cCheck(0x58,"read");

    i2cStop();
    i2cCheck(0xf8,"stop");
    
    return byte;
}

uint8_t sec(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}
uint8_t min(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}
uint8_t hour(uint8_t byte)
{
    uint8_t format = byte & 0x40;
    if(format)
    {
        if(byte & 0x20)
        {
            return 10 * ((byte & 0x10) >> 4) + (byte & 0x0F) + 12;
        } else 
        {
            return 10 * ((byte & 0x10) >> 4) + (byte & 0x0F);
        }
    } else 
    {
        return 20 * ((byte & 0x20) >> 5) + 10 * ((byte & 0x10) >> 4) + (byte & 0x0F);
    }
}


uint8_t get_day(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}

uint8_t get_month(uint8_t byte)
{
    return ((10 * (byte >> 4)) & 0x3) + (byte & 0x0F);
}
uint8_t get_year(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}
uint8_t get_century(uint8_t byte)
{
    return (byte & 0x40) >> 7;
}


void read_current_time()
{
    uint8_t curr_sec = sec(read_byte(0));
    uint8_t curr_min = min(read_byte(1));
    uint8_t curr_hour = hour(read_byte(2));
    
    printf("Current time: %.2d:%.2d:%.2d\r\n", curr_hour, curr_min, curr_sec);

}


void read_current_date()
{
    
    uint8_t curr_day = get_day(read_byte(4));
    uint8_t curr_month = get_month(read_byte(5));
    uint8_t curr_year = get_year(read_byte(6));
    
    printf("Current date: %.2d%.2d-%.2d-%.2d\r\n", ACTUAL_CENTURY, curr_year, curr_month, curr_day);

}

uint8_t convertToUint(char x, char y)
{
    return (x - '0') * 10 + (y - '0');
}

void write_time(const char *str)
{
    uint8_t hour = convertToUint(str[0], str[1]);
    uint8_t minute = convertToUint(str[3], str[4]);
    uint8_t second = convertToUint(str[6], str[7]);

    write_byte(0,((second / 10) << 4) | (second % 10));
    write_byte(1,((minute / 10) << 4) | (minute % 10));
    if(hour >= 20)
        write_byte(2, ((hour / 20 << 5)) | (hour % 10));
    else if (hour >= 10)
        write_byte(2, ((1 << 4)) | (hour % 10));
    else 
        write_byte(2, hour % 10);

}
void write_date(const char *str)
{
    uint8_t day = convertToUint(str[0], str[1]);
    uint8_t month = convertToUint(str[3], str[4]);
    uint8_t year = convertToUint(str[8], str[9]);
    uint8_t century = convertToUint(str[6], str[7]);


    write_byte(4,((day / 10) << 4) | (day % 10));
    write_byte(5,((month / 10) << 4) | (month % 10));
    write_byte(6,((year / 10) << 4) | (year % 10));
    ACTUAL_CENTURY = century;
}


int main()
{
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    i2cInit();

    while (1)
    {

        scanf("%s", &input);
        printf("%s\r\n", &input);

        if (!strcmp_P(&input, &set))
        {
            scanf("%s", &input);
            printf("%s\r\n", &input);


            if (!strcmp_P(&input, &time))
            {
                char time[20];
                scanf("%s", &time);
                printf("%s\r\n", &time);

                write_time(time);

            }
            else if (!strcmp_P(&input, &date))
            {
                char date[20];
                scanf("%s", &date);
                printf("%s\r\n", &date);

                write_date(date);
            }else
                printf("Wrong data!\r\n", &time);

        }
        else if (!strcmp_P(&input, &time))
            read_current_time();
        else if (!strcmp_P(&input, &date))
            read_current_date();
        else
            printf("Wrong data!\r\n", &time);
    }
}
