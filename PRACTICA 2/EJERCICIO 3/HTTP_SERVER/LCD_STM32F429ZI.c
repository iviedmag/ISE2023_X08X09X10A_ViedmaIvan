/*----------------------------------------------------------------------------
 *      Funciones LCD
 *---------------------------------------------------------------------------*/
 
 #include "LCD_STM32F429ZI.h"
 #include "Arial12x12.h"
 
/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct_LCD;
TIM_HandleTypeDef TIM7_Handler;

/* Manejadores ---------------------------------------------------------------*/
extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;

/* Private variables ---------------------------------------------------------*/
unsigned char buffer[512];     // Un array de bytes de al menos 512 elementos, y contiene los datos que se van a escribir en la pantalla. Cada elemento del array representa un píxel en la pantalla.
uint32_t positionL1 = 0;       // Posicion de la para escribir en la Linea 1
uint32_t positionL2 = 256;     // Posicion para escribir en la Linea 2

/* Private functions ---------------------------------------------------------*/

/**
  \fn          void LCD_Initialize_ZI (void)
  \brief       Initializa el LCD
  \returns     No returns
*/
void LCD_Initialize(void)
{
	
	/* Configuración SPI */
	SPIdrv -> Initialize(NULL);																																																										// Inicializa la conexión SPI del microcontrolador.																																			
	SPIdrv -> PowerControl(ARM_POWER_FULL);																																																				// Esta función establece la potencia del dispositivo SPI en "completa"
	SPIdrv -> Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_UNUSED | ARM_SPI_DATA_BITS(8),1000000);		  // Configura los parámetros de la conexión SPI, incluyendo el modo de operación (master), el tipo de señalización (CPOL1_CPHA1), el orden de transmisión de bits (MSB_LSB) y la cantidad de bits por transacción (8). El segundo argumento establece la velocidad de la conexión SPI en un millón de bits por segundo (1 Mbps).
	
	/* Configuracion de la señal RESET como salida*/
	__HAL_RCC_GPIOA_CLK_ENABLE();																	// Macro que se utiliza para habilitar el reloj del puerto GPIOA en el microcontrolador. El reloj es necesario para que el microcontrolador pueda acceder al puerto GPIOA
	GPIO_InitStruct_LCD.Pin = LCD_RESET_PIN;
	GPIO_InitStruct_LCD.Mode = GPIO_MODE_OUTPUT_PP;								// Esta sentencia configura el modo de operación del pin del puerto GPIOA para que sea una salida en modo push-pull. Esto significa que el pin puede proporcionar una salida de voltaje alta o baja.
	GPIO_InitStruct_LCD.Pull = GPIO_PULLUP;												// Esta sentencia habilita una resistencia pull-up interna en el pin. Esto significa que cuando el pin está configurado como entrada, la resistencia pull-up interna asegura que el pin esté en un estado lógico alto (con un valor de voltaje de 3.3V en este caso) cuando no hay una entrada externa conectada.
	HAL_GPIO_Init(LCD_RESET_PORT, &GPIO_InitStruct_LCD);
	
	/* Configuracion de la señal CS como salida*/
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct_LCD.Pin = LCD_CS_PIN;
	GPIO_InitStruct_LCD.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_LCD.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(LCD_CS_PORT, &GPIO_InitStruct_LCD);
	
	/* Configuracion de la señal A0 como salida*/
	__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitStruct_LCD.Pin = LCD_A0_PIN;
	GPIO_InitStruct_LCD.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_LCD.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(LCD_A0_PORT, &GPIO_InitStruct_LCD);
	
	/* Inicializamos los pines */
	HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN, GPIO_PIN_SET);			// Establece el pin de reset de la pantalla LCD en un estado alto.
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_SET);					// Establecer el pin de modo de la pantalla LCD en un estado bajo. Esto probablemente indica que los datos que se transmitirán a continuación son comandos, en lugar de datos de pantalla.
	
	LCD_Reset();
	LCD_Initialize_Display();
	
}

