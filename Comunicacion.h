/********************************************************************
 * FileName:		Comunicacion.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librer�a para Comunicacion.c.									*
 *																	*
 * Change History:			Versi�n 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		18/10/16		Versi�n inicial.				*
 *																	*
 ********************************************************************/

#define BOOT_FLASH_ADDRESS		0xBFC00000

/********************* Declaraci�n de funciones *********************/
void ProcessIO(void);					// Controla coms con INTAB.
void USBCBSendResume(void);
void Calc_Check (void);
void Limpia_Buffer (unsigned char Dire);
void JumpToBoot(void);

/****************************** EOF *********************************/