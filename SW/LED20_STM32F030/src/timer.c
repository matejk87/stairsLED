
//SysTick_Config(SystemCoreClock / 1000);
#include "timer.h"
#include "pin_def.h"
//#include "roleta_rele.h"
#include "stm32f0xx.h"

unsigned long timetick=0,timetick_10us=0;
unsigned long timesec=0;

int timer_init(unsigned long tick_freq){
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / tick_freq);
  return 0;
}

void SysTick_Handler(void)  /// systick interrupt
{
  //TimingDelay_Decrement();
	timetick_10us++;
	//LED1_TOGGLE();
  if((timetick_10us%100)==0)
  {
	  timetick++;
	  if(timetick%1000)timesec++;
  }
}

t_timer timer_duration(t_timer * timer)
{
	return (timetick - *timer);
}

int timeout(t_timer * timer, unsigned long timeout)
{
	volatile unsigned long tmp;
	tmp=timetick;
	if(timeout==0)
	{
		*timer=timetick;
		return 1;
	}
	if((tmp-*timer)>=timeout)
	{
		*timer=timetick;
		return 1;
	}
	return 0;

}


int timeout_10us(t_timer * timer, unsigned long timeout)
{
	volatile unsigned long tmp;
	tmp=timetick_10us;
	if(timeout==0)
	{
		*timer=timetick_10us;
		return 1;
	}
	if((tmp-*timer)>=timeout)
	{
		*timer=timetick_10us;
		return 1;
	}
	return 0;
}

void delay(unsigned int ms)
{
	t_timer delay_timer;
	timeout(&delay_timer,0);
	while(!timeout(&delay_timer,ms));
}

void delay_us(unsigned int us)  //test delay
{
	volatile unsigned long cnt=0;
	int i;
	/*for(i=0;i<us;i++)
	{*/
		cnt = us*2;
		while(cnt)
		{
			cnt--;
		}

	//}
}


