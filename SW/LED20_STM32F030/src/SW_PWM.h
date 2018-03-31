/*
 * SW_PWM.h
 *
 *  Created on: 8.10.2012
 *      Author: Matej Kropivnik
 *      e-mail: matej.kropivnik@gmail.com
 */

#ifndef SW_PWM_H_
#define SW_PWM_H_

#include "pin_def.h"
//#include "led_driver.h"


//#define PWM_OUT_PIN0 LED_PWM_PIN0

typedef struct
{
  short duty_current,duty_selected;
  unsigned long on_time; //se izracuna iz duty in frekvence
  t_timer off_timer,ss_timer;
  char on;
  int ss_cnt;
}t_sw_pwm;

int set_pwm(int chn,int duty);
void sw_pwm_main(void);
int outp_state(int chn);


#endif
