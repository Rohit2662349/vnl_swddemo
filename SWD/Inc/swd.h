
#ifndef __SW_HEADER
#define __SW_HEADER

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "main.h"

/* Exported Macro ------------------------------------------------------------*/
#define SW_AP_ACCESS										1
#define SW_DP_ACCESS										0
	
#define SW_READ													1
#define SW_WRITE												0

#define DP_IDCODE												0x00
#define DP_CONTROL_STATUS								0x01
#define DP_SELECT												0x02
#define DP_RDBUFF												0x03

//BANK0
#define AP_CONTROL_STATUS								0x00
#define AP_TAR													0x01
#define AP_XXX													0x02
#define AP_RW														0x03

//BANK1
#define AP_BANKED_DATA0									0x10
#define AP_BANKED_DATA1									0x14
#define AP_BANKED_DATA2									0x18
#define AP_BANKED_DATA3									0x1C

#define AP_IDCODE												0xFC

#define SW_MSB_FIRST										1
#define SW_LSB_FIRST										0
/* Exported Types ------------------------------------------------------------*/
typedef enum{ACK=1,WAIT=2,FAULT=4,BUS_CONFIG_ERROR=7}SW_RESULT;

typedef struct{
		uint8_t sw_select_dp_ap;
		uint8_t sw_reg_dp_ap;
		uint8_t sw_rw;
		uint8_t sw_cmd_parity;
		uint8_t sw_bit_dir;
		uint8_t sw_cmd;
		uint8_t sw_data_size;
		uint8_t sw_data_parity;
		uint32_t sw_data;
		SW_RESULT sw_result;
}SWD_INFO;

/* Exported Constants --------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/


/* Exported Functions ------------------------------------------------------- */
void HAL_SW_Init(void);
void HAL_SW_Reset(void);
void HAL_SW_Delay(void);
void HAL_SW_IO(SWD_INFO *sw_info);
void HAL_SW_AP_Read(SWD_INFO *sw_info,uint8_t reg_addr);
void HAL_SW_DP_Read(SWD_INFO *sw_info,uint8_t reg_addr);
void HAL_SW_AP_Write(SWD_INFO *sw_info,uint8_t reg_addr);
void HAL_SW_DP_Write(SWD_INFO *sw_info,uint8_t reg_addr);
#endif
/************************ END OF FILE  ****************************************/


