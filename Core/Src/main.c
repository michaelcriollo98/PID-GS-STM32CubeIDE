/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "process_signal.h"
#include "pid.h"
#include "state_space.h"
#include "fgs_control.h"
#include "simulated_annealing.h"
#include "A_star.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Ts 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
float distancia1 , distancia2;									  // Distance

uint32_t IC1_Val1 = 0, IC2_Val1 = 0; // Input Capture Value1 Channel1 y 2
uint32_t IC1_Val2 = 0, IC2_Val2 = 0; // Input Capture  Value2 Channel1 y 2
uint32_t Difference1 = 0, Difference2 = 0;
uint8_t First_Captured1 = 0, First_Captured2 = 0; // is the first value captured ?

State_Space_s ss1;

pid_s pid1;
pid_s pid2;
pid_s pid3;

fgs_s fgs1;
fgs_s fgs2;
fgs_s fgs3;

sa_s sa1;
sa_s sa2;
sa_s sa3;

Astar_s as1;
Astar_s as2;
Astar_s as3;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void  serial_printf(char *str);
void  HAL_Delay_us(uint16_t Delay);
float HCSR04_Read(int select);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void serial_printf(char *str)
{
	HAL_UART_Transmit_DMA(&huart3, (uint8_t *) str, strlen(str));
	//HAL_Delay(100);
}
// Delay microseconds;
void HAL_Delay_us(uint16_t Delay)
{
	__HAL_TIM_SET_COUNTER(&htim3,0);
	while (__HAL_TIM_GET_COUNTER(&htim3) < Delay);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char tx_buffer[52] = {0};
  int select_algorithm;
  int iter = 0;
  int Ts_ms = 1000*Ts;
  int time_on;
  int time_giro;
  int retardo = 0;
  int vaciar;
  int M1 = 1;
  int M2 = 1;
  int sum_h1 = 0;
  int sum_h2 = 0;
  float h1=0;
  float h2=0;
  float H1 = 0;
  float H2 = 0;
  float interval[2] = {0,1};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

  PID_Init(&pid1,1);
  PID_Init(&pid2,2);
  PID_Init(&pid3,3);

  FGS_Init(&fgs1, 1);
  FGS_Init(&fgs2, 2);
  FGS_Init(&fgs3, 3);

  SA_Init(&sa1,1);
  SA_Init(&sa2,2);
  SA_Init(&sa3,3);

  Astar_Init(&as1, 1);
  Astar_Init(&as2, 2);
  Astar_Init(&as3, 3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  pid1.r = 0.0;
  pid3.r = 0.0;
  pid2.r = pid3.r - pid1.r;

  vaciar = 1;
  select_algorithm = 0;

  float rbajo = 2;
  float ralto = 5;
  float ralto2 = 8;
  int espacio = 140;


  while (1)
  {
	  sa1.r  = pid1.r ;			  sa2.r  = pid2.r ; 	  sa3.r  = pid3.r ;
	  as1.r =  pid1.r ;			  as2.r =  pid2.r ;	      as3.r =  pid3.r ;

	  /*
	  if (iter == 0)   {    pid1.r = rbajo;		pid3.r = rbajo; }
	  if (iter == 20)  {    pid1.r = ralto ;    pid3.r = ralto ; }
	  if (iter == espacio +20) {    pid1.r = rbajo;      pid3.r = rbajo; }
	  if (iter == espacio*2 +20) {    pid1.r = ralto ;      pid3.r = ralto ;  }
	  if (iter == espacio*3 + 20) {    pid1.r = rbajo;      pid3.r = rbajo; }
	  if (iter == espacio*4 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*5 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*6 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*7 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*8 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*9 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*10 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*11 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*12 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*13 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*14 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*15 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*16 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*17 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*18 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*19 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*20 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }
	  if (iter == espacio*21 +20) {    pid1.r = rbajo;       pid3.r = rbajo;  }
	  if (iter == espacio*22 +20) {    pid1.r = ralto ;       pid3.r = ralto ;  }

*/
/*
	  if (iter == 0)   {    pid1.r = rbajo;		pid3.r = 0; }
	  if (iter == 20)  {    pid1.r = ralto ;    pid3.r = 0 ; }
	  if (iter == espacio +20) {    pid1.r = rbajo;      pid3.r = 0; }
	  if (iter == espacio*2 +20) {    pid1.r = ralto ;      pid3.r = 0 ;  }
	  if (iter == espacio*3 + 20) {    pid1.r = rbajo;      pid3.r = 0; }
	  if (iter == espacio*4 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*5 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*6 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*7 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*8 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*9 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*10 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*11 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*12 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*13 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*14 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*15 +20) {    pid1.r = rbajo;       pid3.r =0;  }
	  if (iter == espacio*16 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*17 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*18 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*19 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*20 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }
	  if (iter == espacio*21 +20) {    pid1.r = rbajo;       pid3.r = 0;  }
	  if (iter == espacio*22 +20) {    pid1.r = ralto ;       pid3.r = 0 ;  }

*/
	  if (iter == 0)         	{    pid1.r = ralto2;		pid3.r = rbajo; }
	  if (iter == 20)  			{    pid1.r = ralto2 ;    pid3.r = ralto ; }
	  if (iter == espacio +20)	 {    pid1.r = ralto2;      pid3.r = rbajo; }
	  if (iter == espacio*2 +20) {    pid1.r = ralto2;      pid3.r = ralto ;  }
	  if (iter == espacio*3 + 20) {    pid1.r = ralto2;      pid3.r = rbajo; }
	  if (iter == espacio*4 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*5 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*6 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*7 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*8 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*9 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*10 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*11 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*12 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*13 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*14 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*15 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*16 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*17 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*18 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*19 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*20 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }
	  if (iter == espacio*21 +20) {    pid1.r = ralto2;       pid3.r = rbajo;  }
	  if (iter == espacio*22 +20) {    pid1.r = ralto2;       pid3.r = ralto ;  }

	  pid2.r = pid3.r-pid1.r;

	  if (iter == 20){sa1.iniciar = 1; sa2.iniciar = 1; sa3.iniciar = 1;}

	  if ( ( (fabs(sa3.r - pid3.r)>0) || (fabs(as3.r - pid3.r)>0) ) && iter != 20 && iter != 0)
	  {
		  /*
		  sa1.r  = ralto;			  sa2.r  = 0 ; 	  sa3.r  = ralto;
		  as1.r =  ralto;			  as2.r =  0 ;	  as3.r =  ralto;
		  */
		  sa1.enable = 1;	sa2.enable = 1;		sa3.enable = 1;
		  as1.enable = 1;	as2.enable = 1;		as3.enable = 1;
	  }

	  if ( vaciar == 1) {
		  	htim2.Instance -> CCR1 = 0;
			HAL_GPIO_WritePin(VALV3_ON_GPIO_Port, VALV3_ON_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(VALV3_GPIO_Port, VALV3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin( BOMBA_GPIO_Port, BOMBA_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(VALV2_GPIO_Port, VALV2_Pin,GPIO_PIN_RESET);
			int time_vaciar = 0;
			while (time_vaciar < 10000)
			{
				if (time_vaciar % 100 == 0)
				{
					h2 = 25.5 - HCSR04_Read(2);
					h1 = 25.3 - HCSR04_Read(1);
					sprintf(tx_buffer,"%0.2f %0.2f %0.1f %0.1f %0.1f %0.1f %0.1f\r\n",h1, h2, pid1.r, pid3.r,pid1.u, pid2.u,pid3.u);
					serial_printf(tx_buffer);
				}
				HAL_Delay(1);
				time_vaciar++;
			}
			vaciar = 0;
			sprintf(tx_buffer,"H1 H2  Ref1 Ref2/u1 u2 u3 \r\n");
			serial_printf(tx_buffer);
	  }
	  else
	  {
		 HAL_GPIO_WritePin( BOMBA_GPIO_Port, BOMBA_Pin, GPIO_PIN_SET);
		// Medicion de altura de los tanques
	  	h2 = 25.5 - HCSR04_Read(2);
	    h1 = 25.3 - HCSR04_Read(1);
		HAL_Delay(5);
	    // Correcion de medicion de altura
	    h2 = limitar_altura(h2, &M2);
	    h1 = limitar_altura(h1, &M1);
	    // Promedio de medicion de altura.
	    if (M2!= 0){ h2 = (sum_h2+ h2)/M2;}
	    if (M1!= 0){ h1 = (sum_h1+ h1)/M1;}

	    // filtro del valor de las alturas
	    if (iter > 3)
	    {
			 // h1 = fabs(H1-h1) < 6 ? h1 : H1 ;
			  h2 = fabs(H2-h2) < 4 ? h2 : H2 ;
	    }
	    // Guarda el valor de altura para graficar

	    H1 = h1;
	    H2 = h2;
	    // redondeamos un 1 decimal
	    h1 = fround(h1,1);
	    h2 = fround(h2,1);
	  //--------------------------- PID -------------------------------------------------
		// ********* Salidas del sistema ********************
		pid1.y =  h1;   			// VALV1
		pid2.y =  fround(h2-h1,1);	// VALV2
		pid3.y =  h2  ;				// VALV3

		// ***********Señal de error entrada PID ************
		pid1.e[0] = pid1.r - pid1.y ;
		pid2.e[0] = pid2.r - pid2.y ;
		pid3.e[0] = pid3.r - pid3.y ;

		switch (select_algorithm)
		{
		// ***Determinacion de las ganancias FGS-PID ********
		case 1:
			FGS_control(&fgs1, &pid1);
			FGS_control(&fgs2, &pid2);
			FGS_control(&fgs3, &pid3);
			break;
		// ***Determinacion de las ganancias SA-PID ********
		case 2:
			sa1.y[sa1.muestra_i]= pid1.y ;
			sa3.y[sa3.muestra_i]= pid3.y ;
			sa2.y[sa2.muestra_i]= pid2.y ;

			Simulated_Annealing(&sa1, &pid1,Ts);
			Simulated_Annealing(&sa2, &pid2,Ts);
			Simulated_Annealing(&sa3, &pid3,Ts);
			break;
		// ***Determinacion de las ganancias A*-PID ********
		case 3:
			as1.y[as1.muestra_i]= pid1.y;
			as3.y[as3.muestra_i]= pid3.y;
			as2.y[as2.muestra_i]= pid2.y;

			Aestrella(&as1, &pid1,Ts);
			Aestrella(&as2, &pid2,Ts);
			Aestrella(&as3, &pid3,Ts);
			break;
		default:
			break;
		}

		// ************Señal de control *********************
		pid1.u = PID_control(pid1.e,pid1.Gains,Ts);
		pid2.u = PID_control(pid2.e,pid2.Gains,Ts);
		pid3.u = PID_control(pid3.e,pid3.Gains,Ts);

		// ************Señal de control [0 - 1]**************
		pid1.u = saturation(pid1.u,interval);
		pid2.u = saturation(pid2.u,interval);
		pid3.u = saturation(-pid3.u,interval);

		// ************Actualizar e inicia el estado VALVULA 1***********
	    htim2.Instance -> CCR1 = (int) (pid1.u * 750 + 35);
		// ************Define e inicia el estado VALVULA 2***************
	    time_on =  (int)  (pid2.u * Ts_ms);
		HAL_GPIO_WritePin(VALV2_GPIO_Port, VALV2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(VALV3_ON_GPIO_Port, VALV3_ON_Pin,GPIO_PIN_SET);
		// *************Define e inicia el estado VALVULA 3 ***************
	    if (pid3.u == 0 )
	    {
		   HAL_GPIO_WritePin(VALV3_GPIO_Port, VALV3_Pin, GPIO_PIN_RESET);
	       time_giro = Ts_ms;
	    }
	    else
	    {
	       HAL_GPIO_WritePin(VALV3_GPIO_Port, VALV3_Pin, GPIO_PIN_SET);
	       time_giro = (int) (pid3.u * Ts_ms + 50);
	    }
	    // ************** Eviar Datos por el puerto serial *********
		sprintf(tx_buffer,"%0.2f %0.2f %0.1f %0.1f %0.1f %0.1f %0.1f\r\n",H1, H2, pid1.r, pid3.r,pid1.u, pid2.u,pid3.u);
		serial_printf(tx_buffer);

	    retardo = 0;
		//Numero de mediciones
		M1 = 1; 			M2 = 1;
	    sum_h1 = 0;	  	sum_h2 = 0;

	    while (retardo < Ts_ms - 5)
	    {
		  // ************* Finaliza el  estado VALVULA 2 ***** ****************
	      if (retardo == time_on )
	      {
				HAL_GPIO_WritePin(VALV2_GPIO_Port, VALV2_Pin, GPIO_PIN_SET);
	      }
	      // ******************************************************************
	      // ***********  Finaliza el  estado VALVULA 3 ***********************
	      if (retardo  > time_giro )
	      {
				HAL_GPIO_WritePin(VALV3_GPIO_Port, VALV3_Pin, GPIO_PIN_RESET);
	      }
	      // ************* Promedio de mediciones de altura *******************
	      if (retardo % 100 == 0)
	      {
			h2 = 25.5 - HCSR04_Read(2);			    M2 ++;
			h1 = 25.3 - HCSR04_Read(1); 			M1 ++;

			h2 = limitar_altura(h2, &M2);
			h1 = limitar_altura(h1, &M1);

			sum_h2 = h2 + sum_h2;
			sum_h1 = h1 + sum_h1;
	      }
	      //************************ Retardo *********************************
	      HAL_Delay(1);     // 1ms retardo
	      retardo++;
	    }
		iter ++;

	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 96-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 96-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 4;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.battery_charging_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, VALV3_ON_Pin|BOMBA_Pin|TRIG1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, VALV3_Pin|VALV2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, USB_PowerSwitchOn_Pin|TRIG2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : VALV3_ON_Pin BOMBA_Pin TRIG1_Pin */
  GPIO_InitStruct.Pin = VALV3_ON_Pin|BOMBA_Pin|TRIG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : VALV3_Pin VALV2_Pin */
  GPIO_InitStruct.Pin = VALV3_Pin|VALV2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_PowerSwitchOn_Pin TRIG2_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin|TRIG2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//Input Capture callback in non blocking mode.
void HAL_TIM_IC_CaptureCallback( TIM_HandleTypeDef *htim){

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 )
	{
		if(First_Captured2 == 0){
			IC2_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // if the interrupt source is channel2
			First_Captured2 = 1; 									   // set the first captured as true
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_FALLING);
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);

		}
		else if(First_Captured2 == 1){
			IC2_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			__HAL_TIM_SET_COUNTER(htim,0);

			if (IC2_Val2 > IC2_Val1){
				Difference2 = IC2_Val2 - IC2_Val1;
			}
			else if(IC2_Val1 > IC2_Val2){
				Difference2 = (65535 - IC2_Val1) +  IC2_Val2 ;
			}
			 distancia2 = (float) Difference2 * 0.034 /2 ;
			 First_Captured2 = 0;  // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
		}

	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
	{
		if(First_Captured1 == 0){
			IC1_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // if the interrupt source is channel1
			First_Captured1 = 1; 									   // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);

		}
		else if(First_Captured1 == 1){
			IC1_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read second value
			__HAL_TIM_SET_COUNTER(htim,0); // reset the counter

			if ( IC1_Val2 > IC1_Val1 )
			{
				Difference1 = IC1_Val2-IC1_Val1;
			}
			else if ( IC1_Val1 > IC1_Val2 )
			{
				Difference1 = (65535 - IC1_Val1) +  IC1_Val2;
			}
			 distancia1 = (float) Difference1 * 0.034 / 2;
			 First_Captured1 = 0;  // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
		}

	}

}

float HCSR04_Read(int select)
{
	float distancia;
	if (select == 1)
	{
	  HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_SET);
	  HAL_Delay_us(10);
	  HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_RESET);
	  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);
	  distancia =  distancia1 ;
	}
	else if(select == 2)
	{
	  HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_SET);
	  HAL_Delay_us(10);
	  HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_RESET);
	  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2);
	  distancia = distancia2 ;
	}
	return distancia;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
