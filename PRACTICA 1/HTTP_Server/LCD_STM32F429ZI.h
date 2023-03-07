/*Este archivo define constantes, macros y funciones para manejar LEDs RGB en un microcontrolador STM32F4xx.*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "RTE_Device.h"
#include "Driver_SPI.h"
#include "string.h"

/* Private define ------------------------------------------------------------*/
#define LCD_RESET_PORT     GPIOA
#define LCD_CS_PORT        GPIOD
#define LCD_A0_PORT        GPIOF
#define LCD_RESET_PIN      GPIO_PIN_6
#define LCD_CS_PIN         GPIO_PIN_14
#define LCD_A0_PIN         GPIO_PIN_13


/* Exported functions ------------------------------------------------------- */

/**
  \fn          void LCD_Initialize (void)
  \brief       Initialize LCD
  \returns     No returns
*/
extern void LCD_Initialize(void);

/**
  \fn          void LCD_Delay_ZI(uint32_t n_microsegundos)
  \brief       Genera un retraso en microsegundos utilizando el temporizador de hardware TIM7 en un microcontrolador STM32.
  \param[in]   n_microsegundos  Tiempo en microsegundos que queremos que dure el delay
  \returns		 No returns
*/
extern void LCD_Delay(uint32_t n_microsegundos);

/**
  \fn          void LCD_Reset_ZI (void)
  \brief       Configura la conexión SPI del microcontrolador y envia una secuencia de señales para reiniciar la pantalla LCD conectada.
  \returns     No return
*/
extern void LCD_Reset(void);

/**
  \fn          void LCD_Wr_Data_ZI(unsigned char data)
  \brief       Envia datos a la pantalla LCD
  \param[in]   data  Datos a enviar al LCD
  \returns		 No returns
*/
extern void LCD_Wr_Data(unsigned char data);

/**
  \fn          void LCD_Wr_Cmd_ZI (unsigned char cmd)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns		 No returns
*/
extern void LCD_Wr_Cmd(unsigned char cmd);

/**
  \fn          void LCD_Initialize_Display (void)
  \brief       Inicializa el display del LCD
  \return      No returns
*/
extern void LCD_Initialize_Display(void);

/**
  \fn          void LCD_Update(void)
  \brief       Actualiza el contenido de un display LCD con los datos del Buffer. La pantalla LCD se divide en cuatro páginas (o "buffers"), cada una de 128 bytes de longitud.
  \return      No returns
*/
extern void LCD_Update(void);


/**
  \fn          LCD_WriteSentence(char *frase, uint8_t line)
  \brief       
  \return      No returns
*/
extern void LCD_WriteSentence(char *frase, uint8_t line);

/**
  \fn          LCD_WriteCrono(char *frase)
  \brief       
  \return      No returns
*/
extern void LCD_WriteCrono(char *frase);
 
/**
  \fn          void LCD_Reset_Buffer(void)
  \brief       Resetea el Buffer
  \return      No returns
*/
void LCD_Reset_Buffer(void);
	
/**
  \fn          void LCD_Clean(void)
  \brief       Borrar completamente la pantalla LCD
  \return      No returns
*/
extern void LCD_Clean(void);


#endif /* __LCD_H */

