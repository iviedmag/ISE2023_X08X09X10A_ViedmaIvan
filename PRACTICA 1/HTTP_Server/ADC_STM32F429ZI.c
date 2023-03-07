#include "stm32f4xx_hal.h"

/* Private define ------------------------------------------------------------*/
#define RESOLUTION_12B 4096U
#define VREF 3.3f

/* Private typedef -----------------------------------------------------------*/
static ADC_HandleTypeDef hadc1;

/* Private variables ---------------------------------------------------------*/
static volatile uint8_t  AD_done;       /* AD conversion done flag            */

/* Private functions ---------------------------------------------------------*/

/**
    \fn          int32_t ADC1_Initialize (void)
		\brief       Initialize Analog-to-Digital Converter
    \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC1_Initialize(void)
{
	
	ADC_ChannelConfTypeDef sConfig;																			// Definicion de estructura e inicialización a 0
	GPIO_InitTypeDef GPIO_InitStruct_ADC;																// Estructura utilizada para inicializar la configuración de los pines de entrada
	
	__HAL_RCC_ADC1_CLK_ENABLE();																				// Macro que se utiliza para habilitar el reloj del ADC1
	__HAL_RCC_GPIOC_CLK_ENABLE();																				// Macro que se utiliza para habilitar el reloj del puerto GPIOC en el microcontrolador. El reloj es necesario para que el microcontrolador pueda acceder al puerto GPIOC
		
	/*Configuran los pines PC0 y PC3 como entradas analógicas. PC0 --> ADC1_IN10, PC3 --> ADC1_IN13 */
	
  GPIO_InitStruct_ADC.Pin = GPIO_PIN_0;
  GPIO_InitStruct_ADC.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct_ADC.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_ADC);

//  GPIO_InitStruct_ADC.Pin = GPIO_PIN_3;
//  GPIO_InitStruct_ADC.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct_ADC.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_ADC);
	
	
   /** Configure the global features of the ADC1 (Clock, Resolution, Data Alignment and number of conversion) */
 
	hadc1.Instance = ADC1;																							// Establece la instancia del ADC que se va a utilizar, es decir, el registro del microcontrolador al que está conectado el ADC.
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;								// Establece la frecuencia de reloj del ADC. En este caso, se utiliza la mitad de la frecuencia del reloj del bus del sistema.
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;													// Establece la resolución de conversión del ADC. En este caso, se utiliza una resolución de 12 bits.
  hadc1.Init.ScanConvMode = DISABLE;																	// Establece el modo de conversión. En este caso, se utiliza el modo de conversión simple, es decir, solo se convierte un canal de entrada analógico.
  hadc1.Init.ContinuousConvMode = DISABLE;														// Desactiva el modo de conversión continua, lo que significa que el ADC solo realizará una conversión por cada llamada.
  hadc1.Init.DiscontinuousConvMode = DISABLE;													// Desactiva el modo de conversión discontinua, lo que significa que el ADC no realizará conversiones en canales diferentes al del que se está convirtiendo.
	hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;		// Establece el modo de disparo externo de la conversión. En este caso, no se utiliza ningún disparo externo.
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;										// Establece el modo de disparo interno de la conversión. En este caso, se utiliza el modo de disparo por software, lo que significa que la conversión se iniciará mediante una llamada al método HAL_ADC_Start()
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;													// Establece la alineación de los datos de salida del ADC. En este caso, los datos se alinearán a la derecha.
  hadc1.Init.NbrOfConversion = 1;																			// Establece el número de conversiones que se realizarán. En este caso, se realizará solo una conversión por llamada.
  hadc1.Init.DMAContinuousRequests = DISABLE;													// Desactiva las solicitudes continuas de transferencia de datos utilizando DMA.
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;											// Establece el modo de finalización de la conversión. En este caso, se utiliza el modo de finalización de conversión simple, lo que significa que la conversión finalizará después de la conversión del único canal.
	HAL_ADC_Init(&hadc1);																								// Inicializa el ADC con los parámetros establecidos en la estructura de inicialización del ADC
	
	/* Configure the selected ADC channel */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  AD_done = 0;
 
	return 0;

}

