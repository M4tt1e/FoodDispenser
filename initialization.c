#include <avr/io.h>
#include <avr/interrupt.h>

#define MANUAL 1
#define AUTO 0

void initialize(void);


void initialize(void){
	/*
	* Configuration for both interrupt buttons
	*/
	DDRD = 0xFA;// interrupt pin2 set as input and pin0 as input for (manual/auto) control button and rest as output for LEDS
	PORTD = 0x05; //pull up for interrupt and manual/auto button
	
	EICRA = (0<<1)&(0<<0); //Low level activated	(feed dog button)
	EIMSK = (1<<INT0);
	
	PCMSK2 = (1<<PCINT16);//enable pin interrupt
	PCICR = (1<<PCIE2)|(1<<PCIE0); // both mode button and PIR sensor
	/*
	* Configuration for PIR and Buzzer
	*/
	DDRB = 0x02; //interrupt PIR sensor pin0 set as input and pin1 as output for buzzer
	PORTB = 0x03; // interrupt PIR pin0 pulled up and pin1 for buzzer
	
	PCMSK0 = (1<<PCINT0);//enable pin interrupt (PIR)
	
	/*
	* Configuration for Stepper Motor
	*/
	DDRC = 0xFF; //set portC as output (motor)
	
	sei(); //Enable interrupts
}
