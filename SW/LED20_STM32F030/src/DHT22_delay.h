/*
 * DHT22_delay.h
 *
 *  Created on: Feb 3, 2018
 *      Author: Matej
 */

#ifndef DHT22_DELAY_H_
#define DHT22_DELAY_H_


#define DHT22_DATA_BIT_COUNT 40


typedef enum
{
  DHT_ERROR_NONE = 0,
  DHT_BUS_HUNG,
  DHT_ERROR_NOT_PRESENT,
  DHT_ERROR_ACK_TOO_LONG,
  DHT_ERROR_SYNC_TIMEOUT,
  DHT_ERROR_DATA_TIMEOUT,
  DHT_ERROR_CHECKSUM,
} DHT22_ERROR_t;


DHT22_ERROR_t readDHT22( unsigned char *dht_humidity_now, float *dht_temperature_now );



#endif /* DHT22_DELAY_H_ */
