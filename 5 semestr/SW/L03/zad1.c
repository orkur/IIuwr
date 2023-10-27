#include <avr/io.h>
#include <util/delay.h>

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define TONE(step, delay) \
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step) / 2; i++) { \
      BUZZ_PORT |= _BV(BUZZ); \
      _delay_us(step); \
      BUZZ_PORT &= ~_BV(BUZZ); \
      _delay_us(step); \
    }
#define dlugosc 180
#define nuta 64
#define polnuta 48
#define cwiercnuta 32
#define osemka 16
#define szesnastka 0

#define B3 1118
#define C4 1055
#define D4 940
#define E4 837
#define F4 790
#define G4 704
#define A4 627
#define B4 559
#define C5 527
#define D5 498

#define B3_note 9
#define C4_note 0
#define D4_note 1
#define E4_note 2
#define F4_note 3
#define G4_note 4
#define A4_note 5
#define B4_note 6
#define C5_note 7
#define D5_note 8
#define pause 10

void useTONE(uint16_t wavelength, uint16_t ile)
{
    while(ile--)
    {
        switch (wavelength)
        {
            case 0:
                TONE(C4, dlugosc);
                break;
            case 1:
                TONE(D4, dlugosc);
                break;
            case 2:
                TONE(E4, dlugosc);
                break;
            case 3:
                TONE(F4, dlugosc);
                break; 
            case 4:
                TONE(G4, dlugosc);
                break;  
            case 5:
                TONE(A4, dlugosc);
                break;  
            case 6:
                TONE(B4, dlugosc);
                break;  
            case 7:
                TONE(C5, dlugosc);
                break;
            case 8:
                TONE(D5, dlugosc);
                break;
            case 9:
                TONE(B3, dlugosc);
                break;
            default:                
                _delay_ms(dlugosc);      
        }
    }
}

int main() {
  BUZZ_DDR |= _BV(BUZZ);
  //breezeblocks
  const uint16_t utwor[73*3 + 1] ={
//0
A4_note + szesnastka, G4_note + szesnastka, F4_note + szesnastka,// pause+szesnastka,

F4_note + osemka, G4_note + szesnastka, A4_note + osemka, C5_note + szesnastka, A4_note + szesnastka, G4_note + szesnastka,// pause+szesnastka,

F4_note + szesnastka, F4_note + szesnastka, G4_note + szesnastka, F4_note + szesnastka,// pause + szesnastka,


A4_note + szesnastka, C5_note + szesnastka, A4_note + szesnastka, G4_note + szesnastka,// pause + szesnastka,

F4_note + osemka, G4_note + szesnastka, G4_note + osemka, D4_note + osemka,// pause + szesnastka, 

B3_note + szesnastka, B3_note + szesnastka, B3_note + szesnastka, B3_note + szesnastka	, pause + osemka,


A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,

F4_note + osemka, G4_note + szesnastka, A4_note + szesnastka, //pause + szesnastka,

A4_note + szesnastka, C5_note + szesnastka, A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,

F4_note + osemka, G4_note + szesnastka, F4_note + szesnastka, //pause + szesnastka,


//4
A4_note + szesnastka, C5_note + osemka, A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,

F4_note + szesnastka, F4_note + szesnastka, F4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka, 

G4_note + osemka, D4_note + szesnastka, //pause + szesnastka,

D4_note + szesnastka, D4_note + szesnastka, D4_note + szesnastka, D4_note + szesnastka, pause + osemka,


D5_note + szesnastka, C5_note + szesnastka, //pause + szesnastka,

A4_note + osemka, F4_note + szesnastka, //pause + szesnastka,

G4_note + szesnastka, F4_note + szesnastka, G4_note + szesnastka, F4_note + szesnastka, //pause + szesnastka,

A4_note + osemka, F4_note + szesnastka, //pause + szesnastka,


G4_note + szesnastka, F4_note + szesnastka, G4_note + szesnastka, F4_note + szesnastka, //pause + szesnastka,

A4_note + osemka, F4_note + szesnastka,G4_note + szesnastka, pause + osemka,

F4_note + szesnastka, D4_note + osemka, //pause + szesnastka,

D4_note + szesnastka, D4_note + szesnastka, D4_note + szesnastka, D4_note + szesnastka,  pause + szesnastka, //pause + szesnastka,


//7
D4_note + szesnastka, A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,

F4_note + osemka, G4_note + szesnastka, F4_note + szesnastka, //pause + szesnastka,

A4_note + szesnastka, C5_note + szesnastka, A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,


G4_note + cwiercnuta,

A4_note + cwiercnuta,

D5_note + cwiercnuta,

D4_note + szesnastka, D4_note + szesnastka, D4_note + szesnastka, D4_note + szesnastka, pause + szesnastka, //pause + szesnastka,


A4_note + szesnastka, G4_note + szesnastka, F4_note + szesnastka, //pause + szesnastka,

G4_note + osemka, D4_note + szesnastka, D4_note + szesnastka, //pause + szesnastka,

A4_note + szesnastka, F4_note + osemka, G4_note + szesnastka, //pause + szesnastka,
 
A4_note + osemka, pause + szesnastka, //pause + szesnastka,


//10
A4_note + szesnastka, G4_note + szesnastka, F4_note + szesnastka, //pause + szesnastka,

G4_note + osemka, D4_note + szesnastka, D4_note + osemka, F4_note + szesnastka, D4_note + szesnastka, C4_note + szesnastka, //pause + szesnastka, 

D4_note + osemka, pause + szesnastka, //pause + szesnastka, 


A4_note + szesnastka, G4_note + osemka, //pause + szesnastka,

G4_note + szesnastka, D4_note + szesnastka, D4_note + osemka, //pause + szesnastka,

F4_note + osemka, A4_note + szesnastka, //pause + szesnastka,
 
G4_note + szesnastka, A4_note + osemka, pause + szesnastka, //pause + szesnastka,


A4_note + szesnastka, A4_note + szesnastka, A4_note + szesnastka, //pause + szesnastka,

A4_note + osemka, G4_note + osemka, //pause + szesnastka,

F4_note + szesnastka, G4_note + szesnastka, A4_note + szesnastka, G4_note + osemka, F4_note + szesnastka, D4_note + osemka, //pause + szesnastka, 


//13
F4_note + polnuta, //pause + szesnastka,

C4_note + cwiercnuta, //pause + szesnastka,

E4_note + polnuta, //pause + szesnastka,


D4_note + cwiercnuta, //pause + szesnastka,

E4_note + osemka, //pause + szesnastka,

F4_note + polnuta, //pause + szesnastka,


//15
F4_note + polnuta, ///pause + szesnastka,

C4_note + cwiercnuta, //pause + szesnastka,

E4_note + polnuta, ///pause + szesnastka,


D4_note + cwiercnuta, //pause + szesnastka,

A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,

F4_note + osemka, //pause + szesnastka,

F4_note + osemka, //pause + szesnastka,


pause + osemka, A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,

F4_note + osemka, G4_note + szesnastka, A4_note + szesnastka, //pause + szesnastka,

A4_note + szesnastka, D5_note + szesnastka, A4_note + szesnastka, G4_note + szesnastka, //pause + szesnastka,

F4_note + osemka, G4_note + szesnastka, F4_note + szesnastka//, //pause + szesnastka
};
  while (1) {
        for(int i = 0; i < 73*3 + 1; i++)
            useTONE(utwor[i]%16, _BV(utwor[i]>>4));
    _delay_ms(1000);
}
}

