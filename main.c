/*

	Demo of glcd library with AVR8 microcontroller
	
	Tested on a custom made PCB (intended for another project)

	See ../README.md for connection details

*/

#include <avr/io.h>
#include "glcd/glcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "glcd/fonts/Liberation_Sans15x21_Numbers.h"
#include "glcd/fonts/font5x7.h"
#include <avr/pgmspace.h>
#include <util/delay.h>


#define TASTER_R !(PIND&(1<<PD6)) && (entprell==0)
#define TASTER_G !(PIND&(1<<PD5)) && (entprell==0)
#define TASTER_B !(PIND&(1<<PD2))
#define F_CPU 16000000UL  // 1 MHz

#define UP 0
#define DOWN 1
#define LINKS 0
#define RECHTS 1
/* Function prototypes */

volatile uint8_t durchlauf_y=0;
volatile uint8_t durchlauf_x=0;
volatile uint8_t sek=0;
volatile uint8_t min=0;
volatile uint8_t h=0;
uint8_t y_ball=42;
uint8_t x_ball=42;
uint8_t y_recht=45;
uint8_t x_recht=34;
uint8_t x_richtung=0; //0=up/1=down
uint8_t y_richtung=0; //0=l/1=r
uint8_t taster_rot=0;
uint8_t taster_grun=0;
uint8_t taster_blau=0;
uint8_t entprell=0;
uint8_t beginn=0;
uint8_t leben=3;
uint8_t score=0;
uint8_t speed_y=4;
uint8_t speed_x=0;
uint8_t refresh_balk=1;
uint8_t refresh_ball=0;
uint8_t block1=1;
uint8_t block2=1;
uint8_t block3=1;
uint8_t block4=1;
uint8_t block5=1;
uint8_t schreiben1=0;
uint8_t schreiben2=0;
uint8_t schreiben3=0;
uint8_t schreiben4=0;
uint8_t schreiben5=0;

char string[10]={};

ISR(TIMER0_OVF_vect)
{
	static uint8_t ISR_zaehler=0;
	static uint8_t ISR_ms_100=0;
	static uint8_t ISR_sek=0;
	static uint8_t ISR_min=0;
	static uint8_t ISR_h=0;

	
	TCNT0 = 0;
	ISR_zaehler++;//zähler hochrechnen
	durchlauf_y++;
	durchlauf_x++;
	
	if(durchlauf_y>=speed_y)	
	{
		durchlauf_y=0;
		if(speed_y==0)
		{
			speed_y=1;
		}
			
			if(y_richtung==UP)
			{
				y_ball--;
			}else y_ball++;
	}
	
	if(durchlauf_x>=speed_x)	
	{
		durchlauf_x=0;
		if(speed_x!=0)
		{
			if(x_richtung==LINKS)
			{
				x_ball--;
			}else x_ball++;
		}
	}
	
	
	
	if(entprell!=0)
		{
			entprell--;
		}
	
	if(ISR_zaehler==12)
	{
		ISR_ms_100++; //100 milisekunden hochrechnen
		
		if(ISR_ms_100==10)
		{
			sek++; //sekunden hochrechnen
			ISR_sek++;
			ISR_ms_100=0; //milisekunden auf 0 setzen
		}
		
		if(ISR_sek==60)
		{
			min++; //minuten hochrechnen
			ISR_min++;
			ISR_sek=0; //sekunden auf 0 setzen
			sek=0;
		}
		
		if(ISR_min==60)
		{
			h++; //stunden hochrechnen
			ISR_h++;
			ISR_min=0; //minuten auf 0 setzen
			min=0;
		}
		
		ISR_zaehler=0; //zähler auf 0 setzen
	}
	
}

static void setup(void);

static void setup(void)
{
	/* Set up glcd, also sets up SPI and relevent GPIO pins */
	glcd_init();
}

//uint8_t ms, ms10,ms100,sec,min,entprell, state;



const unsigned char batman[] PROGMEM= 
{ 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 
	0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 
	0xff, 0xe7, 0x83, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x80, 0xc0, 0xfe, 0xfc, 0xf8, 0xf8, 0xfc, 0xfe, 0xc0, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x83, 
	0xe7, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfc, 0xfc, 
	0xf8, 0xf8, 0xf8, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 
	0x70, 0x78, 0x3c, 0x3e, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 
	0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3e, 0x3c, 0x78, 0x70, 
	0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0f, 
	0x3f, 0x7f, 0x3f, 0x3f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x3f, 0x0f, 0x03, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x3f, 0xff, 0xff, 0x3f, 
	0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
	};
	
