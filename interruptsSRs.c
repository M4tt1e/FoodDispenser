#include <avr/io.h>
#include <avr/interrupt.h>

#define MANUAL 1
#define AUTO 0

#define FIRST 1
#define SECOND 2
#define COOLDOWN 3

#define  COOLDOWN_TIME 32000

volatile char operating_mode = MANUAL;
volatile char flag_dir_pir = FIRST;
volatile int timer_cool_down = 0;
volatile char count = 0;

/*Display bits*/
volatile char first_bit;
volatile char second_bit;
volatile char third_bit;
volatile char fourth_bit;
volatile char fifth_bit;
volatile char sixth_bit;

void display(void);

void display(void){
	first_bit = count&0b00000001;
	second_bit = (count>>1)&0b00000001;
	third_bit = (count>>2)&0b00000001;
	fourth_bit = (count>>3)&0b00000001;
	fifth_bit = (count>>4)&0b00000001;
	sixth_bit = (count>>5)&0b00000001;
	PORTD = (first_bit<<PORTD1)|(second_bit<<PORTD3)|(third_bit<<PORTD4)|(fourth_bit<<PORTD5)|(fourth_bit<<PORTD6)|(fifth_bit<<PORTD7);
}



//External interrupt on portD2
ISR(INT0_vect){
	if(operating_mode==MANUAL){
		for(char i=0;i<20;i++){ //Perform two full cycles
			if(i<10){
				stepper_move(CLOCKWISE);
			}else{
				stepper_move(COUNTERCLOCKWISE);
			}
		}
		buzz(15);
		count++;
	}
}

//External pin interrupt on portB0 (PIR sensor)
ISR(PCINT0_vect){
	if(operating_mode==AUTO){
		for(char i=0;i<10;i++){
			if(flag_dir_pir==FIRST){
				stepper_move(CLOCKWISE);
			}else if(flag_dir_pir==SECOND){
				stepper_move(COUNTERCLOCKWISE);
			}
		}
		flag_dir_pir++;
		if(flag_dir_pir==COOLDOWN){
			count++;
			PCMSK0 = (0<<PCINT0); //Deactivate itself
			buzz(15);
	
			// Set Timer 1 and interrupt
			TCCR1B =(1<<CS11); // Start timer. Set clk/8 prescaler for timer1
			TIMSK1 = (1<<TOIE1); // Enable timer 1 overflow interrupt
		}
	}
}

//External pin interrupt pcint16 on portD0 (mode button)
ISR(PCINT2_vect){
	if((PIND&0x01)==0x00){
		operating_mode = MANUAL;
		PCMSK0 = (0<<PCINT0);//disable pin interrupt (PIR)
		}else if((PIND&0x01)==0x01){
		PCMSK0 = (1<<PCINT0); //enable pin interrupt (PIR)
		operating_mode = AUTO;
	}
	buzz(1);
}


ISR(TIMER1_OVF_vect){
	timer_cool_down++;
	if(timer_cool_down>=COOLDOWN_TIME){ //Approximately 20minutes.
		
		TCCR1B =(0<<CS11); // Disable timer. Set clk/8 prescaler for timer1
		TIMSK1 = (0<<TOIE1); // Disable timer 1 overflow interrupt
		
		buzz(50); //Let know of PIR activation
		
		flag_dir_pir = FIRST;
		PCMSK0 = (1<<PCINT0); //enable pin interrupt (PIR)
		
		timer_cool_down = 0; //Reset timer
	}
}