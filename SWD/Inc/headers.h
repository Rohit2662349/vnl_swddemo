

#ifndef __HEADER
#define __HEADER

/*------------------ Includes */
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "main.h"
#include "stm32f7xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "cli.h"
#include "swd.h"

/*------------------ Exported Macros */
#define SW_JTAG_TCK_Pin									SWD_CLK_Pin				//MAPPING FOR SWD API
#define SW_JTAG_TMS_Pin									SWD_IO_Pin				//MAPPING FOR SWD API

#define SUCCESS						1
#define FAIL							0

#define InvalidCmd				2
#define InvalidNoOfArgs		3
#define InvalidArgs				4
/*------------------ Exported Typedef */

/*------------------ Exported Variables */
extern uint8_t cmdrcvdflag;
extern uint16_t cmdWriteLoc,cmdDispLoc,cmdWriteLoc2,cmdWriteLoc3;
/*------------------ Exported Constants */

/*------------------ Exported Externs */

/*------------------ Exported Functions */
void DutPrompt(void);
int stdout_putchar (int ch);
int8_t Help(uint8_t *argv);
int8_t Version(uint8_t *argv);
int8_t Reset(uint8_t *argv);
int8_t Delay(uint8_t *argv);
int8_t Sw_ID(uint8_t *argv);
int8_t Sw_Read_Dp(uint8_t *argv);
int8_t Sw_Read_Ap(uint8_t *argv);
int8_t Sw_Write_Dp(uint8_t *argv);
int8_t Sw_Write_Ap(uint8_t *argv);




#endif


