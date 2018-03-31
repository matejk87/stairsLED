/*
 * SW_PWM.c
 *
 *  Created on: 8.10.2012
 *      Author: Matej Kropivnik
 *      e-mail: matej.kropivnik@gmail.com
 */
#include "timer.h"
#include "SW_PWM.h"
#include "SLED.h"
#include "pin_def.h"
//#include "led_driver.h"

#define SET_OUT(A,B) set_output_led(A,B)

#define PWM_SS_SLOPE    1

#define PWM_CHANNELS    MAX_N_OF_LEDS
#define PWM_FREQ        60
#define PWM_PERIOD      (1000000/PWM_FREQ)     // [us]
#define PWM_SLICE       (PWM_PERIOD/PWM_CHANNELS) //[us]

t_timer state_timer,ss_timer;
t_sw_pwm sw_pwm[PWM_CHANNELS];
int soft_start=1;
int pwm_state;
int ss_state=0,ss_slope = PWM_SS_SLOPE;
/*
int outp_state(int chn)
{
  if(PIN_READ(((PWM_OUT_PIN0)+chn)))
  {
     if(time_us(&sw_pwm[chn].off_timer,400))return 1;  //poèakamo 400us po vklopu (pustimo fetu èas za vklop) //todo prilagodi glede na izhodni kamel
  }
  return 0;
}*/
void set_soft_start(int ss)
{
	soft_start = ss;
}

int set_pwm(int chn,int duty)
{
  if(!(chn < PWM_CHANNELS))return 0;
  if(!(duty<=100 && duty>= 0))return 0;
  sw_pwm[chn].duty_selected = duty;
  if(soft_start == 0) sw_pwm[chn].duty_current = duty;
  sw_pwm[chn].on_time = (PWM_PERIOD * sw_pwm[chn].duty_current)/100;
  return 1;
}

void sw_pwm_main(void)
{
  int i;
  if(timeout_10us(&state_timer,PWM_SLICE/10))
  {
    pwm_state++;
    if(pwm_state>=PWM_CHANNELS)pwm_state=0;
    if(sw_pwm[pwm_state].duty_current == 100) SET_OUT(pwm_state,1); //OUT_SET(((PWM_OUT_PIN0)+pwm_state));
    else if(sw_pwm[pwm_state].duty_current == 0)SET_OUT(pwm_state,0); //OUT_CLR(((PWM_OUT_PIN0)+pwm_state));
    else
    {
     	timeout_10us(&sw_pwm[pwm_state].off_timer,0);
     	 SET_OUT(pwm_state,1);
        sw_pwm[pwm_state].on = 1;
    }
  }
  for(i=0;i<PWM_CHANNELS;i++)
  {
    if(sw_pwm[i].on)
    {
      if(timeout_10us(&sw_pwm[i].off_timer,sw_pwm[i].on_time/10))
      {
        sw_pwm[i].on = 0;
        SET_OUT(i,0);
      }
    }
  }
  //soft start
  if(soft_start && timeout(&ss_timer, 1))
  {
	  if(sw_pwm[ss_state].duty_selected != sw_pwm[ss_state].duty_current)
	  {
		  if(sw_pwm[ss_state].duty_selected > sw_pwm[ss_state].duty_current)
		  {
			  sw_pwm[ss_state].duty_current += ss_slope;
			  sw_pwm[ss_state].on_time = (PWM_PERIOD * sw_pwm[ss_state].duty_current)/100;
			  if(sw_pwm[ss_state].duty_current>sw_pwm[ss_state].duty_selected)sw_pwm[ss_state].duty_current =sw_pwm[ss_state].duty_selected;
		  }
		  else if(sw_pwm[ss_state].duty_selected < sw_pwm[ss_state].duty_current)
		  {
			  sw_pwm[ss_state].duty_current -= ss_slope;
			  sw_pwm[ss_state].on_time = (PWM_PERIOD * sw_pwm[ss_state].duty_current)/100;
			  if(sw_pwm[ss_state].duty_current<sw_pwm[ss_state].duty_selected)sw_pwm[ss_state].duty_current =sw_pwm[ss_state].duty_selected;
		  }
	  }
	  ss_state++;
	  if(ss_state >= PWM_CHANNELS)ss_state=0;
  }


}
