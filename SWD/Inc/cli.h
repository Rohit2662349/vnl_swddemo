

#ifndef __CLI_HEADER
#define __CLI_HEADER

/*------------------ Includes */


/*------------------ Exported Macros */
#define HELP									0
#define VERSION								1
#define RESET									2
#define DELAY									3
#define SWD_INIT							4
#define SWD_READ_DP						5
#define SWD_WRITE_DP					6
#define SWD_READ_AP						7
#define SWD_WRITE_AP					8

#define sendResponse(data,buff)		strcat((char *)buff,(char *)data);
#define clrbuff(buff)							memset(buff,'\0',sizeof(buff));\
																	buff[0]='\0';
#define flushBuff()								clrbuff(cmdresbuff.response);
#define flushConsole()						HAL_UART_Transmit(&huart1,(uint8_t *)cmdresbuff.response,strlen((char *)cmdresbuff.response),100);\
																	HAL_UART_Transmit(&huart3,(uint8_t *)cmdresbuff.response,strlen((char *)cmdresbuff.response),100);
																	
#define MAXCMD						25
/*------------------ Exported Typedef */
typedef int8_t (*CmdFuncPtr)(uint8_t *);

typedef struct{
		uint32_t Iarg1;
		uint32_t Iarg2;
		uint32_t Iarg3;
		uint32_t Iarg4;
		float Farg1;
		float Farg2;
		uint8_t string1[20];
		uint8_t string2[20];
		uint8_t cmdType;
		uint8_t *argv[10];
		uint8_t argc;
		CmdFuncPtr FuncPtr;
		int8_t ArgInvalidFlag;
}GEN_CMD;

typedef struct cmd{
		uint8_t cmdName[20];
		uint8_t cmdType;
		CmdFuncPtr FuncPtr;
}CMD;

typedef struct cmdresponse{
		uint8_t cmd[512];
		uint8_t response[512];
}CMDRESPONSE;

typedef struct cmdresponse1{
		uint8_t cmd[50];
}CMDRESPONSE1;

extern CMDRESPONSE1 cmdresbuff1;
extern CMDRESPONSE cmdresbuff;
extern const CMD cmdList[MAXCMD];
/*------------------ Exported Variables */

/*------------------ Exported Constants */

/*------------------ Exported Externs */

/*------------------ Exported Functions */

void GenCmdExtract(GEN_CMD	*cmdinfo);
void cmdSyntax(uint8_t cmdType);
uint8_t shell(uint8_t *cmd,GEN_CMD	*cmdinfo);

#endif

