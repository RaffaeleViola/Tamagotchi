


#include "GLCD/GLCD.h"
#include "timer/timer.h"
#include "Tamagochi.h"
#include "RIT.h"
#include "joystick.h"
#include "ADC/adc.h"
#define MAX_L 240
#define MAX_H 320
#define TAMA_X 95
#define TAMA_Y 155

static int happinnes_level = 4;
static int satiety_level = 4;
volatile int wait_for_completion = 0;
static int left_animation = 0;
static int on_reset = 0;
static int in_cuddles_animation = 0;
static int current_volume = 3;
extern int volume;


uint16_t dark_grey = RGB565CONVERT(0xC0,0xC0,0xC0);

Point meal_text;
Element meal;
Element snack;
Point snack_text;
Element battery_left;
Element battery_right;
Point happ_text;
Point sat_text;
Element reset;
Point reset_text;
Element babbo_face;
Point food_meal;
Point cuddles_heart;
Element speaker;

uint16_t cassa[] = { /*w=14,h=17*/
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,Black,Black,0x0000,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,Black,Black,Black,Black,0x0000,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,Black,Black,Black,Black,Black,Black,0x0000,
  0xFFFF,0x0000,0x0000,0x0000,0x0000,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0x0000,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0x0000,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0x0000,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0x0000,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0x0000,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0x0000,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0x0000,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0xFFFF,0x0000,0x0000,0x0000,0x0000,Black,Black,Black,Black,Black,Black,Black,Black,0x0000,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,Black,Black,Black,Black,Black,Black,0x0000,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,Black,Black,Black,Black,0x0000,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,Black,Black,0x0000,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,0xFFFF,
};


void draw_element(int x0, int y0, int h, int w, uint16_t *array){
	int i, j;
	
	for(i=0; i< h;i++){
		for(j=0;j<w;j++){
			 LCD_SetPoint(x0+j,y0+i, array[i*w+j]);
		}
	}
	
	
}


void draw_Rectangle(uint16_t x0, uint16_t y0, uint16_t height, uint16_t length, uint16_t color){
	
  LCD_DrawLine(x0, y0, x0+length, y0, color); //bottom line
	LCD_DrawLine(x0, y0+height, x0+length, y0+height, color); //upper line
	LCD_DrawLine(x0, y0, x0, y0+height, color);
	LCD_DrawLine(x0+length, y0, x0+length, y0+height, color);
	
}
void draw_colored_Rectangle(uint16_t x0, uint16_t y0, uint16_t height, uint16_t length, uint16_t color){
  int i,j;
	for(j=height;j>=0;j--){
		for(i=length; i>=0;i--){
		LCD_SetPoint(x0+i, y0+j, color);
		}
	}
}
void draw_Battery(uint16_t x0, uint16_t y0){
	 int i, padding;
	 draw_Rectangle(x0, y0, 20, 42, Black); //bigger rectangle
	 draw_colored_Rectangle(x0+42, y0+5, 10, 3, Black);
	 for(i=0; i<5;i++){
		  padding = (i*8) + 2; //6 length + 2 padding
		  draw_colored_Rectangle(x0+padding, y0+2,16, 6, Black); 
	 }
}
void draw_heart(void){
      
	   
	  LCD_DrawRectangle(cuddles_heart.x+4,cuddles_heart.y+1,4,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+8,cuddles_heart.y+2,1,2,Black);
    LCD_DrawRectangle(cuddles_heart.x+8,cuddles_heart.y+3,3,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+10,cuddles_heart.y+2,1,2,Black);
    LCD_DrawRectangle(cuddles_heart.x+11,cuddles_heart.y+1,4,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+15,cuddles_heart.y+2,1,2,Black);
    LCD_DrawRectangle(cuddles_heart.x+16,cuddles_heart.y+3,1,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+17,cuddles_heart.y+4,1,5,Black);
    LCD_DrawRectangle(cuddles_heart.x+15,cuddles_heart.y+9,2,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+15,cuddles_heart.y+10,1,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+13,cuddles_heart.y+11,2,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+11,cuddles_heart.y+12,2,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+11,cuddles_heart.y+13,1,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+8,cuddles_heart.y+14,3,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+7,cuddles_heart.y+13,1,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+6,cuddles_heart.y+12,2,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+4,cuddles_heart.y+11,2,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+3,cuddles_heart.y+10,1,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+2,cuddles_heart.y+9,2,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+1,cuddles_heart.y+4,1,5,Black);
    LCD_DrawRectangle(cuddles_heart.x+2,cuddles_heart.y+3,2,1,Black);
    LCD_DrawRectangle(cuddles_heart.x+3,cuddles_heart.y+2,1,1,Black);

    //RIEMPIMENTO
  
    LCD_DrawRectangle(cuddles_heart.x+4,cuddles_heart.y+2,4,2,Red);
    LCD_DrawRectangle(cuddles_heart.x+11,cuddles_heart.y+2,4,2,Red);
    LCD_DrawRectangle(cuddles_heart.x+2,cuddles_heart.y+4,15,5,Red);
    LCD_DrawRectangle(cuddles_heart.x+4,cuddles_heart.y+9,11,2,Red);
    LCD_DrawRectangle(cuddles_heart.x+6,cuddles_heart.y+11,7,1,Red);
    LCD_DrawRectangle(cuddles_heart.x+8,cuddles_heart.y+12,3,2,Red);
    
    LCD_DrawRectangle(cuddles_heart.x+4,cuddles_heart.y+4,2,1,RGB565CONVERT(0xE4,0xE0,0xE0));


}

