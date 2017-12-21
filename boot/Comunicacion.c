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

extern unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
extern unsigned char ToSendDataBuffer[64] TX_DATA_BUFFER_ADDRESS;
extern USB_HANDLE USBOutHandle;		// USB handle.  Must be initialized to 0 at startup.
extern USB_HANDLE USBInHandle;		// USB handle.  Must be initialized to 0 at startup.
extern unsigned char Hay_App;

volatile unsigned char rxLenght = 0;		// Longuitud en bytes de la trama a recibir.
unsigned char Error_Trama = 0;

unsigned int Cont_ascii = 0;
unsigned char Cont_Frames = 0;
unsigned char Saved_OK = 0;
unsigned int Suma_Bytes = 0;
unsigned char Error_While = 0;
unsigned char Salta = 0;

const char Version_Soft[] = "vB.1.0";

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
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl == 1)) return;	// User Application USB tasks.
    
	if(!HIDRxHandleBusy(USBOutHandle))	// Check if we have received an OUT data packet from the host.
    {	
		Error_Trama = Analizar_Trama();
        
        if (Error_Trama == 0)			// Procesar órdenes.
        {
	        Limpia_Buffer(OUT);
	        if(!HIDTxHandleBusy(USBInHandle))
			{
		        switch(ReceivedDataBuffer[1])
		        {
					case TRAMA_STATUS:	// 2	
						ToSendDataBuffer[0] = 12;	// Bytes_To_Send.
						ToSendDataBuffer[1] = TRAMA_STATUS;
			    		ToSendDataBuffer[2] = 0;
			    		ToSendDataBuffer[3] = 0;
			    		ToSendDataBuffer[4] = 0;
			    		ToSendDataBuffer[5] = 0;
			    		ToSendDataBuffer[6] = 0;
			    		ToSendDataBuffer[7] = 0;
			    		ToSendDataBuffer[8] = 0;
			    		ToSendDataBuffer[9] = 0;
			    		ToSendDataBuffer[10] = 0;
			    		/*
			    		ToSendDataBuffer[0] = 13;	// Bytes_To_Send.
						ToSendDataBuffer[1] = TRAMA_STATUS;
			    		ToSendDataBuffer[2] = 0;
			    		ToSendDataBuffer[3] = 0;
			    		ToSendDataBuffer[4] = 0;
			    		ToSendDataBuffer[5] = 0;
			    		ToSendDataBuffer[6] = 0;
			    		ToSendDataBuffer[7] = 0;
			    		ToSendDataBuffer[8] = 0;
			    		ToSendDataBuffer[9] = 0;
			    		ToSendDataBuffer[10] = 0;
			    		ToSendDataBuffer[11] = 0;*/
			    	break;
			    	case TRAMA_VERSION:	// 3
			    		ToSendDataBuffer[0] = 9;	// Bytes_To_Send.
						ToSendDataBuffer[1] = TRAMA_VERSION;	// 
						ToSendDataBuffer[2] = Version_Soft[0];	//v
				   		ToSendDataBuffer[3] = Version_Soft[1];	//0
				   		ToSendDataBuffer[4] = Version_Soft[2];	//.
				   		ToSendDataBuffer[5] = Version_Soft[3];	//0
				   		ToSendDataBuffer[6] = Version_Soft[4];	//.
				   		ToSendDataBuffer[7] = Version_Soft[5];	//1
					break;
			        case TRAMA_INICIA:	// 8
			        	ToSendDataBuffer[0] = 4;			// Bytes_To_Send.
						ToSendDataBuffer[1] = TRAMA_INICIA;	// Orden a responder.
						
						if (Hay_App == SI) ToSendDataBuffer[2] = SI;
						else ToSendDataBuffer[2] = NO;
			        break;
			        case TRAMA_FIRM:	// 10
						ToSendDataBuffer[0] = 3;			// Bytes_To_Send.
						ToSendDataBuffer[1] = TRAMA_FIRM;	// Orden a responder.
					break;
			        case ERASE_BUF:	// 12
			        	Stop_Timer1();
						LED_RUN = 1;
						EraseFlash();
						Cont_ascii = 0;
						for(Cont_ascii=0;Cont_ascii<100;Cont_ascii++) asciiBuffer[Cont_ascii] = 0;
						Cont_ascii = 0;
						readBytes = 0;
						Cont_Frames = 0;
						Saved_OK = 0;
						Suma_Bytes = 0;
						Error_While = 0;
						
						ToSendDataBuffer[0] = 3;		// Bytes_To_Send.
						ToSendDataBuffer[1] = ERASE_BUF;
					break;	        
		            case ACTUALIZA:	// 60
						for (Cont_Frames=0;Cont_Frames<(rxLenght-3);Cont_Frames++)
						{
							asciiBuffer[readBytes] = ReceivedDataBuffer[2+Cont_Frames];
							readBytes++;
						}
						asciiBuffer[readBytes] = 0x0D;
						readBytes++;
						asciiBuffer[readBytes] = 0x0A;
						readBytes++;
					
						for(Cont_ascii=0;Cont_ascii<(readBytes + pointer);Cont_ascii++)
						{
							// This state machine seperates-out the valid hex records from the read 512 bytes.
							switch(record.status)
							{
								case REC_FLASHED:
								case REC_NOT_FOUND:
									if(asciiBuffer[Cont_ascii] == ':')
									{
										// We have a record found in the 512 bytes of data in the buffer.
										record.start = &asciiBuffer[Cont_ascii];
										record.len = 0;
										record.status = REC_FOUND_BUT_NOT_FLASHED;
									}
								break;
								case REC_FOUND_BUT_NOT_FLASHED:
									if((asciiBuffer[Cont_ascii] == 0x0A) || (asciiBuffer[Cont_ascii] == 0xFF))
									{
										// We have got a complete record. (0x0A is new line feed and 0xFF is End of file)
										// Start the hex conversion from element
										// 1. This will discard the ':' which is
										// the start of the hex record.
										ConvertAsciiToHex(&record.start[1],hexRec);
										WriteHexRecord2Flash(hexRec);
										record.status = REC_FLASHED;
									}
								break;
							}
			                   // Move to next byte in the buffer.
			                   record.len++;
			   			}
			   			
			   			if(record.status == REC_FOUND_BUT_NOT_FLASHED)
			            {
			                // We still have a half read record in the buffer. The next half part of the record is read 
			                // when we read 512 bytes of data from the next file read. 
			                memcpy(asciiBuffer, record.start, record.len);
			                pointer = record.len;
			                record.status = REC_NOT_FOUND;
						}
						else pointer = 0;
	
						readBytes = 0;

						ToSendDataBuffer[0] = 3;		// Bytes_To_Send.
						ToSendDataBuffer[1] = ACTUALIZA;
		            break;
		            case TRANS_BUF:	// 13            	
		            	ToSendDataBuffer[0] = 4;		// Bytes_To_Send.
						ToSendDataBuffer[1] = TRANS_BUF;
						
						if (Saved_OK == SI) ToSendDataBuffer[2] = SI;
						else ToSendDataBuffer[2] = NO;
					break;
					default:				;					break;
		        }
		        
		        Calc_Check();
		        USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);	// Prepare the USB module to send the data packet to the host.
		        
		        switch(ReceivedDataBuffer[1])
		        {
			        case TRAMA_INICIA:
			        	if (Hay_App == SI) Salta = 1;
					break;
			        case TRAMA_FIRM:
			        	Stop_Timer1();
						LED_RUN = 1;
					break;
					case TRANS_BUF:
						if (Saved_OK == SI) Salta = 1;
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
			JumpToApp();
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
 * Function:	(Error_Trama)Analizar_Trama()						*
 * Definition:	Rutina para analizar trama recibida.				*
 ********************************************************************/
unsigned char Analizar_Trama(void)
{
	unsigned char Cont = 0;
	unsigned char Suma_Recep = 0;

	rxLenght = ReceivedDataBuffer[0];
	for (Cont=0;Cont<(rxLenght-1);Cont++) Suma_Recep = Suma_Recep^ReceivedDataBuffer[Cont];
	
	if (Suma_Recep == ReceivedDataBuffer[rxLenght-1]) return (0);
	else return (1);
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
    
    // Aquí entro si recibo alimentación del host. TODO
//	Nop();
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
	
/****************************** EOF *********************************/
