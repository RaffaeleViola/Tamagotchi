/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../timer/timer.h"
#include "../Tamagochi.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */
volatile int volume;


void ADC_IRQHandler(void) {
  	
 AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF); //Read Conversion Result             
	//AD
	if(AD_current!=AD_last){
		  //map volume values
		  if(AD_current < 4096) volume = 3;
			if(AD_current < 3000) volume = 2;
			if(AD_current < 2000) volume = 1;
			if(AD_current < 1000) volume = 0;
		if(volume != get_volume())
				refresh_volume();
		AD_last = AD_current;
	}
}


