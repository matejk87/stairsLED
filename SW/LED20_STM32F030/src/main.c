
/* Includes */
#include <stddef.h>
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_conf.h"
#include "pin_def.h"
#include "bootloader.h"
#include "timer.h"
#include "config.h"
#include "printf.h"
#include "usart1.h"
#include "usart2.h"
#include "getline.h"
#include "mybus.h"
#include "i2c.h"
#include "timer3.h"
#include "ds1820.h"
#include "se95.h"
#include "SW_PWM.h"
#include "SLED.h"
#include "one_wire.h"
#include "DHT22.h"
#include "DHT22_delay.h"

int pwm_duty=50;
uint32_t timer=0;
uint8_t  timerFlag=0;
unsigned long print_temp_time=0;
char version_str[]="<version_data>" VER_OPIS "</version_data>";
char test_str[]="bb b";
t_timer timer_led,timer_temp_print,timer_DHT;

unsigned char humidity;
int dht_temp;
float dht_temperature;

void gpio_init(void)
{
	int i;
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 	// enable the clock to GPIOC			//(RM0091 lists this as IOPCEN, not GPIOCEN)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 	// enable the clock to GPIOC			//(RM0091 lists this as IOPCEN, not GPIOCEN)
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 	// enable the clock to GPIOC			//(RM0091 lists this as IOPCEN, not GPIOCEN)
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN; 	// enable the clock to GPIOC			//(RM0091 lists this as IOPCEN, not GPIOCEN)

	//GPIOC->MODER = (1 << 16)|(1 << 0)|(1 << 2);


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;


//leds
	GPIO_InitStructure.GPIO_Pin = LED0;
	GPIO_Init(LED0_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED1;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SLED0;
	GPIO_Init(SLED0_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED1;
	GPIO_Init(SLED1_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED2;
	GPIO_Init(SLED2_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED3;
	GPIO_Init(SLED3_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED4;
	GPIO_Init(SLED4_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED5;
	GPIO_Init(SLED5_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED6;
	GPIO_Init(SLED6_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED7;
	GPIO_Init(SLED7_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED8;
	GPIO_Init(SLED8_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED9;
	GPIO_Init(SLED9_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED10;
	GPIO_Init(SLED10_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED11;
	GPIO_Init(SLED11_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED12;
	GPIO_Init(SLED12_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED13;
	GPIO_Init(SLED13_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED14;
	GPIO_Init(SLED14_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED15;
	GPIO_Init(SLED15_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED16;
	GPIO_Init(SLED16_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED17;
	GPIO_Init(SLED17_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED18;
	GPIO_Init(SLED18_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SLED19;
	GPIO_Init(SLED19_PORT, &GPIO_InitStructure);






// one wire open drain
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = ONEW;
	GPIO_Init(ONEW_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	ONEW_SET();


//inputs
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = INP0;
	GPIO_Init(INP0_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = INP1;
	GPIO_Init(INP1_PORT, &GPIO_InitStructure);
}


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  uint32_t ii = 0,i=0,tmp;

  copy_vector_table_sram();
  /* TODO - Add your application code here */
 // SysTick_Config(4800);  /* 0.1 ms = 100us if clock frequency 12 MHz */

 // SystemCoreClockUpdate();
 // ii = SystemCoreClock;
 // ii = 0;


  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	gpio_init();

	SysTick_Config(SystemCoreClock/1000);
	SystemCoreClockUpdate();
USART_InitTypeDef USART_InitStructure;

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
//Configure USART2 pins:  Rx and Tx ----------------------------
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;  //todo odstrani inicializacijo uart 2
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOA, &GPIO_InitStructure);
//Configure USART2 setting:           ----------------------------
USART_InitStructure.USART_BaudRate = 115200;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_Init(USART2, &USART_InitStructure);
USART_Cmd(USART2,ENABLE);

timer_init(100000);
timeout(&timer_led,0);

usart2_init();
printf("\n\n\n");
printf(version_str);

mybus_init();
timer3_init();
I2C2_init();
SLED_init();

  while(1)
  {
	  if(timeout(&timer_led,200))
	  {
		/*LED0_TOGGLE();
		delay_us(2);
		LED0_TOGGLE();
		delay_us(2);*/
	  	LED0_TOGGLE();
	  /*	i++;
	 	for(tmp=0;tmp<20;tmp++)
	  	{
	  		set_pwm(tmp,i);
	  	}
	  	if(i>=100)i=0;*/
	  }
	  if(timeout(&timer_DHT,3000))
	  {
		/*  readDHT22( &humidity, &dht_temperature );
		  dht_temp = dht_temperature*10;
		  printf("\nDHT22 hum:%d temp:%d",humidity,dht_temp);*/
	  }
	  if(print_temp_time)
	  {
		 if(timeout(&timer_temp_print,print_temp_time))
		 {
			 printf("\n temp_DS1820:%d temp_SE95:%d",temp_DS1820.temperature,temp_SE95.temperature);
		 }
	  }
	  SLED_main();
	  sw_pwm_main();
	  getline();
	  mybus_main();
	  se95_main();
	  i2c_main();

	 // DHT_main();
	 // ds1820_main();
  }
}


int cmd_print_temp(int argc, char **argv)
{
	if(argc>1)
	{
		print_temp_time = strtoul(argv[1],0,0);
		printf("\nprint temperature time:%dms",print_temp_time);
	}
	else
	{
		//printf("\n argc error");
		print_temp_time=0;
		 printf("\n temp_DS1820:%d temp_SE95:%d",temp_DS1820.temperature,temp_SE95.temperature);
	}
	return 1;
}

int cmd_reset(int argc,char ** argv)
{
	resetcpu();
	return 1;
}

int cmd_set_pwm(int argc,char ** argv)
{
	if(argc>1)
	{
		pwm_duty = strtoul(argv[1],0,0);
		printf("\npwm duty:%d",pwm_duty);
	}
	return 1;
}