/**
  \fn          void LCD_Delay_ZI(uint32_t n_microsegundos)
  \brief       Genera un retraso en microsegundos utilizando el temporizador de hardware TIM7 en un microcontrolador STM32.
  \param[in]   n_microsegundos  Tiempo en microsegundos que queremos que dure el delay
  \returns		 No returns
*/
void LCD_Delay_ZI(uint32_t n_microsegundos)
{
	
	/* Configurar y arrancar el timer para generar un evento pasados n_microsegundos */	

	// Configuracion del temporizador TIM7
	TIM7_Handler.Instance = TIM7;													// Asigna la instancia de TIM7 al manejador de temporizador TIM7_Handler
	TIM7_Handler.Init.Prescaler = 20;											// Configura el divisor del reloj interno del temporizador para obtener una frecuencia de reloj de 1 MHz. Prescaler=84M/21=4Mhz
	TIM7_Handler.Init.Period = (n_microsegundos/4) - 1;				// Configura el valor del contador del temporizador para que genere el retraso deseado en microsegundos.
	
	__HAL_RCC_TIM7_CLK_ENABLE(); 													// Habilita el reloj del temporizador TIM7 en el microcontrolador	
	
	HAL_TIM_Base_Init(&TIM7_Handler);											// Se inicializa el temporizador
	HAL_TIM_Base_Start(&TIM7_Handler);										// Se inicia la cuenta de tiempo
	
	// Espera hasta que el contador del temporizador alcance el valor deseado. Esperar a que se active el flag del registro de Match 
	while(TIM7 -> CNT);
	
	// Se detiene la cuenta de tiempo del temporizador
	HAL_TIM_Base_DeInit(&TIM7_Handler);
	HAL_TIM_Base_Stop(&TIM7_Handler);
}

/**
  \fn          void LCD_Reset(void)
  \brief       Configura la conexión SPI del microcontrolador y envia una secuencia de señales para reiniciar la pantalla LCD conectada.
  \returns     No return
*/
void LCD_Reset(void)
{
	
	/* Apagamos el LCD */
	LCD_Delay_ZI(2);	
	HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN, GPIO_PIN_RESET);
																																								// Esperar 1 milisegundo antes de continuar																					
	/* Encendemos el LCD */
	LCD_Delay_ZI(1000);	
	HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN, GPIO_PIN_SET);
																									
}


/**
  \fn          void LCD_Wr_Data_ZI(unsigned char data)
  \brief       Envia datos a la pantalla LCD
  \param[in]   data  Datos a enviar al LCD
  \returns		 No returns
*/
void LCD_Wr_Data(unsigned char data)
{
	ARM_SPI_STATUS status;																								// Se utiliza para verificar el estado de la transmisión SPI.
	
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);						// Establece el pin CS (Chip Select) de la pantalla LCD en un estado bajo
	HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_SET);							// Establece el pin A0 (Address/Data) de la pantalla LCD en un estado alto. Esto indica que los datos que se transmitirán a continuación son datos de pantalla, en lugar de comandos.
	SPIdrv -> Send(&data, sizeof(data));																	// Utiliza la función Send() del driver de la SPI para enviar el byte de datos data a la pantalla LCD a través de la conexión SPI.
	
	// Esperar a que la transmisión SPI se complete
	do{
		status = SPIdrv -> GetStatus();																			// La función GetStatus() del driver de la SPI se utiliza para verificar el estado de la transmisión
	}while(status.busy);																									// El bucle se ejecuta mientras que la transmisión está en progreso (status.busy es verdadero).
	
	LCD_Delay_ZI(1000);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);							// Establece el pin CS de la pantalla LCD en un estado alto, indicando que la comunicacion ha finalizado
}

