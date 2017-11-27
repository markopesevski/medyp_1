/********************************************************************
 * FileName:		Tratamientos.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librería para Tratamientos.c.									*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/09/16		Versión inicial.				*
 *																	*
 ********************************************************************/

/********************* Declaración de funciones *********************/
unsigned char Analizar_Trama(void);							// Rutina para analizar trama recibida.
void Procesar_Ordenes(unsigned char Ordre);					// Rutina para procesar órden de INTAB.
void Enciende_Stim(unsigned char V_Stim);					// Rutina para poner en marcha la estimulación.
void Apaga_Stim(void);										// Rutina para apagar estimulación.
void Enciende_Galva(unsigned char V_Galva);					// Rutina para poner en marcha la galvánica.
void Apaga_Galva(void);										// Rutina para parar galvánica.
void Enciende_Lift(unsigned char V_Lift);					// Rutina para poner en marcha el lifting.
void Apaga_Lift(void);										// Rutina para apagar el lifting
void Enciende_RF(unsigned char V_RF, unsigned char F_RF);	// Rutina para poner en marcha la RF.
void Apaga_RF(void);										// Rutina para apagar la RF.
void Responder(unsigned char Orden_Emb);					// Contesta las tramas de la INTAB.

/****************************** EOF *********************************/
