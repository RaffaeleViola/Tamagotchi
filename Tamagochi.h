#ifndef _TAMAGOCHI_H_
#define _TAMAGOCHI_H_

#include "GLCD/GLCD.h"
typedef struct Point{
	 uint16_t x;
	 uint16_t y;	
}Point; 

typedef struct Rectangle{
	uint16_t h;
	uint16_t l;
	uint16_t color;
}Rectangle;

typedef struct Element{
	Point p;
	Rectangle r;
}Element;


void draw_Rectangle(uint16_t x0, uint16_t y0,uint16_t height,uint16_t length,uint16_t color);
void draw_Battery(uint16_t x0, uint16_t y0);
void draw_colored_Rectangle(uint16_t x0, uint16_t y0, uint16_t height, uint16_t length, uint16_t color);
void reset_call(void);
void init_grapich(void);
void draw_tamagochi(int i,int j);
void clear_tamagochi(void);
void refresh_Battery(int left,int increment);
void increment_battery(void);
int getSatiety(void);
int getHappiness(void);
void meal_animation(int left);
void stop_animation(void);
int get_wait(void);
void draw_meal(void);
void clear_meal(void);
void reset_face(void);
void reset_click(void);
int on_Reset(void);
void set_wait(int wait);
void cuddles_animation(void);
void stop_cuddles_animation(void);
int on_cuddles_animation(void);
void draw_heart(void);
void clear_heart(void);
void refresh_volume(void);
int get_volume(void);
#endif