void clear_heart(void){
	draw_colored_Rectangle(cuddles_heart.x, cuddles_heart.y, 15, 22,White);
}

void cuddles_animation(void){
	wait_for_completion = 1; //the system waits
	disable_timer(2);
	in_cuddles_animation  = 1;
	enable_timer(1);
	return;
}

void stop_cuddles_animation(void){
	in_cuddles_animation  = 0;
	disable_timer(1);
	refresh_Battery(1,1);
	clear_heart();
	wait_for_completion = 0; //the system restarts
	enable_timer(2);
	return;
}

int on_cuddles_animation(void){
	return in_cuddles_animation;
}

void reset_click(void){
	
	//reset the text to White
	GUI_Text(50,140,(uint8_t *)"               ",White, White);
	GUI_Text(56,160,(uint8_t *)"              ",White, White);
	
	//drawing batteries
	draw_Battery(battery_left.p.x, battery_left.p.y);
	draw_Battery(battery_right.p.x, battery_right.p.y);
	happinnes_level = 4;
	satiety_level = 4;
	
	//drawing tamagochi
	clear_tamagochi();
	reset_face();
	//drawing buttons
	GUI_Text(reset_text.x, reset_text.y, (uint8_t *) "     ", White, White);
	draw_Rectangle(meal.p.x, meal.p.y, meal.r.h, meal.r.l, meal.r.color);
	draw_Rectangle(snack.p.x, snack.p.y, snack.r.h, snack.r.l, snack.r.color);
	GUI_Text(meal_text.x, meal_text.y, (uint8_t *) "Meal", Black, White);
	GUI_Text(snack_text.x, snack_text.y, (uint8_t *) "Snack", Black, White);
	
	enable_timer(0);
	on_reset = 0;
	enable_timer(2);
}
void reset_call(void){
	wait_for_completion = 1;
	disable_timer(2);
	reset.p.x = meal.p.x;
	reset.p.y = meal.p.y;
	reset.r.h = meal.r.h;
	reset.r.l = meal.r.l*2;
	reset.r.color = Red;
	reset_text.x = 100;
	reset_text.y = 285;
	LCD_DrawLine(meal.p.x+meal.r.l,meal.p.y,meal.p.x+meal.r.l,meal.p.y+meal.r.h, White);
	GUI_Text(meal_text.x, meal_text.y, (uint8_t *) "    ", Black, White);
	GUI_Text(snack_text.x, snack_text.y, (uint8_t *) "     ", Black, White);
	draw_Rectangle(reset.p.x,reset.p.y,reset.r.h,reset.r.l,reset.r.color);
	GUI_Text(reset_text.x, reset_text.y, (uint8_t *) "Reset", Black, White);
	if(babbo_face.p.x != TAMA_X){ //tamagochi is involved into animation
		  disable_timer(1);
			clear_tamagochi();
		  reset_face();
	}
	on_reset = 1;
	clear_meal();
	clear_heart();
	enable_timer(1);
}

