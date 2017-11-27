/********************************************************************
 * FileName:		Lectura_ADC.c									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Configura y lee los diferentes canales analógicos Mesotreat.	*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/02/17		Versión inicial.				*
 *																	*
 ********************************************************************/
#include "main.h"

extern unsigned char Estado_Maquina;
extern unsigned char Estado_Stim;
extern unsigned char Estado_Galva;
extern unsigned char Estado_Lift;
extern unsigned char Estado_RF;
extern unsigned char Salida_Uno;
extern unsigned char Salida_Dos;
extern unsigned char Salida_Tres;
extern unsigned char Aplicador;
extern unsigned char Estado_Aplicador;
extern volatile unsigned int Segundos;
//extern unsigned char Valor_Galva;
extern unsigned char Valor_RF;
extern volatile unsigned char Pitar;
//extern unsigned char Dac_Gal;
//extern unsigned int Media_Galva ;

extern volatile struct u_sem{
	unsigned char Lect_Gal:1;		// Semáforo de lectura de galvánica.
	unsigned char Lect_Man:1;		// Semáforo de lectura de aplicadores.
	unsigned char Flag_Barrido:1;	// Semáforo de barrido RF.
	unsigned char Time_out:1;		// Semáforo timeout coms.	
	unsigned char Sube_Baja:1;		// Semáforo para aumentar o disminuir frecuencia en pulsos.
	unsigned char Polaridad:1;		// Senáforo de polaridad galvánica.
	unsigned char Tecla:1;			// Semáforo lectura tecla.
	unsigned char Temper:1;			// Semáforo para leer temperatura.
}Sem;

