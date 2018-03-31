/*
 * DHT22.c
 *
 *  Created on: Jan 30, 2018
 *      Author: Matej
 */
#include "DHT22.h"
#include "stm32f0xx_conf.h"
#include "timer3.h"
#include "timer.h"
#include "one_wire.h"
#include "pin_def.h"
#include "printf.h"


char DHT_busy=0;
int DHTn=0,DHTdata_cnt;
int DHTdata_w=0,DHTdata_r=0;
int DHTcmd = OW_idle;
t_timer DHT_timer;

int DHT_busy_get(void)
{
	return DHT_busy;
}

void DHT_reset()
{
 if(DHT_busy)return;
 DHT_busy = 1;
 DHTn=0;
 DHTcmd = DHT_rst;
 ONEW_CLR();
 timer3_set(1200); //500
}

void DHT_main_int(void)
{
	switch(DHTcmd){
		case DHT_rst:
			switch(DHTn){
				case 0:
					ONEW_SET();
					timer3_set(500);
					break;
				case 1:
					ONEW_SET();
					timer3_stop();
					DHT_busy=0;
					break;
			}
			DHTn++;
			break;
		case DHT_w:
			break;
		case DHT_r:
			break;
		default:
		 break;
	}
}

void DHT_main(void)
{
	 if(timeout(&DHT_timer,5000))
	 {
		 DHT_reset();

	 }

}




