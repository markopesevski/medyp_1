/********************************************************************
 * FileName:		Control_TLC5620.c								*
 * Dependencies:	Control_TLC5620.h								*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Configura y controla el TLC5620 (DAC).						*
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
 * Function:	Carga_TLC5620(datos_dac,Num_Dac)					*
 * Definition:	Rutina para cargar el TLC5620 (DAC).				*
 ********************************************************************/
void Carga_TLC5620 (unsigned int datos_dac, unsigned char Num_Dac)
{
	unsigned char cont;				
	unsigned int imagen_datosdac;   //	Imagen datos a cargar en DAC.
	
	/* data is shifted bit by bit, and loaded into IC on clk falling edges */
	for (cont=0;cont<=10;cont++)	// XH entiendo que primero carga el buffer de data
	{
		imagen_datosdac = datos_dac&0x400;			// XH envia 11 bits, A1 A0 (indiquen num DAC), RNG (sempre 1), 8bits dades.
		if (imagen_datosdac == 0x400)
		{
			DATA_DAC = 1;	// XH carrega el bit 0x400, i els va shiftant a l'esquerra, fins que fa els 11 i carrega tot buffer
		}
		else
		{
			DATA_DAC = 0;
		}
		
		/* falling edge to clock data into IC */
		CLK_DAC = 1;
		Nop();
		Nop();
		CLK_DAC = 0;
		datos_dac<<=0x01;
	}
	switch (Num_Dac)			// Carga los valores del buffer al dac que quiere.
	{
		case 1:
			LOAD_DAC1 = 1;		//	Para hacer ____	   ____
			Nop();				//				  |	   |
		 	LOAD_DAC1 = 0;		//				  |____| flanco de bajada
		 	Nop();					
			LOAD_DAC1 = 1;
		break;
		case 2:
			LOAD_DAC2 = 1;		//	Para hacer ____	   ____
			Nop();				//				  |	   |
		 	LOAD_DAC2 = 0;		//				  |____| flanco de bajada
		 	Nop();					
			LOAD_DAC2 = 1;
		break;
		case 3:
			LOAD_DAC3 = 1;		//	Para hacer ____	   ____
			Nop();				//				  |	   |
		 	LOAD_DAC3 = 0;		//				  |____| flanco de bajada
		 	Nop();					
			LOAD_DAC3 = 1;
		break;
		default:		;		break;
	}
	LDAC = 0;
	Nop();	
	LDAC = 1;
}

/****************************** EOF *********************************/
