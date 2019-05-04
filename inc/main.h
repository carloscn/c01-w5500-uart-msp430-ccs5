/*
 * main.h
 *
 *  Created on: 2019-3-2
 *      Author: lifim
 */

#ifndef MAIN_H_
#define MAIN_H_


void Process_Socket_Data(SOCKET s);
void W5500_Socket_Set(void);
void Load_Net_Parameters(void);
void W5500_Initialization(void);
void system_clk_init(void);
void Delay(unsigned int  x);
#endif /* MAIN_H_ */
