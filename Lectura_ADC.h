/********************************************************************
 * FileName:		Lectura_ADC.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librer�a para Lectura_ADC.c.									*
 *																	*
 * Change History:			Versi�n 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/02/17		Versi�n inicial.				*
 *																	*
 ********************************************************************/

/********************* Declaraci�n de funciones *********************/
unsigned int Lectura_Galva(void);						// Rutina para leer la galv�nica.
float Lectura_Temp(void);								// Rutina para leer temperatura sensor.
unsigned int Lectura_Stim(unsigned char Canal_Sti);		// Rutina para leer la estimulaci�n.
unsigned int Lectura_RF(void);							// Rutina para leer la RF.
unsigned int Lectura_Probe(unsigned char Canal_Prob);	// Rutina para leer resistencia aplicador.
void Mirar_Aplicador(void);								// Mira qu� aplicador y d�nde.
void Activar_Salida (void);								// Activa salida.
void Control_Temperatura (void);						// Rutina para leer temperatura y encender ventilador.

/****************************** EOF *********************************/
