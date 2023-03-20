#include "SNTP.h"

int hola = 0;
TIM_HandleTypeDef TIM2_Handler;
struct tm SNTP_time;

const NET_ADDR4 ntp_server = { NET_ADDR_IP4, 0, 185, 179, 104, 7 };
//const uint8_t ntp_server[4] = {130,206,0,1};
static void time_callback (uint32_t seconds, uint32_t seconds_fraction);


void TIM2_IRQHandler(void){
	HAL_TIM_IRQHandler(&TIM2_Handler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
	if(htim->Instance == TIM2){
		get_time();
		RTC_CalendarConfig(1);
	}
}

void TIMER_EVERY_3_MIN(void)
{
	
	/* Configurar y arrancar el timer para generar un evento pasados n_microsegundos */	

	// Configuracion del temporizador TIM7
	TIM2_Handler.Instance = TIM2;							// Asigna la instancia de TIM7 al manejador de temporizador TIM7_Handler
	TIM2_Handler.Init.Prescaler = 83999;			// 84MHZ/48000 = 1000 Configura el divisor del reloj interno del temporizador para obtener una frecuencia de reloj de 1 MHz. Prescaler=84M/21=4Mhz
	TIM2_Handler.Init.Period = 24999;				// 30 SEG 1000/180000 = 0,0055556 Hz = 180s = 3min Configura el valor del contador del temporizador para que genere el retraso deseado en microsegundos.
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	__HAL_RCC_TIM2_CLK_ENABLE(); 													// Habilita el reloj del temporizador TIM7 en el microcontrolador	
	
	HAL_TIM_Base_Init(&TIM2_Handler);											// Se inicializa el temporizador
	HAL_TIM_Base_Start_IT(&TIM2_Handler);										// Se inicia la cuenta de tiempo
	
}

void EXTI15_10_IRQHanler(void){
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET){
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	}
	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	
	if (GPIO_Pin == GPIO_PIN_13){
		LED_On(1);
	}
}

void Boton_Init(void){
  
	GPIO_InitTypeDef GPIO_InitStruct_Boton;
	
  //Habilitamos los puertos C
  __HAL_RCC_GPIOC_CLK_ENABLE();
	
	//Configuramos el pin 13
	GPIO_InitStruct_Boton.Pin = GPIO_PIN_13;
	GPIO_InitStruct_Boton.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct_Boton.Pull = GPIO_PULLDOWN;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_Boton);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void get_time (void) {
  if (netSNTPc_GetTime ((NET_ADDR *)&ntp_server, time_callback) == netOK) {
    //printf ("SNTP request sent.\n");

  }
  else {
    //printf ("SNTP not ready or bad parameters.\n");

  }
}
 
static void time_callback (uint32_t seconds, uint32_t seconds_fraction) {
	
  if (seconds == 0) {
   // printf ("Server not responding or bad response.\n");
		
  }
  else {
    //printf ("%d seconds elapsed since 1.1.1970\n", seconds);
	
    time_t current_time = seconds;
    SNTP_time = *localtime(&current_time);

		LED_SetOut (0x00);	//Apagamos todos los Leds
		
		for(int i=0; i<4;i++){  // Pasa cada 500 ms, debe dar 4 vueltas 
      LED_On(2);
			HAL_Delay(250);
			LED_Off(2);
			HAL_Delay(250);
    }		
  }
}

