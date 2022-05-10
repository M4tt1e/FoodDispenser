#include <avr/io.h>

void buzz(unsigned char L);
void wait(unsigned char N);

void wait(unsigned char N)
{
	TCNT0=0xFF-N; //Load timer initial value.
	TCCR0B=(1<<CS02)|(1<<CS00); //Start timer. Set clk/1024 prescaler
	while((TIFR0 & (1<<TOV0))==0 ); //Wait for timer to roll over
	TCCR0B=0; //Stop timer
	TIFR0=(1<<TOV0); //Clear overflow flag
	return;
}

/*
********* Takes the amount of buzzes to perform
*/
void buzz(unsigned char L){
	for(char i=0;i<L;i++){
		wait(255);
		PORTB = (0<<1); //activate buzzer (clear bit 1)
		wait(255);
		PORTB=(1<<1);
		wait(255);
	}
}