/**
  \fn          void LCD_Wr_Cmd_ZI (unsigned char cmd)
  \brief       Escribe un comando en el lcd
  \param[in]   val  value to be displayed on LEDs
  \returns		 No returns
*/
void LCD_Wr_Cmd(unsigned char cmd)
{
	ARM_SPI_STATUS status;
	
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);						// Indicamos que queremos comunicarnos con el LCD. CS = 0
	HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_RESET);						// Indicamos que vamos a mandar comandos. A0 = 0
	SPIdrv -> Send(&cmd, sizeof(cmd));																		// Enviamos el comando
	
	// Esperar a que la transmisión SPI se complete
	do{
		status = SPIdrv -> GetStatus();
	}while(status.busy);																									// delay(1000)
	
	LCD_Delay_ZI(1000);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);							// Finalizamos la comunicación. CS = 1
}

/**
  \fn          void LCD_Initialize_Display (void)
  \brief       Inicializa el display del LCD
  \return      No returns
*/
void LCD_Initialize_Display(void)
{
	LCD_Wr_Cmd(0xAE);  //Comand(1)  -> Display OFF: 0xAE, Display ON: 0xAF. Display off
	LCD_Wr_Cmd(0xA2);  //Comand(11) -> Voltage bias ratio 1/9: 0xA2, Voltage bias ratio 1/7: 0xA3. Polarizacion lcd a 1/9
	LCD_Wr_Cmd(0xA0);  //Comand(8)  -> Display RAM adress NORMAL: 0xA0, REVERSE: 0xA1. Direccionamiento ram
	LCD_Wr_Cmd(0xC8);  //Comand(1)  -> Display OFF: 0xAE, Display ON: 0xAF. Salidas com
	LCD_Wr_Cmd(0x22);  //Comand(17) -> Internar Resistor ratio (Rb/Ra) 0x2n -> n = Rb/Ra = 2 (nuestro caso). Resistencias internas
	LCD_Wr_Cmd(0x2F);  //Comand(16) -> Power control set: 0x2F power on. Power on
	LCD_Wr_Cmd(0x40);  //Comand(2)  -> Display start line: 0x4n -> n: numero de linea = 0 (nuestro caso). Display en la linea 0
	LCD_Wr_Cmd(0xAF);  //Comand(1)  -> Display OFF: 0xAE, Display ON: 0xAF. Dislpay ON. 
	LCD_Wr_Cmd(0x81);  //Comand(18) -> Contrast. Contraste
	LCD_Wr_Cmd(0x13);  //Comand(18) -> Contrasr value: 0x3F maximo contraste. Valor contraste
	LCD_Wr_Cmd(0xA4);  //Comand(10) -> Display all points NORMAL: 0xA4, ALL POINTS ON: 0xA5. Display all points normal
	LCD_Wr_Cmd(0xA6);  //Comand(9)  -> Display NORMAL: 0xA6, REVERSE: 0xA7. LCD display normal
	
}

/**
  \fn          void LCD_Update (void)
  \brief       Actualiza el contenido de un display LCD. La pantalla LCD se divide en cuatro páginas (o "buffers"), cada una de 128 bytes de longitud.
  \return      No returns
*/
void LCD_Update(void)
{
	int i;
	
	// PAGINA 0
	LCD_Wr_Cmd(0x00);										// Ajuste del voltaje de polarización del panel LCD. 4 bits de la parte baja de la dirección a 0
	LCD_Wr_Cmd(0x10);										// Dirección de columna establecida bit superior. 4 bits de la parte alta de la dirección a 0
	LCD_Wr_Cmd(0xB0);										// Establece la direccion de la pagina. Página 0
	for(i = 0; i < 128; i++){
		LCD_Wr_Data(buffer[i]);					
	}
	
	// PAGINA 1
	LCD_Wr_Cmd(0x00);
	LCD_Wr_Cmd(0x10);
	LCD_Wr_Cmd(0xB1);
	for(i = 128; i < 256; i++){
		LCD_Wr_Data(buffer[i]);
	}
	
	// PAGINA 2 
	LCD_Wr_Cmd(0x00);
	LCD_Wr_Cmd(0x10);
	LCD_Wr_Cmd(0xB2);
	for(i = 256; i < 384; i++){
		LCD_Wr_Data(buffer[i]);
	}
	
	// PAGINA 3
	LCD_Wr_Cmd(0x00);
	LCD_Wr_Cmd(0x10);
	LCD_Wr_Cmd(0xB3);
	for(i = 384; i < 512; i++){
		LCD_Wr_Data(buffer[i]);
	}
	
}

