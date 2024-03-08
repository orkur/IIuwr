/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdint.h>
//#include "uart.h"
#include <queue.h>

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainLED_TASK_PRIORITY   2

#define mainSERIAL_TASK_PRIORITY 1

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

FILE uart_file;

static void collect();

static void light();
int uart_transmit(uint8_t data, FILE *stream);
int uart_receive(FILE *stream);
QueueHandle_t xWrite, xRead;

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0) | _BV(UDRIE0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}


ISR(USART_RX_vect)
{
  uint8_t data;
  data = UDR0;
  xQueueSendFromISR(xRead, &data, 0);

}
ISR(USART_UDRE_vect){
  uint8_t data;
  if(xQueueReceiveFromISR(xWrite, &data, 0) == pdTRUE)
    UDR0 = data;
  
}


int uart_transmit(uint8_t data, FILE *stream)
{
  
  xQueueSend( xWrite, &data, portMAX_DELAY);
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  uint8_t a;
  xQueueReceive( xRead, &a, portMAX_DELAY); 
  return a;
}

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
    // skonfiguruj strumienie wejścia/wyjścia
    xWrite = xQueueCreate(128, sizeof(char));
    xRead = xQueueCreate(128, sizeof(char));
    // uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);

    // Create task.
    xTaskHandle HandleCol;
    xTaskHandle HandleLight;



    xTaskCreate
        (
         collect,
         "collect",
         200,
         NULL,
         2,
         &HandleCol
        );

    xTaskCreate
        (
         light,
         "light",
         200,
         NULL,
         1,
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
  stdin = stdout = stderr = &uart_file;

    while(1)
    {
        uint8_t a = getchar();
        putchar(a);
        if(a == '\r')
          putchar('\n');
    }

}
static void light()
{
    LED_DDR |= _BV(LED);
  while (1) {
    LED_PORT |= _BV(LED);
    vTaskDelay(500/portTICK_PERIOD_MS );
    LED_PORT &= ~_BV(LED);
    vTaskDelay(500/portTICK_PERIOD_MS );
  }

}
