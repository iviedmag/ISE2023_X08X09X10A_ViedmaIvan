#include "ADC_STM32F429ZI.h"
 
/* Private define ------------------------------------------------------------*/
#define RESOLUTION_12B 4096U
#define VREF 3.3f

/* Private typedef -----------------------------------------------------------*/
static ADC_HandleTypeDef hadc1;
static ADC_HandleTypeDef hadc2;

/* Private variables ---------------------------------------------------------*/
//static volatile uint8_t  AD_done;       /* AD conversion done flag          */

/* Private functions ---------------------------------------------------------*/


/** CANALES ADCs 

ADC1:

Canal 0: ?
Canal 1: ?
Canal 2: ?
Canal 3: PA3 
Canal 4: ?
Canal 5: ?
Canal 6: ?
Canal 7: ?
Canal 8: ?
Canal 9: PB1 
Canal 10: PC0 
Canal 11: ?
Canal 12: PC2 
Canal 13: PC3 
Canal 14: ?
Canal 15: ?

ADC2:

Canal 0: ?
Canal 1: ?
Canal 2: ?
Canal 3: PA3 
Canal 4: ?
Canal 5: ?
Canal 6: ?
Canal 7: ?
Canal 8: ?
Canal 9: PB1 
Canal 10: PC0 
Canal 11: ?
Canal 12: PC2 
Canal 13: PC3 
Canal 14: ?
Canal 15: ?

ADC3:

Canal 0: ?
Canal 1: ?
Canal 2: ?
Canal 3: PA3 
Canal 4: ?
Canal 5: ?
Canal 6: ?
Canal 7: ?
Canal 8: PF10 
Canal 9: PF3 
Canal 10: PC0 
Canal 11: ?
Canal 12: PC2 
Canal 13: PC3 
Canal 14: PF4 
Canal 15: PF5 

**/

/**
    \fn          int32_t ADC1_Initialize (void)
		\brief       Initialize Analog-to-Digital Converter
    \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC1_Initialize(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct_ADC1;																// Estructura utilizada para inicializar la configuración de los pines de entrada
	
	__HAL_RCC_ADC1_CLK_ENABLE();																				// Macro que se utiliza para habilitar el reloj del ADC1
	__HAL_RCC_GPIOC_CLK_ENABLE();																				// Macro que se utiliza para habilitar el reloj del puerto GPIOC en el microcontrolador. El reloj es necesario para que el microcontrolador pueda acceder al puerto GPIOC
		
	/*Configuran los pines PC0 y PC3 como entradas analógicas. PC0 --> ADC1_IN10, PC3 --> ADC1_IN13 */
	
  GPIO_InitStruct_ADC1.Pin = GPIO_PIN_0;
  GPIO_InitStruct_ADC1.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct_ADC1.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_ADC1);

  GPIO_InitStruct_ADC1.Pin = GPIO_PIN_3;
  GPIO_InitStruct_ADC1.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct_ADC1.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_ADC1);
	
	return 0;
}

/**
    \fn          int32_t ADC1_Initialize (void)
		\brief       Initialize Analog-to-Digital Converter
    \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC2_Initialize(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct_ADC2;																// Estructura utilizada para inicializar la configuración de los pines de entrada
	
	__HAL_RCC_ADC2_CLK_ENABLE();																				// Macro que se utiliza para habilitar el reloj del ADC1
	__HAL_RCC_GPIOA_CLK_ENABLE();																				// Macro que se utiliza para habilitar el reloj del puerto GPIOC en el microcontrolador. El reloj es necesario para que el microcontrolador pueda acceder al puerto GPIOC
		
	/*Configuran los pines PC0 y PC3 como entradas analógicas. PC0 --> ADC2_IN10, PA3 --> ADC2_IN3 */
	
//  GPIO_InitStruct_ADC2.Pin = GPIO_PIN_0;
//  GPIO_InitStruct_ADC2.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct_ADC2.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_ADC2);

  GPIO_InitStruct_ADC2.Pin = GPIO_PIN_3;
  GPIO_InitStruct_ADC2.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct_ADC2.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_ADC2);
	
	return 0;
}

/**
  * @brief Initialize the ADC1 to work with single conversions. 12 bits resolution, software start, 1 conversion
  * @param ADC handle
	* @param ADC instance
  * @retval HAL_StatusTypeDef HAL_ADC_Init
  */
int ADC1_Init_Single_Conversion()
{
	
   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)*/
	
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
	
  if (HAL_ADC_Init(&hadc1) != HAL_OK)																	// Inicializa el ADC con los parámetros establecidos en la estructura de inicialización del ADC
  {
    return -1;
  }
 
	return 0;

}

/**
  * @brief Initialize the ADC1 to work with single conversions. 12 bits resolution, software start, 1 conversion
  * @param ADC handle
	* @param ADC instance
  * @retval HAL_StatusTypeDef HAL_ADC_Init
  */