uint8_t taster(uint8_t tast_nr) // Flankenerkennung
{
	static uint8_t pegelg=0;		//Variabeln initialisieren
	static uint8_t pegelaltg=0;
	static uint8_t pegelr=0;
	static uint8_t pegelaltr=0;
	uint8_t rueckgabe=0;

	switch(tast_nr)
	{
		case 1: 	if(TASTER_R) //taster gedrückt
					{
						pegelr=1;
						
						if(pegelaltr==0)
						{
							rueckgabe=1;
						}
					}
					else
					{
						pegelr=0;
						rueckgabe=0;
					}
						
						
						
					pegelaltr=pegelr;
					break;
					
		case 2:		if(TASTER_G) //taster gedrückt
					{
						pegelg=1;
						
						if(pegelaltg==0)
						{
							rueckgabe=1;
						}
					}
					else
					{
						pegelg=0;
						rueckgabe=0;
					}
						
						
						
					pegelaltg=pegelg; // Beide Pegel gleichsetzen
					break;
	}
	return rueckgabe; //rueckgabe wird zurückgegeben
}







void block_1(uint8_t x1, uint8_t y1)
{
	const static uint8_t block_1x=10;
	const static uint8_t block_1y=1;
	
	if(schreiben1==0)
		{ 
			schreiben1=1;
			glcd_fill_rect(10, 1, 16, 5, BLACK);
			block1=1;
		}
		
	if(block1==1)
	{		
		if((y1==block_1y+3) && (x1>=block_1x) && (x1<=block_1x+16))
		{
			block1=0;
			y_richtung=	DOWN;
			glcd_fill_rect(block_1x, block_1y, 16, 5, WHITE);
			score++;
		}
	}
}


void block_2(uint8_t x2, uint8_t y2)
{
	const static uint8_t block_2x=36;
	const static uint8_t block_2y=1;
	
	if(schreiben2==0)
	{ 
		schreiben2=1;
		glcd_fill_rect(36, 1, 16, 5, BLACK);
		block2=1;
	}
	
	if(block2==1)
	{
		if((y2==block_2y+3) && (x2>=block_2x) && (x2<=block_2x+16))
		{
			block2=0;
			y_richtung=	DOWN;
			glcd_fill_rect(block_2x, block_2y, 16, 5, WHITE);
			score++;
		}
	}
}


void block_3(uint8_t x3, uint8_t y3)
{
	const static uint8_t block_3x=68;
	const static uint8_t block_3y=1;
	
	if(schreiben3==0)
	{ 
		schreiben3=1;
		glcd_fill_rect(68, 1, 16, 5, BLACK);
		block3=1;
	}
	
	if(block3==1)
	{
		if((y3==block_3y+3) && (x3>=block_3x) && (x3<=block_3x+16))
		{
			block3=0;
			y_richtung=	DOWN;
			glcd_fill_rect(block_3x, block_3y, 16, 5, WHITE);
			score++;
		}
	}
}


void block_4(uint8_t x4, uint8_t y4)
{
	const static uint8_t block_4x=18;
	const static uint8_t block_4y=7;
	
	if(schreiben4==0)
	{ 
		schreiben4=1;
		glcd_fill_rect(18, 7, 16, 5, BLACK);
		block4=1;
	}
	
	if(block4==1)
	{
		if((y4==block_4y+3) && (x4>=block_4x) && (x4<=block_4x+16))
		{
			block4=0;
			y_richtung=	DOWN;
			glcd_fill_rect(block_4x, block_4y, 16, 5, WHITE);
			score++;
		}
	}
}


void block_5(uint8_t x5, uint8_t y5)
{
	const static uint8_t block_5x=44;
	const static uint8_t block_5y=7;
	
	if(schreiben5==0)
	{ 
		schreiben5=1;
		glcd_fill_rect(44, 7, 16, 5, BLACK);
		block5=1;
	}
	
	if(block5==1)
	{
		if((y5==block_5y+3) && (x5>=block_5x) && (x5<=block_5x+16))
		{
			block5=0;
			y_richtung=	DOWN;
			glcd_fill_rect(block_5x, block_5y, 16, 5, WHITE);
			score++;
		}
	}
}









	
	
