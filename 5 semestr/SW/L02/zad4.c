#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_DDRC DDRC
#define LED_PORTC PORTC
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

void twinkle_number(int number,int  number2)
{
	LED_PORTC = _BV(0);
	light_number(number2);
	_delay_ms(10);
	turn_off_number();
	LED_PORTC = _BV(1);
	light_number(number);
	_delay_ms(10);
	turn_off_number();
}

int main() {
        UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
        LED_DDR |= (1<<8) - 1;
        LED_DDRC |= 3;
	//while(1) {twinkle_number(22);}
	while (1) {
		for(int i = 0; i < 6; i++){
               		for(int j = 0; j < 10; j++){
				int times = 50;
				while(times--)
					twinkle_number(i, j);
			}
		}
		//twinkle_number(00);
	}
}

