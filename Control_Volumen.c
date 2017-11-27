/********************************************************************
 * FileName:		Control_Volumen.c								*
 * Dependencies:	Control_Volumen.h								*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Configura y controla el zumbador.								*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		09/03/17		Versión inicial.				*
 *																	*
 ********************************************************************/
#include "main.h"

/********************************************************************
 * Function:	Control_Volumen(volum)								*
 * Definition:	Configura y controla el zumbador.					*
 ********************************************************************/
 void Control_Volumen(unsigned char volum)
{
	switch (volum)
	{
		case 0:	volum = 0;		break;
		case 1:	volum = 25;		break;
		case 2:	volum = 50;		break;
		case 3:	volum = 75;		break;
		case 4:	volum = 100;	break;
		case 5:	volum = 125;	break;
		case 6:	volum = 150;	break;
		case 7:	volum = 175;	break;
		case 8:	volum = 200;	break;
		case 9:	volum = 225;	break;
		case 10:volum = 250;	break;
		default:volum = 5;		break;
	}
	Carga_TLC5620(SOUND | volum,1);
}
	
/****************************** EOF *********************************/
