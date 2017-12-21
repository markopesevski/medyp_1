/********************************************************************
 * FileName:		Comunicacion.c									*
 * Dependencies:	Comunicacion.h									*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Controla la comunicación con INTAB.							*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		18/10/16		Versión inicial.				*
 *																	*
 ********************************************************************/
#include "main.h"






extern unsigned char Ant_Estado_Maquina;
extern calibration_process_t calibration_status;
extern unsigned char last_rf_value;













extern unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
extern unsigned char ToSendDataBuffer[64] TX_DATA_BUFFER_ADDRESS;
extern USB_HANDLE USBOutHandle;	// USB handle.  Must be initialized to 0 at startup.
extern USB_HANDLE USBInHandle;	// USB handle.  Must be initialized to 0 at startup.
extern unsigned char Frecuencia;
extern volatile unsigned int tick_Timeout;
extern unsigned int Num_Serie;
extern volatile unsigned char Pitar;
extern unsigned char Retorno_Galva;
extern unsigned char Aplicador;
extern volatile unsigned char Senyal_Stim;
extern unsigned char Temperatura_Alarma;
extern unsigned char Connected;
extern unsigned char Estoy_Test;
extern unsigned char No_Apli;
extern unsigned char Estado_Galva;
extern unsigned char Estado_RF;
extern unsigned char Reducir;

extern volatile struct u_sem
{
	unsigned char Lect_Gal:1;		// Semáforo de lectura de galvánica.
	unsigned char Lect_Man:1;		// Semáforo de lectura de aplicadores.
	unsigned char Flag_Barrido:1;	// Semáforo de barrido RF.
	unsigned char Time_out:1;		// Semáforo timeout coms.
	unsigned char Sube_Baja:1;		// Semáforo para aumentar o disminuir frecuencia en pulsos.
	unsigned char Polaridad:1;		// Senáforo de polaridad galvánica.
	unsigned char Tecla:1;			// Semáforo lectura tecla.
	unsigned char Temper:1;			// Semáforo para leer temperatura.
} Sem;

machine_states_t Estado_Maquina = READY;
unsigned char Valor_Stim = 0;
unsigned char Valor_Galva = 0;
unsigned char Valor_Lift = 0;
rf_values_t Valor_RF = 0;

unsigned char Error_Trama = 0;
unsigned char Alarma = 0;	//todonova?¿

unsigned int Cont_ascii = 0;
unsigned char Cont_Frames = 0;
unsigned char Saved_OK = 0;
unsigned int Suma_Bytes = 0;
unsigned char Error_While = 0;	// TODO controlar
unsigned char Salta = 0;
/**/
unsigned char Cont_Apli = 0;

UINT8 asciiBuffer[100];
UINT pointer = 0;
UINT readBytes;
UINT8 hexRec[100];

typedef struct
{
    UINT8 *start;
    UINT8 len;
    UINT8 status;
}T_REC;
T_REC record;

/********************************************************************
 * Function:	ProcessIO()											*
 * Definition:	Gestión mensajes INTAB.								*
 ********************************************************************/
