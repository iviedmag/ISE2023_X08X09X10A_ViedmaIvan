#include "main.h"
#include "time.h"
#include "RTC.h"
#include "LEDS_STM32F429ZI.h" 
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <rl_net_ds.h>

/************************************************
							Defines
************************************************/

/************************************************
							Structures
************************************************/

///************************************************
//							Functions
//************************************************/

extern void get_time (void);
extern void TIMER_EVERY_3_MIN(void);
extern void Boton_Init(void);
