/********************************************************************
 * FileName:		AD9834.c										*
 * Dependencies:	AD9834.h										*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Configura y controla el AD9834 (DDS).							*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		07/12/16		Versión inicial.				*
 *																	*
 ********************************************************************/
#include "main.h"

unsigned char Sem_Reg = 0;		// Semaforo para alternar registro frecuencia.

/********************************************************************
 * Function:	Config_AD9834()										*
 * Definition:	Configura comunicación serie para AD9834 (DDS).		*
 ********************************************************************/
void Config_AD9834 (void)
{
	unsigned int BR_REG = 0;
	
	// The SPI baudrate BR is given by: BR = Fpb/(2*(SPIBRG+1))
	// SPIBRG = ((80M/500K)/2)-1) = 79
	// SPIBRG = ((80M/1M)/2)-1) = 39
	
	FSYNC_AD9834 = 1;	// Inicial.
	SCLK_AD9834  = 1;	// Inicial. 
	
	BR_REG = ((GetPeripheralClock()/AD9834_BAUDRATE) / 2) - 1;
	SpiChnOpen(SPI_CHANNEL3, SPI_OPEN_MSTEN | SPI_OPEN_MODE16, BR_REG);	// Configuro SPI3.
}

/********************************************************************
 * Function:	Set_Freq_AD9834(Frecuencya)							*
 * Definition:	Rutina para cargar el AD9834 (DDS).					*
 ********************************************************************/
void Set_Freq_AD9834 (unsigned long Frecuencya)
{
	unsigned long Freq_Reg = 0;
	unsigned int Freq_MSB = 0;
	unsigned int Freq_LSB = 0;

	Freq_Reg = (Frecuencya * pow(2,28)) / 75000000;	// Los 75MHz vienen dados por el xtal montado.
	
	if (Sem_Reg == 0)
	{
		Freq_MSB = (Freq_Reg >> 14) | FREQ0;
		Freq_LSB = (Freq_Reg & 0x3FFF) | FREQ0;
		
		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, 0x2100);			// Senoidal y reset.
		FSYNC_AD9834 = 1;
	
		Nop();
		
		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, Freq_LSB);			// Cargas parte baja frecuencia.
		SpiChnPutC(SPI_CHANNEL3, Freq_MSB);			// Cargas parte alta frecuencia.
		FSYNC_AD9834 = 1;
	
		Nop();

		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, 0xE000);			// Phase angle 0.
		FSYNC_AD9834 = 1;

		Nop();
	
		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, 0x2000);			// Senoidal y out of reset.
		while (SpiChnIsBusy(SPI_CHANNEL3));
		FSYNC_AD9834 = 1;
		Sem_Reg = 1;
	}
	else
	{
		Freq_MSB = (Freq_Reg >> 14) | FREQ1;
		Freq_LSB = (Freq_Reg & 0x3FFF) | FREQ1;
		
		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, 0x2D00);			// Senoidal y reset.
		FSYNC_AD9834 = 1;
	
		Nop();

		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, Freq_LSB);			// Cargas parte baja frecuencia.
		SpiChnPutC(SPI_CHANNEL3, Freq_MSB);			// Cargas parte alta frecuencia.
		FSYNC_AD9834 = 1;

		Nop();
	
		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, 0xC000);			// Phase angle 0.
		FSYNC_AD9834 = 1;
		
		Nop();

		FSYNC_AD9834 = 0;
		SpiChnPutC(SPI_CHANNEL3, 0x2C00);			// Senoidal y out of reset.
		while (SpiChnIsBusy(SPI_CHANNEL3));
		FSYNC_AD9834 = 1;		
		Sem_Reg = 0;
	}
}

/********************************************************************
 * Function:	Suspend_AD9834()									*
 * Definition:	Stop internal clk; DAC remains at its current value.*
 ********************************************************************/
void Suspend_AD9834 (void)
{
	FSYNC_AD9834 = 0;
	SpiChnPutC(SPI_CHANNEL3, 0x2180);	// Sinusoidal, reset y SLEEP1.
	while (SpiChnIsBusy(SPI_CHANNEL3));
	FSYNC_AD9834 = 1;
}	

/****************************** EOF *********************************/
