#ifndef __ADC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

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
  \fn          int32_t ADC1_Uninitialize (void)
  \brief       De-initialize Analog-to-Digital Converter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t ADC1_Uninitialize (void);
	
/**
  \fn          int32_t ADC1_StartConversion (void)
  \brief       Start conversion
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
extern int32_t ADC1_StartConversion (void);

/**
  \fn          int32_t ADC1_ConversionDone (void)
  \brief       Check if conversion finished
  \returns
   - \b  0: conversion finished
   - \b -1: conversion in progress
*/
extern int32_t ADC1_ConversionDone (void);

/**
  \fn          int32_t ADC1_GetValue (void)
  \brief       Get converted value
  \returns
   - <b> >=0</b>: converted value
   - \b -1: conversion in progress or failed
*/
extern int32_t ADC1_GetValue (void);

/**
  \fn          uint32_t ADC1_GetResolution (void)
  \brief       Get resolution of Analog-to-Digital Converter
  \returns     Resolution (in bits)
*/
extern uint32_t ADC1_GetResolution (void);

/**
  * @brief Mide el voltaje en un canal de entrada analógico de un microcontrolador usando un ADC. Configure a specific channels ang gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param Canal que quiero medir. channel number
	* @retval voltage in float (resolution 12 bits and VRFE 3.3)
  */
extern float ADC_getVoltage(void);
	
#endif /* __ADC_H */
