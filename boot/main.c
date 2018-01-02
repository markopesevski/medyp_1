/********************************************************************
 * FileName:		main.c 											*
 * Dependencies:	main.h											*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Software del equipo Mesotreat de Rös's.						*
 *																	*
 * Change History:		(See file "History.txt")					*
 *																	*
 *														J. Tejedor	*
 ********************************************************************/
/*
	TODO:
	- Hacer un timeout y si no se conecta resetear hub???
*/
/*
	Timer 1 --> Tick general
*/
#include "main.h"

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)

#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_1, FWDTEN = OFF 
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1, OSCIOFNC = OFF
#pragma config CP = ON, BWP = OFF, PWP = OFF, FSOSCEN = OFF, FCKSM = CSECME, DEBUG = OFF 	// Programar.
//#pragma config CP = OFF, BWP = OFF, PWP = OFF, FSOSCEN = OFF, FCKSM = CSECME				// Debugar. 
#pragma config IESO = OFF, ICESEL = ICS_PGx2 
#pragma config UPLLEN = ON, UPLLIDIV = DIV_2
#pragma config FVBUSONIO = OFF
/**/
#pragma config FUSBIDIO = OFF
/**/

/********************* Declaración de variables *********************/
/* Variables generales */
volatile unsigned char tick_T1 = 0;		// Tick general (cada 50 ms).
volatile unsigned char Cont_Blink = 0;
unsigned char Hay_App = 0;
unsigned char Blink_PB = 0;

// Usb
unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
unsigned char ToSendDataBuffer[64] TX_DATA_BUFFER_ADDRESS;

USB_HANDLE USBOutHandle = 0;    //USB handle.  Must be initialized to 0 at startup.
USB_HANDLE USBInHandle = 0;     //USB handle.  Must be initialized to 0 at startup.

/* Estructuras, enumeraciones y uniones */ 
union unio_rel{
	unsigned int Byte;
	struct u_rel{
		unsigned char Rel3:1;		// 
		unsigned char Rel2:1;		// 
		unsigned char Rel1:1;		// 
		unsigned char TstCh2:1;		// 
		unsigned char TstCh1:1;		// 
		unsigned char OnCh3:1;		// 
		unsigned char OnCh2:1;		// 
		unsigned char OnCh1:1;		// 	
	}Bit;	
}Rele;
union unio_rel Rele;

union unio_led{
	unsigned int Byte;
	struct u_led{
		unsigned char Out8:1;		// 
		unsigned char Out7:1;		// 
		unsigned char RampOff:1;	// 
		unsigned char LedP3:1;		// 
		unsigned char LedP2:1;		// 
		unsigned char LedP1:1;		// 
		unsigned char ON5V:1;		// 
		unsigned char LedPB:1;		// 
	}Bit;	
}Led;
union unio_led Led;

/********************* Declaración de funciones *********************/
void Config_System (void);					// Configura oscilador del sistema.
void Config_Ports (void);					// Configura puertos.
void Init_Regs (void);						// Inicializa registros.
void Config_Timer1 (void);					// Configura el Timer1 (tick general).
void Stop_Timer1(void);						// Para el Timer 1.

/*********************************************************************
 *********************************************************************
 ** Function:		MAIN()											**
 ** Description:	PROGRAMA PRINCIPAL.								**
 *********************************************************************
 *********************************************************************/
int main(void)
{
	Config_System();
	Config_Ports();
	Init_Regs();
		
	Config_Timer1();

	USBDeviceAttach();

	while(1)
	{
		ProcessIO();
		
		if (Cont_Blink >= 5)	// Cada 250 ms hago blink.
		{		
			Cont_Blink = 0;
			if (Blink_PB == 0)
			{
				Led.Bit.LedPB = 0;
				Blink_PB = 1;
			}
			else
			{
				Led.Bit.LedPB = 1;
				Blink_PB = 0;	
			}	
			Control_TPIC();
		}
	}
}//End Main	

/********************************************************************
 * Function:	Config_System()										*
 * Description:	Configura el oscilador del sistema.					*
 ********************************************************************/
