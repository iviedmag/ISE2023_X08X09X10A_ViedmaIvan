/*----------------------------------------------------------------------------
 *      Funciones RTC
 *---------------------------------------------------------------------------*/


#include "RTC.h"

/* Private typedef -----------------------------------------------------------*/
RTC_HandleTypeDef hrtc;
extern osThreadId_t TID_Led;

// Rutina de interrupción de la alarma RTC
void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&hrtc);
}

//// Rutina de interrupción del temporizador RTC
//void RTC_WKUP_IRQHandler(void)
//{
//  HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
//}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	osThreadFlagsSet (TID_Led, 0x01);
}

//void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
//{
//  osThreadFlagsSet (TID_Led, 0x01);
//}

/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources 
  * @param hrtc RTC handle pointer
  * 
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *        the RTC clock source; in this case the Backup domain will be reset in  
  *        order to modify the RTC Clock source, as consequence RTC registers (including 
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *             
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTc clock source */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  
  /*##-2- Configure LSE as RTC clock source ###################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    Error_Handler();
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    Error_Handler();
  }
  
  /*##-3- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}

/**
  * @brief RTC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();

  /*##-2- Disables the PWR Clock and Disables access to the backup domain ###################################*/
  HAL_PWR_DisableBkUpAccess();
  __HAL_RCC_PWR_CLK_DISABLE();
  
}

/**
  @brief     RTC_Initialize: Main RTC program body
  @param[in] None
  @return    int
*/
void RTC_Initialize(void)
{
	RTC_AlarmTypeDef sAlarm;
	
	/* Inicialización del RTC */
	
	/*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */ 
	
  hrtc.Instance = RTC; 
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;																		// En este caso, RTC_ASYNCH_PREDIV es igual a 0x7F y RTC_SYNCH_PREDIV es igual a 0x00FF, lo que significa que el reloj RTC utilizará el cristal externo de baja velocidad (LSE) como fuente de reloj y se dividirá por 127 (asincrónico) y 255 (sincrónico) para generar la frecuencia de reloj deseada.
  hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;																			// Estos valores pueden variar dependiendo de la frecuencia del cristal utilizado y las necesidades específicas del sistema.
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  __HAL_RTC_RESET_HANDLE_STATE(&hrtc);																					// Asegurarse de que todas las estructuras internas del RTC están en un estado de reinicio
	
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0);																
	
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Encendemos LED si se ha inicializado el RTC */
  //BSP_LED_On(LED1);

	/*Verifica si hay datos almacenados en el registro de copia de seguridad 1. Si no hay datos almacenados, entonces se configura el calendario del RTC*/
	/*Permite recuperar la última configuración de tiempo y fecha del RTC y tomar medidas apropiadas si se ha producido un restablecimiento del sistema.*/
  /*##-2- Check if Data stored in BackUp register1: No Need to reconfigure RTC#*/
  /* Read the Back Up Register 1 Data */
	
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
  {
    /* Configure RTC Calendar */
    RTC_CalendarConfig();
  }
  else
  {
		/*Si se ha producido un restablecimiento de encendido, el código encenderá el LED2*/
    /* Check if the Power On Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
			/*Si se ha producido un restablecimiento de pin, el código encenderá el LED1*/
      /* Turn on LED2: Power on reset occurred */
      //BSP_LED_On(LED2);
    }
    /* Check if Pin Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      /* Turn on LED1: External reset occurred */
      //BSP_LED_On(LED1);
    }
		/*Se borran las banderas de restablecimiento de fuente*/
    /* Clear source Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }

	// Configurar la alarma
  sAlarm.AlarmTime.Hours = 0x00;					
  sAlarm.AlarmTime.Minutes = 0x00;																																													// Se establece la hora de la alarma para que se active cada minuto
  sAlarm.AlarmTime.Seconds = 0x05;
	sAlarm.AlarmTime.SubSeconds = 0x00;
	sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES;																		// Se establece que la alarma no tiene máscara, lo que significa que se activará en cualquier momento de la hora, minuto y segundo configurados en la alarma
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;																																		// Se establece la máscara de sub-segundo para que la alarma se active en cualquier valor de sub-segundo
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;																																// Se establece que la alarma se activará solo en una fecha (no en un día de la semana) 
  sAlarm.AlarmDateWeekDay = 0x01;																																														// Se establece el valor de la fecha en 0x01 (es decir, el primer día del mes)
  sAlarm.Alarm = RTC_ALARM_A;																																																// Establecemos la Alarma como ALARMA A
  
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)																												// Configurar la alarma en el RTC y habilitar la interrupción de alarma. La función toma tres argumentos: el manejador del RTC (&hrtc), la estructura de alarma (&sAlarm), y el formato de tiempo utilizado (RTC_FORMAT_BIN). El último argumento especifica que el formato de tiempo es binario.
	{
		Error_Handler();
	}								

//	// Configurar el temporizador RTC
//	uint32_t WakeUpCounter = 10 * 32768 - 1; // 10 segundos a 32768 Hz
//	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, WakeUpCounter, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
	
	// Habilitar la interrupción de la alarma A del RTC
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
	
//	// Habilitar la interrupción del temporizador RTC en el NVIC
//	HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}	
	
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  sdatestructure.Year = RTC_YEAR;
  sdatestructure.Month = RTC_MONTH;
  sdatestructure.Date = RTC_DAY;
  sdatestructure.WeekDay = RTC_WEEKDAY;
  
  if(HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the Time #################################################*/
  stimestructure.Hours = RTC_HOURS;
  stimestructure.Minutes = RTC_MINUTES;
  stimestructure.Seconds = RTC_SECONDS;
  stimestructure.TimeFormat = RTC_TIME_FORMAT;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;																	// Establece que no se debe realizar ningún ajuste para el horario de verano. El ajuste del horario de verano implica adelantar o retrasar una hora en el reloj para aprovechar la luz solar durante el verano
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;																	// Significa que cualquier valor anterior almacenado en la estructura se restablecerá antes de almacenar el nuevo valor de tiempo.

  if (HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	
	/*El registro de respaldo se utiliza para almacenar datos que deben mantenerse incluso si la fuente de alimentación se interrumpe, como el estado de una aplicación o la última vez que se ejecutó una tarea.
  	Al escribir este valor en el registro de respaldo, se asegura que se mantendrá incluso después de que se pierda la alimentación.*/
  /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
}


/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
	
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
	
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "%02d:%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
	
  /* Display date Format : mm-dd-yy */
  sprintf((char *)showdate, "%02d-%02d-%02d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
}


