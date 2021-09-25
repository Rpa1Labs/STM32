#include <stm32f3xx_hal.h>

/** LED */
#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

/** UART */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_BAUDRATE                  115200