void Config_System (void)
{
	int value;
	
	DDPCONbits.JTAGEN = 0;		// Habilitar al programar.
	    
	value = SYSTEMConfigWaitStatesAndPB (GetSystemClock());

	CheKseg0CacheOn();			// Enable the cache for the best performance.

	INTEnableSystemMultiVectoredInt();

	value = OSCCON;
	while (!(value & 0x00000020))
	{
		value = OSCCON;			// Wait for PLL lock to stabilize.
	}

//	The USB specifications require that USB peripheral devices must never source
//	current onto the Vbus pin.  Additionally, USB peripherals should not source
//	current on D+ or D- when the host/hub is not actively powering the Vbus line.
//	When designing a self powered (as opposed to bus powered) USB peripheral
//	device, the firmware should make sure not to turn on the USB module and D+
//	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
//	firmware needs some means to detect when Vbus is being powered by the host.
//	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
// 	can be used to detect when Vbus is high (host actively powering), or low
//	(host is shut down or otherwise not supplying power).  The USB firmware
// 	can then periodically poll this I/O pin to know when it is okay to turn on
//	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
//	peripheral device, it is not possible to source current on D+ or D- when the
//	host is not actively providing power on Vbus. Therefore, implementing this
//	bus sense feature is optional.  This firmware can be made to use this bus
//	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
//	HardwareProfile.h file.    
#if defined(USE_USB_BUS_SENSE_IO)
	tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
#endif
    
//	If the host PC sends a GetStatus (device) request, the firmware must respond
//	and let the host know if the USB peripheral device is currently bus powered
//	or self powered.  See chapter 9 in the official USB specifications for details
//	regarding this request.  If the peripheral device is capable of being both
//	self and bus powered, it should not return a hard coded value for this request.
//	Instead, firmware should check if it is currently self or bus powered, and
//	respond accordingly.  If the hardware has been configured like demonstrated
//	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
//	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
//	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
//	has been defined in HardwareProfile - (platform).h, and that an appropriate I/O pin 
//  has been mapped	to it.
#if defined(USE_SELF_POWER_SENSE_IO)
	tris_self_power = INPUT_PIN;	// See HardwareProfile.h
#endif
}
 	
/********************************************************************
 * Function:	Config_Ports()										*
 * Description:	Configura los puertos I/O.							*
 ********************************************************************/
void Config_Ports (void)	// 1->input, 0->output
{
	// Drive low & drive high por seguridad de inicio.
//	RST_USB = 0;
//	DelayMs(20);
	RST_USB = 1;		// Habilito HUB.
	
	ON_GN = OFF_POL;
	ON_GP = OFF_POL;
	PULSE_N = 0;
	PULSE_P = 0;
//	TEST1 = 0;
//	TEST2 = 0;
	EXT_FAN = 0;		// Apago ventilador externo.
	HS_FAN = 0;			// Apago ventilador radiador.
	
	LDAC = 1;
	BEEP = 0;
		
	AD1PCFG = 0xFFFF;	// Analog inputs in digital mode.

	// Configuración de puertos.
	// PUERTO B
	mPORTBSetPinsAnalogIn  (BIT_1 | BIT_2 | BIT_4 | BIT_5 | BIT_8 | BIT_9 | BIT_10 | BIT_11 | BIT_12 | BIT_13 | BIT_14 | BIT_15);
	mPORTBSetPinsDigitalOut(BIT_0 | BIT_3); 
	
	// PUERTO C
	mPORTCSetPinsDigitalOut(BIT_13 | BIT_14); 
	
	// PUERTO D
	mPORTDSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4 | BIT_5 | BIT_6 | BIT_7 | BIT_8 | BIT_9 | BIT_10 | BIT_11); 
	
	// PUERTO E
	mPORTESetPinsDigitalIn (BIT_2 | BIT_3);
	mPORTESetPinsDigitalOut(BIT_4 | BIT_5 | BIT_6 | BIT_7); 
	
	// PUERTO F
	mPORTFSetPinsDigitalIn (BIT_4);
	mPORTFSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_3 | BIT_5);

	// PUERTO G
	mPORTGSetPinsDigitalIn (BIT_7);
	mPORTGSetPinsDigitalOut(BIT_6 | BIT_8 | BIT_9); 

	// Salidas a 0.
