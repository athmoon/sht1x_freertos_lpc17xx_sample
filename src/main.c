/**
 * Name        : main.c
 * Version     :
 * Description : main definition for FreeRTOS application
 */

/*
 * FreeRTOS includes
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


#include "lpc17xx_can.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"


#include "sht.h"

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE

void __error__(char *pcFilename, unsigned long ulLine) {
}

static void setupHardware(void) {
	// TODO: Put hardware configuration and initialisation in here
	
	// Warning: If you do not initialize the hardware clock, the timings will be inaccurate
	debug_frmwrk_init();
}






TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;
int lolz = 0;

void TIMER0_IRQHandler(void);

void TIMER0_IRQHandler(void)

{
	TIM_Cmd(LPC_TIM0,DISABLE);
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	TIM_ResetCounter(LPC_TIM0);
	lolz++;
	TIM_Cmd(LPC_TIM0,ENABLE);
}

/**
 * Simple task that just toggles between to states
 */
void vUserTask1(void *pvParameters) {
	static int iState = 0;

	GPIO_SetDir(1, (1 << 18) | (1 << 20) | (1 << 21), 1);


	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;


	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Disable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	// Set Match value, count value of 1000 (1000 * 100uS = 100mS --> 10Hz)
	TIM_MatchConfigStruct.MatchValue  = 10;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 1 */
	//NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);

	TIM_Cmd(LPC_TIM0,ENABLE);


	while (1) {
		if (iState == 0) {
			iState = 1;
			GPIO_SetValue(1, (1 << 18) | (1 << 20) | (1 << 21));
		} else {
			iState = 0;
			GPIO_ClearValue(1, (1 << 18) | (1 << 20) | (1 << 21));
		}
		printf("lolz: %d\n", lolz);
		vTaskDelay(100);
	}
}

/**
 * Simple task that increments a counter
 */
void vUserTask2(void *pvParameters) {
	int count = 0;
	t_sht1x s;
	s.data = 27;
	s.sclk = 15;
	s.pinsel_port = PINSEL_PORT_0;
	s.pinsel_data = PINSEL_PIN_27;
	s.pinsel_sclk = PINSEL_PIN_15;
	sht1x_init(&s);

	while (1) {
		count++;
		vTaskDelay(101) ;
		printf("task2\n");
	}
}

/**
 * Program entry point 
 */
int main(void) {
	setupHardware();

	/* 
	 * Start the tasks defined within this file/specific to this demo. 
	 */
	xTaskCreate( vUserTask1, ( signed portCHAR * ) "Task1", USERTASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	//xTaskCreate( vUserTask2, ( signed portCHAR * ) "Task2", USERTASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	/* 
	 * Start the scheduler. 
	 */
	vTaskStartScheduler();

	/* 
	 * Will only get here if there was insufficient memory to create the idle task. 
	 */
	return 1;
}