void ProcessIO (void)
{
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;		// User Application USB tasks.

	if(!HIDRxHandleBusy(USBOutHandle))	// Check if we have received an OUT data packet from the host.
	{
		tick_Timeout = 0;
		Sem.Time_out = 0;

		Error_Trama = Analizar_Trama();

		if (Error_Trama == 0)			// Procesar órdenes.
		{
			Limpia_Buffer(OUT);
			if(!HIDTxHandleBusy(USBInHandle))
			{
				switch(ReceivedDataBuffer[1])
		        {
			        case TRAMA_STATUS:	// 2
						Estado_Maquina = ReceivedDataBuffer[2];
						Valor_Stim  = ReceivedDataBuffer[3];
						Valor_Galva = ReceivedDataBuffer[4];
						Valor_Lift  = ReceivedDataBuffer[5];
						if (ReceivedDataBuffer[6] != Valor_RF)
						{
							last_rf_value = Valor_RF;
							Valor_RF = ReceivedDataBuffer[6];
						}

						Frecuencia  = ReceivedDataBuffer[7];
						if (Estoy_Test == NO_TEST)
						{
							Aplicador 	= ReceivedDataBuffer[8];
						}
						Senyal_Stim = ReceivedDataBuffer[9];

						if ((Estado_RF == O_N)||(Estado_Galva == O_N))
						{
							// TODO SIVA hacer de alguna manera fuera, que intab se entere que no apli
							if (No_Apli == 1)
							{
								Estado_Maquina = READY;
								Cont_Apli = 0;
							}
							else
							{
								Cont_Apli++;
								if (Cont_Apli <= 2)
								{
									Estado_Maquina = READY;
								}
								else
								{
									Cont_Apli = 4;
								}
							}
						}
						#ifdef DEBUGAR
						if (Connected == 0)	// TODO NOVA
						{
							Connected = 1;
							// Pitar = 2;
						}
						#endif
						if ((Reducir == 1)&&(Valor_RF >= RF_value_10))
						{
							Valor_RF = RF_value_5;
						}

						Procesar_Ordenes(TRAMA_STATUS);
			    		Responder(TRAMA_STATUS);
					break;
					case TRAMA_VERSION:	// 3
			    		Responder(TRAMA_VERSION);
					break;
					case TRAMA_INICIA:	// 8
			        	ToSendDataBuffer[0] = 4;			// Bytes_To_Send.
						ToSendDataBuffer[1] = TRAMA_INICIA;	// Orden a responder.
						ToSendDataBuffer[2] = 2;			// Digo que ya estoy en app.
			        break;
					case TRAMA_FIRM:	// 10
						ToSendDataBuffer[0] = 3;			// Bytes_To_Send.
						ToSendDataBuffer[1] = TRAMA_FIRM;	// Orden a responder.
					break;
					case READ_NSERIE:	// 50
						if(Estoy_Test != CALIBRA_RF)
						{
							Responder(READ_NSERIE);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_READ_SERIAL);
						}
					break;
					case REC_NSERIE:	// 51
						if(Estoy_Test != CALIBRA_RF)
						{
							Procesar_Ordenes(REC_NSERIE);
							Responder(REC_NSERIE);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_SAVE_SERIAL);
						}
					break;
					case CONT_BUZZER:	// 5
						Procesar_Ordenes(NIVEL_BUZZER);
						Responder(NIVEL_BUZZER);
					break;
					case NIVEL_BUZZER:	// 6
						Procesar_Ordenes(NIVEL_BUZZER);
						Responder(NIVEL_BUZZER);
					break;
					case READ_TEMP:		// 25
						if(Estoy_Test != CALIBRA_RF)
						{
							Responder(READ_TEMP);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_READ_TEMP);
						}
					break;
					case READ_ALARM_TEMP:// 26
						if(Estoy_Test != CALIBRA_RF)
						{
							Responder(READ_ALARM_TEMP);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_READ_ALARM_TEMP);
						}
					break;
					case SET_ALARM_TEMP:// 27
						if(ReceivedDataBuffer[2] > 0)
						{
							Procesar_Ordenes(SET_ALARM_TEMP);
							ToSendDataBuffer[0] = 3;				// Bytes_To_Send.
							ToSendDataBuffer[1] = SET_ALARM_TEMP;	// Orden a responder.
						}
						else if  (ReceivedDataBuffer[2] == 0)
						{
							if(Estoy_Test != CALIBRA_RF)
							{
								/* CALIBRATION PROCESS */
								Estoy_Test = CALIBRA_RF;
								calibration_status = CALIBRATION_STARTING_RF;
							}
							else
							{
								Estoy_Test = NO_TEST;
								Ant_Estado_Maquina = GO_OUT_OF_TEST; // Para parar los test.
							}
						}
					break;
					case READ_GAL:		// 30
						if(Estoy_Test != CALIBRA_RF)
						{
							Responder(READ_GAL);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_READ_GAL);
						}
					break;
					case SET_GAL:		// 31
						if(Estoy_Test != CALIBRA_RF)
						{
							Procesar_Ordenes(SET_GAL);
							ToSendDataBuffer[0] = 3;			// Bytes_To_Send.
							ToSendDataBuffer[1] = SET_GAL;		// Orden a responder.
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_SET_GAL);
						}
					break;
					case READ_STIM:		// 35
						if(Estoy_Test != CALIBRA_RF)
						{
							Responder(READ_STIM);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_READ_STIM);
						}
					break;
					case SET_STIM:		// 36
						if(Estoy_Test != CALIBRA_RF)
						{
							Procesar_Ordenes(SET_STIM);
							ToSendDataBuffer[0] = 3;			// Bytes_To_Send.
							ToSendDataBuffer[1] = SET_STIM;		// Orden a responder.
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_SET_STIM);
						}
					break;
					case READ_RF:		// 40
						if(Estoy_Test != CALIBRA_RF)
						{
							Responder(READ_RF);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_READ_RF);
						}
					break;
					case SET_RF:		// 41
						if(Estoy_Test != CALIBRA_RF)
						{
							Procesar_Ordenes(SET_RF);
							ToSendDataBuffer[0] = 3;			// Bytes_To_Send.
							ToSendDataBuffer[1] = SET_RF;		// Orden a responder.
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_SET_RF);
						}
					break;
					case READ_BIAS:		// 43
						if(Estoy_Test != CALIBRA_RF)
						{
							Responder(READ_BIAS);
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_READ_BIAS);
						}
					break;
					case SET_BIAS:		// 44
						if(Estoy_Test != CALIBRA_RF)
						{
							Procesar_Ordenes(SET_BIAS);
							ToSendDataBuffer[0] = 3;			// Bytes_To_Send.
							ToSendDataBuffer[1] = SET_BIAS;		// Orden a responder.
						}
						else
						{
							/* respond with real corresponding value */
							calibration_process(CALIBRATION_SET_BIAS);
						}
					break;
					case REC_CAL:		// 48
						Procesar_Ordenes(REC_CAL);
						ToSendDataBuffer[0] = 3;			// Bytes_To_Send.
						ToSendDataBuffer[1] = REC_CAL;		// Orden a responder.
					break;
					case RST_FAB:		// 88
						Procesar_Ordenes(RST_FAB);
						Responder(RST_FAB);
					break;
					default:		;		break;
				}

				Calc_Check();
				USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);	// Prepare the USB module to send the data packet to the host.

				switch(ReceivedDataBuffer[1])
		        {
					case TRAMA_FIRM:
			        	Salta = 1;		// Ojo, en este momento, cuando se salta al bootloader, se corta la conexión con el host.
					break;
					default:				;					break;
				}
			}
		}
		Limpia_Buffer(IN);

        // Re-arm the OUT endpoint, so we can receive the next OUT data packet that the host may try to send us.
        USBOutHandle = HIDRxPacket(HID_EP, (BYTE*)&ReceivedDataBuffer, 64);

        if (Salta == 1)		// Por actualización o inicio normal.
		{
			Salta = 0;
			DelayMs(250);	// Espero a que acabe de enviar la trama.
			USBDeviceDetach();
			USBDisableInterrupts();
			JumpToBoot();
		}
    }
}

