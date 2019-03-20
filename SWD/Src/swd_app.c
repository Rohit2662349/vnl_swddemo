
/* Includes ------------------------------------------------------------------*/
#include "headers.h"

/* Macros ------------------------------------------------------------------*/


/* Private Constants ---------------------------------------------------------*/


/* External Variables ---------------------------------------------------------*/


/* Private Variables ---------------------------------------------------------*/


/* Private Function -----------------------------------------------*/


/* App Start -----------------------------------------------------------------*/

void DutPrompt(){
		flushBuff();
		sendResponse("\n\r\n\rSwd_Demo>>",cmdresbuff.response);
		flushConsole();
		flushBuff();
}

int stdout_putchar (int ch){
		HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,100);
		HAL_UART_Transmit(&huart3,(uint8_t *)&ch,1,100);
		return SUCCESS;
}

int8_t Help(uint8_t *argv){
		uint8_t i=0;
		flushBuff();
		sendResponse("\n\r/********* Supported Command Set ********/ ",cmdresbuff.response);
		for(i=0;i<MAXCMD;i++){
				if(cmdList[i].cmdName[0]=='\0')break;
				sendResponse("\n\r--> ",cmdresbuff.response);
				sendResponse(cmdList[i].cmdName,cmdresbuff.response);
		}
		sendResponse("\n\r\n\r\0",cmdresbuff.response);
		flushConsole();
		flushBuff();
		return SUCCESS;
}

int8_t Version(uint8_t *argv){
		flushBuff();
		sendResponse("\n\rVersion : 1.0.0",cmdresbuff.response);
		flushConsole();
		flushBuff();
		return SUCCESS;
}

int8_t Reset(uint8_t *argv){
		NVIC_SystemReset();
		return SUCCESS;
}

int8_t Delay(uint8_t *argv){
		GEN_CMD *local_info;
		local_info=(GEN_CMD *)(argv);
		HAL_Delay(local_info->Iarg1);
		return SUCCESS;
}

int8_t Sw_ID(uint8_t *argv){
		SWD_INFO sw_info;
//		GEN_CMD *cmd;
//		cmd=(GEN_CMD *)argv;
	
		HAL_SW_Init();
	
		memset(&sw_info,0,sizeof(SWD_INFO));
		HAL_SW_DP_Read(&sw_info,DP_IDCODE);
		HAL_SW_IO(&sw_info);
		printf("\n\rTARGET_ID = 0x%x, Result:%d, Parity:%d_%x\n\r",sw_info.sw_data,sw_info.sw_result,sw_info.sw_data_parity,sw_info.sw_cmd);
		return SUCCESS;
}


void CalParity(uint32_t  data_write,uint8_t * parity){
		uint8_t i=0,j=0;
		for(i=0;i<32;i++){
				if((data_write >> i) & 0x01)j++;
		}
		if(!(j%2)) *parity=0;
		else *parity=1;
}
	
int8_t Sw_Read_Dp(uint8_t *argv){
		SWD_INFO sw_info;
		GEN_CMD *cmd;
		cmd=(GEN_CMD *)argv;
	
		memset(&sw_info,0,sizeof(SWD_INFO));
		HAL_SW_DP_Read(&sw_info,cmd->Iarg1);
		HAL_SW_IO(&sw_info);
	  printf("\n\rDP_Read = 0x%x, ACK:%d, Parity:%d, Cmd:%x\n\r",sw_info.sw_data,sw_info.sw_result,sw_info.sw_data_parity,sw_info.sw_cmd);
		return SUCCESS;
}

int8_t Sw_Read_Ap(uint8_t *argv){
		SWD_INFO sw_info;
		GEN_CMD *cmd;
		cmd=(GEN_CMD *)argv;
	
		memset(&sw_info,0,sizeof(SWD_INFO));
		HAL_SW_AP_Read(&sw_info,cmd->Iarg1);
		HAL_SW_IO(&sw_info);
	  printf("\n\rAP_Read = 0x%x, ACK:%d, Parity:%d, Cmd:%x\n\r",sw_info.sw_data,sw_info.sw_result,sw_info.sw_data_parity,sw_info.sw_cmd);
		return SUCCESS;
}

int8_t Sw_Write_Dp(uint8_t *argv){
		SWD_INFO sw_info;
		GEN_CMD *cmd;
		cmd=(GEN_CMD *)argv;
	
		
		memset(&sw_info,0,sizeof(SWD_INFO));
		sw_info.sw_data = cmd->Iarg2;
		CalParity(sw_info.sw_data,&sw_info.sw_data_parity);
		HAL_SW_DP_Write(&sw_info,cmd->Iarg1);
		HAL_SW_IO(&sw_info);

		memset(&sw_info,0,sizeof(SWD_INFO));
		sw_info.sw_data = cmd->Iarg2;
		CalParity(sw_info.sw_data,&sw_info.sw_data_parity);
		HAL_SW_AP_Write(&sw_info,cmd->Iarg1);
		HAL_SW_IO(&sw_info);
	
	  printf("\n\rDP_Write = 0x%x, ACK:%d, Parity:%d, Cmd:%x\n\r",sw_info.sw_data,sw_info.sw_result,sw_info.sw_data_parity,sw_info.sw_cmd);
		return SUCCESS;
}

int8_t Sw_Write_Ap(uint8_t *argv){
		SWD_INFO sw_info;
		GEN_CMD *cmd;
		cmd=(GEN_CMD *)argv;
	
		memset(&sw_info,0,sizeof(SWD_INFO));
		sw_info.sw_data = cmd->Iarg2;
		CalParity(sw_info.sw_data,&sw_info.sw_data_parity);
		HAL_SW_AP_Write(&sw_info,cmd->Iarg1);
		HAL_SW_IO(&sw_info);
	
	  printf("\n\rAP_Write = 0x%x, ACK:%d, Parity:%d, Cmd:%x\n\r",sw_info.sw_data,sw_info.sw_result,sw_info.sw_data_parity,sw_info.sw_cmd);
		return SUCCESS;
}


/************************ END OF FILE  ****************************************/

