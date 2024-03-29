/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "LEDS_STM32F429ZI.h"
#include "LCD_STM32F429ZI.h"
#include "ADC_STM32F429ZI.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
	
// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

extern uint16_t AD1_in (uint32_t ch);
extern void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);


bool LEDrun;
char lcd_text[2][20+1];

extern osThreadId_t TID_Display;
extern osThreadId_t TID_Led;


/* Thread IDs */
osThreadId_t TID_Display;
osThreadId_t TID_Led;

/* Thread declarations */
static void BlinkLed (void *arg);
static void Display  (void *arg);

__NO_RETURN void app_main (void *arg);

                                           
//Read analog inputs 
uint16_t AD1_in (uint32_t ch) {
  int32_t val = 0;
	
	val = ADC1_getVoltage(ch);
  
  return ((uint16_t)val);
}

uint16_t AD2_in (uint32_t ch) {
  int32_t val = 0;
	
	val = ADC2_getVoltage(ch);
  
  return ((uint16_t)val);
}


/* IP address change notification */
//void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

//  (void)if_num;
//  (void)val;
//  (void)len;

//  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
//    /* IP address change, trigger LCD update */
//    osThreadFlagsSet (TID_Display, 0x01);
//  }
//}

static __NO_RETURN void Display (void *arg) {
	
  static char    lcd_buf[24];												   //  b�fer temporal para almacenar una cadena de caracteres antes de mostrarla en la pantalla LCD
	
	(void)arg;
	
  while(1) {
		
		/* ESPERA DE LA SIGNAL 0X01 CUANDO EL NAVEGADOR ACTUALICE LA INFORMACION DEL LCD */
    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);
		
		sprintf (lcd_buf, "%-20s", lcd_text[0]);
	  LCD_WriteSentence(lcd_buf, 1);
			
		sprintf (lcd_buf, "%-20s", lcd_text[1]);
		LCD_WriteSentence(lcd_buf, 2);
			
		LCD_Reset_Buffer();

  }
}

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LEDs on an eval board
 *---------------------------------------------------------------------------*/
static __NO_RETURN void BlinkLed (void *arg) {
const uint8_t led_val[7] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07};
  uint32_t cnt = 0U;

  (void)arg;

  LEDrun = false;

  while(1) {
    /* Every 100 ms */
    if (LEDrun == true) {
      LED_SetOut (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0U;
      }
    }
    osDelay (100);
		
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;
	
	netInitialize ();

  TID_Led     = osThreadNew (BlinkLed, NULL, NULL);
  TID_Display = osThreadNew (Display,  NULL, NULL);

  osThreadExit();
}
