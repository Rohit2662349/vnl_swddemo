
/* Includes ------------------------------------------------------------------*/
#include "headers.h"
/* Includes ------------------------------------------------------------------*/


/* Macros ------------------------------------------------------------------*/

#define GET_ACK()												HAL_GPIO_ReadPin(GPIOI,SW_JTAG_TMS_Pin)
#define GET_DATA()											HAL_GPIO_ReadPin(GPIOI,SW_JTAG_TMS_Pin)
#define TMS_HIGH()											GPIOI->BSRR = SW_JTAG_TMS_Pin
#define TMS_LOW()												GPIOI->BSRR = (uint32_t)(SW_JTAG_TMS_Pin << 16)
#define TMS_READ()											(GPIOI->IDR & SW_JTAG_TMS_Pin) != 0?0x00000001:0x00000000 
#define RELEASE_CLOCK()									GPIOI->BSRR = (uint32_t)SW_JTAG_TCK_Pin << 16;\
																				HAL_SW_Delay();\
																				GPIOI->BSRR = SW_JTAG_TCK_Pin;\
																				HAL_SW_Delay();		
//																				__NOP();\
//																				GPIOI->BSRR = SW_JTAG_TCK_Pin;\
//																				__NOP();	
#define SEL_DP													0
#define SEL_AP													1
/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
void HAL_SW_AP_BankSel(uint8_t bankAddr);
void HAL_SW_Delay(){
		uint16_t i=0;
		for(i=0;i<2000;i++);
}

/* App Start -----------------------------------------------------------------*/

void HAL_SW_Init() {
		uint16_t i=0;uint16_t default_seq=0xE79E;
		GPIO_InitTypeDef GPIO_InitStruct;
							
		HAL_GPIO_WritePin(GPIOI,SW_JTAG_TCK_Pin |SW_JTAG_TMS_Pin, GPIO_PIN_SET);
		GPIO_InitStruct.Pin   = SW_JTAG_TCK_Pin;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
		GPIO_InitStruct.Pin   = SW_JTAG_TMS_Pin;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
		

		for(i=0;i<60;i++){
				RELEASE_CLOCK();
		}
		
		for(i=0;i<16;i++){													// Seq : 0111100111100111
				if(((default_seq>>i)&0x01))TMS_HIGH();
				else TMS_LOW();
				RELEASE_CLOCK();
		}
		
		TMS_HIGH();
		for(i=0;i<60;i++){
				RELEASE_CLOCK();
		}

		
		//Ideal State
		TMS_LOW();
		RELEASE_CLOCK();
		RELEASE_CLOCK();
}

void HAL_SW_Reset() {
		uint8_t i=0;
		TMS_HIGH();
		for(i=0;i<50;i++){RELEASE_CLOCK();}
}

void HAL_SW_DP_Write(SWD_INFO *sw_info,uint8_t reg_addr){
		sw_info->sw_select_dp_ap = SW_DP_ACCESS;
		sw_info->sw_rw = SW_WRITE;
		sw_info->sw_reg_dp_ap = reg_addr;
		if(reg_addr == DP_IDCODE){sw_info->sw_cmd_parity=0;}
		else if(reg_addr == DP_CONTROL_STATUS){sw_info->sw_cmd_parity=1;}
		else if(reg_addr == DP_SELECT){sw_info->sw_cmd_parity=1;}
		else if(reg_addr == DP_RDBUFF){sw_info->sw_cmd_parity=0;}
}

void HAL_SW_DP_Read(SWD_INFO *sw_info,uint8_t reg_addr){
		sw_info->sw_select_dp_ap = SW_DP_ACCESS;
		sw_info->sw_rw = SW_READ;
		sw_info->sw_reg_dp_ap = reg_addr;
		if(reg_addr == DP_IDCODE){sw_info->sw_cmd_parity=1;}
		else if(reg_addr == DP_CONTROL_STATUS){sw_info->sw_cmd_parity=0;}
		else if(reg_addr == DP_SELECT){sw_info->sw_cmd_parity=0;}
		else if(reg_addr == DP_RDBUFF){sw_info->sw_cmd_parity=1;} 
}

