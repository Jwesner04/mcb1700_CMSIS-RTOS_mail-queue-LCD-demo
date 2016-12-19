/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    rtx_blinky_ind.c
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2014 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include "cmsis_os.h"
#include "LPC17xx.h"
#include "Board_LED.h"
#include "ADC.h"
#include "GLCD.h"


#define __FI	1	/* Font Index */
#define MutexIsr 26458
#define __USE_LCD
osMutexId mutex_1;
osMutexDef(mutex_1);

uint8_t info[5][3][10] = {{"1","China     ","Beijing   "},{"2","USA       ","Washington"},{"3","France    ","Paris     "},{"4","England   ","London    "},{"5","Japan     ","Tokoya    "}};
typedef struct {                                 // Mail object structure
  uint8_t  *number;                           	
  uint8_t  *country;           
  uint8_t	 *capital;                              
} T_MEAS;
 
osMailQDef(mail, 16, T_MEAS);                    // Define mail queue
osMailQId  mail;

typedef struct{
	uint32_t adc_value;
	uint32_t tick;
} properties_t;

osThreadId tid_Thread1;                  /* Thread id of task: LED1_Control */
osThreadId tid_Thread2;                  /* Thread id of task: LED2_Control */
osThreadId tid_Thread3;                  /* Thread id of task: LED3_Control */
osThreadId tid_Thread4;                  /* Thread id of task: LED4_Control */


osPoolDef(mpool, 16, properties_t);                    // Define memory pool
osPoolId  mpool;
osMessageQDef(MsgBox, 16, properties_t);               // Define message queue
osMessageQId  MsgBox;

/*----------------------------------------------------------------------------
 *      Function Hex to BCD converter
 *---------------------------------------------------------------------------*/
void Hex32ToBCD(uint32_t Hex, uint8_t *Dec)
{
  uint32_t index = 9;
	Dec[10]=0;
	while(Hex>=10)
	{
		Dec[index--] = Hex%10 + '0';
		Hex /= 10;
	}	
	Dec[index]=Hex+'0';
}

/*----------------------------------------------------------------------------
*      Thread 1: GET ADC VALUE FROM BOARD AND SYSTEM CLOCK
 *---------------------------------------------------------------------------*/
void Thread1 (void const *argument) {
	properties_t    *pptr;
	uint32_t ADC_value;
	uint32_t tick;
	
	
	for(;;){
		ADC_StartCnv();
		ADC_value = ADC_GetCnv();
		ADC_StopCnv();
		tick = osKernelSysTick();

		
		pptr = osPoolAlloc(mpool);                     					 // Allocate memory for the message
		pptr->adc_value = ADC_value;                        		 // Set the message content
		pptr->tick = tick;                        				 			 // Set the message content
		osMessagePut(MsgBox, (uint32_t)pptr, osWaitForever);  	 // Send Message
		osDelay(1000);
	}
}

/*----------------------------------------------------------------------------
*      Thread 2: DISPLAY CLOCK AND ADC VALUE TO LCD
 *---------------------------------------------------------------------------*/
void Thread2 (void const *argument) {

	properties_t *rptr;
  osEvent  evt;
	
	
  for (;;) {
		uint8_t bcd[10]= "          ";
		uint8_t tickTock[10] = "          ";
    evt = osMessageGet(MsgBox, osWaitForever);   // wait for message
    if (evt.status == osEventMessage) {
      rptr = evt.value.p;
			
			
			osMutexWait(mutex_1,osWaitForever);
			Hex32ToBCD(rptr->adc_value,bcd);
      GLCD_DisplayString (0, 7, __FI, bcd);
			Hex32ToBCD(rptr->tick,tickTock);
			GLCD_DisplayString (1, 7, __FI, tickTock);
			osMutexRelease(mutex_1);
      osPoolFree(mpool, rptr);                  // free memory allocated for message
    }
  }
}

/*----------------------------------------------------------------------------
*      Thread 3: FILL STRUCTURE WITH INFO
 *---------------------------------------------------------------------------*/
