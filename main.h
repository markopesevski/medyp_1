/********************************************************************
 * FileName:		main.h											*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librer�a que incluye todas las librer�as.						*
 *																	*
 * Change History:			Versi�n 0.0								*
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
#include "Delay.h"
#include "Tratamientos.h"
#include "usb.h"
#include "usb_function_hid.h"
#include "Control_TPIC.h"
#include "AD9834.h"
#include "Lectura_ADC.h"
#include "Control_Volumen.h"

#define DEBUGAR

// System command
#define SYS_FREQ (80000000L)
#define GetSystemClock()		(SYS_FREQ)
#define	GetPeripheralClock()	(SYS_FREQ/(1 << OSCCONbits.PBDIV))
#define GetInstructionClock()	(SYS_FREQ)

#define T1_TICK					62500		// Tick general -> 	1tick = 50ms 	(a 80MHz).
#define T3_TICK					3125		// Tick galva -> 	1tick = 10ms 	(a 80MHz).
#define T4_TICK					62500		// Tick barrido ->	1tick = 50ms 	(a 80MHz).
#define T5_TICK					125			// Tick electrolif->1tick = 25us 	(a 80MHz).

// Valores predeterminados programa.
#define RX_DATA_BUFFER_ADDRESS
#define TX_DATA_BUFFER_ADDRESS

#define O_N					1
#define OFF					0
#define SI					1
#define NO					0

#define ON_POL				0
#define OFF_POL				1

#define IN					0
#define OUT					1

#define FACIAL				1	// Aplicador facial.
#define ESPE				2	// Aplicador espec�fico.
#define CORPORAL			3	// Aplicador corporal.

#define PROBE1				1
#define PROBE2				2
#define PROBE3				3

#define TEMPERATURA_HS		30
#define TEMPERATURA_ALARMA	90	// A esta temperatura se para la RF.
#define SEGS_HS				60	// 1 minuto.
#define VOL_FAB				0//1//5 TODO

#define DAC_GAL_FAB			56//150	2.10.17 Se rebaja m�xima corriente a 1.5mA
#define DAC_STIM_FAB		110
#define DAC_RF_FAB			114//195//200
#define DAC_BIAS_FAB		82	// Con 82 hay 1.41V en DACBIAS (R153)

#define TIEMPO_REDUCIR		10	// Tiempo seguido sin detectar conducci�n para reducir RF. (0,2s x 10 = 2s)

// �rdenes
#define TRAMA_STATUS		2
#define TRAMA_VERSION		3
#define CONT_BUZZER			5
#define NIVEL_BUZZER		6
#define TRAMA_INICIA		8

#define TRAMA_FIRM			10
#define ERASE_BUF			12
#define TRANS_BUF			13

#define READ_TEMP			25
#define READ_ALARM_TEMP		26
#define SET_ALARM_TEMP		27

#define	READ_GAL			30
#define	SET_GAL				31
#define	READ_STIM			35
#define	SET_STIM			36
#define	READ_RF				40
#define	SET_RF				41
#define	READ_BIAS			43
#define	SET_BIAS			44
#define REC_CAL				48

#define READ_NSERIE			50
#define REC_NSERIE			51

#define ACTUALIZA			60
#define RST_FAB				88

// Valores predeterminados para TLV5620
#define REFDAC				0x100	// Selecciona salida A del dac.
#define DACGAL				0x300	// Selecciona salida B del dac.
#define SOUND				0x500	// Selecciona salida C del dac.
#define LEVEL				0x700	// Selecciona salida D del dac. 0x700 -Las salidas est�n x 2 (RNG = 1).

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
