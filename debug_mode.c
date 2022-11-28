#include "debug_mode.h"

uint16_t DataLength=0;
char DE_SetParameter[10]={0};
char DE_UARTBUFFER[150]={0};
// reverses a string 'str' of length 'len' 
void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 
  
 // Converts a given integer x to string str[].  d is the number 
 // of digits required in output. If d is more than the number 
 // of digits in x, then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    //str[i] = ' '; 
    return i; 
} 
  
// Converts a floating point number to string. 
void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
    if(ipart == 0 )
		{
			res[i++]='0';
		}
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

int  debug_print(void)
{
	memset(DE_SetParameter,0,sizeof(DE_SetParameter));	
	for(int index=1; index <= 8; index++)
	{
		ftoa(*(writePtr+index),DE_SetParameter,1); 
		strcat(( char *)DE_SetParameter,", ");
		strcat(( char *)DE_UARTBUFFER,( char *)DE_SetParameter);
		memset(DE_SetParameter,0,sizeof(DE_SetParameter));	
	}
//	UARTFlushTx2(1);
	if(ctrlReadParam.Status)
	{
		intToStr((int)ctrlReadParam.Status,DE_SetParameter,0);
		strcat(( char *)DE_SetParameter,"\n");
	}
	else
	{
		DE_SetParameter[0]='0';
		DE_SetParameter[1]='.';
		DE_SetParameter[2]='0';
		DE_SetParameter[3]='\n';
	}
	
	  strcat(( char *)DE_UARTBUFFER,( char *)DE_SetParameter);
		DataLength = strlen(DE_UARTBUFFER);
		UARTFlushTx2(1);
		UARTwrite2((const char *)DE_UARTBUFFER, DataLength);
		memset(DE_UARTBUFFER,0,sizeof(DE_UARTBUFFER));

//			DataLength=strlen(DE_SetParameter);
//			UARTwrite2((const char *)DE_SetParameter, DataLength);

	//		UARTCharPutNonBlocking(UART2_BASE,'.');
//		UARTCharPutNonBlocking(UART2_BASE,'\n');
	return 0;
	
}