
#ifndef CMDTABLE_H_
#define CMDTABLE_H_

#include "cmdproc.h"



/* declaration of cmd functions*/

int help(int argc,char **argv);
int bla(int argc,char **argv);
void debug(int argc,char **argp);
int cmd_stats(int argc, char **argv);
int cmd_set_rs485_addr(int argc, char **argv);
int cmd_enter_bl(int argc, char **argv);
int cmd_reset(int argc,char ** argv);
int cmd_set_pwm(int argc,char ** argv);
int cmd_print_temp(int argc, char **argv);
int cmd_set_leds_up(int argc,char ** argv);
int cmd_set_leds_down(int argc,char ** argv);



//int get_inb1(int argc, char ** argp);
/* end of declaration of cmd functions*/
/* CMD table */

// definicija ukazov

const t_cmd_str cmd_table[] =
    {
 /*   "cmd string" , cmd_function,"help string"*/
		{ "help",help,"<-l or cmd> print help"},  // cmdproc.c embedded function
		{ "bla",bla,"test run cmd_process"},
		{ "set_rs485_addr",cmd_set_rs485_addr,"rs485address"},
		{ "enterBL",cmd_enter_bl,"enter Boot Loader"},
		{ "reset",cmd_reset,"reset device"},
		{ "stats",cmd_stats,"RS485 stats"},
		{ "set_pwm",cmd_set_pwm,"pwm"},
		{ "set_leds_up",cmd_set_leds_up,"set leds up"},
		{ "set_leds_down",cmd_set_leds_down,"set leds down"},
		{ "print_temp",cmd_print_temp,"print temperature every n miliseconds"},
		//{"enter_bl",cmd_enter_bl,""}

	//	{ "inpb1",get_inb1," get GPIO PB1"},
    };

#endif /* CMDTABLE_H_ */
