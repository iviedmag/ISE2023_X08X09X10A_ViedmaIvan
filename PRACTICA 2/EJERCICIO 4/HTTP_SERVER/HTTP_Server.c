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
#include "RTC.h"
#include "SNTP.h"
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
extern uint16_t AD2_in (uint32_t ch);
extern void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);
//extern uint8_t get_button (void);


bool LEDrun;
char lcd_text[2][20+1];

/* Buffers used for displaying Time and Date */
uint8_t RTC_Time[60] = {0};
uint8_t RTC_Date[60] = {0};

extern osThreadId_t TID_Display;
extern osThreadId_t TID_Led;
extern osThreadId_t TID_RTC;
extern osThreadId_t TID_RTC_LED;


/* Thread IDs */
osThreadId_t TID_Display;
osThreadId_t TID_Led;
osThreadId_t TID_RTC;
osThreadId_t TID_RTC_LED;

/* Thread declarations */
static void BlinkLed (void *arg);
//static void Display  (void *arg);
static void RTC_Update (void *arg);
static void RTC_Led (void *arg);

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


/* Read digital inputs 
uint8_t get_button (void) {
  return ((uint8_t)Buttons_GetState ());
}
*/

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

/*Hilo  RTC */
static __NO_RETURN void RTC_Update (void *arg) {
	
	(void)arg;
	
  RTC_CalendarConfig(0);

		
  while(1) {
		
		RTC_CalendarShow(RTC_Time,RTC_Date);
		LCD_RTC_Show_DT(RTC_Time,RTC_Date);
		LCD_Reset_Buffer();
		HAL_Delay (100);	

  }
}


/*Leds para el RTC */
static __NO_RETURN void RTC_Led (void *arg) {

  (void)arg;
	bool prev_LEDrun = false;
	
  while(1) {
		
		// ESPERA DE LA SIGNAL 0X01 CUANDO EL RTC CUENTE CADA MINUTO
    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);
		
		// Si Blinked estaba activo entonces mando que se detenga 
		if (LEDrun == true){
			prev_LEDrun = true;
			LEDrun = false;
		}
		
		LED_SetOut (0x00);	//Apagamos todos los Leds
		
		for(int i=0; i<5;i++){  // Pasa cada 500 ms, debe dar 10 vueltas
      LED_On(0);
			HAL_Delay(1000);
			LED_Off(0);
			HAL_Delay(1000);
    }
		
		// Si Blinked previamente estaba activo entonces mando que se vuelva a activar
		if (prev_LEDrun == true){
			prev_LEDrun = false;
			LEDrun = true;
		}
		
  }
}

/*Display para el servidor*/
//static __NO_RETURN void Display (void *arg) {
//	
//  static char    lcd_buf[24];												   //  búfer temporal para almacenar una cadena de caracteres antes de mostrarla en la pantalla LCD
//	
//	(void)arg;
//	
//	//osThreadFlagsSet (TID_Display, 0x01);
//	LCD_Clean();
//	LCD_WriteSentence ("    BIENVENIDO    ",1);
//	LCD_WriteSentence ("  HTTP SERVER: STM  ",2);
//	LCD_Reset_Buffer();
//	HAL_Delay(5000);
//  LCD_Clean();
//	
//  while(1) {
//		
//		// ESPERA DE LA SIGNAL 0X01 CUANDO EL NAVEGADOR ACTUALICE LA INFORMACION DEL LCD 
//    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);
//		
//		sprintf (lcd_buf, "%-20s", lcd_text[0]);
//	  LCD_WriteSentence(lcd_buf, 1);
//			
//		sprintf (lcd_buf, "%-20s", lcd_text[1]);
//		LCD_WriteSentence(lcd_buf, 2);
//			
//		LCD_Reset_Buffer();

//  }
//}

/*Leds para el servidor*/
static __NO_RETURN void BlinkLed (void *arg) {
const uint8_t led_val[7] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07};
  uint32_t cnt = 0U;

  (void)arg;

  LEDrun = false;
	
  while(1) {
		
		
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
	
	LCD_Clean();
	netInitialize ();
	HAL_Delay(5000);
	get_time();																								//Sincronizamos SNTP
	HAL_Delay(5000);
	RTC_Initialize();
	TIMER_EVERY_3_MIN();                                			// Llamamos al timer para que se sincronice cada 3 minutos
	Boton_Init();
	
  TID_Led     = osThreadNew (BlinkLed, NULL, NULL);
  //TID_Display = osThreadNew (Display,  NULL, NULL);
	TID_RTC   = osThreadNew (RTC_Update, NULL, NULL);
	TID_RTC_LED    = osThreadNew (RTC_Led, NULL, NULL);

  osThreadExit();
}