//	RST_USB = 0;		// HUB deshabilitado.
	RST_USB = 1;		// Habilito HUB.	
	
	ON_GN = OFF_POL;
	ON_GP = OFF_POL;
	PULSE_N = 0;
	PULSE_P = 0;
//	TEST1 = 0;
//	TEST2 = 0;
	
	BEEP = 0;
	LDAC = 1;
	
	Carga_TLC5620(REFDAC,1);	// TODO VER si el nivel bajo es 0 ó 0xFF
	Carga_TLC5620(DACGAL,1);
	Carga_TLC5620(SOUND,1);
	Carga_TLC5620(LEVEL,1);
	
	Carga_TLC5620(REFDACA,2);
	Carga_TLC5620(DACA,2);
	Carga_TLC5620(REFDACB,2);
	Carga_TLC5620(DACB,2);
	
	Carga_TLC5620(REFDACDDS,3);
	Carga_TLC5620(DACDDS,3);
	Carga_TLC5620(REFDACBIAS,3);
	Carga_TLC5620(DACBIAS,3);
	
	Rele.Byte = 0;
	Led.Byte = 0;
	
	Led.Bit.ON5V = 1;			// De momento enciendo alimentaciones.
	Led.Bit.LedPB = 1;			// Todo mirar
	Led.Bit.RampOff = 0;
	
	Rele.Bit.OnCh1 = OFF;		// Desactivo salida 1.
	Rele.Bit.OnCh2 = OFF;		// Desactivo salida 2.
	Rele.Bit.OnCh3 = OFF;		// Desactivo salida 3.
	Rele.Bit.TstCh1 = OFF;		// Deshabilito test
	Rele.Bit.TstCh2 = OFF;		// Deshabilito test	
	
	Control_TPIC();
			
	EXT_FAN = 1;		// Enciendo ventilador externo.
}

/********************************************************************
 * Function:	Init_Regs()											*
 * Description:	Inicializa los registros.							*
 ********************************************************************/
void Init_Regs (void)
{
	USBOutHandle = 0;	// Initialize the variable holding the handle for the last transmission.
	USBInHandle = 0;
	USBDeviceInit();	// usb_device.c. Initializes USB module SFRs and firmware variables to known states.
	
	// Lectura de valores en memoria.
	Hay_App = *(int *)(NVM_PROGRAM_PAGE);	
	/* ORIGINAL */if (Hay_App > 1) Hay_App = NO;
	///* 20171222 */if (Hay_App != SI) Hay_App = NO;
}	
								
/********************************************************************
 * Function:	Config_Timer1()										*
 * Definition:	Configura el timer 1 (tick general).				*
 ********************************************************************/
void Config_Timer1(void)
{
	tick_T1 = 0;			// Inicio contador timer general.
	
	// Timer 1 configurado para que produzca una interrupción cada 50 ms (Scheduler).
	ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_3);				// Enable Timer1 interrupt, interrupt priority 3.
	OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_64, T1_TICK);	// Enable Timer1, internal clk, prescaler 1:64; PR1.
	// Ahora mismo voy a 80MH
	// Un tick del Timer1 = (1/80M)*64 = 1250KHz (0.8us). Su interrupción es cada 62500 => 62500/1250KHz = 50 ms
}

/********************************************************************
 * Function:	Stop_Timer1()										*
 * Definition:	Para el timer 1.									*
 ********************************************************************/
void Stop_Timer1(void)
{
	T1CONbits.TON = 0;		// Paro Timer1.
	DisableIntT1;			// Paro interrupción Timer1.
	mT1ClearIntFlag();		// Clear the interrupt flag.
}

/********************************************************************
 * Function:	Timer1Handler()										*
 * Definition:	ISR del Timer1. Cada 50 ms.							*
 ********************************************************************/
void __ISR(_TIMER_1_VECTOR, ipl3) Timer1Handler(void)
{
    mT1ClearIntFlag();			// Clear the interrupt flag.
	tick_T1++;
	Cont_Blink++;
	
	if (tick_T1 >= 5)			// Blink de 250 ms.
  	{
	  	tick_T1 = 0;
	 	LED_RUN = ~LED_RUN;
	}
}
	
/****************************** EOF *********************************/
