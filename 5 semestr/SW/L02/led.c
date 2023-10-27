#include <avr/io.h>
#include <util/delay.h>

#define LED PD5
#define LED_DDR DDRD
#define LED_PORT PORTD

int main() {
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
	uint8_t digit[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111,
			     0b01100110, 0b01101101, 0b01111101, 0b00000111,
			     0b01111111, 0b01101111};	
	uint8_t WAZNE[] = {0b01110011, 0b01111001, 0b00110111, 0b00110000, 0b01101101};
	for(int i = 0; i <= 7; i++)
		LED_DDR |= _BV(i);
	while (1) {
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 8; j++)
				if((~digit[i] >> j) & 1)
				       LED_PORT |= _BV(j);
			_delay_ms(500);	
			for(int j = 0; j < 8; j++)
				if((~digit[i] >> j) & 1)
				       LED_PORT &= ~_BV(j);
		}
	}
}

