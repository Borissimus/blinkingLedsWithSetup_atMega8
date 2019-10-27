/*
 * GccApplication1.c
 *
 * Created: 10.10.2019 12:05:05
 * Author : Borissimus
 */ 
#include "libs/delay.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_CPU 8000000L

#define FREQ_MIN 0.1
#define FREQ_MAX 10
#define HALF_PERIOD_MAX 5000

float vds_freq[6]  = {1, 1, 1, 1, 1, 1}; 
int half_periods_vd[6] = {0, 0, 0, 0, 0, 0};
int half_period_counter = 0;

void calc_half_periods(void){
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		half_periods_vd[i] = 1 / vds_freq[i] * 1000 / 2; 
	}
}

void start_setup(void){
	while(bit_is_clear(PINC, PC0)) _delay_ms(100);
	PORTB = 0xff;
	int current_vd = 1;
	int time_out = 6000;
	int time_out_count = 0;
	while (1){
		if(bit_is_clear(PINC, PC0)){
			_delay_ms(50);
			if(bit_is_clear(PINC, PC0)){
				if(current_vd < 6) current_vd++;
				else current_vd = 1;
				time_out = 0;
				PORTB = 0xff; 
			}
			_delay_ms(100);
		}
		if(bit_is_clear(PINC, PC1)){
			_delay_ms(50);
			if(bit_is_clear(PINC, PC1)){
				vds_freq[current_vd - 1] = vds_freq[current_vd - 1] * 2;
				if(vds_freq[current_vd - 1] > FREQ_MAX) vds_freq[current_vd - 1] = FREQ_MIN;
				calc_half_periods();
				time_out = 0;
			}
			_delay_ms(100);
		}
		if(bit_is_clear(PINC, PC2)){
			_delay_ms(50);
			if(bit_is_clear(PINC, PC2)){
				PORTB = 0xff;
				return;
			}
		}
		if(++half_period_counter == HALF_PERIOD_MAX) half_period_counter = 0;
		
		if(!(half_period_counter % half_periods_vd[current_vd - 1])) {toggle_vd(current_vd);}
		time_out_count++;
		if (time_out_count == time_out) break;
		_delay_ms(1); 
		}
}

void toggle_vd(int diode_num){
	switch(diode_num){
		case 1:
		if (bit_is_clear(PINB, PB0)) PORTB |= (1 << PB0);
		else PORTB &= ~(1 << PB0);
		break;
		case 2:
		if (bit_is_clear(PINB, PB1)) PORTB |= (1 << PB1);
		else PORTB &= ~(1 << PB1);
		break;
		case 3:
		if (bit_is_clear(PINB, PB2)) PORTB |= (1 << PB2);
		else PORTB &= ~(1 << PB2);
		break;
		case 4:
		if (bit_is_clear(PINB, PB3)) PORTB |= (1 << PB3);
		else PORTB &= ~(1 << PB3);
		break;
		case 5:
		if (bit_is_clear(PINB, PB4)) PORTB |= (1 << PB4);
		else PORTB &= ~(1 << PB4);
		break;
		case 6:
		if (bit_is_clear(PINB, PB5)) PORTB |= (1 << PB5);
		else PORTB &= ~(1 << PB5);
		break;
	}
}

int main(void){
	//configure the ports 
	DDRB = 0xff;
    PORTB = 0xff;
	DDRC = 0x00;
	PORTC = 0b00000111;
	
	calc_half_periods();
	
	while (1){
		half_period_counter++;
		
		if(!(half_period_counter % half_periods_vd[0])) {toggle_vd(1);}
		if(!(half_period_counter % half_periods_vd[1])) {toggle_vd(2);}
		if(!(half_period_counter % half_periods_vd[2])) {toggle_vd(3);}
		if(!(half_period_counter % half_periods_vd[3])) {toggle_vd(4);}
		if(!(half_period_counter % half_periods_vd[4])) {toggle_vd(5);}
		if(!(half_period_counter % half_periods_vd[5])) {toggle_vd(6);}					
		
		_delay_ms(1);
		
		if(half_period_counter == HALF_PERIOD_MAX) {half_period_counter = 0;}
		//start setup
		if (bit_is_clear(PINC, PC0)){
			_delay_ms(1000);
			if(bit_is_clear(PINC, PC0)) {start_setup();}
		}
    }
}

