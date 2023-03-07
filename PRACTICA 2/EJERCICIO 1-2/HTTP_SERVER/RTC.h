/*------------------------------------------------------------------------------
 * PRACTICA 2 - INGENIERIA DE SISTEMAS ELECTRONICOS 
 * PROFESOR: Manuel
 *------------------------------------------------------------------------------
 * Name:    RTC.c
 * Purpose: 
 *----------------------------------------------------------------------------
 * AUTORES: IVAN VIEDMA GONZALEZ
 *----------------------------------------------------------------------------*/
 
 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

 /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "LCD_STM32F429ZI.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/

/* Defines related to Clock configuration */
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */

 /* Defines related Time and Date configuration */
#define RTC_YEAR 0x23
#define RTC_MONTH RTC_MONTH_MARCH
#define RTC_WEEKDAY RTC_WEEKDAY_TUESDAY

#define RTC_DAY 0x07
#define RTC_HOURS 0x04
#define RTC_MINUTES 0x00
#define RTC_SECONDS 0x00

#define RTC_TIME_FORMAT RTC_HOURFORMAT12_AM

/* Exported functions ------------------------------------------------------- */
	
/**
  @brief     RTC_Initialize: Main RTC program body
  @param[in] None
  @return    int
*/
extern void RTC_Initialize(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_CalendarConfig(void);

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
extern void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);

#endif /* __RTC_H */