int main(void)
{	

	DDRB=0xFF; //Ganzer Port B als Ausgang 
	PORTB=0x00; //Alle LEDs ausschalten

	TCCR0A		= 0x00; 			//Timer Konfigurieren
	TCCR0B		= 0x04; 			//Timer Konfigurieren
	TIMSK0		|= (1 << TOIE0);	//Timer Konfigurieren
	TIFR0	|= (1 << TOV0);			//Timer Konfigurieren
	sei();
	
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	PORTB |= (1<<PB2);
	
	DDRC |= (1<<PC3);
	PORTC |= (1<<PC3);
	
	DDRD &= ~(1<<PD5);
	PORTD |= (1<<PD5);
	
	DDRD &= ~(1<<PD6);
	PORTD |= (1<<PD6);
	
	DDRD |= (1<<PD4);
	PORTD |= (1<<PD4);

    sei();
    // enable interrupts
	
	setup();
	
	glcd_clear();
	glcd_write();
	
	
		
	// Display
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
	
	
	
	
	while(1) 
	{
		
//		if(TASTER_B)													//Anfang Taster abfragen
//		{
//			taster_blau=1;
//			entprell=2;

//		}
		
		if(TASTER_G)													//Anfang Taster abfragen
		{
			taster_grun++;
			entprell=2;

		}
		
		if(TASTER_R)
		{
			taster_rot++;
			entprell=2;

		}																//Ende Taster Abfragen
		
		
		if(taster_grun>0)												//Anfang Tasteraktion bestimmen
		{
			refresh_balk=1;
			x_recht++;
			
			if(x_recht>68)
			{
				x_recht=68;
			}
		}
		
		if(taster_rot>0)
		{
			refresh_balk=1;
			x_recht--;
			
			if(x_recht<1)
			{
				x_recht=1;
			}
		}																//Ende Tasteraktion bestimmen
		
		
		
		

		if(x_ball==3)													//Anfang Ballrichtung Festlegen / Anfang Ball
		{
			x_richtung=RECHTS;
		}
				
		if(x_ball==81)
		{
			x_richtung=LINKS;
		}
				
			if(y_ball==3)
			{
				x_richtung=x_richtung;
				y_richtung=DOWN;
			}
			
			if((y_ball==42) && (x_ball>x_recht) && (x_ball<x_recht+16))
			{
				
			if((x_ball>x_recht) && (x_ball<x_recht+4))
			{
				speed_x=4;
				speed_y=7;
				x_richtung=LINKS;
			}
				
			if((x_ball>x_recht+4) && (x_ball<x_recht+8))
			{
				speed_x=6;
				x_richtung=LINKS;
			}
				
			if((x_ball>x_recht+8) && (x_ball<x_recht+12))
			{
				speed_x=6;
				x_richtung=RECHTS;
			}
				
			if((x_ball>x_recht+12) && (x_ball<x_recht+16))
			{
				speed_x=4;
				speed_y=7;
				x_richtung=RECHTS;
			}
					
				y_richtung=UP;
		}															//Ende Ballrichtung festlegen
			
			
			
		
		
		if((y_richtung==DOWN) && (y_ball!=42))							//Anfang Ball schreiben
		{
			glcd_fill_circle(x_ball, y_ball-1, 3, WHITE);
		}
		
		
		if((y_richtung==UP) && (y_ball!=42))
		{
			glcd_fill_circle(x_ball, y_ball+1, 3, WHITE);
		}
		
		
		if(x_richtung==LINKS)
		{
			glcd_fill_circle(x_ball+1, y_ball, 3, WHITE);
		}
		
		if(x_richtung==RECHTS)
		{
			glcd_fill_circle(x_ball-1, y_ball, 3, WHITE);
		}
		
		glcd_fill_circle(x_ball, y_ball, 3, BLACK);						//Ende Ball schreiben
		
		
		
		
		
		if(y_ball==47)
		{
			beginn=1;
			leben--;
		
			if(score==0)
			{
//				glcd_draw_string_xy(20, 24, "GAME OVER");
				score=3;
			}
		}
		
		
		if(beginn==1)
		{
			glcd_fill_circle(x_ball, y_ball-1, 3, WHITE);
			y_ball=0;
			beginn=0;
			speed_x=0;
		}															
			
		if(y_ball>=41)
		{
			refresh_balk=1;
		}															//Ende Ball
			
			
			
		block_1(x_ball, y_ball);									// Anfang Blöcke
		block_2(x_ball, y_ball);
		block_3(x_ball, y_ball);
		block_4(x_ball, y_ball);
		block_5(x_ball, y_ball);
		
		
		if((block1==0) && (block2==0) && (block3==0) && (block4==0) && (block5==0))
		{
			schreiben1=0;
			schreiben2=0;
			schreiben3=0;
			schreiben4=0;
			schreiben5=0;
			leben=3;
			x_ball=42;
			y_ball=42;
		}
			
		if(leben==0)
		{
			schreiben1=0;
			schreiben2=0;
			schreiben3=0;
			schreiben4=0;
			schreiben5=0;
			leben=3;
			x_ball=42;
			y_ball=42;
		}
			
		if(refresh_balk==1)												//Anfang Balkenbewegungsabfrage
		{
			if(taster_grun>0)
			{
				glcd_fill_rect(x_recht-1, y_recht, 16, 2, WHITE);
			}
			
			if(taster_rot>0)
			{
				glcd_fill_rect(x_recht+1, y_recht, 16, 2, WHITE);
			}
			
			else
			{
				glcd_fill_rect(x_recht-1, y_recht, 16, 2, WHITE);
			}
			
			glcd_fill_rect(x_recht, y_recht, 16, 2, BLACK);
			
//			glcd_fill_circle(x_ball, y_ball-1, 3, WHITE);
			taster_grun=0;
			taster_rot=0;
			refresh_balk=0;
		}					 											//Ende Balkenbewegungsabfrage
	
	
	
		if(y_ball>=41)
			{
				refresh_balk=1;
			}
		
		sprintf(string,"%d", leben);
		glcd_draw_string_xy(1, 1, string);
		
		glcd_write();
		
		
		PORTD ^= (1<<PD4);
		
	}//End of while
	
	//---------------------------------------------
	
	return 0;
}//end of main