void HAL_SW_AP_Write(SWD_INFO *sw_info,uint8_t reg_addr){
		sw_info->sw_select_dp_ap = SW_AP_ACCESS;
		sw_info->sw_rw = SW_WRITE;
		sw_info->sw_reg_dp_ap = reg_addr;
//		HAL_SW_AP_BankSel();
		if(reg_addr == AP_CONTROL_STATUS){sw_info->sw_cmd_parity=1;}
		else if(reg_addr == AP_TAR){sw_info->sw_cmd_parity=0;}
		else if(reg_addr == AP_XXX){sw_info->sw_cmd_parity=0;}
		else if(reg_addr == AP_RW){sw_info->sw_cmd_parity=1;}
}	

void HAL_SW_AP_Read(SWD_INFO *sw_info,uint8_t reg_addr){
		sw_info->sw_select_dp_ap = SW_AP_ACCESS;
		sw_info->sw_rw = SW_READ;
		sw_info->sw_reg_dp_ap = reg_addr;
//		HAL_SW_AP_BankSel();
		if(reg_addr == AP_CONTROL_STATUS){sw_info->sw_cmd_parity=0;}
		else if(reg_addr == AP_TAR){sw_info->sw_cmd_parity=1;}
		else if(reg_addr == AP_XXX){sw_info->sw_cmd_parity=1;}
		else if(reg_addr == AP_RW){sw_info->sw_cmd_parity=0;}
}

//void HAL_SW_AP_BankSel(uint8_t bankAddr){
//		
//		
//}

void HAL_SW_IO(SWD_INFO *sw_info){
		uint8_t i=0,ack=0;
//		GPIO_InitTypeDef GPIO_InitStruct;

		sw_info->sw_cmd= 1<<7 | 0<<6 | sw_info->sw_cmd_parity <<5 | sw_info->sw_select_dp_ap<<1 | sw_info->sw_rw <<2 | sw_info->sw_reg_dp_ap<<3 | 1;
	
		//SW CMD
		for(i=0;i<8;i++){
				if(((sw_info->sw_cmd>>i)&0x01))TMS_HIGH();
				else TMS_LOW();
				RELEASE_CLOCK();
		}
		//SW TURNAROUND
		RELEASE_CLOCK();
		
		//ACK
		for(i=0;i<3;i++){
				ack|=TMS_READ() << i;
				RELEASE_CLOCK();
		}
		
		if(ack == ACK){
				sw_info->sw_result = ACK;
				if(sw_info->sw_rw == SW_READ){
						//SW GET DATA
						for(i=0;i<32;i++){
								sw_info->sw_data|=(TMS_READ()) <<i ;
								RELEASE_CLOCK();
						}
						sw_info->sw_data_parity=TMS_READ();
						RELEASE_CLOCK();
						
						//SW TURNAROUND
						RELEASE_CLOCK();
				}
				else if(sw_info->sw_rw == SW_WRITE){
						//SW TURNAROUND
						RELEASE_CLOCK();
					
						//SW SEND DATA
						for(i=0;i<32;i++){
								if(((sw_info->sw_data>>i)&0x01))TMS_HIGH();
								else TMS_LOW();
								RELEASE_CLOCK();
						}
						if((sw_info->sw_cmd_parity&0x01))TMS_HIGH();
						else TMS_LOW();
						RELEASE_CLOCK();

				}
				
				TMS_LOW();
				for(i=0;i<8;i++){RELEASE_CLOCK();}
			
				return;
		}
		else if(ack == WAIT || ack == FAULT){
				sw_info->sw_result = ack;
			
				//SW GET DUMMY DATA
				if (sw_info->sw_rw == SW_READ)for(i=0;i<33;i++)RELEASE_CLOCK();
				
				//SW TURNAROUND
				RELEASE_CLOCK();
				
				//SW SEND DUMMY DATA
				if (sw_info->sw_rw == SW_WRITE){
						TMS_LOW();
						for(i=0;i<33;i++){RELEASE_CLOCK();}
				}
				
				TMS_HIGH();
				return;
		
		}
		else {
				sw_info->sw_result = BUS_CONFIG_ERROR;
				for(i=0;i<33;i++){RELEASE_CLOCK();}
				TMS_HIGH();
				return;
		}
		
}

/************************* END OF FILE jtag.c ************************/

