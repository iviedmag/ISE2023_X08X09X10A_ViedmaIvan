#ifndef __ADC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"

/* Exported functions ------------------------------------------------------- */

/**
    \fn          int32_t ADC1_Initialize (void)
		\brief       Initialize Analog-to-Digital Converter
    \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t ADC1_Initialize(void);

/**
    \fn          int32_t ADC2_Initialize (void)
		\brief       Initialize Analog-to-Digital Converter
    \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t ADC2_Initialize(void);

/**
  * @brief Initialize the ADC1 to work with single conversions. 12 bits resolution, software start, 1 conversion
  * @param ADC handle
	* @param ADC instance
  * @retval HAL_StatusTypeDef HAL_ADC_Init
  */
extern int ADC1_Init_Single_Conversion(void);

/**
  * @brief Initialize the ADC2 to work with single conversions. 12 bits resolution, software start, 1 conversion
  * @param ADC handle
	* @param ADC instance
  * @retval HAL_StatusTypeDef HAL_ADC_Init
  */
extern int ADC2_Init_Single_Conversion(void);

/**
  \fn          int32_t ADC1_Uninitialize (void)
  \brief       De-initialize Analog-to-Digital Converter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t ADC1_Uninitialize (void);

/**
  \fn          int32_t ADC2_Uninitialize (void)
  \brief       De-initialize Analog-to-Digital Converter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t ADC2_Uninitialize (void);

/**
  \fn          uint32_t ADC1_GetResolution (void)
  \brief       Get resolution of Analog-to-Digital Converter
  \returns     Resolution (in bits)
*/
extern uint32_t ADC1_GetResolution(void);

/**
  \fn          uint32_t ADC2_GetResolution (void)
  \brief       Get resolution of Analog-to-Digital Converter
  \returns     Resolution (in bits)
*/
extern uint32_t ADC2_GetResolution(void);

/**
  * @brief Mide el voltaje en un canal de entrada analógico de un microcontrolador usando un ADC. Configure a specific channels ang gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param Canal que quiero medir. channel number
	* @retval voltage in float (resolution 12 bits and VRFE 3.3)
  */
extern int32_t ADC1_getVoltage(uint32_t Channel);

/**
  * @brief Mide el voltaje en un canal de entrada analógico de un microcontrolador usando un ADC. Configure a specific channels ang gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param Canal que quiero medir. channel number
	* @retval voltage in float (resolution 12 bits and VRFE 3.3)
  */
extern int32_t ADC2_getVoltage(uint32_t Channel);
	
#endif /* __ADC_H */
