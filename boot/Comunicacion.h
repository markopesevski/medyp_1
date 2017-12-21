/********************************************************************
 * FileName:		Comunicacion.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librería para Comunicacion.c.									*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		18/10/16		Versión inicial.				*
 *																	*
 ********************************************************************/

/********************* Declaración de funciones *********************/
void ProcessIO(void);					// Controla coms con INTAB.
void USBCBSendResume(void);
void Calc_Check (void);
unsigned char Analizar_Trama(void);
void Limpia_Buffer (unsigned char Dire);

/****************************** EOF *********************************/