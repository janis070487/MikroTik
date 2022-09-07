/*
 * MikroTik.c
 *
 * Created: 07.09.2022 21:02:33
 * Author : janis
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

//_________ LED ____________________________
int ledStatus = 1;
int timeLED = 0;
int _pause = 500;
char dir = 1;
//_____________ Poga Dir ___________________
int dirTimeBtnPause =  0;
char dirBtnFlage = 1;
char dirTimeBtnStart = 0;
char dirButtonState = 1;
//_____________ Poga Add time ______________
int timeAddBtnPause = 0;
char timeAddBtnFlage = 1;
char timeAddBtnStart = 0;
char timeAddButtonState = 1;
//_____________ Poga Del time ______________
int timeDelBtnPause = 0;
char timeDelBtnFlage = 1;
char timeDelBtnStart = 0;
char timeDelButtonState = 1;

void init();

ISR(TIMER1_COMPA_vect)
{
	timeLED++;
	if(dirTimeBtnStart == 1) dirTimeBtnPause++;
	if(timeAddBtnStart == 1) timeAddBtnPause++;
	if(timeDelBtnStart == 1) timeDelBtnPause++;
}



int main(void)
{
	init();
	
	while (1)
	{
		
		if(timeLED > _pause)
		{
			Led();
			timeLED = 0;
		}
		//_____________ Buton Read ___________________________
		dirButtonState = (PINC & 1);
		timeAddButtonState = (PINC & 2);
		timeDelButtonState = (PINC & 4);
		
		//_____________ Pogas apstrade "Virziena maiòa" ________________________
		if((dirBtnFlage == 1) && (dirButtonState == 0))  // ja flage ir 1 un poga nospiesta
		{
			dirTimeBtnStart = 1;
			if(dirTimeBtnPause > 50)
			{
				dirTimeBtnStart = 0;
				dirTimeBtnPause = 0;
				dirBtnFlage = 0;
				ChangeDirection();
			}
		}else
		{
			if(dirButtonState == 1) dirBtnFlage = 1;
			dirTimeBtnPause = 0;
		}
		//_____________ Pogas apstrade "Pauzes palielinâðana" ________________________
		if((timeAddBtnFlage == 1) && (timeAddButtonState == 0))  // ja flage ir 1 un poga nospiesta
		{
			timeAddBtnStart = 1;
			if(timeAddBtnPause > 50)
			{
				timeAddBtnStart = 0;
				timeAddBtnPause = 0;
				timeAddBtnFlage = 0;
				ChangePause(1);
			}
		}else
		{
			if(timeAddButtonState == 2) timeAddBtnFlage = 1;
			timeAddBtnPause = 0;
		}
		//_____________ Pogas apstrade "Pauzes samazinâðana" ________________________
		if((timeDelBtnFlage == 1) && (timeDelButtonState == 0))  // ja flage ir 1 un poga nospiesta
		{
			timeDelBtnStart = 1;
			if(timeDelBtnPause > 50)
			{
				timeDelBtnStart = 0;
				timeDelBtnPause = 0;
				timeDelBtnFlage = 0;
				ChangePause(0);
			}
		}else
		{
			if(timeDelButtonState == 4) timeDelBtnFlage = 1;
			timeDelBtnPause = 0;
		}
		//______________________________________________________________________
		
	}
}
//__________________________________ FUNKCIJAS _______________________________
//_____________________________________________________________________________

//____________________________________ iNIT _______________________________________
void init()
{
	DDRD = 255;
	DDRB = 0;
	DDRC = 0;
	PORTD = 1;   //???????????????
	DDRB |= (1);
	DDRB |= (1 << 1);
	DDRC &= ~(1);
	DDRC &= ~(1 << 1);
	
	cli();
	
	TCCR1A = 0;
	TCCR1B = 0;
	PORTC = 0b00000111;         // pogu ieejas pievelkam pie barosanas
	OCR1A = 1000;              // salidzinasanai
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1);
	TIMSK1 |= (1 << OCIE1A);
	
	sei();
}
//___________________________________________ Led ___________________________________

void Led()
{
	if(dir == 1)
	{
		
		ledStatus = (ledStatus << 1);
		if(ledStatus > 512)
		{
			ledStatus = 1;
		}
		PORTD = ledStatus;
		PORTB = (ledStatus >> 8);
	}
	
	if(dir == 0)
	{
		ledStatus = (ledStatus >> 1);
		if(ledStatus == 0)
		{
			ledStatus = 512;
		}
		PORTD = ledStatus;
		PORTB = (ledStatus >> 8);
	}
	
}

void ChangeDirection()
{
	if(dir == 1)
	{
		dir = 0;
	}else
	{
		dir = 1;
	}
}
//___________________________________________ Led spidesanas laika maina ___________________________________
void ChangePause(char AddOrDel)
{
	if(AddOrDel == 1)
	{
		_pause += 50;
		if(_pause > 3000) _pause = 3000;
	}else
	{
		_pause -= 50;
		if(_pause < 50) _pause = 50;
	}
}

