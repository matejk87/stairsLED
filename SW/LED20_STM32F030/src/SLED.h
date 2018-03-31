/*
 * SLED.h
 *
 *  Created on: Dec 9, 2017
 *      Author: Matej
 */

#ifndef SLED_H_
#define SLED_H_

#define DEFAULT_LED_DELAY_TIME 100
#define DEFAULT_N_OF_LEDS 20
#define MAX_PWM 100


typedef enum{
SLED_PROG_SERIAL,
SLED_PROG_ALLONCE
}program_t;

typedef enum{
SLED_DIR_UP,
SLED_DIR_DOWN
}led_direction_t;

typedef enum{
SLED_IDLE,
SLED_PROG,
SLED_WAIT,
SLED_WAIT2,
SLED_PROG_OFF,
}serial_led_t;

typedef struct{
	program_t program;
    int n;
    int inp_up,inp_down;
    int led_dir;
    int inp_up_before,inp_down_before;

    serial_led_t sled_state;
    int n_of_led;
    int led_cnt;
    int led_delay_time;
    int led_on_time;
    int soft_start;

}SLED_t;




void SLED_init(void);
void set_output_led(int led_n,int state);

void SLED_main(void);

#endif /* SLED_H_ */