int on_Reset(void){
	return on_reset;
}

void set_wait(int wait){
	wait_for_completion = wait;
}

void draw_tamagochi(int i, int j){
	babbo_face.p.x+=i;
	babbo_face.p.y+=j;
	
	//drawing tamagochi
	
	draw_colored_Rectangle(babbo_face.p.x-6, babbo_face.p.y-6,12,12+babbo_face.r.l,dark_grey);
	//draw mustache
	draw_colored_Rectangle(babbo_face.p.x-6, babbo_face.p.y+16,30,12+babbo_face.r.l,RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x-3, babbo_face.p.y+16+30,3,6+babbo_face.r.l,RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x, babbo_face.p.y+16+30+3,3,babbo_face.r.l,RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x+3, babbo_face.p.y+16+30+6,3,babbo_face.r.l-6,RGB565CONVERT(0xE4,0xE0,0xE0));
	//draw face
	draw_colored_Rectangle(babbo_face.p.x, babbo_face.p.y, babbo_face.r.h, babbo_face.r.l, babbo_face.r.color);
	draw_colored_Rectangle(babbo_face.p.x-3,babbo_face.p.y+3, babbo_face.r.h-8, 3, babbo_face.r.color); //left part
	draw_colored_Rectangle(babbo_face.p.x+babbo_face.r.l,babbo_face.p.y+3, babbo_face.r.h-8, 3, babbo_face.r.color);// right part
	draw_colored_Rectangle(babbo_face.p.x+10,babbo_face.p.y+8,8,8,Black); //left eye
	draw_colored_Rectangle(babbo_face.p.x+32,babbo_face.p.y+8,8,8,Black); //right eye
	//draw mouth
	draw_colored_Rectangle(babbo_face.p.x,babbo_face.p.y+babbo_face.r.h+8,4,4,dark_grey); 
	draw_colored_Rectangle(babbo_face.p.x+4,babbo_face.p.y+babbo_face.r.h+12,4,4,dark_grey);
	draw_colored_Rectangle(babbo_face.p.x+8,babbo_face.p.y+babbo_face.r.h+16,4,9,dark_grey);
	draw_colored_Rectangle(babbo_face.p.x+17,babbo_face.p.y+babbo_face.r.h+12,4,4,dark_grey);
	draw_colored_Rectangle(babbo_face.p.x+21,babbo_face.p.y+babbo_face.r.h+8,4,8,dark_grey);
	draw_colored_Rectangle(babbo_face.p.x+29,babbo_face.p.y+babbo_face.r.h+12,4,4,dark_grey);
	draw_colored_Rectangle(babbo_face.p.x+33,babbo_face.p.y+babbo_face.r.h+16,4,9,dark_grey);
	draw_colored_Rectangle(babbo_face.p.x+42,babbo_face.p.y+babbo_face.r.h+12,4,4,dark_grey);
	draw_colored_Rectangle(babbo_face.p.x+46,babbo_face.p.y+babbo_face.r.h+8,4,4,dark_grey);
	//draw sopracciglia
	draw_colored_Rectangle(babbo_face.p.x+6,babbo_face.p.y+4,4,16,dark_grey); //left 
	draw_colored_Rectangle(babbo_face.p.x+28,babbo_face.p.y+4,4,16,dark_grey); //right 
	//draw hat
	draw_colored_Rectangle(babbo_face.p.x-22,babbo_face.p.y-11,10,10,RGB565CONVERT(0xE4,0xE0,0xE0)); //pon pon
	  //red parts
	draw_colored_Rectangle(babbo_face.p.x-12,babbo_face.p.y-21,15,babbo_face.r.l+18,Red); //base rectangle
	for(i=0;i<5;i++){
		draw_colored_Rectangle(babbo_face.p.x-12+(i*3),babbo_face.p.y-21-(i*3),3,babbo_face.r.l+18-(i*7),Red);
	}
	draw_colored_Rectangle(babbo_face.p.x-12+16,babbo_face.p.y-21-15,3,babbo_face.r.l+18-38,Red);
	draw_colored_Rectangle(babbo_face.p.x-16,babbo_face.p.y-16,8,4,Red);
	draw_colored_Rectangle(babbo_face.p.x-19,babbo_face.p.y-13,5,3,Red);
	//white parts
	draw_colored_Rectangle(babbo_face.p.x,babbo_face.p.y-12,8,babbo_face.r.l, RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x-3,babbo_face.p.y-12,12,3, RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x+babbo_face.r.l,babbo_face.p.y-12,12,3, RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x-6,babbo_face.p.y-8,12,3, RGB565CONVERT(0xE4,0xE0,0xE0));
	draw_colored_Rectangle(babbo_face.p.x+babbo_face.r.l+3,babbo_face.p.y-8,12,3, RGB565CONVERT(0xE4,0xE0,0xE0));
	//draw nose
	draw_colored_Rectangle(babbo_face.p.x+18,babbo_face.p.y+24,9,14,RGB565CONVERT(0xFF,0xAA,0x55)); //big part
	draw_colored_Rectangle(babbo_face.p.x+16,babbo_face.p.y+28,3,2,RGB565CONVERT(0xFF,0xB2,0x66)); //left 
	draw_colored_Rectangle(babbo_face.p.x+32,babbo_face.p.y+28,3,2,RGB565CONVERT(0xFC,0xB0,0x63)); //right
}

