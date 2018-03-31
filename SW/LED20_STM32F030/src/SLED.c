/*
 * SLED.c
 *
 *  Created on: Dec 9, 2017
 *      Author: Matej
 */
#include "stm32f0xx_conf.h"
#include "timer.h"
#include "printf.h"
#include "pin_def.h"
#include "bootloader.h"
#include "SLED.h"
#include "SW_PWM.h"


int SLED_PINS[MAX_N_OF_LEDS]={SLED0,SLED1,SLED2,SLED3,SLED4,SLED5,SLED6,SLED7,SLED8,SLED9,SLED10,SLED11,SLED12,SLED13,SLED14,SLED15,SLED16,SLED17,SLED18,SLED19};
GPIO_TypeDef * SLED_PORTS[MAX_N_OF_LEDS]={SLED0_PORT,SLED1_PORT,SLED2_PORT,SLED3_PORT,SLED4_PORT,SLED5_PORT,SLED6_PORT,SLED7_PORT,SLED8_PORT,SLED9_PORT,SLED10_PORT,SLED11_PORT,SLED12_PORT,SLED13_PORT,SLED14_PORT,SLED15_PORT,SLED16_PORT,SLED17_PORT,SLED18_PORT,SLED19_PORT};

SLED_t SLED;

t_timer main_timer,led_delay_timer,sw_delay_timer;



void SLED_init(void)
{
	SLED.n_of_led = DEFAULT_N_OF_LEDS;
	SLED.led_delay_time = DEFAULT_LED_DELAY_TIME;
	SLED.soft_start = 0;
	SLED.led_on_time= 5000;
	/*samples[0]=0xFFFFFFFF;
	samples[1]=0xFFFFFFFF;*/
}


void set_output_led(int led_n,int state)
{
	if(led_n >= MAX_N_OF_LEDS)return;
	if(state) //on
	{
		SLED_PORTS[led_n]->ODR |= SLED_PINS[led_n];  //SLED_PINS[led_n]
	}
	else//off
	{
		SLED_PORTS[led_n]->ODR &= ~(SLED_PINS[led_n]);
	}
}

void sample_inputs(void)
{
	int i,cnt[2];
	static unsigned long samples[2];
	samples[0]<<=1;
	samples[0] |= !INP0_READ();
	samples[1]<<=1;
	samples[1] |= !INP1_READ();

	cnt[0]=0;
	cnt[1]=0;
	for(i=0;i<30;i++)
	{
		if(samples[0]&(1<<i)) cnt[0]++;
		if(samples[1]&(1<<i)) cnt[1]++;
	}
	if(cnt[0]>20)SLED.inp_up = 1;
	else SLED.inp_up = 0;
	if(cnt[1]>20)SLED.inp_down = 1;
	else SLED.inp_down = 0;
}

void set_led_state(int led_n,int offon)
{
	if(SLED.soft_start)
	{

	}
	else
	{
		if(offon)set_pwm(led_n,MAX_PWM);
		else set_pwm(led_n,0);
	}


}
void set_all_led_state(int offon)
{
	int i;
	for(i=0;i<SLED.n_of_led;i++)
	{
		set_led_state(i,offon);
	}
}

void SLED_main(void)
{
	static t_timer input_sample_timer;
	if(timeout(&input_sample_timer,6))
	{
		sample_inputs();
	}
	if(SLED.program == SLED_PROG_SERIAL)
	{
		if(timeout(&main_timer,5))
		{
		switch(SLED.sled_state)
		{
		case SLED_IDLE:
			if(SLED.inp_up)
			{
				//printf("\n sled up");
				SLED.sled_state=SLED_PROG;
				SLED.led_dir = SLED_DIR_UP;
				SLED.led_cnt=0;
			}
			if(SLED.inp_down)
			{
				//printf("\n sled down");
				SLED.sled_state=SLED_PROG;
				SLED.led_dir = SLED_DIR_DOWN;
				SLED.led_cnt = SLED.n_of_led-1;
			}
			break;
		case SLED_PROG:
			if(timeout(&led_delay_timer,SLED.led_delay_time))
			{
				if(SLED.led_dir == SLED_DIR_UP)
				{
					set_led_state(SLED.led_cnt,1);
					if(SLED.led_cnt < (SLED.n_of_led-1))SLED.led_cnt++;
					else
					{
						SLED.sled_state = SLED_WAIT;
						SLED.led_cnt=0;
					}
				}
				else if(SLED.led_dir == SLED_DIR_DOWN)
				{
					set_led_state(SLED.led_cnt,1);
					if(SLED.led_cnt > 0)SLED.led_cnt--;
					else
					{
						SLED.sled_state = SLED_WAIT;
						SLED.led_cnt = SLED.n_of_led-1;
					}
				}

			}
			break;
		case SLED_WAIT:

			if(!SLED.inp_up && !SLED.inp_down)
			{
				timeout(&sw_delay_timer,0);
				SLED.sled_state = SLED_WAIT2;
			}

			break;
		case SLED_WAIT2:
			if(timeout(&sw_delay_timer,SLED.led_on_time))
			{
				SLED.sled_state = SLED_PROG_OFF;
			}
			break;
		case SLED_PROG_OFF:
			if(timeout(&led_delay_timer,SLED.led_delay_time))
			{
				if(SLED.led_dir == SLED_DIR_DOWN)
				{
					set_led_state(SLED.led_cnt,0);
					if(SLED.led_cnt > 0)SLED.led_cnt--;
					else SLED.sled_state = SLED_IDLE;
				}
				else if(SLED.led_dir == SLED_DIR_UP)
				{
					set_led_state(SLED.led_cnt,0);
					if(SLED.led_cnt < (SLED.n_of_led-1))SLED.led_cnt++;
					else SLED.sled_state = SLED_IDLE;
				}
			}
			break;
		}
		}

	}
	else if(SLED.program == SLED_PROG_ALLONCE)
	{
		if(timeout(&main_timer,5))
			{
				switch(SLED.sled_state)
				{
				case SLED_IDLE:
					if(SLED.inp_up || SLED.inp_down)
					{
						set_all_led_state(1);
						SLED.sled_state=SLED_WAIT;
					}
					break;
				case SLED_WAIT:

					if(!SLED.inp_up && !SLED.inp_down)
					{
						timeout(&sw_delay_timer,0);
						SLED.sled_state = SLED_WAIT2;
					}

					break;
				case SLED_WAIT2:
					if(timeout(&sw_delay_timer,SLED.led_on_time))
					{
						set_all_led_state(0);
						SLED.sled_state = SLED_IDLE;
					}
					break;

				}
			}
	}
}


//SLED.inp_up
int cmd_set_leds_up(int argc,char ** argv)
{
	SLED.inp_up = 1;
	return 1;
}
int cmd_set_leds_down(int argc,char ** argv)
{
	SLED.inp_down = 1;
	return 1;
}