void Thread3(void const *argument) {
     T_MEAS *mptr;
	  for (;;) {
			 mptr = osMailAlloc(mail, osWaitForever);         // Allocate memory
			 mptr->number = &info[0][0][0];   								// Set the mail content
			 mptr->country = &info[0][1][0];
			 mptr->capital = &info[0][2][0];
			 osMailPut(mail, mptr);                           // Send Mail
			 osDelay(1000);
		 
			 mptr = osMailAlloc(mail, osWaitForever);         // Allocate memory
			 mptr->number = &info[1][0][0];   								// Set the mail content
			 mptr->country = &info[1][1][0];
			 mptr->capital = &info[1][2][0];
			 osMailPut(mail, mptr);                           // Send Mail
			 osDelay(1000);
			
			 mptr = osMailAlloc(mail, osWaitForever);         // Allocate memory
			 mptr->number = &info[2][0][0];   								// Set the mail content
			 mptr->country = &info[2][1][0];
			 mptr->capital = &info[2][2][0];
			 osMailPut(mail, mptr);                           // Send Mail
			 osDelay(1000);
			 
			 mptr = osMailAlloc(mail, osWaitForever);         // Allocate memory
			 mptr->number = &info[2][0][0];   								// Set the mail content
			 mptr->country = &info[2][1][0];
			 mptr->capital = &info[2][2][0];
			 osMailPut(mail, mptr);                           // Send Mail
			 osDelay(1000);
			 
			 mptr = osMailAlloc(mail, osWaitForever);         // Allocate memory
			 mptr->number = &info[2][0][0];   								// Set the mail content
			 mptr->country = &info[2][1][0];
			 mptr->capital = &info[2][2][0];
			 osMailPut(mail, mptr);                           // Send Mail
			 osDelay(1000);
			 
			 mptr = osMailAlloc(mail, osWaitForever);         // Allocate memory
			 mptr->number = &info[3][0][0];   								// Set the mail content
			 mptr->country = &info[3][1][0];
			 mptr->capital = &info[3][2][0];
			 osMailPut(mail, mptr);                           // Send Mail
			 osDelay(1000);
			 
			 mptr = osMailAlloc(mail, osWaitForever);         // Allocate memory
			 mptr->number = &info[4][0][0];   								// Set the mail content
			 mptr->country = &info[4][1][0];
			 mptr->capital = &info[4][2][0];
			 osMailPut(mail, mptr);                           // Send Mail
			 osDelay(1000);
		}
}

/*----------------------------------------------------------------------------
*      Thread 4: OUTPUT COUNTRY INFO TO LCD
 *---------------------------------------------------------------------------*/
void Thread4 (void  const *argument) {
  T_MEAS  *rptr;
  osEvent  evt;

  for (;;) {
    evt = osMailGet(mail, osWaitForever);          // wait for mail
    if (evt.status == osEventMail) {
      rptr = evt.value.p;
			GLCD_DisplayString(3,10,__FI,rptr->number);
			GLCD_DisplayString(4,10,__FI,rptr->country);
			GLCD_DisplayString(5,10,__FI,rptr->capital);
      osMailFree(mail, rptr);                      // free memory allocated for mail
    }
  }
}


osThreadDef(Thread1, osPriorityNormal, 1, 0);
osThreadDef(Thread2, osPriorityNormal, 1, 0);
osThreadDef(Thread3, osPriorityNormal, 1, 0);
osThreadDef(Thread4, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 *      Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {
  ADC_Init();                                /* ADC Initialization            */
	#ifdef __USE_LCD
	GLCD_Init();                               /* Initialize graphical LCD      */

	GLCD_Clear(White);                         
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(0, 0, __FI, "AD: ");
	GLCD_DisplayString(1, 0, __FI, "TIME: ");
	GLCD_DisplayString(3, 0, __FI, "NO: ");
	GLCD_DisplayString(4, 0, __FI, "COUNTRY: ");
	GLCD_DisplayString(5, 0, __FI, "CAPITAL: ");
	#endif
	
	mutex_1 = osMutexCreate(osMutex(mutex_1));
	
	mpool = osPoolCreate(osPool(mpool));                 // create memory pool
  MsgBox = osMessageCreate(osMessageQ(MsgBox), NULL);  // create msg queue
	
  tid_Thread1 = osThreadCreate(osThread(Thread1), NULL);
  tid_Thread2 = osThreadCreate(osThread(Thread2), NULL);
	
	
	mail = osMailCreate(osMailQ(mail), NULL);            // create mail queue
	
	tid_Thread3 = osThreadCreate(osThread(Thread3), NULL);
  tid_Thread4 = osThreadCreate(osThread(Thread4), NULL);
	
	osMutexRelease(mutex_1);
  while(1);
}
