/*
 * board_def.h
 *
 *  Created on: 30 lis 2024
 *      Author: maxdu
 */

#ifndef SRC_BOARD_DEF_H_
#define SRC_BOARD_DEF_H_

// -----------------------------------------------

#define BOARD_LED PA_8
#define RELAY     PA_6
#define VOL_SET   PA_0
#define TIME_SET  PA_1

#ifdef STM32C0xx
	#define COIN_DET  PA_3
#else
	#define COIN_DET  PB_10
#endif

// -----------------------------------------------


#endif /* SRC_BOARD_DEF_H_ */