void reset_face(void){
	babbo_face.p.x = TAMA_X;
	babbo_face.p.y = TAMA_Y;
	draw_tamagochi(0,0);
}

void increment_battery(void){
	refresh_Battery(left_animation,1);
}

void refresh_Battery(int left,int increment){ //left -> 0 for right 1 for left
	 if(increment > 0){
		 if(happinnes_level < 4 && left == 1){
					happinnes_level++;
			    draw_colored_Rectangle(42 + (happinnes_level*8),72 ,16, 6, Black);
	    }
		  if(satiety_level < 4 && left == 0){
					satiety_level++;
				  draw_colored_Rectangle(162 + (satiety_level*8),72 ,16, 6, Black);
			}
	 }else{
		 if(happinnes_level >= 0 && left == 1){
					draw_colored_Rectangle(42 + (happinnes_level*8),72 ,16, 6, White);
					happinnes_level--;
			}
		 if(satiety_level >= 0 && left == 0){
					draw_colored_Rectangle(162 + (satiety_level*8),72 ,16, 6, White);
					satiety_level--;
			} 
	 }
	
}
int getSatiety(void){
	return satiety_level;
} 

int getHappiness(void){
	 return happinnes_level;
}
void clear_tamagochi(void){
	draw_colored_Rectangle(babbo_face.p.x-22,babbo_face.p.y-36,100,85, White);
}

void draw_meal(void){
	
	if(left_animation == 0){
		draw_Rectangle(food_meal.x,food_meal.y,8,16,Black);
		draw_Rectangle(food_meal.x+8,food_meal.y+8,23,8,Black);
		draw_colored_Rectangle(food_meal.x+1,food_meal.y+1,6,2,Red);
		draw_colored_Rectangle(food_meal.x+6,food_meal.y+1,6,2,Red);
		draw_colored_Rectangle(food_meal.x+11,food_meal.y+1,6,2,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+9,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+14,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+19,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+23,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+28,2,6,Red);
	}else{
		draw_Rectangle(food_meal.x+8,food_meal.y+8,23,8,Black);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+9,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+14,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+19,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+23,2,6,Red);
		draw_colored_Rectangle(food_meal.x+9,food_meal.y+28,2,6,Red);
	}
}

void clear_meal(void){
	draw_colored_Rectangle(food_meal.x,food_meal.y,31,16,White);
}

