/*----------------------------------------------------------------------------
 *      Funciones LEDS
 *---------------------------------------------------------------------------*/
 
 #include "LEDS_STM32F429ZI.h"
 
/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct_LEDS;

/* Private constants ---------------------------------------------------------*/
const uint16_t LEDS_PIN[] = {LED1_GREEN_PIN,LED2_BLUE_PIN,LED3_RED_PIN}; 

/* Private functions ---------------------------------------------------------*/

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns     No returns
*/
void LED_Initialize (void) {
	
  //Habilitamos el puerto B 
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//Configuramos el pin 0,pin 7 y pin 14 
	GPIO_InitStruct_LEDS.Pin = (LED1_GREEN_PIN  | LED2_BLUE_PIN | LED3_RED_PIN);
	GPIO_InitStruct_LEDS.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_LEDS.Pull = GPIO_PULLUP;
	GPIO_InitStruct_LEDS.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_LEDS);

}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns     No return
*/
void LED_Uninitialize (void) {
 
}


/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_On (uint32_t num) {
	
  int32_t retCode = 0;
	
  if (num < LED_COUNT) {
		HAL_GPIO_WritePin(GPIOB,LEDS_PIN[num],GPIO_PIN_SET);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    HAL_GPIO_WritePin(GPIOB,LEDS_PIN[num],GPIO_PIN_RESET);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut (uint32_t val) {
  uint32_t n;

  for (n = 0; n < LED_COUNT; n++) {
    if (val & (1 << n)) LED_On (n);
    else                LED_Off(n);
  }

  return 0;
}

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t LED_GetCount (void) {

  return LED_COUNT;
}

