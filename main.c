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
#define TASTER_B !(PIND&(1<<PD2)) && (entprell==0)
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
uint8_t speed_y=0;
uint8_t speed_x=0;
uint8_t refresh_balk=1;
uint8_t refresh_ball=0;
uint8_t block1=0;
uint8_t block2=0;
uint8_t block3=0;
uint8_t block4=0;
uint8_t block5=0;
uint8_t block6=0;
uint8_t schreiben1=1;
uint8_t schreiben2=1;
uint8_t schreiben3=1;
uint8_t schreiben4=1;
uint8_t schreiben5=1;
uint8_t schreiben6=1;
uint8_t start=0;
uint8_t x0=4;
uint8_t x1=4;
uint8_t x2=4;
uint8_t x2_1=4;
uint8_t x3_1=4;
uint8_t x3=4;
uint8_t x4=4;
uint8_t y0=4;
uint8_t y1_1=4;
uint8_t y2=4;
uint8_t y2_1=4;
uint8_t y3_1=4;
uint8_t y3=4;
uint8_t y4=4;
uint8_t win=0;
uint8_t w0=0;
uint8_t w1=0;
uint8_t w2=0;
uint8_t w3=0;
uint8_t w4=0;
uint8_t muster=0;
uint8_t ende=0;

struct balls
{
	uint8_t posx;
	uint8_t posy;
}ball_neu, ball_alt;


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
				ball_neu.posy--;
				refresh_ball=1;
			}
			else 
			{
				ball_neu.posy++;
				refresh_ball=1;
			}
	}
	
	if(durchlauf_x>=speed_x)	
	{
		durchlauf_x=0;
		if(speed_x!=0)
		{
			if(x_richtung==LINKS)
			{
				ball_neu.posx--;
				refresh_ball=1;
			}
			else 
			{
				ball_neu.posx++;
				refresh_ball=1;
			}
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
			if((y1==block_1y+7) && (x1>=block_1x-2) && (x1<=block_1x+18))
			{
				block1=0;
				y_richtung=DOWN;
				glcd_fill_rect(block_1x, block_1y, 16, 5, WHITE);
				score++;
			}
			
			if((y1==block_1y-2) && (x1>=block_1x-2) && (x1<=block_1x+18))
			{
				block1=0;
				y_richtung=UP;
				glcd_fill_rect(block_1x, block_1y, 16, 5, WHITE);
				score++;
			}
			
			if((x1==block_1x-2) && (y1>=block_1y-2) && (y1<=block_1y+7))
			{
				block1=0;
				x_richtung=LINKS;
				glcd_fill_rect(block_1x, block_1y, 16, 5, WHITE);
				score++;
			}
			
			if((x1==block_1x+18) && (y1>=block_1y-2) && (y1<=block_1y+7))
			{
				block1=0;
				x_richtung=RECHTS;
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
			if((y2==block_2y+7) && (x2>=block_2x-2) && (x2<=block_2x+18))
			{
				block2=0;
				y_richtung=DOWN;
				glcd_fill_rect(block_2x, block_2y, 16, 5, WHITE);
				score++;
			}
			
			if((y2==block_2y-2) && (x2>=block_2x-2) && (x2<=block_2x+18))
			{
				block2=0;
				y_richtung=UP;
				glcd_fill_rect(block_2x, block_2y, 16, 5, WHITE);
				score++;
			}
			
			if((x2==block_2x-2) && (y2>=block_2y-2) && (y2<=block_2y+7))
			{
				block2=0;
				x_richtung=LINKS;
				glcd_fill_rect(block_2x, block_2y, 16, 5, WHITE);
				score++;
			}
			
			if((x2==block_2x+18) && (y2>=block_2y-2) && (y2<=block_2y+7))
			{
				block2=0;
				x_richtung=RECHTS;
				glcd_fill_rect(block_2x, block_2y, 16, 5, WHITE);
				score++;
			}
		}
	}


	void block_3(uint8_t x3, uint8_t y3)
	{
		const static uint8_t block_3x=62;
		const static uint8_t block_3y=1;
		
		if(schreiben3==0)
		{ 
			schreiben3=1;
			glcd_fill_rect(62, 1, 16, 5, BLACK);
			block3=1;
		}
		
		if(block3==1)
		{
			if((y3==block_3y+7) && (x3>=block_3x-2) && (x3<=block_3x+18))
			{
				block3=0;
				y_richtung=DOWN;
				glcd_fill_rect(block_3x, block_3y, 16, 5, WHITE);
				score++;
			}
			
			if((y3==block_3y-2) && (x3>=block_3x-2) && (x3<=block_3x+18))
			{
				block3=0;
				y_richtung=UP;
				glcd_fill_rect(block_3x, block_3y, 16, 5, WHITE);
				score++;
			}
			
			if((x3==block_3x-2) && (y3>=block_3y-2) && (y3<=block_3y+7))
			{
				block3=0;
				x_richtung=LINKS;
				glcd_fill_rect(block_3x, block_3y, 16, 5, WHITE);
				score++;
			}
			
			if((x3==block_3x+18) && (y3>=block_3y-2) && (y3<=block_3y+7))
			{
				block3=0;
				x_richtung=RECHTS;
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
			if((y4==block_4y+7) && (x4>=block_4x-2) && (x4<=block_4x+18))
			{
				block4=0;
				y_richtung=DOWN;
				glcd_fill_rect(block_4x, block_4y, 16, 5, WHITE);
				score++;
			}
			
			if(block4==1)
			{	
				if((y4==block_4y-2) && (x4>=block_4x-2) && (x4<=block_4x+18))
				{
					block4=0;
					y_richtung=UP;
					glcd_fill_rect(block_4x, block_4y, 16, 5, WHITE);
					score++;
				}
			}
			
			if((x4==block_4x-2) && (y4>=block_4y-2) && (y4<=block_4y+7))
			{
				block4=0;
				x_richtung=LINKS;
				glcd_fill_rect(block_4x, block_4y, 16, 5, WHITE);
				score++;
			}
			
			if((x4==block_4x+18) && (y4>=block_4y-2) && (y4<=block_4y+7))
			{
				block4=0;
				x_richtung=RECHTS;
				glcd_fill_rect(block_4x, block_4y, 16, 5, WHITE);
				score++;
			}
		}
	}


	void block_5(uint8_t x5, uint8_t y5)
	{
		const static uint8_t block_5x=54;
		const static uint8_t block_5y=7;
		
		if(schreiben5==0)
		{ 
			schreiben5=1;
			glcd_fill_rect(54, 7, 16, 5, BLACK);
			block5=1;
		}
		
		if(block5==1)
		{
			if((y5==block_5y+7) && (x5>=block_5x-2) && (x5<=block_5x+18))
			{
				block5=0;
				y_richtung=DOWN;
				glcd_fill_rect(block_5x, block_5y, 16, 5, WHITE);
				score++;
			}
			
			if((y5==block_5y-2) && (x5>=block_5x-2) && (x5<=block_5x+18))
			{
				block5=0;
				y_richtung=UP;
				glcd_fill_rect(block_5x, block_5y, 16, 5, WHITE);
				score++;
			}
			
			if((x5==block_5x-2) && (y5>=block_5y-2) && (y5<=block_5y+7))
			{
				block5=0;
				x_richtung=LINKS;
				glcd_fill_rect(block_5x, block_5y, 16, 5, WHITE);
				score++;
			}
			
			if((x5==block_5x+18) && (y5>=block_5y-2) && (y5<=block_5y+7))
			{
				block5=0;
				x_richtung=RECHTS;
				glcd_fill_rect(block_5x, block_5y, 16, 5, WHITE);
				score++;
			}
		}
	}


	void block_6(uint8_t x6, uint8_t y6)
	{
		const static uint8_t block_6x=36;
		const static uint8_t block_6y=7;
		
		if(schreiben6==0)
		{ 
			schreiben6=1;
			glcd_fill_rect(36, 7, 16, 5, BLACK);
			block6=1;
		}
		
		if(block6==1)
		{
			if((y6==block_6y+7) && (x6>=block_6x-2) && (x6<=block_6x+18))
			{
				block6=0;
				y_richtung=DOWN;
				glcd_fill_rect(block_6x, block_6y, 16, 5, WHITE);
				score++;
			}
			
			if((y6==block_6y-2) && (x6>=block_6x-2) && (x6<=block_6x+18))
			{
				block6=0;
				y_richtung=UP;
				glcd_fill_rect(block_6x, block_6y, 16, 5, WHITE);
				score++;
			}
			
			if((x6==block_6x-2) && (y6>=block_6y-2) && (y6<=block_6y+7))
			{
				block6=0;
				x_richtung=LINKS;
				glcd_fill_rect(block_6x, block_6y, 16, 5, WHITE);
				score++;
			}
			
			if((x6==block_6x+18) && (y6>=block_6y-2) && (y6<=block_6y+7))
			{
				block6=0;
				x_richtung=RECHTS;
				glcd_fill_rect(block_6x, block_6y, 16, 5, WHITE);
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
	
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);
	
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
	
	
	ball_neu.posx=42;
	ball_neu.posy=42;
	y_richtung=UP;
	
	while(1) 
	{
		
		if(TASTER_B)													//Anfang Taster abfragen
		{
			glcd_fill_rect(15, 21, 54, 7, WHITE);
			taster_blau=1;
			entprell=2;
			refresh_ball=1;
			speed_y=4;
			w0=0;
			w1=0;
			w2=0;
			w3=0;
			w4=0;
		}
		
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
		
		
	if(taster_blau==0)
	{	
		if(start==0)
		{
			refresh_ball=1;
			refresh_balk=1;
			start=1;
		}
		ball_neu=ball_alt;
		ball_neu.posy=42;
		ball_neu.posx=x_recht+8;
	}


		
	
	switch(muster)
	{
		case 0:		if(ende>0)
					{
						if((block1==0) && (block2==0) && (block3==0) && (block4==0) && (block5==0) && (block6==0))
						{
							
							win=0;
							glcd_draw_string_xy(25, 21, "FINISH");
						}		
					}
					
					if(win==0)
					{
						x0=3;
						x1=5;
						x2=7;
						x2_1=8;
						x3_1=8;
						x3=7;
						x4=5;
						y0=15;
						y1_1=8;
						y2=5;
						y2_1=6;
						y3_1=6;
						y3=5;
						y4=8;
						w0=1;
						schreiben1=0;
						schreiben2=0;
						schreiben3=0;
						schreiben4=1;
						schreiben5=1;
						schreiben6=1;
						muster=1;
						
					}
					break;
					
		case 1:		if(win==1)
					{
						
						x0=2;
						x1=4;
						x2=6;
						x2_1=7;
						x3_1=7;
						x3=6;
						x4=4;
						y0=14;
						y1_1=7;
						y2=4;
						y2_1=5;
						y3_1=5;
						y3=4;
						y4=7;
						w1=1;
						schreiben1=0;
						schreiben2=1;
						schreiben3=0;
						schreiben4=1;
						schreiben5=1;
						schreiben6=0;
						muster=2;
						
					}
					break;
					
		case 2:		if(win==2)
					{
						
						x0=2;
						x1=3;
						x2=5;
						x2_1=6;
						x3_1=6;
						x3=5;
						x4=3;
						y0=13;
						y1_1=6;
						y2=3;
						y2_1=4;
						y3_1=4;
						y3=3;
						y4=6;
						w2=1;
						schreiben1=0;
						schreiben2=1;
						schreiben3=0;
						schreiben4=0;
						schreiben5=0;
						schreiben6=1;
						muster=3;
						
					}
					break;
					
		case 3:		if(win==3)
					{
						
						x0=1;
						x1=2;
						x2=4;
						x2_1=5;
						x3_1=5;
						x3=4;
						x4=2;
						y0=12;
						y1_1=5;
						y2=2;
						y2_1=3;
						y3_1=3;
						y3=2;
						y4=5;
						w3=1;
						schreiben1=0;
						schreiben2=0;
						schreiben3=0;
						schreiben4=0;
						schreiben5=0;
						schreiben6=0;
						muster=4;
						
					}
					
		case 4:		if(win==4)
					{
						
						x0=1;
						x1=2;
						x2=2;
						x2_1=2;
						x3_1=2;
						x3=2;
						x4=1;
						y0=20;
						y1_1=2;
						y2=1;
						y2_1=2;
						y3_1=2;
						y3=1;
						y4=20;
						w4=1;
						schreiben1=0;
						schreiben2=0;
						schreiben3=0;
						schreiben4=0;
						schreiben5=0;
						schreiben6=0;
						muster=0;
						ende++;
						
					}
	}
	
		
		
		if(w0==1)
		{
			glcd_draw_string_xy(25, 21, "LEVEL1");
		}
		
		if(w1==1)
		{
			glcd_draw_string_xy(25, 21, "LEVEL2");
		}
		
		if(w2==1)
		{
			glcd_draw_string_xy(25, 21, "LEVEL3");
		}
		
		if(w3==1)
		{
			glcd_draw_string_xy(25, 21, "LEVEL4");
		}
		
		if(w4==1)
		{
			glcd_draw_string_xy(25, 21, "LEVEL5");
		}																
		
		
		

		if((ball_neu.posx==3) && (ball_neu.posy>=3))					//Anfang Ballrichtung Festlegen / Anfang Ball
		{
			x_richtung=RECHTS;
			score++;
		}
				
		if((ball_neu.posx==81) && (ball_neu.posy>=3))
		{
			x_richtung=LINKS;
			score++;
		}
				
		if(ball_neu.posy<=3)
		{
			x_richtung=x_richtung;
			y_richtung=DOWN;
		}
			
			
		if((ball_neu.posy==42) && (ball_neu.posx>x_recht-3) && (ball_neu.posx<x_recht+19))
		{
				if((ball_neu.posx>x_recht-3) && (ball_neu.posx<x_recht))
				{
					speed_x=x0;
					speed_y=y0;
					x_richtung=LINKS;
				}
			
				if((ball_neu.posx>x_recht) && (ball_neu.posx<x_recht+4))
				{
					speed_x=x1;
					speed_y=y1_1;
					x_richtung=LINKS;
				}
				
				if((ball_neu.posx>x_recht+4) && (ball_neu.posx<x_recht+6))
				{
					speed_x=x2;
					speed_y=y2;
					x_richtung=LINKS;
				}
					
				if((ball_neu.posx>x_recht+6) && (ball_neu.posx<x_recht+8))
				{
					speed_x=x2_1;
					speed_y=y2_1;
					x_richtung=LINKS;
				}
					
				if((ball_neu.posx>x_recht+8) && (ball_neu.posx<x_recht+10))
				{
					speed_x=x3_1;
					speed_y=y3_1;
					x_richtung=RECHTS;
				}
					
				if((ball_neu.posx>x_recht+10) && (ball_neu.posx<x_recht+12))
				{
					speed_x=x3;
					speed_y=y3;
					x_richtung=RECHTS;
				}
					
				if((ball_neu.posx>x_recht+12) && (ball_neu.posx<x_recht+16))
				{
					speed_x=x4;
					speed_y=y4;
					x_richtung=RECHTS;
				}
				
				if((ball_neu.posx>x_recht+16) && (ball_neu.posx<x_recht+19))
				{
					speed_x=x0;
					speed_y=y0;
					x_richtung=RECHTS;
				}
						
					y_richtung=UP;
		}															//Ende Ballrichtung festlegen
			
			
			
		
		
		if(refresh_ball==1)
		{
			refresh_ball=0;
			glcd_fill_circle(ball_alt.posx, ball_alt.posy, 3, WHITE);
			glcd_fill_circle(ball_neu.posx, ball_neu.posy, 3, BLACK);						//Ende Ball schreiben
		
			ball_alt=ball_neu;
		}
		
		
		
		if(ball_neu.posy==47)
		{
			beginn=1;
			leben--;
			start=0;
			taster_blau=0;
			
		
//			if(score==0)
//			{
//				glcd_draw_string_xy(20, 24, "GAME OVER");
//				score=3;
//			}
		}
		
		
		if(beginn==1)
		{
			glcd_fill_circle(ball_alt.posx, ball_alt.posy, 3, WHITE);
			ball_neu.posy=0;
			beginn=0;
			speed_x=0;
		}															
			
		if(ball_neu.posy>=41)
		{
			refresh_balk=1;
		}																//Ende Ball
			
			
			
		block_1(ball_neu.posx, ball_neu.posy);							// Anfang Blöcke
		block_2(ball_neu.posx, ball_neu.posy);
		block_3(ball_neu.posx, ball_neu.posy);
		block_4(ball_neu.posx, ball_neu.posy);
		block_5(ball_neu.posx, ball_neu.posy);
		block_6(ball_neu.posx, ball_neu.posy);
		
		
		
		if(leben==0)
		{
			start=0;
			taster_blau=0;
			schreiben1=0;
			schreiben2=0;
			schreiben3=0;
			schreiben4=0;
			schreiben5=0;
			schreiben6=0;
			leben=3;
			ball_neu.posx=42;
			ball_neu.posy=42;
			win=0;
			glcd_draw_string_xy(15, 21, "GAME OVER");

			
		}
		
		if((block1==0) && (block2==0) && (block3==0) && (block4==0) && (block5==0) && (block6==0))
		{
			start=0;
			taster_blau=0;
			schreiben1=0;
			schreiben2=0;
			schreiben3=0;
			schreiben4=0;
			schreiben5=0;
			schreiben6=0;
			leben=3;
			ball_neu.posx=42;
			ball_neu.posy=42;
			win++;
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
			
//			glcd_fill_circle(ball_neu.posx, ball_neu.posy-1, 3, WHITE);
			taster_grun=0;
			taster_rot=0;
			refresh_balk=0;
		}					 											//Ende Balkenbewegungsabfrage
	
	
	
		if(ball_neu.posy>=41)
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
