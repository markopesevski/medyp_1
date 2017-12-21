/********************************************************************
 * FileName:		Control_TLC5620.h								*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librería para Control_TLC5620.c.								*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/09/16		Versión inicial.				*
 *																	*
 ********************************************************************/

/********************* Declaración de funciones *********************/
void Carga_TLC5620(unsigned int datos_dac, unsigned char Num_Dac);		// Rutina para cargar el TLC5620 (DAC).

/****************************** EOF *********************************/
