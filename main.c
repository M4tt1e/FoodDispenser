/*
 * steppermotor.c
 *
 * Created: 27.4.2022 13:33:29
 * Author : Mateusz Edwardsson
 */ 

#include <avr/io.h>

#include "stepper.c"
#include "buzzer.c"
#include "initialization.c"
#include "interruptsSRs.c"


int main(void){
	initialize();

	while(1){
		display();
	}
	return 0;
}