/**
  \fn          LCD_SymbolToLocalBuffer_L1(uint8_t symbol)
  \brief       
  \return      No returns
*/
void LCD_SymbolToLocalBuffer_L1(uint8_t symbol)
{
	uint8_t i, value1, value2;
	uint16_t offset = 0;
	
	//Linea1
	offset = 25 * (symbol - ' '); 																// Avanza a la linea donde se encuentra la letra que quiero representar. Todas las letras de arial 12x12 ocupan 25 bytes (1 columna = 1 byte)
	
	/* Vamos a escribir las 12 primeras columnas de la letra */
	
	for(i=0; i<12; i++){
		
		value1 = Arial12x12[offset+i*2+1];													// valor1= arial[1,3,5,7,9,....]
		value2 = Arial12x12[offset+i*2+2];													// valor2= arial[2,4,6,8,10,....]

		buffer[i+positionL1] = value1;															//pagina 0 escribimos los valores impares de arial
		buffer[i+128+positionL1] = value2;													//pagina 1 escribimos los valores pares de arial
	}	
	
	positionL1 = positionL1 + Arial12x12[offset];									// posicionL1 indica la columna donde se debe comenzar a escribir
}


/**
  \fn          LCD_SymbolToLocalBuffer_L1(uint8_t symbol)
  \brief       
  \return      No returns
*/
void LCD_SymbolToLocalBuffer_L2(uint8_t symbol)
{
	uint8_t i, value1, value2;
	uint16_t offset = 0;
	
	offset = 25 * (symbol - ' ');
	
	for(i=0; i<12; i++){
		value1 = Arial12x12[offset+i*2+1];
		value2 = Arial12x12[offset+i*2+2];
		
		buffer[i+positionL2] = value1;
		buffer[i+128+positionL2] = value2;
	}
	positionL2 = positionL2 + Arial12x12[offset];
}


/**
  \fn          LCD_WriteSentence(char *frase, uint8_t line)
  \brief       
  \return      No returns
*/
void LCD_WriteSentence(char *frase, uint8_t line)
{
	uint8_t i;
	
	if(line == 1){
		for(i=0; i<strlen(frase); i++){
			if(positionL1+12 < 127)
				LCD_SymbolToLocalBuffer_L1(frase[i]);
		}
	}
	
	if(line == 2){
		for(i=0; i<strlen(frase); i++){
			if(positionL2+12 < 383)
				LCD_SymbolToLocalBuffer_L2(frase[i]);
		}
	}
	
	positionL1 = 0;
	positionL2 = 256;
	
	LCD_Update();
	
}

/**
  \fn          void LCD_Reset_Buffer(void)
  \brief       Resetea el Buffer
  \return      No returns
*/
void LCD_Reset_Buffer(void)
{
	memset(buffer,0,512);				// Limpia el buffer de memoria
}

/**
  \fn          void LCD_Clean(void)
  \brief       Borrar completamente la pantalla LCD
  \return      No returns
*/
void LCD_Clean(void)
{
	memset(buffer,0,512);				// Limpia el buffer de memoria
	LCD_Update();
}

/**
  * @brief  Muestra en el LCD la fecha y hora 
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void LCD_RTC_Show_DT (uint8_t *showtime, uint8_t *showdate)
{
	static char    lcd_buf[24];

	sprintf (lcd_buf, "FECHA:  %-20s", showdate);
	LCD_WriteSentence (lcd_buf,1);
	
	sprintf (lcd_buf, "HORA:  %-20s", showtime);
	LCD_WriteSentence (lcd_buf,2);

}
