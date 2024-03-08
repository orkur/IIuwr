#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_DDRC DDRC
#define LED_PORTC PORTC

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_MOSI PB3
#define SPI_SCK PB5
#define SPI_LA PB1
#define SPI_OE PB2

uint8_t digit[10] = {
	    0b00111111, //0
       	0b00000110, //1
       	0b01011011, //2
       	0b01001111, //3
	    0b01100110, //4
       	0b01101101, //5
	    0b01111101, //6
       	0b00000111, //7
	    0b01111111, //8
       	0b01101111  //9
};
void light_number(int number)
{
	for(int j = 0; j < 8; j++)
		if((~digit[number] >> j) & 1)
			LED_PORT |= _BV(j);
}

void turn_off_number()
{
	LED_PORT = 0;
}

void twinkle_number(uint8_t number)
{
	SPI_PORT |= _BV(SPI_LA);
	SPDR = number;
    sleep_mode();
    SPI_PORT &= ~_BV(SPI_LA);
}

void timer_init(){
    TCCR2A = _BV(WGM22);
    TCCR2B = _BV(CS22) | _BV(CS20) | _BV(CS21);
    OCR2A = 195;
    TIMSK2 |= _BV(OCIE2A);
}

volatile int8_t time = 0;
volatile int8_t num = 0;

ISR(TIMER2_COMPA_vect){
    time = (time + 1)%75;
    if(time == 0)
        num = (num +1) % 10;
}
void spi_init(){
    SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_LA) | _BV(SPI_OE);
    SPI_PORT &= ~_BV(SPI_OE);
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPIE);
}
ISR(SPI_STC_vect)
{}

int main() {
    timer_init();
    spi_init();
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    twinkle_number(digit[num]);

    while (1) {
		twinkle_number(digit[num]);
		sleep_mode();
	}
}
