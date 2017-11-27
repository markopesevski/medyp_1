/********************************************************************
 * FileName:		Control_TPIC.c									*
 * Dependencies:	Control_TPIC.h									*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Configura y controla el TPIC565.								*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		11/11/15		Versión inicial.				*
 *																	*
 ********************************************************************/
#include "main.h"

extern union unio_rel{
	unsigned int Byte;
	struct u_rel{
		unsigned char Rel3:1;		// 
		unsigned char Rel2:1;		// 
		unsigned char Rel1:1;		// 
		unsigned char TstCh2:1;		// 
		unsigned char TstCh1:1;		// 
		unsigned char OnCh3:1;		// 
		unsigned char OnCh2:1;		// 
		unsigned char OnCh1:1;		// 	
	}Bit;	
}Rele;
union unio_rel Rele;

extern union unio_led{
	unsigned int Byte;
	struct u_led{
		unsigned char Out8:1;		// 
		unsigned char Out7:1;		// 
		unsigned char RampOff:1;	// 
		unsigned char LedP3:1;		// 
		unsigned char LedP2:1;		// 
		unsigned char LedP1:1;		// 
		unsigned char ON5V:1;		// 
		unsigned char LedPB:1;		// 
	}Bit;	
}Led;
union unio_led Led;

/********************************************************************
 * Function:	Control_TPIC()										*
 * Description:	Carga tpics.										*
 ********************************************************************/
void Control_TPIC (void)
{
	unsigned int datos = 0;			// Datos a cargar en TPIC.
	unsigned char imagen_datos = 0;	// Imagen datos a cargar en TPIC.
	unsigned char contador = 0;
	
	datos = Rele.Byte;
	datos<<=0x08;
	datos = datos + Led.Byte;

	for (contador = 0;contador <= 15;contador++)	// Cargo los dos tpics.
	{
		imagen_datos = datos & 0x01;
		if (imagen_datos == 1)
		{
			DATA_TPIC = 1;
			Nop();
			CLK_TPIC = 0;
			Nop();
			Nop();
			Nop();
			CLK_TPIC = 1; 
			Nop();
			Nop();
			Nop();
		}
		else
		{
			DATA_TPIC = 0;
			Nop();
			CLK_TPIC = 0;
			Nop();
			Nop();
			Nop();
			CLK_TPIC = 1; 
			Nop();
			Nop();
			Nop();
		}  
		datos>>=0x01;   
	}
	LOAD_TPIC = 0;
	Nop();
	Nop();
	Nop();
	Nop();
	LOAD_TPIC = 1;
} 

/******************************* EOF ********************************/
