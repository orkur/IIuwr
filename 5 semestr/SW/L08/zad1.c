/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
#include "uart.h"

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainLED_TASK_PRIORITY   2

#define mainSERIAL_TASK_PRIORITY 1

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB
#define BTN_DDR DDRB
#define BUF_SIZE 1000

#define LED2 PD5
#define LED2_DDR DDRD
#define LED2_PORT PORTD
/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

uint8_t buffor[BUF_SIZE/8];

static void set(uint16_t i);

static void light(uint16_t i);

static void move();

static void delay();
/******************************************************************************
 * Public function definitions.
 ******************************************************************************/

/**************************************************************************//**
 * \fn int main(void)
 *
 * \brief Main function.
 *
 * \return
 ******************************************************************************/
int main(void)
{
    // Create task.
    xTaskHandle HandleDelay;
    xTaskHandle HandleMove;

    xTaskCreate
        (
         delay,
         "delay",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLED_TASK_PRIORITY,
         &HandleDelay
        );

    xTaskCreate
        (
         move,
         "move",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainSERIAL_TASK_PRIORITY,
         &HandleMove
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

/**************************************************************************//**
 * \fn static vApplicationIdleHook(void)
 *
 * \brief
 ******************************************************************************/
void vApplicationIdleHook(void)
{

}

/******************************************************************************
 * Private function definitions.
 ******************************************************************************/

static void set(uint16_t i)
{
  if(i == 0) i = BUF_SIZE;
  i--;
  buffor[i/8] |= _BV(7 - (i % 8));
}
static void light(uint16_t i)
{
  if (buffor[i/8] & _BV(7 - (i % 8))){
      LED_PORT |= _BV(LED);
      buffor[i/8] &= ~_BV(7-(i % 8));
  }else{
      LED_PORT &= ~_BV(LED);
  }
}
static void delay()
{
  BTN_DDR &= ~_BV(BTN);
  BTN_PORT |= _BV(BTN);
  LED_DDR |= _BV(LED);
  uint16_t i = 0;
  while (1) {
    if (!(BTN_PIN & _BV(BTN))){
      set(i);
    }
    light(i);
    i = (i+1)%BUF_SIZE;
    vTaskDelay(1/portTICK_PERIOD_MS );
  }
}

static void move()
{
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
	for(int i = 0; i <= 7; i++)
	    LED2_DDR |= _BV(i);
//    LED2_PORT = 0;    
	while (1) {
	    for(int i = 0; i < 7; i++){
		    LED2_PORT |= _BV(i);
		    vTaskDelay(50/portTICK_PERIOD_MS );
		    LED2_PORT &= ~_BV(i);
	    }
	    for(int i = 7; i > 0; i--){
		    LED2_PORT |= _BV(i);
		    vTaskDelay(50/portTICK_PERIOD_MS);
		    LED2_PORT &= ~_BV(i);
	    }
            
	}
}