/********************************************************************
 * Function:	Calc_Check()										*
 * Definition:	Calcula el checksum.								*
 ********************************************************************/
void Calc_Check (void)
{
	unsigned char Suma_Envio = 0;
	unsigned char Cont_Envio = 0;

	for (Cont_Envio=0;Cont_Envio<(ToSendDataBuffer[0]);Cont_Envio++)	// Cálculo checksum.
	{
		Suma_Envio = Suma_Envio^ToSendDataBuffer[Cont_Envio];
	}
	ToSendDataBuffer[ToSendDataBuffer[0] - 1] = Suma_Envio;
}

/********************************************************************
 * Function:	Limpia_Buffer(Dire)									*
 * Definition:	Limpia buffer comunicación.							*
 ********************************************************************/
void Limpia_Buffer (unsigned char Dire)
{
	unsigned char Cont_Bu = 0;

	if (Dire == IN)
	{
		for (Cont_Bu=0;Cont_Bu<64;Cont_Bu++)
		{
			ReceivedDataBuffer[Cont_Bu] = 0;
		}
	}
	if (Dire == OUT)
	{
		for (Cont_Bu=0;Cont_Bu<64;Cont_Bu++)
		{
			ToSendDataBuffer[Cont_Bu] = 0;
		}
	}
}

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:

	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause
	//things to not work as intended.


    #if defined(__C30__) || defined __XC16__
        //This function requires that the _IPL level be something other than 0.
        //  We can set it here to something other than
        #ifndef DSPIC33E_USB_STARTER_KIT
        _IPL = 1;
        USBSleepOnSuspend();
        #endif
    #endif
}



/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *
 *					This call back is invoked when a wakeup from USB suspend
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// 10+ milliseconds of wakeup time, after which the device must be
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
	// Make sure the selected oscillator settings are consistent with USB
    // operation before returning from this function.
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.

	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This
 *					callback function should initialize the endpoints
 *					for the device's usage according to the current
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior,
 *                  as a USB device that has not been armed to perform remote
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex:
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup.
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;

    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager
    //properties page for the USB device, power management tab, the
    //"Allow this device to bring the computer out of standby." checkbox
    //should be checked).
    if(USBGetRemoteWakeupStatus() == TRUE)
    {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();

            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0;
            USBBusIsSuspended = FALSE;  //So we don't execute this code again,
                                        //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;
            do
            {
                delay_count--;
            }while(delay_count);

            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT
            //      endpoints).
            break;
        default:
            break;
    }
    return TRUE;
}

/********************************************************************
 * Function:	JumpToBoot()										*
 * Definition:	Jumps to bootloader.								*
 ********************************************************************/
void JumpToBoot(void)
{
	void (*fptr)(void);
	fptr = (void (*)(void))BOOT_FLASH_ADDRESS;
	fptr();
}

/****************************** EOF *********************************/
