/********************************************************************
 * FileName:		iomap.h											*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Mapa de entradas salidas.										*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*	
 * -----			-----			--------						*
 * J. Tejedor		08/09/16		Versión inicial.				*
 *																	*
 ********************************************************************/

// Salidas
#define LOAD_DAC2		LATBbits.LATB0
//#define PULSE_P			LATBbits.LATB2
#define PULSE_N			LATBbits.LATB3

#define ON_GP			LATCbits.LATC13
#define PULSE_P			LATCbits.LATC14

#define LOAD_DAC1		LATDbits.LATD0
#define SCLK_AD9834		LATDbits.LATD1
#define ONGNL			LATDbits.LATD2
#define SDAT_AD9834		LATDbits.LATD3
#define ONGPL			LATDbits.LATD4
#define LOAD_DAC3		LATDbits.LATD5
#define RST_BT			LATDbits.LATD6
#define LDAC			LATDbits.LATD7
#define FSYNC_AD9834	LATDbits.LATD8
#define DATA_DAC		LATDbits.LATD9
#define CLK_DAC			LATDbits.LATD10
#define ON_GN			LATDbits.LATD11

//#define TEST1			LATEbits.LATE0
//#define TEST2			LATEbits.LATE1
#define LOAD_TPIC		LATEbits.LATE4
#define LED_RUN			LATEbits.LATE5
#define HS_FAN			LATEbits.LATE6
#define EXT_FAN			LATEbits.LATE7

#define CLK_TPIC		LATFbits.LATF0
#define DATA_TPIC		LATFbits.LATF1
#define RST_USB			LATFbits.LATF3
#define TXD				LATFbits.LATF5

#define BEEP			LATGbits.LATG6
#define TXBT			LATGbits.LATG8
#define RST_AD9834		LATGbits.LATG9

// Entradas
#define T5VTAB			PORTBbits.RB1
#define T5V				PORTBbits.RB2
#define SP2				PORTBbits.RB4
#define SP1				PORTBbits.RB5
#define AN_RF			PORTBbits.RB8
#define SP3				PORTBbits.RB9
#define SENS1			PORTBbits.RB10
#define SENS2			PORTBbits.RB11
#define AN_TEMP			PORTBbits.RB12
#define AN_GAL			PORTBbits.RB13
#define T24V			PORTBbits.RB14
#define T24VRF			PORTBbits.RB15

#define FSW				PORTEbits.RE2
#define ON_OFF			PORTEbits.RE3

#define RXD				PORTFbits.RF4

#define RXBT			PORTGbits.RG7

/****************************** EOF *********************************/
