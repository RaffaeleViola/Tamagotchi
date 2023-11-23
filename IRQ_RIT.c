/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "Tamagochi.h"      
#include "timer/timer.h"
#include "ADC/adc.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern Element meal;
extern Element snack;
extern int wait_for_completion;
volatile int reset_sound = 0;

void RIT_IRQHandler (void)
{					
	static int select=0;
	static int J_left = 0;
	static int J_right = 0;
	
	ADC_start_conversion();
	if(wait_for_completion==1){
		LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
		return;
	}
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		select++;
		switch(select){
			case 1:
		 if(on_Reset()==1){
			 reset_sound = 1;
			 init_timer(3, 1062);
			 enable_timer(3);
			 enable_timer(1);
			 J_left = 0;
			 J_right = 0;
		 }
		 if(J_left!=0){
			 init_timer(3, 1062);
			 enable_timer(3);
			 meal_animation(0);
		}else if(J_right!=0){
			 meal_animation(1);
			init_timer(3, 1062);
			 enable_timer(3);
		}
			default:
				break;
		}
	}
	else{
			select=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && on_Reset()==0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		
		J_left++;
		switch(J_left){
			case 1:
				  J_right = 0;
					draw_Rectangle(snack.p.x,snack.p.y,snack.r.h,snack.r.l,snack.r.color);
				  draw_Rectangle(meal.p.x,meal.p.y,meal.r.h,meal.r.l,Red);
			    return;
			default:
				break;
		}
	}
	
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && on_Reset()==0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		
		J_right++;
		switch(J_right){
			case 1:
				 J_left = 0;
				 draw_Rectangle(meal.p.x,meal.p.y,meal.r.h,meal.r.l,meal.r.color);
				 draw_Rectangle(snack.p.x,snack.p.y,snack.r.h,snack.r.l,Red);
			   return;
			default:
				break;
		}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}



/******************************************************************************
**                            End Of File
******************************************************************************/