int ADC2_Init_Single_Conversion(void)
{
	
   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)*/
	
  hadc2.Instance = ADC2;																							// Establece la instancia del ADC que se va a utilizar, es decir, el registro del microcontrolador al que está conectado el ADC.
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;								// Establece la frecuencia de reloj del ADC. En este caso, se utiliza la mitad de la frecuencia del reloj del bus del sistema.
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;													// Establece la resolución de conversión del ADC. En este caso, se utiliza una resolución de 12 bits.
  hadc2.Init.ScanConvMode = DISABLE;																	// Establece el modo de conversión. En este caso, se utiliza el modo de conversión simple, es decir, solo se convierte un canal de entrada analógico.
  hadc2.Init.ContinuousConvMode = DISABLE;														// Desactiva el modo de conversión continua, lo que significa que el ADC solo realizará una conversión por cada llamada.
  hadc2.Init.DiscontinuousConvMode = DISABLE;													// Desactiva el modo de conversión discontinua, lo que significa que el ADC no realizará conversiones en canales diferentes al del que se está convirtiendo.
	hadc2.Init.NbrOfDiscConversion = 1;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;		// Establece el modo de disparo externo de la conversión. En este caso, no se utiliza ningún disparo externo.
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;										// Establece el modo de disparo interno de la conversión. En este caso, se utiliza el modo de disparo por software, lo que significa que la conversión se iniciará mediante una llamada al método HAL_ADC_Start()
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;													// Establece la alineación de los datos de salida del ADC. En este caso, los datos se alinearán a la derecha.
  hadc2.Init.NbrOfConversion = 1;																			// Establece el número de conversiones que se realizarán. En este caso, se realizará solo una conversión por llamada.
  hadc2.Init.DMAContinuousRequests = DISABLE;													// Desactiva las solicitudes continuas de transferencia de datos utilizando DMA.
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;											// Establece el modo de finalización de la conversión. En este caso, se utiliza el modo de finalización de conversión simple, lo que significa que la conversión finalizará después de la conversión del único canal.
	
  if (HAL_ADC_Init(&hadc2) != HAL_OK)																	// Inicializa el ADC con los parámetros establecidos en la estructura de inicialización del ADC
  {
    return -1;
  }
 
	return 0;

}

/**
  * @brief Configure a specific channels ang gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param channel number
	* @retval voltage in float (resolution 12 bits and VRFE 3.3
  */
int32_t ADC1_getVoltage(uint32_t Channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};																			// Definicion de estructura e inicialización a 0
	HAL_StatusTypeDef status;

	uint32_t raw = 0;
		
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.*/
  sConfig.Channel = Channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    return -1;
  }
		
	HAL_ADC_Start(&hadc1);
		
	do {
		status = HAL_ADC_PollForConversion(&hadc1, 0); //This funtions uses the HAL_GetTick(), then it only can be executed wehn the OS is running
	} while(status != HAL_OK);
		
	raw = (int32_t)HAL_ADC_GetValue(&hadc1);
		
	return raw;
}

/**
  * @brief Configure a specific channels ang gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param channel number
	* @retval voltage in float (resolution 12 bits and VRFE 3.3
  */
int32_t ADC2_getVoltage(uint32_t Channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};																			// Definicion de estructura e inicialización a 0
	HAL_StatusTypeDef status;

	uint32_t raw = 0;
		
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.*/
  sConfig.Channel = Channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    return -1;
  }
		
	HAL_ADC_Start(&hadc2);
		
	do {
		status = HAL_ADC_PollForConversion(&hadc2, 0); //This funtions uses the HAL_GetTick(), then it only can be executed wehn the OS is running
	} while(status != HAL_OK);
		
	raw = (int32_t)HAL_ADC_GetValue(&hadc2);
		
	return raw;
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
	
	/* ADC3 GPIO Configuration: PC3 --> ADC1_IN13 */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);

  return 0;
}

/**
  \fn          int32_t ADC2_Uninitialize (void)
  \brief       De-initialize Analog-to-Digital Converter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC2_Uninitialize (void) {

  /* Peripheral clock disable */
  __HAL_RCC_ADC2_CLK_DISABLE();

  /* ADC3 GPIO Configuration: PC0 -> ADC1_IN10 */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
	
	/* ADC3 GPIO Configuration: PC3 --> ADC1_IN13 */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);

  return 0;
}

/**
  \fn          uint32_t ADC1_GetResolution (void)
  \brief       Get resolution of Analog-to-Digital Converter
  \returns     Resolution (in bits)
*/
uint32_t ADC1_GetResolution (void) {
  return RESOLUTION_12B;
}

/**
  \fn          uint32_t ADC2_GetResolution (void)
  \brief       Get resolution of Analog-to-Digital Converter
  \returns     Resolution (in bits)
*/
uint32_t ADC2_GetResolution (void) {
  return RESOLUTION_12B;
}



/* Funciones Antiguas */

///**
//  \fn          int32_t ADC1_StartConversion (void)
//  \brief       Start conversion
//  \returns
//   - \b  0: function succeeded
//   - \b -1: function failed
//*/
//int32_t ADC1_StartConversion (void) {
//	
//  __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_EOC);
//  HAL_ADC_Start(&hadc1);

//  AD_done = 0;

//  return 0;
//}

///**
//  \fn          int32_t ADC1_ConversionDone (void)
//  \brief       Check if conversion finished
//  \returns
//   - \b  0: conversion finished
//   - \b -1: conversion in progress
//*/
//int32_t ADC1_ConversionDone (void) {
//  HAL_StatusTypeDef status;

//  status = HAL_ADC_PollForConversion(&hadc1, 0);
//	
//  if (status == HAL_OK) {
//    AD_done = 1;
//    return 0;
//  } else {
//    AD_done = 0;
//    return -1;
//  }
//}


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
