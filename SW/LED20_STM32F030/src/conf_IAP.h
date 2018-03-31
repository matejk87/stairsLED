/*
 * conf_IAP.h
 *
 *  Created on: 5. apr. 2013
 *      Author: Matej
 */
#ifndef CONF_IAP_H_
#define CONF_IAP_H_

#include "stm32f0xx_conf.h"

#define STM32F_FLASH_ADDR 			FLASH_BASE
#define STM32F_FLASH_SIZE			0x10000
#define STM32F_PAGE_SIZE			1024

#define PROGRAM_START_OFFSET          0x5000 //20k
#define PROGRAM_START_ADDRESS         (STM32F_FLASH_ADDR + PROGRAM_START_OFFSET)


#define CFG_PAGE_ADDR     (STM32F_FLASH_ADDR+STM32F_FLASH_SIZE-STM32F_PAGE_SIZE)

#define MYISP_FB_ADDR              (CFG_PAGE_ADDR+4)
#define MYISP_FB_SWSEL_ADDR 	    CFG_PAGE_ADDR


#endif /* CONF_IAP_H_ */
