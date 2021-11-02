#include "stm32f3xx_hal.h"

/** UART */
#define     USARTx_TX_PIN       GPIO_PIN_2
#define     USARTx_RX_PIN       GPIO_PIN_3
#define     USARTx_BAUDRATE     115200

/** DHT22 */
#define     GPIO_DHT22          GPIOA
#define     PIN_DHT22           GPIO_PIN_9

/** RCSwitch */
#define     GPIO_RF             GPIOB
#define     GPIO_PIN_RF         GPIO_PIN_10

/** LED */
#define     LED_PIN             GPIO_PIN_5
#define     LED_GPIO_PORT       GPIOA