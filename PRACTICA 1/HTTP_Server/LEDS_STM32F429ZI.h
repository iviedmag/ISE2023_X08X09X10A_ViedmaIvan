/*Este archivo define constantes, macros y funciones para manejar LEDs RGB en un microcontrolador STM32F4xx.*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private define ------------------------------------------------------------*/
#define LED_COUNT (3)
#define LEDS_PUERTO_GPIOB   GPIOB
#define LED1_GREEN_PIN			GPIO_PIN_0
#define LED2_BLUE_PIN				GPIO_PIN_7
#define LED3_RED_PIN				GPIO_PIN_14

/* Exported functions ------------------------------------------------------- */

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns     No returns
*/
extern void  LED_Initialize (void);

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns     No return
*/
extern void  LED_Uninitialize (void);

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t  LED_On (uint32_t num);

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t  LED_Off (uint32_t num);

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t  LED_SetOut (uint32_t val);

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
extern uint32_t LED_GetCount (void);

/* Exported variables ------------------------------------------------------- */


#endif /* __LED_H */

