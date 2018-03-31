/*
 * pin_def.h
 *
 *  Created on: 30. dec. 2015
 *      Author: Matej
 */

#ifndef INC_PIN_DEF_H_
#define INC_PIN_DEF_H_
#include "stm32f0xx.h"


#define MAX_N_OF_LEDS 20

#define SLED0 GPIO_Pin_1
#define SLED0_PORT GPIOA
#define SLED1 GPIO_Pin_0
#define SLED1_PORT GPIOA
#define SLED2 GPIO_Pin_14
#define SLED2_PORT GPIOC
#define SLED3 GPIO_Pin_13
#define SLED3_PORT GPIOC
#define SLED4 GPIO_Pin_9
#define SLED4_PORT GPIOB
#define SLED5 GPIO_Pin_8
#define SLED5_PORT GPIOB
#define SLED6 GPIO_Pin_7
#define SLED6_PORT GPIOB
#define SLED7 GPIO_Pin_6
#define SLED7_PORT GPIOB
#define SLED8 GPIO_Pin_5
#define SLED8_PORT GPIOB
#define SLED9 GPIO_Pin_4
#define SLED9_PORT GPIOB
#define SLED10 GPIO_Pin_3
#define SLED10_PORT GPIOB
#define SLED11 GPIO_Pin_15
#define SLED11_PORT GPIOA
#define SLED12 GPIO_Pin_7
#define SLED12_PORT GPIOF
#define SLED13 GPIO_Pin_6
#define SLED13_PORT GPIOF
#define SLED14 GPIO_Pin_15
#define SLED14_PORT GPIOB
#define SLED15 GPIO_Pin_14
#define SLED15_PORT GPIOB
#define SLED16 GPIO_Pin_4
#define SLED16_PORT GPIOA
#define SLED17 GPIO_Pin_5
#define SLED17_PORT GPIOA
#define SLED18 GPIO_Pin_6
#define SLED18_PORT GPIOA
#define SLED19 GPIO_Pin_7
#define SLED19_PORT GPIOA

#define INP0 GPIO_Pin_1
#define INP0_PORT GPIOB
#define INP1 GPIO_Pin_2
#define INP1_PORT GPIOB

#define INP0_READ() (((INP0_PORT->IDR)&INP0)?1:0)
#define INP1_READ() (((INP1_PORT->IDR)&INP1)?1:0)

//#define POUT0_TOGGLE() ((POUT_PORT->ODR)^=(POUT0))

#define LED0 GPIO_Pin_12
#define LED0_PORT GPIOB
#define LED0_CLR() ((LED0_PORT->ODR)&=~(LED0))
#define LED0_SET() ((LED0_PORT->ODR)|=(LED0))
#define LED0_TOGGLE() ((LED0_PORT->ODR)^=(LED0))

#define LED1 GPIO_Pin_13
#define LED1_PORT GPIOB
#define LED1_CLR() ((LED1_PORT->ODR)&=~(LED1))
#define LED1_SET() ((LED1_PORT->ODR)|=(LED1))
#define LED1_TOGGLE() ((LED1_PORT->ODR)^=(LED1))
#define LED1_READ() (((LED1_PORT->IDR)&LED1)?1:0)

#define ONEW GPIO_Pin_0
#define ONEW_PORT GPIOB
#define ONEW_CLR() ((ONEW_PORT->ODR)&=~(ONEW))
#define ONEW_SET() ((ONEW_PORT->ODR)|=(ONEW))
#define ONEW_TOGGLE() ((ONEW_PORT->ODR)^=(ONEW))
#define ONEW_READ() (((ONEW_PORT->IDR)&ONEW)?1:0)



#endif /* INC_PIN_DEF_H_ */