/**
  \fn          int32_t ADC1_Uninitialize (void)
  \brief       De-initialize Analog-to-Digital Converter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC1_Uninitialize (void) {

  /* Peripheral clock disable */
  __HAL_RCC_ADC1_CLK_DISABLE();

  /* ADC3 GPIO Configuration: PC0 -> ADC1_IN10 */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);

  return 0;
}

/**
  \fn          int32_t ADC1_StartConversion (void)
  \brief       Start conversion
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC1_StartConversion (void) {
	
  __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_EOC);
  HAL_ADC_Start(&hadc1);

  AD_done = 0;

  return 0;
}

/**
  \fn          int32_t ADC1_ConversionDone (void)
  \brief       Check if conversion finished
  \returns
   - \b  0: conversion finished
   - \b -1: conversion in progress
*/
int32_t ADC1_ConversionDone (void) {
  HAL_StatusTypeDef status;

  status = HAL_ADC_PollForConversion(&hadc1, 0);
	
  if (status == HAL_OK) {
    AD_done = 1;
    return 0;
  } else {
    AD_done = 0;
    return -1;
  }
}

/**
  \fn          int32_t ADC1_GetValue (void)
  \brief       Get converted value
  \returns
   - <b> >=0</b>: converted value
   - \b -1: conversion in progress or failed
*/
int32_t ADC1_GetValue (void) {
  HAL_StatusTypeDef status;
  int32_t val;

  if (AD_done == 0) {
    status = HAL_ADC_PollForConversion(&hadc1, 0);											// Espera a que la conversión ADC se complete. This funtions uses the HAL_GetTick(), then it only can be executed wehn the OS is running
    if (status != HAL_OK) return -1;
  } else {
    AD_done = 0;
  }

  val = (int32_t)HAL_ADC_GetValue(&hadc1);															// Obtiene el valor digital de la conversión ADC

  return val;
}

/**
  \fn          uint32_t ADC1_GetResolution (void)
  \brief       Get resolution of Analog-to-Digital Converter
  \returns     Resolution (in bits)
*/
uint32_t ADC1_GetResolution (void) {
  return RESOLUTION_12B;
}


/* Funciones Antiguas */

/**
  * @brief Mide el voltaje en un canal de entrada analógico de un microcontrolador usando un ADC. Configure a specific channels ang gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param Canal que quiero medir. channel number
	* @retval voltage in float (resolution 12 bits and VRFE 3.3)
  */
float ADC_getVoltage(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};						// Definicion de estructura e inicialización a 0
	HAL_StatusTypeDef status;

	uint32_t raw = 0;
	float voltage = 0;
		
	/* Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.*/
		
  sConfig.Channel = ADC_CHANNEL_10;																			// Configura el canal que se va a medir en la estructura sConfig a través del argumento Channel.
  sConfig.Rank = 1;																								// Configura el rango de medición en la estructura
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;									// Configura el tiempo de muestreo en la estructura
		
	/*Configurar el canal del ADC*/
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    return -1;
  }
		
	HAL_ADC_Start(&hadc1);																						// Inicia la conversión ADC
		
	do {
		status = HAL_ADC_PollForConversion(&hadc1, 0); 									//Espera a que la conversión ADC se complete. This funtions uses the HAL_GetTick(), then it only can be executed wehn the OS is running
	}while(status != HAL_OK);
		
	raw = HAL_ADC_GetValue(&hadc1);																		// Obtiene el valor digital de la conversión ADC
		
	voltage = raw*VREF/RESOLUTION_12B; 
		
	return voltage;
}


///* Example of using this code from a Thread 

//void Thread (void *argument) {
//	
//  ADC_HandleTypeDef adchandle; //handler definition
//	ADC1_Initialize_GPIO(); //specific PINS configuration
//	
//	float value;
//	ADC_Init_Single_Conversion(&adchandle , ADC1); //ADC1 configuration
//	
//  while (1) {
//    
//		value=ADC_getVoltage(&adchandle , 10 ); //get values from channel 10->ADC123_IN10
//		value=ADC_getVoltage(&adchandle , 13 );
//		osDelay(1000);
//   
//  }
//}
//*/
