/*
 * DHT22.h
 *
 *  Created on: Jan 30, 2018
 *      Author: Matej
 */

#ifndef DHT22_H_
#define DHT22_H_


typedef enum{
	DHT_idle,
	DHT_r,
	DHT_w,
	DHT_rst
}DHT_command_t;

void DHT_main_int(void);
void DHT_main(void);

#endif /* DHT22_H_ */