extern union unio_rel{
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

extern union unio_led{
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

float Temperatura = 0;
float Vector_Temp[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned char Cont_Temp = 0;
unsigned char Primera_Temp = 0;
unsigned char Cont_Interm = 0;
unsigned char Alarma_Temp = 0;	// Alarma de temperatura radiador RF. sem!todo
unsigned char Apagada_RF = 0;	//sem
unsigned char Temperatura_Alarma = TEMPERATURA_ALARMA;

unsigned char No_Apli = 0;
unsigned char Sombra_Apli = 0;

unsigned int Valor_Probe2 = 0;
unsigned int Valor_Probe3 = 0;
unsigned int Valor_Probe1 = 0;

/********************************************************************
 * Function:	(ADC1BUF0)Lectura_Galva()							*
 * Definition:	Rutina para leer galvánica.							*
 ********************************************************************/
unsigned int Lectura_Galva(void)	// OK
{
	AD1CHS = (ADC_CH0_POS_SAMPLEA_AN13 | ADC_CH0_NEG_SAMPLEA_NVREF);	
	DelayUs(1);
			
	AD1CON1bits.ADON = 1;			// Turn on ADC.
	DelayUs(1);		
	while(!IFS1bits.AD1IF);			// Esperar conversión realizada.
	DelayUs(1);	
	IFS1bits.AD1IF = 0;

	return (ADC1BUF0);
}

/********************************************************************
 * Function:	(Temp)Lectura_Temp()								*
 * Definition:	Rutina para leer temperatura radiador RF.			*
 ********************************************************************/
float Lectura_Temp(void)	// OK
{
	float aux_temp = 0;
	
	AD1CHS = (ADC_CH0_POS_SAMPLEA_AN12 | ADC_CH0_NEG_SAMPLEA_NVREF);
	DelayUs(1);
			
	AD1CON1bits.ADON = 1;			// Turn on ADC.
	DelayUs(1);		
	while(!IFS1bits.AD1IF);			// Esperar conversión realizada.
	DelayUs(1);	
	IFS1bits.AD1IF = 0;

	aux_temp = ADC1BUF0;
	aux_temp = (aux_temp * 330) / 1023;	// Paso a grados.

	return (aux_temp);
}

/********************************************************************
 * Function:	(ADC1BUF0)Lectura_Stim(Canal_Sti)					*
 * Definition:	Rutina para leer estimulación.						*
 ********************************************************************/
unsigned int Lectura_Stim(unsigned char Canal_Sti)//todo
{
	if (Estado_Stim == O_N)
	{
//		TEST1 = 1;	// Voy ha hacer test
		
		AD1CHS = (ADC_CH0_POS_SAMPLEA_AN10 | ADC_CH0_NEG_SAMPLEA_NVREF);	
		DelayUs(1);
			
		AD1CON1bits.ADON = 1;			// Turn on ADC.
		DelayUs(1);		
		while(!IFS1bits.AD1IF);			// Esperar conversión realizada.
		DelayUs(1);	
		IFS1bits.AD1IF = 0;

		return (ADC1BUF0);
	}
	
	// TSTCH1
	//TSTCH2 TODO
}

/********************************************************************
 * Function:	(ADC1BUF0)Lectura_Galva()							*
 * Definition:	Rutina para leer galvánica.							*
 ********************************************************************/
unsigned int Lectura_RF(void)
{	
	AD1CHS = (ADC_CH0_POS_SAMPLEA_AN8 | ADC_CH0_NEG_SAMPLEA_NVREF);	
	DelayUs(1);
	
	AD1CON1bits.ADON = 1;			// Turn on ADC.
	DelayUs(1);	
		
	while(!IFS1bits.AD1IF);			// Esperar conversión realizada.
	DelayUs(1);	
	IFS1bits.AD1IF = 0;

	return (ADC1BUF0);
}

/********************************************************************
 * Function:	(ADC1BUF0)Lectura_Probe(Canal_Prob)					*
 * Definition:	Rutina para leer resistencias aplicadores.			*
 ********************************************************************/
unsigned int Lectura_Probe (unsigned char Canal_Prob)	// OK
{
	// Aquí se mira qué aplicador en qué salida:
	//	- Nada -> 986 pasos.
	//	- Aplicador corporal (270 ohms) -> 530 pasos.
	//	- Aplicador facial   (120 ohms) -> 448 pasos.
	//	- Aplicador específico (22 ohm)	-> 388 pasos.
	
	switch (Canal_Prob)
	{
		case PROBE1: AD1CHS = (ADC_CH0_POS_SAMPLEA_AN5 | ADC_CH0_NEG_SAMPLEA_NVREF);	break;
		case PROBE2: AD1CHS = (ADC_CH0_POS_SAMPLEA_AN4 | ADC_CH0_NEG_SAMPLEA_NVREF);	break;
		case PROBE3: AD1CHS = (ADC_CH0_POS_SAMPLEA_AN9 | ADC_CH0_NEG_SAMPLEA_NVREF);	break;
		default:										;								break;
	}		
	DelayUs(1);			
	AD1CON1bits.ADON = 1;		// Turn on ADC.
	DelayUs(1);		
	while(!IFS1bits.AD1IF);		// Esperar conversión realizada.
	DelayUs(1);	
	IFS1bits.AD1IF = 0;

	return (ADC1BUF0);
}

/********************************************************************
 * Function:	Mirar_Aplicador()									*
 * Definition:	Rutina para Mirar aplicadores.						*
 ********************************************************************/
void Mirar_Aplicador (void)
{	
	Led.Bit.LedP1 = 1;
	Led.Bit.LedP2 = 1;
	Led.Bit.LedP3 = 1;
	Control_TPIC();
	DelayUs(30/*60*/);//30	todo POner a 25 con condesnadores de 270pf
	
	Valor_Probe1 = Lectura_Probe(PROBE1);	
	if (Valor_Probe1 < 420) Salida_Uno = ESPE;//388
	else if ((Valor_Probe1 < 483)&&(Valor_Probe1 > 425)) Salida_Uno = FACIAL;//448
		else if ((Valor_Probe1 < 565)&&(Valor_Probe1 > 495)) Salida_Uno = CORPORAL;//530
			else if (Valor_Probe1 > 750) Salida_Uno = 0;//986 nc

	Valor_Probe2 = Lectura_Probe(PROBE2);	
	if (Valor_Probe2 < 420) Salida_Dos = ESPE;
	else if ((Valor_Probe2 < 483)&&(Valor_Probe2 > 425)) Salida_Dos = FACIAL;
		else if ((Valor_Probe2 < 565)&&(Valor_Probe2 > 495)) Salida_Dos = CORPORAL;
			else if (Valor_Probe2 > 750) Salida_Dos = 0;

	Valor_Probe3 = Lectura_Probe(PROBE3);	
	if (Valor_Probe3 < 420) Salida_Tres = ESPE;
	else if ((Valor_Probe3 < 483)&&(Valor_Probe3 > 425)) Salida_Tres = FACIAL;
		else if ((Valor_Probe3 < 565)&&(Valor_Probe3 > 495)) Salida_Tres = CORPORAL;
			else if (Valor_Probe3 > 750) Salida_Tres = 0;
			
	Led.Bit.LedP1 = 0;
	Led.Bit.LedP2 = 0;
	Led.Bit.LedP3 = 0;
	Control_TPIC();			
}

/********************************************************************
 * Function:	Activar_Salida()									*
 * Definition:	Rutina para activar salida.							*
 ********************************************************************/
void Activar_Salida (void)
{	
	if (Estado_RF == OFF)
	{		
		Rele.Bit.Rel1 = 0;
		Rele.Bit.Rel2 = 0;
		Rele.Bit.Rel3 = 0;	
		Led.Bit.LedP1 = 0;	
		Led.Bit.LedP2 = 0;
		Led.Bit.LedP3 = 0;
		No_Apli = 0;
	
		Estado_Aplicador = 0;
		if ((Salida_Uno != 0)&&(Salida_Uno == Aplicador))	// Activar salida 1.
		{	
			Rele.Bit.Rel1 = 1;
			Led.Bit.LedP1 = 1;	
			Estado_Aplicador = Aplicador;
			if (Sombra_Apli == 1) Sombra_Apli = 2;
		}
	
		if ((Salida_Dos != 0)&&(Salida_Dos == Aplicador))	// Activar salida 2.
		{
			Rele.Bit.Rel2 = 1;
			Led.Bit.LedP2 = 1;		
			Estado_Aplicador = Aplicador;
			if (Sombra_Apli == 1) Sombra_Apli = 2;
		}
		
		if ((Salida_Tres != 0)&&(Salida_Tres == Aplicador))	// Activar salida 3.
		{
			Rele.Bit.Rel3 = 1;
			Led.Bit.LedP3 = 1;
			Estado_Aplicador = Aplicador;
			if (Sombra_Apli == 1) Sombra_Apli = 2;
		}
	}
	else
	{
		if (Rele.Bit.Rel1 == 1)
		{
			Led.Bit.LedP1 = 1;
			if (Salida_Uno == 0)
			{
				Salida_Dos = 0;
				Salida_Tres = 0;
				Sombra_Apli = 1;
			}	
		}
		if (Rele.Bit.Rel2 == 1)
		{
			Led.Bit.LedP2 = 1;
			if (Salida_Dos == 0)
			{
				Salida_Uno = 0;
				Salida_Tres = 0;
				Sombra_Apli = 1;
			}	
		}
		if (Rele.Bit.Rel3 == 1)
		{
			Led.Bit.LedP3 = 1;
			if (Salida_Tres == 0)
			{
				Salida_Uno = 0;
				Salida_Dos = 0;
				Sombra_Apli = 1;
			}	
		}
	}		
	
	if (Estado_Maquina != 1)
	{
		if (Led.Bit.LedP1 == 1)	// Intermitencias si está aplicador seleccionado y tratamiento parado.
		{
			Cont_Interm++;
			if (Cont_Interm >= 5) Led.Bit.LedP1 = 0;
			if (Cont_Interm >= 10) Cont_Interm = 0;
		}
		if (Led.Bit.LedP2 == 1)
		{
			Cont_Interm++;
			if (Cont_Interm >= 5) Led.Bit.LedP2 = 0;
			if (Cont_Interm >= 10) Cont_Interm = 0;
		}
		if (Led.Bit.LedP3 == 1)
		{
			Cont_Interm++;
			if (Cont_Interm >= 5) Led.Bit.LedP3 = 0;
			if (Cont_Interm >= 10) Cont_Interm = 0;
		}
	}

	if ((Salida_Uno == 0)&&(Salida_Dos == 0)&&(Salida_Tres == 0)) No_Apli = 1;	// TODO el aplicador que toca mirar!!!!
	else No_Apli = 0;
}

/********************************************************************
 * Function:	Control_Temperatura()								*
 * Definition:	Rutina para leer temperatura y encender ventilador.	*
 ********************************************************************/
void Control_Temperatura (void)
{
	Vector_Temp[Cont_Temp] = Lectura_Temp();
	if (Primera_Temp == 0)
	{
		Primera_Temp = 1;
		Temperatura = Vector_Temp[Cont_Temp];
		for (Cont_Temp=0;Cont_Temp<10;Cont_Temp++) Vector_Temp[Cont_Temp] = Temperatura;
		Cont_Temp = 0;				
	}	
	Temperatura = (Vector_Temp[0] + Vector_Temp[1] + Vector_Temp[2] + Vector_Temp[3] + Vector_Temp[4] + Vector_Temp[5] + Vector_Temp[6] + Vector_Temp[7] + Vector_Temp[8] + Vector_Temp[9]) / 10;
	
	if (Vector_Temp[Cont_Temp] >= Temperatura_Alarma)
	{
		Alarma_Temp = 1;		
		if (Apagada_RF == 0)
		{
			Apagada_RF = 1;
			Apaga_RF();		// Si paso temperatura de alarma, parar tratamiento RF.
		}	

		// Hacer control volumen a ki a tope?!?!? todo
	}
	else
	{
		if (Temperatura < (Temperatura_Alarma - 10)) Alarma_Temp = 0;
	}
	
	if (Alarma_Temp == 1) Pitar = 1;
	
	if (Cont_Temp < 9) Cont_Temp++;
	else Cont_Temp = 0;	
	
	
	if (Temperatura >= TEMPERATURA_HS)
	{
		HS_FAN = 1;				// Enciendo ventilador radiador.
		Segundos = 0;
	}
	else
	{
		if (Segundos >= SEGS_HS)// Apago pasado un minuto seguido sin llegar a la temperatura.
		{
			if (Estado_RF == OFF) HS_FAN = 0;	// Apago ventilador radiador y no está la RF en marcha.
			Segundos = SEGS_HS;	
		}
	}
}

/****************************** EOF *********************************/