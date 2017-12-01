/********************************************************************
 * FileName:		Tratamientos.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librer�a para Tratamientos.c.									*
 *																	*
 * Change History:			Versi�n 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/09/16		Versi�n inicial.				*
 *																	*
 ********************************************************************/

/********************* Declaraci�n de funciones *********************/
unsigned char Analizar_Trama(void);							// Rutina para analizar trama recibida.
void Procesar_Ordenes(unsigned char Ordre);					// Rutina para procesar �rden de INTAB.
void Enciende_Stim(unsigned char V_Stim);					// Rutina para poner en marcha la estimulaci�n.
void Apaga_Stim(void);										// Rutina para apagar estimulaci�n.
void Enciende_Galva(unsigned char V_Galva);					// Rutina para poner en marcha la galv�nica.
void Apaga_Galva(void);										// Rutina para parar galv�nica.
void Enciende_Lift(unsigned char V_Lift);					// Rutina para poner en marcha el lifting.
void Apaga_Lift(void);										// Rutina para apagar el lifting
void Enciende_RF(unsigned char V_RF, unsigned char F_RF);	// Rutina para poner en marcha la RF.
void Apaga_RF(void);										// Rutina para apagar la RF.
void Responder(unsigned char Orden_Emb);					// Contesta las tramas de la INTAB.

/****************************** EOF *********************************/