void meal_animation(int left){
	   
		 wait_for_completion = 1;
	   disable_timer(2);
	   left_animation = left;
	   draw_meal();
	   enable_timer(1);
		 return;
}

int get_wait(void){
	return wait_for_completion;
}

void stop_animation(void){
	wait_for_completion = 0;
	enable_timer(2);
	disable_timer(1);
}

void draw_speaker(void){
	draw_element(speaker.p.x,speaker.p.y,17,14,cassa);
}

void refresh_volume(void){
		do{
			current_volume--;
			draw_colored_Rectangle(speaker.p.x + speaker.r.l + 2 + 5*(current_volume), speaker.p.y - (current_volume)*2, speaker.r.h + (current_volume)*4, 2, White);
		}while(current_volume >= volume);
		while(current_volume < volume){
			draw_colored_Rectangle(speaker.p.x + speaker.r.l + 2 + 5*(current_volume), speaker.p.y - (current_volume)*2, speaker.r.h + (current_volume)*4, 2, Black);
			current_volume++;
		}	
}

int get_volume(void){
	return current_volume;
}

void init_grapich(void){
	int i;
	//Initialize batteries
	battery_left.p.x = 40;
	battery_left.p.y = 70;
	battery_right.p.x = 160;
	battery_right.p.y = battery_left.p.y;
	//Initialize meal button
	meal.p.x = 1;
	meal.p.y = 255;
	meal.r.h = 62;
	meal.r.l=(MAX_L-2)/2 - 1;
	meal.r.color = Black;
	meal_text.x = 45;
	meal_text.y = 275;
	//Initialize snack button
	snack.p.x = meal.p.x + meal.r.l;
	snack.p.y = meal.p.y;
	snack_text.x = 160;
	snack_text.y = meal_text.y;
	snack.r = meal.r;
	//Initiazlize battery texts
	happ_text.x = 32;
	happ_text.y = 50;
	sat_text.x = 155;
	sat_text.y = 50;
	//Initialize tamagochi elements
	babbo_face.p.x = 95;
	babbo_face.p.y = 155;
	babbo_face.r.h = 30;
	babbo_face.r.l = 50;
	babbo_face.r.color = RGB565CONVERT(0xFF,0xCC,99);
	
	//initialize meal pointer 
	food_meal.x = 40;
	food_meal.y = 200;
	
	//initialize cuddles_heart
	cuddles_heart.x = 185;
	cuddles_heart.y = babbo_face.p.y;
	
	//iniialize speaker pointer
	speaker.p.x = 10;
	speaker.p.y = 15;
	speaker.r.h = 17;
	speaker.r.l = 14;
	
	//drawing the initial state
	//drawing batteries
	draw_Battery(battery_left.p.x, battery_left.p.y);
	draw_Battery(battery_right.p.x, battery_right.p.y);
	GUI_Text(happ_text.x, happ_text.y, (uint8_t *) "Happiness", Black, White);
	GUI_Text(sat_text.x, sat_text.y,(uint8_t *) "Satiety", Black, White);
	
	//drawing tamagochi
	reset_face();
	//drawing buttons
	draw_Rectangle(meal.p.x, meal.p.y, meal.r.h, meal.r.l, meal.r.color);
	draw_Rectangle(snack.p.x, snack.p.y, snack.r.h, snack.r.l, snack.r.color);
	GUI_Text(meal_text.x, meal_text.y, (uint8_t *) "Meal", Black, White);
	GUI_Text(snack_text.x, snack_text.y, (uint8_t *) "Snack", Black, White);
	
	draw_speaker();
	for(i=0; i<3;i++)
			draw_colored_Rectangle(speaker.p.x + speaker.r.l + 2 + 5*i, speaker.p.y - i*2, speaker.r.h + i*4, 2, Black);
			
	joystick_init();
	ADC_init();
	
	init_timer(0, 25000000);
	enable_timer(0);
	
	init_RIT(0x004C4B40); //50 ms
	enable_RIT();
	
	init_timer(1, 8350000);
	
  init_timer(2, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */ //for touch panel
	enable_timer(2);
	
}







