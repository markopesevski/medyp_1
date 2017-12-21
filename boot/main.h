/********************************************************************
 * FileName:		main.h											*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librería que incluye todas las librerías.						*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		16/11/15		Version inicial.				*
 *																	*
 ********************************************************************/
#include <plib.h>
#include <p32xxxx.h>
#include <stdlib.h>
#include <math.h>

#include "iomap.h"
#include "Control_TLC5620.h"
#include "Comunicacion.h"
#include "Actualizacion.h"
#include "usb.h"
#include "usb_function_hid.h"
#include "Control_TPIC.h"
#include "Control_TLC5620.h"

// System command
#define SYS_FREQ (80000000L)
#define GetSystemClock()		(SYS_FREQ)
#define	GetPeripheralClock()	(SYS_FREQ/(1 << OSCCONbits.PBDIV))
#define GetInstructionClock()	(SYS_FREQ)

#define T1_TICK					62500		// Tick general -> 	1tick = 50ms 	(a 80MHz).
#define T2_TICK					375			// Tick stim -> 	1tick = 150us 	(a 80MHz).
#define T3_TICK					3125		// Tick tiempo -> 	1tick = 10ms 	(a 80MHz).
#define T4_TICK					62500		// Tick tiempo ->	1tick = 50ms 	(a 80MHz).

// Valores predeterminados programa.
#define RX_DATA_BUFFER_ADDRESS
#define TX_DATA_BUFFER_ADDRESS

#define REC_FLASHED 				0
#define REC_NOT_FOUND 				1
#define REC_FOUND_BUT_NOT_FLASHED 	2

#define O_N					1
#define OFF					0

#define SI					1
#define NO					0

#define ON_POL				0
#define OFF_POL				1

#define POSITIVO			0
#define NEGATIVO			1

#define IN					0
#define OUT					1

// Órdenes
#define TRAMA_STATUS		2
#define TRAMA_VERSION		3
#define TRAMA_INICIA		8

#define TRAMA_FIRM			10
#define ERASE_BUF			12
#define TRANS_BUF			13

#define ACTUALIZA			60

// Valores predeterminados para TLV5620
#define REFDAC				0x100	// Selecciona salida A del dac.
#define DACGAL				0x300	// Selecciona salida B del dac.
#define SOUND				0x500	// Selecciona salida C del dac.
#define LEVEL				0x700	// Selecciona salida D del dac. 0x700 -Esta salida está x 2 (RNG = 1).

#define REFDACA				0x100	// Selecciona salida A del dac.
#define DACA				0x300	// Selecciona salida B del dac.
#define REFDACB				0x500	// Selecciona salida C del dac.
#define DACB				0x700	// Selecciona salida D del dac.

#define REFDACDDS			0x100	// Selecciona salida A del dac.
#define DACDDS				0x300	// Selecciona salida B del dac.
#define REFDACBIAS			0x500	// Selecciona salida C del dac.
#define DACBIAS				0x700	// Selecciona salida D del dac.

// Valores predeterminados NVM.
#define NVM_PROGRAM_PAGE 	0xBD01F000		// MIRAR BIEN DONDE ESCRIBO (en el .map)

/****************************** EOF *********************************/
