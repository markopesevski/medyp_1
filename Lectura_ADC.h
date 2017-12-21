/********************************************************************
 * FileName:		Lectura_ADC.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librería para Lectura_ADC.c.									*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/02/17		Versión inicial.				*
 *																	*
 ********************************************************************/

/********************* Declaración de funciones *********************/
unsigned int Lectura_Galva(void);						// Rutina para leer la galvánica.
float Lectura_Temp(void);								// Rutina para leer temperatura sensor.
unsigned int Lectura_Stim(unsigned char Canal_Sti);		// Rutina para leer la estimulación.
unsigned int Lectura_RF(void);							// Rutina para leer la RF.
unsigned int Lectura_Probe(unsigned char Canal_Prob);	// Rutina para leer resistencia aplicador.
void Mirar_Aplicador(void);								// Mira qué aplicador y dónde.
void Activar_Salida (void);								// Activa salida.
void Control_Temperatura (void);						// Rutina para leer temperatura y encender ventilador.

/****************************** EOF *********************************/
