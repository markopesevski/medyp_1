/********************************************************************
 * FileName:		Delay.c											*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Realiza delays.												*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/09/16		Versión inicial.				*
 *																	*
 ********************************************************************/
#include "main.h"

/********************************************************************
 * Function:	DelayMs(time)										*
 * Description:	Delays execution on time specified in ms.			*
 ********************************************************************/
void DelayMs(WORD time)
{
	unsigned int ij = 0;

    while(time--)
    {
	    for (ij=0;ij<20000;ij++) Nop();	// OK
	}
}

/********************************************************************
 * Function:	DelayUs(time)										*
 * Description:	Delays execution on time specified in us.			*
 ********************************************************************/
void DelayUs(WORD time)
{
	unsigned char ji = 0;
	
    while(time--)
    {
	    for (ji=0;ji<16;ji++) Nop();	// OK
	}
}

/****************************** EOF *********************************/
