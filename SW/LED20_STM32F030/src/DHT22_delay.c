/*
 * DHT22_delay.c
 *
 *  Created on: Feb 3, 2018
 *      Author: Matej
 */


#include "DHT22_delay.h"
#include "timer.h"
#include "one_wire.h"
#include "pin_def.h"
#include "printf.h"
//typedef uint8_t unsigned char;
#define TERESHOLD 12//20


DHT22_ERROR_t readDHT22( unsigned char *dht_humidity_now, float *dht_temperature_now )
{

	unsigned char retryCount = 0;
	unsigned char bitTimes[DHT22_DATA_BIT_COUNT];
	int rawHumidity = 0;
	int rawTemperature = 0;
	unsigned char checkSum = 0, csPart1, csPart2, csPart3, csPart4;
	int i;

	// Pin needs to start HIGH, wait until it is HIGH with a timeout
	retryCount = 0;
	/*cli();
	DDRC &= ~(1 << ( PC3 )); 							//DIRECT_MODE_INPUT(reg, bitmask);
	sei();*/
	do
	{
		if(retryCount > 125) return DHT_BUS_HUNG;
		retryCount++;
		delay_us(2);
	} while( !ONEW_READ()/*!( PINC & ( 1 << PC3 ) )*/ );				//!DIRECT_READ(reg, bitmask)

	// Send the activate pulse
/*	PORTC &= ~(1 << ( PC3 )); 							//DIRECT_WRITE_LOW(reg, bitmask);
	DDRC |= 1 << ( PC3 );	*/							//DIRECT_MODE_OUTPUT(reg, bitmask); // Output Low
	ONEW_CLR();

	delay(5); 										// spec is 1 to 10ms
//	DDRC &= ~(1 << ( PC3 ));;							// Switch back to input so pin can float
	ONEW_SET();

	// Find the start of the ACK signal
	retryCount = 0;
	do
	{
		if (retryCount > 25) 							//(Spec is 20 to 40 us, 25*2 == 50 us)
		{
			return DHT_ERROR_NOT_PRESENT;
		}
		retryCount++;
		delay_us(2);
	} while( ONEW_READ() /*PINC & ( 1 << PC3 )*/ );


	// Find the transition of the ACK signal
	retryCount = 0;
	do
	{
		if (retryCount > 50) 							//(Spec is 80 us, 50*2 == 100 us)
		{
			return DHT_ERROR_ACK_TOO_LONG;
		}
		retryCount++;
		delay_us(2);
	} while( !ONEW_READ()/*!(PINC & ( 1 << PC3 ))*/ );

	// Find the end of the ACK signal
	retryCount = 0;
	do
	{
		if (retryCount > 50) 							//(Spec is 80 us, 50*2 == 100 us)
		{
			return DHT_ERROR_ACK_TOO_LONG;
		}
		retryCount++;
		delay_us(2);
	} while( ONEW_READ()/*PINC & ( 1 << PC3 )*/ );

	// Read the 40 bit data stream
	for(i = 0; i < DHT22_DATA_BIT_COUNT; i++)
	{
		// Find the start of the sync pulse
		retryCount = 0;
		do
		{
			if (retryCount > 35) 						//(Spec is 50 us, 35*2 == 70 us)
			{
				return DHT_ERROR_SYNC_TIMEOUT;
			}
			retryCount++;
			delay_us(2);
		} while( !ONEW_READ()/*!(PINC & ( 1 << PC3 ))*/ );

		// Measure the width of the data pulse
		retryCount = 0;
		do
		{
			if (retryCount > 50) 						//(Spec is 80 us, 50*2 == 100 us)
			{
				return DHT_ERROR_DATA_TIMEOUT;
			}
			retryCount++;
			delay_us(1);
		} while( ONEW_READ()/*PINC & ( 1 << PC3 )*/ );
		bitTimes[i] = retryCount;
	}

	// translate bitTimes
	// 26~28us == logical 0
	// 70us	   == logical 1
	// here treshold is 40us
	for(i = 0; i < 16; i++)
	{
		if( bitTimes[i] > TERESHOLD ) rawHumidity |= (1 << (15 - i));
	}

	for(i = 0; i < 16; i++)
	{
		if( bitTimes[i + 16] > TERESHOLD ) rawTemperature |= (1 << (15 - i));
	}

	for(i = 0; i < 8; i++)
	{
		if(bitTimes[i + 32] > TERESHOLD) checkSum |= (1 << (7 - i));
	}

	// calculate checksum
	csPart1 = rawHumidity >> 8;
	csPart2 = rawHumidity & 0xFF;
	csPart3 = rawTemperature >> 8;
	csPart4 = rawTemperature & 0xFF;

	printf("\n bit times:");
	for(i = 0; i < 16; i++)
	{
		printf(" %d",bitTimes[i]);

	}

	if( checkSum == ( (csPart1 + csPart2 + csPart3 + csPart4) & 0xFF ) )
	{
		// raw data to sensor values
		*dht_humidity_now = ( (rawHumidity + 5) / 10 );

		if(rawTemperature & 0x8000)											// Below zero, non standard way of encoding negative numbers!
		{
			rawTemperature &= 0x7FFF;
			*dht_temperature_now = (rawTemperature / 10.0) * -1.0;
		} else *dht_temperature_now = (rawTemperature) / 10.0;
		printf("\n dht OK:%d  %d  checksum:%d",rawTemperature,rawHumidity,checkSum);

		return DHT_ERROR_NONE;
	}

	return DHT_ERROR_CHECKSUM;
}

