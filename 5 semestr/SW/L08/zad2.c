/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
//#include "uart.h"
#include <queue.h>

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainLED_TASK_PRIORITY   1

#define mainSERIAL_TASK_PRIORITY 1

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/



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

QueueHandle_t xQueue;

static void collect();

static void light();
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
    // zainicjalizuj UART
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    // Create task.
    xTaskHandle HandleCol;
    xTaskHandle HandleLight;
    xQueue = xQueueCreate(100, sizeof(uint16_t));

    xTaskCreate
        (
         collect,
         "collect",
         200,
         NULL,
         mainLED_TASK_PRIORITY,
         &HandleCol
        );

    xTaskCreate
        (
         light,
         "light",
         200,
         NULL,
         mainSERIAL_TASK_PRIORITY,
         &HandleLight
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

static void collect()
{
    uart_init();
    while(1)
    {
        uint16_t a;
        scanf("%"SCNd16, &a);
        printf("Odczytano: %"PRId16"\r\n", a);
        xQueueSend( xQueue, ( void * ) &a, ( TickType_t ) 10); 
    }

}
static void light()
{
    LED_DDR |= _BV(LED);
  while (1) {
    uint16_t a = -1;
    if( xQueueReceive( xQueue, &a, 0) == pdPASS ){
      LED_PORT |= _BV(LED);
      printf("wart: %"PRId16"\r\n", a);
      while(a-- > 0)
          vTaskDelay(1/portTICK_PERIOD_MS );
      LED_PORT &= ~_BV(LED);
      vTaskDelay(500/portTICK_PERIOD_MS );
    }
  }

}
