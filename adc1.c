#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
void GPIO_Config(void);
void ADC_Config(void);
void Systick_Config(void);
void SystemClock_Config(void);
void SysTick_Handler(void);
void Error_Handler(void);
void MX_GPIO_Init(void);
ADC_HandleTypeDef myADCtypeDef;
uint32_t myAdcValue;
uint32_t ADC0v=0;
uint32_t ADC1v=85;
uint32_t ADC2v=170;
uint32_t ADC3v=255;
int main(void)
{
	HAL_Init();
	GPIO_Config();
	ADC_Config();
	Systick_Config();
	SystemClock_Config();
	while(1)
	{
		HAL_ADC_Start(&myADCtypeDef);
		if(HAL_ADC_PollForConversion(&myADCtypeDef,5)==HAL_OK)
		{
			myAdcValue=HAL_ADC_GetValue(&myADCtypeDef);
		}
		HAL_ADC_Stop(&myADCtypeDef);
		if((myAdcValue>=ADC0v)&&(myAdcValue<=ADC1v))
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
		}
		else if((myAdcValue>ADC1v)&&(myAdcValue<=ADC2v))
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
		}
		else if((myAdcValue>ADC2v)&&(myAdcValue<=ADC3v))
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
		}
		//HAL_Delay(100);
	}
}
void GPIO_Config(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef myADC;
	myADC.Pin=GPIO_PIN_0;
	myADC.Mode=GPIO_MODE_ANALOG;
	myADC.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&myADC);
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}
void ADC_Config(void)
{
	__HAL_RCC_ADC1_CLK_ENABLE();
	myADCtypeDef.Instance=ADC1;
	myADCtypeDef.Init.Resolution=ADC_RESOLUTION8b;
	myADCtypeDef.Init.EOCSelection=ADC_EOC_SINGLE_CONV;
	myADCtypeDef.Init.DataAlign=ADC_DATAALIGN_RIGHT;
	HAL_ADC_Init(&myADCtypeDef);
	ADC_ChannelConfTypeDef myADCchannelTypeDef;
	myADCchannelTypeDef.Channel=ADC_CHANNEL_0;
	myADCchannelTypeDef.Rank=1;
	myADCchannelTypeDef.SamplingTime=ADC_SAMPLETIME_480CYCLES;
	HAL_ADC_ConfigChannel(&myADCtypeDef,&myADCchannelTypeDef);
}
void Systick_Config(void)
{
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
	HAL_NVIC_EnableIRQ(SysTick_IRQn);
}
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
void Error_Handler(void)
{
;
}
void SysTick_Handler(void)
{
	HAL_IncTick();
}
