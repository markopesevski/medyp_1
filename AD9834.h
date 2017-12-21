/********************************************************************
 * FileName:		AD9834.h										*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librer�a para AD9834.c.										*
 *																	*
 * Change History:			Versi�n 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		07/12/16		Versi�n inicial.				*
 *																	*
 ********************************************************************/

#define AD9834_BAUDRATE		1000000
#define FREQ0				0x4000
#define FREQ1				0x8000

/********************* Declaraci�n de funciones *********************/
void Config_AD9834 (void);							// Configura coms serie para AD9834.
void Set_Freq_AD9834 (unsigned long Frecuencya);	// Rutina para cargar el AD9834 (DDS).
void Suspend_AD9834 (void);							// Suspende AD9834.

/****************************** EOF *********************************/
