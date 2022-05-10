#include <avr/io.h>
	
	
typedef enum {
		COUNTERCLOCKWISE, CLOCKWISE
} StepperDirection_Type;
	
void stepper_move(StepperDirection_Type direction);
	
void stepper_move(StepperDirection_Type direction){	
	int start[] = {0, 3};
	int end[] = {4, 0xFF}; //in clockwise when i=0 was executed i is changed to 0xFF not -1
	int step[] = {1, -1};

	
	for(char u=0; u<=51; u++){ //approximately 36 degrees (10% of a circle)
		for(volatile char i=start[direction]; i!=end[direction]; i+=step[direction]){
			PORTC = (1<<i);
			for(int d=0;d<6000;d++){} //delay as the motor requires at least 2ms delay between steps 
		}
	}
}