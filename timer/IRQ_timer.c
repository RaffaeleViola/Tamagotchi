/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../Tamagochi.h"
#include "../RIT.h"
#include "../ADC/adc.h"



/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

//Timer0
extern int volume;
static int animation = 0;
extern Element babbo_face;
extern Point food_meal;
extern int reset_sound;



//Timer1
static int dx_food_animation = 0;
static int dy_food_animation = 0;
static int step_food_animation = 0;

static int dy_reset_animation = 0;
static int step_reset_animation = 0;

static int step_cuddles_animation = 0;

extern int wait_for_completion;
uint16_t SinTable[45] =                                       /*                      */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};
const int freqs[8]={2120/3,1890/3,1684/3,1592/3,1417/3,1263/3,1125/3,1062/3};
static int note_pointer = 0;

void TIMER0_IRQHandler (void)
{
	static int five_seconds = 1;
	static int counter=0; //23:59:59
	char str[8];
	int hh,mm,ss,i;
	
	//time management
	counter ++;
	if(counter <= 86399){
	hh = counter/3600;
	mm = counter/60 - hh*60;
	ss = counter - mm*60 - hh * 3600;
	}else{
		hh = 0;
		mm = 0;
		ss = 0;
		counter = -1;
	}
	sprintf(str, "Age: %02d:%02d:%02d", hh, mm, ss);
	GUI_Text(80, 20, (uint8_t *) str, Black, White);
	if(five_seconds == 5){
	   refresh_Battery(1,0);
	   refresh_Battery(0,0);
	  five_seconds = 0;
	}
	five_seconds++;
	
	//animaion
	if(animation == 0){
	draw_colored_Rectangle(babbo_face.p.x+21,babbo_face.p.y+babbo_face.r.h+12,4,8,RGB565CONVERT(0xFF,0xB2,0x66));
	draw_colored_Rectangle(babbo_face.p.x+17,babbo_face.p.y+babbo_face.r.h+16,4,15,RGB565CONVERT(0xFF,0xB2,0x66));
	draw_colored_Rectangle(babbo_face.p.x+17,babbo_face.p.y+babbo_face.r.h+20,6,15,RGB565CONVERT(0xFF,0xB2,0x66));
	draw_colored_Rectangle(babbo_face.p.x+18,babbo_face.p.y+babbo_face.r.h+26,1,13,RGB565CONVERT(0xFF,0xB2,0x66));
	for(i=1;i<4;i++){
		draw_colored_Rectangle(babbo_face.p.x+17+(i*2),babbo_face.p.y+babbo_face.r.h+27,1,15-(i*4),RGB565CONVERT(0xFF,0xB2,0x66));
	}
	draw_colored_Rectangle(babbo_face.p.x+24,babbo_face.p.y+babbo_face.r.h+18,9,1,Black);
	animation = 1;
  }else if(animation == 1){
		draw_colored_Rectangle(babbo_face.p.x+21,babbo_face.p.y+babbo_face.r.h+12,4,8,RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x+17,babbo_face.p.y+babbo_face.r.h+16,4,15,RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x+17,babbo_face.p.y+babbo_face.r.h+20,6,15,RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x+17,babbo_face.p.y+babbo_face.r.h+26,1,15,White);
	for(i=1;i<4;i++){
		draw_colored_Rectangle(babbo_face.p.x+17+(i*2),babbo_face.p.y+babbo_face.r.h+27,1,15-(i*4),White);
	}
	animation = 0;
	}
	
	//reset evaluation
	if(getHappiness() ==-1 || getSatiety() == -1){
		counter = 0;
		five_seconds = 1;
		reset_call();
		LPC_TIM1->IR = 1;			/* clear interrupt flag */
		disable_timer(0);
		step_food_animation = 0;
		animation = 0;
		dx_food_animation = 0;
		dy_food_animation = 0;
		step_reset_animation = 0;
		dy_reset_animation = 0;
	}
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  if(reset_sound==1 && wait_for_completion == 0){
		disable_timer(3);
		reset_timer(3);
		disable_timer(1);
		reset_sound = 0;
		reset_click();
		LPC_TIM1->IR = 1;			/* clear interrupt flag */
		return;
	}
	//animzaione meal-snack
	if(on_Reset()==0  && on_cuddles_animation()==0 && reset_sound == 0){
		animation = 2;
		if(step_food_animation < 3){
			disable_timer(3);
			reset_timer(3);
			init_timer(3,freqs[note_pointer]);
			enable_timer(3);
			note_pointer ++;
			dx_food_animation-=4;
			dy_food_animation+=2;
			clear_tamagochi();
			draw_tamagochi(dx_food_animation,dy_food_animation);
			step_food_animation ++;
		}else{
			dx_food_animation = 0;
			dy_food_animation = 0;
			note_pointer = 0;
			clear_meal();
			disable_timer(3);
			reset_timer(3);
			stop_animation();
			increment_battery();
			clear_tamagochi();
			reset_face();
			animation = 0;
			step_food_animation = 0;
		}
		//animazione cuddles
	}else if(on_cuddles_animation()==1 && on_Reset()==0){
		animation = 2; //stop Timer0 animation
		if(step_cuddles_animation%2 == 0 && step_cuddles_animation < 8){
			disable_timer(3);
			reset_timer(3);
			init_timer(3,freqs[note_pointer]);
			enable_timer(3);
			note_pointer+=2;
			 draw_heart();
			 step_cuddles_animation++;
		}else if(step_cuddles_animation%2 != 0 && step_cuddles_animation < 8){
			disable_timer(3);
			reset_timer(3);
			init_timer(3,freqs[note_pointer]);
			enable_timer(3);
			note_pointer--;
			clear_heart();
			step_cuddles_animation++;
		}
		if(step_cuddles_animation == 7){
			 stop_cuddles_animation();
			 disable_timer(3);
			 reset_timer(3);
			 note_pointer = 0;
       animation = 0; //enable timer0 animation
			 step_cuddles_animation = 0;
		}
		//animazione reset
  }else{
		if(step_reset_animation < 3){
			disable_timer(3);
			reset_timer(3);
			init_timer(3,freqs[note_pointer]);
			enable_timer(3);
			note_pointer++;
			if(step_reset_animation > 0){
				draw_colored_Rectangle(babbo_face.p.x-11, babbo_face.p.y+82-((step_reset_animation-1)*25),12, babbo_face.r.l+12,White);
				draw_colored_Rectangle(babbo_face.p.x-19, babbo_face.p.y+59-((step_reset_animation-1)*25),23, babbo_face.r.l+28,White);
			}
			draw_colored_Rectangle(babbo_face.p.x-11, babbo_face.p.y+82-dy_reset_animation,12, babbo_face.r.l+12,Black);
			draw_colored_Rectangle(babbo_face.p.x-19, babbo_face.p.y+59-dy_reset_animation,23, babbo_face.r.l+28,Black);
			draw_colored_Rectangle(babbo_face.p.x+babbo_face.r.l+2, babbo_face.p.y+82-dy_reset_animation,10,6,White);
			step_reset_animation++;
			dy_reset_animation += 25;
		}else if(step_reset_animation == 3)
		{ //last step
			disable_timer(3);
			reset_timer(3);
			init_timer(3,freqs[note_pointer]);
			enable_timer(3);
			note_pointer++;
			draw_colored_Rectangle(babbo_face.p.x-11, babbo_face.p.y+82-((step_reset_animation-1)*25),12, babbo_face.r.l+12,White);
			draw_colored_Rectangle(babbo_face.p.x-19, babbo_face.p.y+59-((step_reset_animation-1)*25),23, babbo_face.r.l+28,White);
			draw_colored_Rectangle(babbo_face.p.x-11, babbo_face.p.y+82-dy_reset_animation,12, babbo_face.r.l+12,Black);
			draw_colored_Rectangle(babbo_face.p.x-19, babbo_face.p.y+59-dy_reset_animation,23, babbo_face.r.l+28,Black);
			GUI_Text(50,140,(uint8_t *)"I'v fallen into",Black, Red);
	    GUI_Text(56,160,(uint8_t *)"the chimney!!!",Black, Red);
			step_reset_animation++;
		}else if(step_reset_animation == 4){
			disable_timer(3);
			reset_timer(3);
			init_timer(3,freqs[note_pointer]);
			enable_timer(3);
			step_reset_animation++;
		}
		else if(step_reset_animation==5){
		  note_pointer = 0;
			disable_timer(3);
			disable_timer(1);
			set_wait(0);
		}
	}
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void){
	
	if(wait_for_completion==1)
		return;
  if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){
		if(display.x > 70 && display.x < 160 && display.y > 90 && display.y < 220){
			 cuddles_animation();
		}
	}
	else{
		//do nothing if touch returns values out of bounds
	}
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void){
  
	static int ticks=0;
	/* DAC management */	
	
	
	LPC_DAC->DACR = ((uint16_t)(SinTable[ticks] * volume/3.3))<<6;
	ticks++;
	if(ticks==45){ 
		ticks=0;
	}
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}
