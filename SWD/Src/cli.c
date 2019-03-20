

/*------------------ Includes */
#include "headers.h"
/*------------------ Macros */


/*------------------ Typedef */

/*------------------ Variables */
CMDRESPONSE cmdresbuff;
CMDRESPONSE1 cmdresbuff1;

/*------------------ Constants */
const CMD cmdList[MAXCMD]={
		{.cmdName="help",							.cmdType=HELP,						.FuncPtr=&Help},
		{.cmdName="version",					.cmdType=VERSION,					.FuncPtr=&Version},
		{.cmdName="reset",						.cmdType=RESET,						.FuncPtr=&Reset},
		{.cmdName="delay",						.cmdType=DELAY,						.FuncPtr=&Delay},
		{.cmdName="swd_init",					.cmdType=SWD_INIT,				.FuncPtr=&Sw_ID},
		{.cmdName="swd_read_dp",			.cmdType=SWD_READ_DP,			.FuncPtr=&Sw_Read_Dp},
		{.cmdName="swd_write_dp",			.cmdType=SWD_WRITE_DP,		.FuncPtr=&Sw_Write_Dp},
		{.cmdName="swd_read_ap",			.cmdType=SWD_READ_AP,			.FuncPtr=&Sw_Read_Ap},
		{.cmdName="swd_write_ap",			.cmdType=SWD_WRITE_AP,		.FuncPtr=&Sw_Write_Ap},
		{.cmdName=NULL}	
};

/*------------------ Externs */
extern UART_HandleTypeDef huart1;
/*------------------ Private Functions */

int8_t validateCmd(GEN_CMD	*cmdinfo){
		uint8_t *cmdname,i=0;
		cmdname = cmdinfo->argv[0];
		
		for(i=0;i<MAXCMD;i++){
				if((!strcmp((char *)cmdname,(char *)cmdList[i].cmdName))){ 
						cmdinfo->cmdType=i;
						cmdinfo->FuncPtr=cmdList[cmdinfo->cmdType].FuncPtr;	
						return SUCCESS;
				}
		}
		return FAIL;
}

int8_t validateNoOfArgs(GEN_CMD	*cmdinfo){
		uint8_t maxAllowedArgs=1;
	
		if(cmdinfo->cmdType == HELP){maxAllowedArgs=1;}
		else if(cmdinfo->cmdType == VERSION){maxAllowedArgs=1;}
		else if(cmdinfo->cmdType == RESET){maxAllowedArgs=1;}
		else if(cmdinfo->cmdType == DELAY){maxAllowedArgs=2;}
		else if(cmdinfo->cmdType == SWD_INIT){maxAllowedArgs=1;}
		else if(cmdinfo->cmdType == SWD_READ_DP){maxAllowedArgs=2;}
		else if(cmdinfo->cmdType == SWD_WRITE_DP){maxAllowedArgs=3;}
		else if(cmdinfo->cmdType == SWD_READ_AP){maxAllowedArgs=2;}
		else if(cmdinfo->cmdType == SWD_WRITE_AP){maxAllowedArgs=3;}
			
		if(cmdinfo->argc==maxAllowedArgs){}
		else if(cmdinfo->argc > maxAllowedArgs)return FAIL;
		else if((cmdinfo->argc < maxAllowedArgs) )return FAIL;
			
		return SUCCESS;
}

int8_t validateArguments(GEN_CMD	*cmdinfo){
		GenCmdExtract(cmdinfo);
		if(cmdinfo->ArgInvalidFlag)return FAIL;
	
		return SUCCESS;
}

void cmdSyntax(uint8_t cmdType){
		uint8_t Syntax[]="\n\rSYNTAX: CmdName.";
		uint8_t DelayArgument[]="\n\rArgs:1=Msec[1-10000].";
		uint8_t ReadArgument[]="\n\rArgs:1=RegAddr[0-3].";
		uint8_t WriteArgument[]="\n\rArgs:1=RegAddr[0-3] 2=DataToWrite.";
		flushBuff(); 
		if(cmdType == HELP){sendResponse(Syntax,cmdresbuff.response);}
		else if(cmdType == VERSION){sendResponse(Syntax,cmdresbuff.response);}
		else if(cmdType == RESET){sendResponse(Syntax,cmdresbuff.response);}
		else if(cmdType == DELAY){sendResponse(Syntax,cmdresbuff.response);sendResponse(DelayArgument,cmdresbuff.response);}
		else if(cmdType == SWD_INIT){sendResponse(Syntax,cmdresbuff.response);}
		else if(cmdType == SWD_READ_DP){sendResponse(Syntax,cmdresbuff.response);sendResponse(ReadArgument,cmdresbuff.response);}
		else if(cmdType == SWD_WRITE_DP){sendResponse(Syntax,cmdresbuff.response);sendResponse(WriteArgument,cmdresbuff.response);}
		else if(cmdType == SWD_READ_AP){sendResponse(Syntax,cmdresbuff.response);sendResponse(ReadArgument,cmdresbuff.response);}
		else if(cmdType == SWD_WRITE_AP){sendResponse(Syntax,cmdresbuff.response);sendResponse(WriteArgument,cmdresbuff.response);}
		flushConsole();
};
	
uint8_t shell(uint8_t *cmd,GEN_CMD	*cmdinfo){
		int8_t result;
		uint8_t * args;uint8_t i=0;	
		uint8_t Error1[]="\n\rERROR: Invalid Command.";
		uint8_t Error2[]="\n\rERROR: Invalid No. of Argument.";
		uint8_t Error3[]="\n\rERROR: Invalid Argument.";
	
		args= (uint8_t *)strtok((char *)cmd," ");
		
		while(args!=NULL){
				cmdinfo->argv[i] =args;
				i++;
				args= (uint8_t *)strtok(NULL," ");
		}
		cmdinfo->argc=i;
			
		
		result=validateCmd(cmdinfo);
		if(result == FAIL) {flushBuff();sendResponse(Error1,cmdresbuff.response); flushConsole(); cmdSyntax(cmdinfo->cmdType);return InvalidCmd;}
		
		result=validateNoOfArgs(cmdinfo);
		if(result == FAIL) {flushBuff();sendResponse(Error2,cmdresbuff.response); flushConsole(); cmdSyntax(cmdinfo->cmdType);return InvalidNoOfArgs;}

		result=validateArguments(cmdinfo);
		if(result == FAIL) {flushBuff();sendResponse(Error3,cmdresbuff.response);  flushConsole(); cmdSyntax(cmdinfo->cmdType);return InvalidArgs;}
		
		
		cmdinfo->FuncPtr((uint8_t *)cmdinfo);

		return SUCCESS;
}

