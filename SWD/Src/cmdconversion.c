

/*------------------ Includes */
#include "headers.h"
/*------------------ Macros */

/*------------------ Typedef */

/*------------------ Variables */



/*------------------ Constants */

/*------------------ Externs */

/*------------------ Private Functions */

int32_t StrToInt(char *str){
		int8_t i=0,len;
		int32_t temp_int=0;
		len = strlen(str);
		for(i=0; i<len; i++){
				temp_int = temp_int * 10 + ( str[i] - '0' );
		}
		return temp_int;
}

void GenCmdExtract(GEN_CMD	*cmdinfo){
		if(cmdinfo->cmdType == HELP){}
		else if(cmdinfo->cmdType == VERSION){}
		else if(cmdinfo->cmdType == RESET){}
		else if(cmdinfo->cmdType == DELAY){cmdinfo->Iarg1 = StrToInt((char *)cmdinfo->argv[1]);}
		else if(cmdinfo->cmdType == SWD_INIT){}
		else if(cmdinfo->cmdType == SWD_READ_DP){
				cmdinfo->Iarg1 = StrToInt((char *)cmdinfo->argv[1]);
		}
		else if(cmdinfo->cmdType == SWD_WRITE_DP){
				cmdinfo->Iarg1 = StrToInt((char *)cmdinfo->argv[1]);
				cmdinfo->Iarg2 = StrToInt((char *)cmdinfo->argv[2]);
		}
		else if(cmdinfo->cmdType == SWD_READ_AP){
				cmdinfo->Iarg1 = StrToInt((char *)cmdinfo->argv[1]);
		}
		else if(cmdinfo->cmdType == SWD_WRITE_AP){
				cmdinfo->Iarg1 = StrToInt((char *)cmdinfo->argv[1]);
				cmdinfo->Iarg2 = StrToInt((char *)cmdinfo->argv[2]);
		}
}


