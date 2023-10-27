#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
#define BTN_PIN PINC
#define BTN_DDR DDRC
#define BTN_PORT PORTC
#define DEBOUNCE_TIME 1000
#define GRAY_LIMIT 16
#define BITES_LIMIT 4
uint8_t toGray(uint8_t n){
  return n^(n>>1);
}
uint8_t debounce(uint8_t BTN) {
  if(!(BTN_PIN & _BV(BTN))) {
    _delay_us(DEBOUNCE_TIME);
    if (!(BTN_PIN & _BV(BTN))) {
      return 1;
    }
  }
  return 0;
}
void light(uint8_t index){
  index = toGray(index);
  LED_PORT = 0;
  for(int i = 0 ; i < BITES_LIMIT; i++){
	  if(index & (1 << i))
		LED_PORT |= _BV(BITES_LIMIT-1-i);
  }
}
int main() {
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
	for(int i = 0 ; i < 3; i++){
	  BTN_DDR &= ~_BV(i);
  	  BTN_PORT |= _BV(i);
	}
        for(int i = 0; i <= 7; i++){
          LED_DDR |= _BV(i);
	  //LED_PORT |= _BV(i);
	}
	//while(1){}
	uint8_t index = 0;
	uint8_t pressed = 0;
	while(1){
	  
	  if (debounce(0)){ //przypadek reseta
	    if((pressed & _BV(0)) == 0){
              index = 0;
	      pressed |= _BV(0);
	    }
	  } else pressed &= ~_BV(0);
		    
	  if (debounce(1)){ //przypadek prev
	    if((pressed & _BV(1) ) == 0){
              index = (index == 0 ? 0 : index - 1);
	      pressed |= _BV(1);
	    }
	  } else pressed &= ~_BV(1);
	  
          if (debounce(2)){ //przypadek next
	    if((pressed & _BV(2)) == 0){
	      index = (index + 1) % GRAY_LIMIT;
	      pressed |= _BV(2);
	    }
	  } else pressed &= ~_BV(2);
	  
 	  light(index);
	}
}

