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
	- Lectura analogica de stim
	- Autoregular lectura analogica RF
	- pwm boton led?

	PREGUNTAS
	- beeps
	- que hacer timeout?
	- Alarmas!?
	- QUe hacer si no aplicador=!=
	- Ir a stnadby si pasan x minutos???

	Genis:
	- Al instalar APK, abrir no listo!!!
	- Origenes desconocidos (al intentar sintalar apk) y va menu andorid no!!!!
	- quitar escritorio
	- cuando no hay aplic, no dejar empezar!!
	- Numero de serie siempre aparezcan 5 cifras.
*/
/*
	Timer 1 --> Tick general.
	Timer 2 --> Estimulación.
	Timer 3 --> Galvánica.
	Timer 4 -->	Barrido.
	Timer 5 -->	Electrolifting.
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

/********************* Declaración de variables *********************/
/* Variables generales */
volatile unsigned int tick_Led = 0;			// Tick led run.
volatile unsigned int tick_Timeout = 0;		// Tick timeout coms.
volatile unsigned int tickCambio = 0;		// Tick para cambio de polaridad de galvánica.
volatile unsigned int tick_T4 = 0;			// Tick para barrido.
volatile unsigned int tick_Galva = 0;		// Tick para lectura galvánica.
volatile unsigned int tick_Manac = 0;		// Tick para lectura aplicadores.
volatile unsigned int tick_Tecla = 0;		// Tick para lectura tecla.

volatile unsigned int tick_pushbutton_calibra = 0; // Tick para intermitencias pushbutton cuando estoy en calibración.
volatile unsigned int tick_calibra = 0; // Tick para medidas y pasos cuando estoy en calibración.
volatile unsigned int tick_warmup = 0; // Tick para esperar warmup de la máquina cuando estoy en calibración.
// #define DEBUG_MARKO
#ifdef DEBUG_MARKO
	volatile unsigned int tick_level = 0; // Debug ticks, generally not used
	volatile unsigned int tick_mitjana = 0; // Debug ticks, generally not used
#endif // DEBUG_MARKO

volatile unsigned int  Contador_Stim = 0;		// Contador para generar los pulsos en interrupción.
volatile unsigned int  Contador_Tim = 0;		// Contador para generar los pulsos en interrupción.
volatile unsigned char Freq_Stim = 76;			// Frecuencia de barrido de pulsos.
volatile unsigned char Freq_Min = 0;			// Frecuencia mínima de barrido.
volatile unsigned char Mitad_Freq_Stim = 38;	// Para disparar a mitad.
volatile unsigned char Sem_Stim = 0;
volatile unsigned char Contador_Lift = 0;		// Contador para generar los pulsos en interrupción.
volatile unsigned char Modular = 0;
volatile unsigned char Bajando = 0;
volatile unsigned char Cont_Mod = 0;
volatile unsigned int Segundos = 0;
volatile unsigned char Target_Galva = 0;
volatile float Pasos_Galva = 0;
volatile unsigned char Aux_Galva = 0;

unsigned char Hay_App = SI;
unsigned int Num_Serie = 0;
int int_stats = 0;

unsigned char Frecuencia = 30;
unsigned int Frecuencia_Barrido = 100;
unsigned int Frecuencia_Resulta = 1000000;
unsigned char Subiendo = 0;
unsigned int Aux_Mem = 0;
unsigned char Canal_Stim = 0;

unsigned char Estado_Galva = OFF;
unsigned int Vector_Galva[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned int Media_Galva = 0;
unsigned char Cont_Galva = 0;
unsigned char Retorno_Galva = 0;

unsigned char Aplicador = 0;
unsigned char Salida_Uno = 0;
unsigned char Salida_Dos = 0;
unsigned char Salida_Tres = 0;
unsigned char Estado_Aplicador = 0;

unsigned int Vector_Stim[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned int Media_Stim = 0;
unsigned char Cont_Stim = 0;
unsigned char Volumen = VOL_FAB;
unsigned char Standby = 1;	// sem
volatile unsigned char Pitar = 0;

machine_states_t Mi_Estado_Maquina = STANDBY;	// Empiezo en standby
unsigned char Blink_PB = 0;//3;
unsigned int T2_TICK = 400;
volatile unsigned char Senyal_Stim = 0;
unsigned int Aux_Contador_Stim = 0;
/**/
unsigned int Lect_RF = 0;
volatile unsigned char Sem_Lect_RF = 0; //sem
unsigned char Offset_RF = 170;
unsigned int Val_RF = 0;

unsigned char Dac_Gal = DAC_GAL_FAB;
unsigned char Dac_Stim = DAC_STIM_FAB;
unsigned char Dac_RF = DAC_RF_FAB;
unsigned char Dac_Bias = DAC_BIAS_FAB;

unsigned char Connected = 0;		// sem
unsigned char Reducir = 0;			//sem
unsigned char Cont_Reducir = 0;









signed int dacdds_drift_correction = 0;
signed int level_drift_correction = 0;
unsigned char drift_dacdds_adjust_over = 0;
unsigned char drift_level_adjust_over = 0;
float voltage_anrf = 0.0f;
extern rf_frequencies_t freq_value;
extern unsigned char handle_value;
extern unsigned char dacdds_value;
extern unsigned char level_value;
extern unsigned char refdacdds_value;
extern unsigned char index_percentage_value;
extern calibration_process_t calibration_status;
extern unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
extern unsigned char array_dacdds[RF_arrays_max][RF_max_values];
extern unsigned char array_level[RF_arrays_max][RF_max_values];
extern unsigned char array_dacdds_fab[RF_arrays_max][RF_max_values];
extern unsigned char array_level_fab[RF_arrays_max][RF_max_values];
extern unsigned char last_rf_value;
static calibration_arrays_saved_t arrays_saved = ARRAYS_NOT_SAVED;

#ifdef DEBUG_MARKO
	static float tensio_fora[256] = {0.0f};
	static float accumulator = 0.0f;
	static unsigned int mostres[256] = {0};
	static unsigned int comptador_level = 0;
	static unsigned char stop = 1;
	static unsigned char freq = 1;
	static unsigned char barrido = 0;
	static unsigned char pujar = 0;
	static unsigned char baixar = 0;
	static unsigned char bias_value = 82;
	static float temperatura = 0.0f;
#endif // DEBUG_MARKO










unsigned char Vector_Seno[256] = {0,3,6,9,13,16,19,22,25,28,
31,35,38,41,44,47,50,53,56,59,
62,65,69,72,75,78,81,84,87,90,
92,95,98,101,104,107,110,113,115,118,
121,124,127,129,132,135,137,140,143,145,
148,150,153,155,158,160,163,165,168,170,
172,175,177,179,181,184,186,188,190,192,
194,196,198,200,202,204,206,208,210,211,
213,215,217,218,220,221,223,224,226,227,
229,230,231,233,234,235,236,238,239,240,
241,242,243,244,245,246,246,247,248,249,
249,250,251,251,252,252,253,253,253,254,
254,254,255,255,255,255,255,255,255,255,255,255,255,255,254,254,
254,253,253,253,252,252,251,251,250,249,
249,248,247,246,246,245,244,243,242,241,
240,239,238,236,235,234,233,231,230,229,
227,226,224,223,221,220,218,217,215,213,
211,210,208,206,204,202,200,198,196,194,
192,190,188,186,184,181,179,177,175,172,
170,168,165,163,160,158,155,153,150,148,
145,143,140,137,135,132,129,127,124,121,
118,115,113,110,107,104,101,98,95,92,
90,87,84,81,78,75,72,69,65,62,
59,56,53,50,47,44,41,38,35,31,
28,25,22,19,16,13,9,6,3,0};

// Usb
USB_HANDLE USBOutHandle = 0;    //USB handle.  Must be initialized to 0 at startup.
USB_HANDLE USBInHandle = 0;     //USB handle.  Must be initialized to 0 at startup.

extern machine_states_t Estado_Maquina;
extern unsigned char Estado_RF;
extern unsigned char Estado_Stim;
extern unsigned char Estado_Lift;
extern unsigned char Valor_Stim;
extern unsigned char Valor_Galva;
extern unsigned char Valor_Lift;
extern unsigned char Valor_RF;
extern unsigned char No_Apli;
extern unsigned char Temperatura_Alarma;
extern unsigned char Estoy_Test;
extern unsigned char Cont_Test;
extern unsigned char Ant_Estado_Maquina;

/* Estructuras, enumeraciones y uniones */
volatile struct u_sem{
	unsigned char Lect_Gal:1;		// Semáforo de lectura de galvánica.
	unsigned char Lect_Man:1;		// Semáforo de lectura de aplicadores.
	unsigned char Flag_Barrido:1;	// Semáforo de barrido RF.
	unsigned char Time_out:1;		// Semáforo timeout coms.
	unsigned char Sube_Baja:1;		// Semáforo para aumentar o disminuir frecuencia en pulsos.
	unsigned char Primera_Galva:1;	// Semáforo de galvánica.
	unsigned char Tecla:1;			// Semáforo lectura tecla.
	unsigned char Temper:1;			// Semáforo para leer temperatura.
}Sem;

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

/*********************************************************************
 *********************************************************************
 ** Function:		MAIN()											**
 ** Description:	PROGRAMA PRINCIPAL.								**
 *********************************************************************
 *********************************************************************/
#ifdef DEBUG_MARKO
	int main(void)
	{
		Config_System();
		Config_Ports();
		Init_Regs();

		Config_Timer1();	// Timer general.
		Config_Timer4();
		HS_FAN = 1;				// Enciendo ventilador radiador.
		Rele.Bit.Rel1 = 1;
		Rele.Bit.Rel2 = 1;
		Rele.Bit.Rel3 = 1;
		Control_TPIC();

		Carga_TLC5620(REFDACBIAS | 200, 3); /* SAFE VALUE <- CRUCIAL */
		Carga_TLC5620(DACBIAS | bias_value, 3); /* original 82 SAFE VALUE <- CRUCIAL */
		Carga_TLC5620(REFDACDDS | 127, 3); /* SAFE VALUE */
		Carga_TLC5620(DACDDS | 127, 3); /* SAFE VALUE */
		Carga_TLC5620(LEVEL | 255, 1); /* SAFE VALUE <- CRUCIAL */

		while(1)
		{
			/* 500 ms */
			if(stop == 2)
			{
				/* 500 ms */
				if(tick_level >= 500)
				{
					tick_level = 0;
					if(pujar == 1)
					{
						comptador_level++;
						if(comptador_level >= 60)
						{
							comptador_level = 0;
						}
					}
					else if(baixar == 1)
					{
						comptador_level--;
						if(comptador_level == 0)
						{
							comptador_level = 60;
						}
					}
					tensio_fora[comptador_level] = (accumulator/500.0f);
					tensio_fora[comptador_level] = ((0.1237 * tensio_fora[comptador_level]) + 13.133); //0,1237x + 13,133
					accumulator = (0.0f);
					Carga_TLC5620(LEVEL | (0xFF - comptador_level), 1);
					Carga_TLC5620(DACBIAS | bias_value, 3);
				}

				/* 1 ms -> 500 samples */
				if(tick_Led >= 1)
				{
					mostres[0] = Lectura_RF();
					accumulator += Lectura_RF();
					temperatura = Lectura_Temp();
					if(temperatura >= 90)
					{
						Led.Bit.LedPB = ~Led.Bit.LedPB;
						Control_TPIC();
					}
					Nop();
					tick_Led = 0;
				}

				if(barrido == 1)
				{
					if (Sem.Flag_Barrido == 1)
					{
						Sem.Flag_Barrido = 0;
						if (Subiendo == 0)
						{
							Frecuencia_Barrido = Frecuencia_Barrido + 5;
							if (Frecuencia_Barrido >= 300)
							{
								Subiendo = 1;
							}
						}
						else
						{
							Frecuencia_Barrido = Frecuencia_Barrido - 5;
							if (Frecuencia_Barrido <= 100)
							{
								Subiendo = 0;
							}
						}
						Frecuencia_Resulta = Frecuencia_Barrido*10000;
						Set_Freq_AD9834(Frecuencia_Resulta);
					}
				}
			}

			if(stop == 1)
			{
				Suspend_AD9834();				// Pone a 0 el nivel del DDS.
				if(barrido == 1)
				{
					barrido = 0;
					Stop_Timer4();
				}
				stop = 3;
			}
			else if (stop == 0)
			{
				if(barrido == 1)
				{
					Config_Timer4();
				}
				else
				{
					Set_Freq_AD9834(freq*1000000);	// Pongo frecuencia.
				}
				stop = 2;
			}
			else if (stop == 2 || stop == 3)
			{
				Nop();
			}
		}
	}
#endif // DEBUG_MARKO

#ifndef DEBUG_MARKO
int main(void)
{
	Config_System();
	Config_Ports();
	Init_Regs();

	Config_Timer1();	// Timer general.

	USBDeviceAttach();

	while(1)
	{
		ProcessIO();	// Atención al USB.

		if (Estado_Galva == O_N)	// OK
		{
			/* every 100ms */
			if (Sem.Lect_Gal == 1)
			{
				Sem.Lect_Gal = 0;

				if (((tickCambio > 10)&&(tickCambio < 190))||((tickCambio > 212)&&(tickCambio < 390)))
				{
					Vector_Galva[Cont_Galva] = Lectura_Galva();
					if (Sem.Primera_Galva == 0)
					{
						Sem.Primera_Galva = 1;
						DelayMs(50);
						Media_Galva = Lectura_Galva();
						for (Cont_Galva=0;Cont_Galva<10;Cont_Galva++) Vector_Galva[Cont_Galva] = Media_Galva;
						Cont_Galva = 0;
					}
					Media_Galva = (Vector_Galva[0] + Vector_Galva[1] + Vector_Galva[2] + Vector_Galva[3] + Vector_Galva[4] + Vector_Galva[5] + Vector_Galva[6] + Vector_Galva[7] + Vector_Galva[8] + Vector_Galva[9]) / 10;

					if (Media_Galva <= 50)
					{
						Retorno_Galva = 0;
					}
					else if ((Media_Galva > 50)&&(Media_Galva <= 135))
					{
						Retorno_Galva = 1;	//90
					}
					else if ((Media_Galva > 135)&&(Media_Galva <= 225))
					{
						Retorno_Galva = 2;	//180
					}
					else if ((Media_Galva > 225)&&(Media_Galva <= 315))
					{
						Retorno_Galva = 3;	//270
					}
					else if ((Media_Galva > 315)&&(Media_Galva <= 405))
					{
						Retorno_Galva = 4;	//360
					}
					else if ((Media_Galva > 405)&&(Media_Galva <= 495))
					{
						Retorno_Galva = 5;	//450
					}
					else if ((Media_Galva > 495)&&(Media_Galva <= 585))
					{
						Retorno_Galva = 6;	//540
					}
					else if ((Media_Galva > 585)&&(Media_Galva <= 675))
					{
						Retorno_Galva = 7;	//630
					}
					else if ((Media_Galva > 675)&&(Media_Galva <= 765))
					{
						Retorno_Galva = 8;	//720
					}
					else if ((Media_Galva > 765)&&(Media_Galva <= 855))
					{
						Retorno_Galva = 9;	//810
					}
					else
					{
						Retorno_Galva = 10;	//900
					}

					if (Cont_Galva < 9)
					{
						Cont_Galva++;
					}
					else
					{
						Cont_Galva = 0;
					}
				}
			}
		}

		if(Estoy_Test == CALIBRA_RF)
		{
			/* since it is timed with the TIM1, and 50ms period, every 50 ms: */
			if(tick_pushbutton_calibra >= 10)
			{
				tick_pushbutton_calibra = 0;
				Led.Bit.LedPB = ~Led.Bit.LedPB;
				Control_TPIC();
			}

			/* since it is timed with the TIM1, and 50ms period, every 250 ms: */
			if(tick_calibra > 0)
			{
				/* every 250 ms */
				if(tick_calibra > CALIBRATION_ACTION_TICKS)
				{
					tick_warmup++;
					tick_calibra = 0;
					calibration_process(calibration_status);
				}
				calibration_process(CALIBRATION_RF_READ_VOLTAGE);
			}
		}

		/* every 200ms */
		if (Sem.Lect_Man == 1)	// Cada 200 ms miro aplicadores.
		{
			if (Blink_PB == 0)
			{
				Blink_PB = 1;
				Led.Bit.LedPB = 0;
			}
			else if (Blink_PB == 1)
			{
				Blink_PB = 0;
				Led.Bit.LedPB = 1;
			}

			if (Blink_PB == 2)
			{
				Blink_PB = 0;
				Led.Bit.LedPB = 1;
			}
			else if (Blink_PB == 3)
			{
				if (Standby == 1)
				{
					Led.Bit.LedPB = 1;
				}
				else
				{
					Led.Bit.LedPB = 0;
				}
			}
			Sem.Lect_Man = 0;
			Mirar_Aplicador();
			Activar_Salida();
			Control_TPIC();

			if (Estado_RF == O_N)
			{
				if (Estado_Galva == OFF)
				{
					Carga_TLC5620(REFDAC | Dac_Gal,1);		// 150
					Carga_TLC5620(DACGAL | 20,1);			// Tiro a nivel 1
			//		Carga_TLC5620(DACGAL | 10,1);			// Tiro a nivel 1
					ON_GN = ON_POL;							// Positivo ON.
					DelayMs(5);	// Se podría rebajar?? TODO
					Media_Galva = Lectura_Galva();
					Carga_TLC5620(REFDAC,1);// Pongo a cero nivel salida.
					Carga_TLC5620(DACGAL,1);// Pongo a cero nivel salida.
					ON_GN = OFF_POL;		// Desconecto polaridad.

					if (Media_Galva > 20)	// Tengo continuidad, sólo paro por temperatura.	//8.7.16
					{
						Reducir = 0;
						Cont_Reducir = 0;
						/**/
						LED_RUN = 0;
						/**/
					}
					else
					{
						if (Media_Galva <= 20)//8.7.16
						{
							Cont_Reducir++;
							if (Cont_Reducir >= TIEMPO_REDUCIR)
							{
								Reducir = 1; 		// No tengo continuidad.
								Cont_Reducir = TIEMPO_REDUCIR;
								//#ifdef DEBUGAR
									//Pitar = 1;//todo nova
									/**/
									LED_RUN = 1;
									/**/
								//#endif
							}
						}
					}
				}
				else
				{
					if (Media_Galva > 20)
					{
						Reducir = 0;
						Cont_Reducir = 0;
						/**/
						LED_RUN = 0;
						/**/
					}
					else if (Media_Galva <= 20)
						{
							Cont_Reducir++;
							if (Cont_Reducir >= TIEMPO_REDUCIR)
							{
								Reducir = 1; 		// No tengo continuidad.
								Cont_Reducir = TIEMPO_REDUCIR;
								//#ifdef DEBUGAR
								//Pitar = 1;//todo nova
								/**/
								LED_RUN = 1;
								/**/
								//#endif
							}
						}
				}
			}
			Control_Temperatura();
		}

		if (Estado_RF == O_N)
		{
			if (Sem.Flag_Barrido == 1)
			{
				Sem.Flag_Barrido = 0;

				if (Subiendo == 0)
				{
					Frecuencia_Barrido = Frecuencia_Barrido + 5;
					if (Frecuencia_Barrido >= 300)
					{
						Subiendo = 1;
					}
				}
				else
				{
					Frecuencia_Barrido = Frecuencia_Barrido - 5;
					if (Frecuencia_Barrido <= 100)
					{
						Subiendo = 0;
					}
				}
				Frecuencia_Resulta = Frecuencia_Barrido*10000;
				Set_Freq_AD9834(Frecuencia_Resulta);
			}

			// Sem_Lect_RF = 0; /* ERASE WHEN DONE */
			if (Sem_Lect_RF == 1)
			{
				Sem_Lect_RF = 0;
				if(Estoy_Test != CALIBRA_RF)
				{
					if(Frecuencia == 10)
					{
						voltage_moving_average(read_voltage_anrf_1mhz(Lectura_RF()), &voltage_anrf);
					}
					else if (Frecuencia == 30)
					{
						voltage_moving_average(read_voltage_anrf_3mhz(Lectura_RF()), &voltage_anrf);
					}

					if (last_rf_value != Valor_RF)
					{
						last_rf_value = Valor_RF;
						dacdds_drift_correction = 0;
						level_drift_correction = 0;
						drift_dacdds_adjust_over = 0;
						drift_level_adjust_over = 0;
					}

					/* TODO: FOR SAFETY REASONS DO MORE AGGRESSIVE CONTROL IF MUCH OVER DESIRED VALUE. */
					if((calibration_values_t) is_voltage_correct(voltage_anrf, Valor_RF, Aplicador, Frecuencia) == CALIBRATION_VALUE_OVER)
					{
						if((dacdds_drift_correction >= (-DACDDS_DRIFT_CORRECTION_MAX)) && (dacdds_drift_correction < DACDDS_DRIFT_CORRECTION_MAX))
						{
							dacdds_drift_correction++;
						}
						else
						{
							dacdds_drift_correction = DACDDS_DRIFT_CORRECTION_MAX;
							if((level_drift_correction >= (-LEVEL_DRIFT_CORRECTION_MAX)) && (level_drift_correction < LEVEL_DRIFT_CORRECTION_MAX))
							{
								level_drift_correction++;
								dacdds_drift_correction = 0;
							}
							else
							{
								level_drift_correction = LEVEL_DRIFT_CORRECTION_MAX;
							}
						}

						if(Frecuencia == 10)
						{
							if(Aplicador == FACIAL)
							{
								dacdds_value = array_dacdds[RF_arrays_1mhz_facial][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_1mhz_facial][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == CORPORAL)
							{
								dacdds_value = array_dacdds[RF_arrays_1mhz_corporal][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_1mhz_corporal][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == ESPE)
							{
								dacdds_value = array_dacdds[RF_arrays_1mhz_especific][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_1mhz_especific][Valor_RF] + level_drift_correction;
							}
						}
						else if (Frecuencia == 30)
						{
							if(Aplicador == FACIAL)
							{
								dacdds_value = array_dacdds[RF_arrays_3mhz_facial][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_3mhz_facial][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == CORPORAL)
							{
								dacdds_value = array_dacdds[RF_arrays_3mhz_corporal][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_3mhz_corporal][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == ESPE)
							{
								dacdds_value = array_dacdds[RF_arrays_3mhz_especific][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_3mhz_especific][Valor_RF] + level_drift_correction;
							}
						}
						else if (Frecuencia == 0xBA)
						{
							if(Aplicador == FACIAL)
							{
								dacdds_value = array_dacdds[RF_arrays_ba_facial][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_ba_facial][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == CORPORAL)
							{
								dacdds_value = array_dacdds[RF_arrays_ba_corporal][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_ba_corporal][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == ESPE)
							{
								dacdds_value = array_dacdds[RF_arrays_ba_especific][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_ba_especific][Valor_RF] + level_drift_correction;
							}
						}

						Carga_TLC5620(REFDACDDS | REFDACDDS_VALUE, 3);
						Carga_TLC5620(DACDDS | dacdds_value, 3);
						Carga_TLC5620(LEVEL | level_value, 1);
					}
					else if((calibration_values_t) is_voltage_correct(voltage_anrf, Valor_RF, Aplicador, Frecuencia) == CALIBRATION_VALUE_UNDER)
					{
						if((dacdds_drift_correction > (-DACDDS_DRIFT_CORRECTION_MAX)) && (dacdds_drift_correction <= DACDDS_DRIFT_CORRECTION_MAX))
						{
							dacdds_drift_correction--;
						}
						else
						{
							dacdds_drift_correction = -DACDDS_DRIFT_CORRECTION_MAX;
							if((level_drift_correction > (-LEVEL_DRIFT_CORRECTION_MAX)) && (level_drift_correction <= LEVEL_DRIFT_CORRECTION_MAX))
							{
								level_drift_correction--;
								dacdds_drift_correction = 0;
							}
							else
							{
								level_drift_correction = -LEVEL_DRIFT_CORRECTION_MAX;
							}
						}

						if(Frecuencia == 10)
						{
							if(Aplicador == FACIAL)
							{
								dacdds_value = array_dacdds[RF_arrays_1mhz_facial][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_1mhz_facial][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == CORPORAL)
							{
								dacdds_value = array_dacdds[RF_arrays_1mhz_corporal][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_1mhz_corporal][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == ESPE)
							{
								dacdds_value = array_dacdds[RF_arrays_1mhz_especific][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_1mhz_especific][Valor_RF] + level_drift_correction;
							}
						}
						else if (Frecuencia == 30)
						{
							if(Aplicador == FACIAL)
							{
								dacdds_value = array_dacdds[RF_arrays_3mhz_facial][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_3mhz_facial][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == CORPORAL)
							{
								dacdds_value = array_dacdds[RF_arrays_3mhz_corporal][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_3mhz_corporal][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == ESPE)
							{
								dacdds_value = array_dacdds[RF_arrays_3mhz_especific][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_3mhz_especific][Valor_RF] + level_drift_correction;
							}
						}
						else if (Frecuencia == 0xBA)
						{
							if(Aplicador == FACIAL)
							{
								dacdds_value = array_dacdds[RF_arrays_ba_facial][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_ba_facial][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == CORPORAL)
							{
								dacdds_value = array_dacdds[RF_arrays_ba_corporal][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_ba_corporal][Valor_RF] + level_drift_correction;
							}
							else if (Aplicador == ESPE)
							{
								dacdds_value = array_dacdds[RF_arrays_ba_especific][Valor_RF] + dacdds_drift_correction;
								level_value = array_level[RF_arrays_ba_especific][Valor_RF] + level_drift_correction;
							}
						}

						Carga_TLC5620(REFDACDDS | REFDACDDS_VALUE, 3);
						Carga_TLC5620(DACDDS | dacdds_value, 3);
						Carga_TLC5620(LEVEL | level_value, 1);
					}
				}
			}
			if (No_Apli == 1)
			{
				Apaga_RF();
			}
		}

		if (Estado_Stim == O_N)
		{
			switch (Senyal_Stim)
			{
				case 4:
					if (Aux_Contador_Stim != Contador_Stim)
					{
						Aux_Contador_Stim = Contador_Stim;
						Modular++;
						if (Contador_Tim >= 21500)
						{
							Cont_Mod = 0;
						}

						if (Modular >= 17)
						{
							Modular = 0;
							Cont_Mod++;
							Carga_TLC5620(REFDACA | (Vector_Seno[Cont_Mod])/2,2);	// Canal A
							Carga_TLC5620(REFDACB | (Vector_Seno[Cont_Mod])/2,2);	// Canal B
							if (Cont_Mod >= 255)
							{
								Cont_Mod = 0;
							}
						}
					}
				break;
				case 5:
					if (Aux_Contador_Stim != Contador_Stim)
					{
						Aux_Contador_Stim = Contador_Stim;
						Modular++;
						if (Modular >= 50)
						{
							Modular = 0;
							Cont_Mod++;
							Carga_TLC5620(REFDACA | (Vector_Seno[Cont_Mod])/2,2);	// Canal A
							Carga_TLC5620(REFDACB | (Vector_Seno[Cont_Mod])/2,2);	// Canal B
							if (Cont_Mod >= 255)
							{
								Cont_Mod = 0;
							}
						}
					}
				break;
				default:
				break;
			}
		}

		/* every 1000ms */
		if (Sem.Temper == 1)	// Miro temperatura cada 1 segundo.	// TODO se ha movido a cada 200ms se podria quitar?
		{
			if (Estoy_Test != NO_TEST && Estoy_Test != CALIBRA_RF)
			{
				Cont_Test++;
				if (Cont_Test >= 10)
				{
					 Estoy_Test = NO_TEST;
					 Cont_Test = 0;
					 Aplicador = 0;
					 Ant_Estado_Maquina = GO_OUT_OF_TEST; // Para parar los test.
				}
			}

			Sem.Temper = 0;
			// Control_Temperatura();
		}

		/* every 100ms */
		if (Sem.Tecla == 1)		// Cada 100 ms.
		{
			Sem.Tecla = 0;

			if (Connected == 0)
			{
				// Blink_PB = 0;
				// tick_Tecla = 0;
			}
			else
			{
				if (ON_OFF == 0)	// Botón apretado.
				{
					if ((Mi_Estado_Maquina == READY)&&(Estado_Maquina != ACTIVE)&&(Standby == 0))	// Voy a standby si no estoy en tratamiento.
					{
						Led.Bit.LedPB = 1;
						Mi_Estado_Maquina = STANDBY;
						Blink_PB = 2;
					}
					if ((Mi_Estado_Maquina == STANDBY)&&(Standby == 1))	// Despierto de standby.
					{
						Led.Bit.LedPB = 0;
						Mi_Estado_Maquina = READY;
						Blink_PB = 2;
					}
					tick_Tecla = 0;
				}
				else
				{
					if (tick_Tecla >= 200)	// 1 segundo.
					{
						if (Mi_Estado_Maquina == STANDBY)
						{
							Standby = 1;
						}
						if (Mi_Estado_Maquina == READY)
						{
							Standby = 0;
						}
					}
					if (tick_Tecla >= 40)
					{
						Blink_PB = 3;
					}
				}
			}
		}

		/* every 4100ms */
		if (Sem.Time_out == 1)	// Si hay timeout en tratamiento, paro todas las salidas. // TODO FALTA ALARMA ¿¿??
		{
			if (Estado_Maquina == ACTIVE)
			{
				Sem.Time_out = 0;
				tick_Timeout = 0;

				Apaga_Galva();		// De momento apago tratamientos. TODO alarmas.
				Apaga_Stim();
				Apaga_Lift();
				Apaga_RF();
				Aplicador = 0;
				Estado_Maquina = READY;
			}
			else
			{
				#ifdef DEBUGAR
				if (Connected == 1)
				{
					Connected = 0;
					// Pitar = 1;
				}
				#endif
			}
		}
	}//End while
}//End Main
#endif //#ifndef DEBUG_MARKO

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
	tris_usb_bus_sense = INPUT_PIN;		// See HardwareProfile.h
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
	tris_self_power = INPUT_PIN;		// See HardwareProfile.h
#endif

}

/********************************************************************
 * Function:	Config_Ports()										*
 * Description:	Configura los puertos I/O.							*
 ********************************************************************/
void Config_Ports (void)	// 1->input, 0->output
{
	// Drive low & drive high por seguridad de inicio.
	RST_USB = 1;
///	RST_USB = 0;		// Hub deshabilitado.
//	DelayMs(20);

	ON_GN = OFF_POL;
	ON_GP = OFF_POL;
	PULSE_N = 0;		// No hay pulso.
	PULSE_P = 0;		// No hay pulso.
//	TEST1 = 0;			// Desactivo lectura de pulso
//	TEST2 = 0;			// Desactivo lectura de pulso
	EXT_FAN = 0;		// Apago ventilador externo.
	HS_FAN = 0;			// Apago ventilador radiador.

	BEEP = 0;
	LDAC = 1;

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
	RST_USB = 1;		// Habilito HUB.

	ON_GN = OFF_POL;
	ON_GP = OFF_POL;
	PULSE_N = 0;		// No hay pulso.
	PULSE_P = 0;		// No hay pulso.
//	TEST1 = 0;			// Desactivo lectura de pulso
//	TEST2 = 0;			// Desactivo lectura de pulso

	BEEP = 0;
	LDAC = 1;

	Carga_TLC5620(REFDAC,1);
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

	Led.Byte = 0;
	Rele.Byte = 0;

	Led.Bit.ON5V = 1;			// De momento enciendo alimentaciones.
	Led.Bit.LedPB  = 1;			// Enciendo led botón.
	Led.Bit.RampOff = 1;		// Para que no haya rampa.

	Rele.Bit.OnCh1 = OFF;		// Desactivo salida 1.
	Rele.Bit.OnCh2 = OFF;		// Desactivo salida 2.
	Rele.Bit.OnCh3 = OFF;		// Desactivo salida 3.
	Rele.Bit.TstCh1 = OFF;		// Deshabilito test
	Rele.Bit.TstCh2 = OFF;		// Deshabilito test

	Control_TPIC();

	Config_AD9834();			// Configura DDS.
	Suspend_AD9834();			// Pone a 0 el nivel.

	EXT_FAN = 1;				// Enciendo ventilador externo.
//	HS_FAN = 1;					// Enciendo ventilador radiador RF de un inicio.

	// Configuro entradas analógicas.
	AD1PCFGbits.PCFG13 = 0;			// AN_GAL.
	AD1PCFGbits.PCFG5  = 0;			// SP1.
	AD1PCFGbits.PCFG4  = 0;			// SP2.
	AD1PCFGbits.PCFG9  = 0;			// SP3.
	AD1PCFGbits.PCFG10 = 0;			// SENS1.
	AD1PCFGbits.PCFG11 = 0;			// SENS2.
	AD1PCFGbits.PCFG12 = 0;			// AN_TEMP.
	AD1PCFGbits.PCFG8 = 0;			// AN_RF.
	/**/
	AD1PCFGbits.PCFG1 = 0;			// T5V.
	AD1PCFGbits.PCFG2 = 0;			// T5VTAB.
	AD1PCFGbits.PCFG14 = 0;			// T24V.
	AD1PCFGbits.PCFG15 = 0;			// T24RF.
	/**/

	AD1CON1 = 0x00E0;				// Integer 16-bit, auto-convert -> ends sampling and start converting.
	AD1CON1bits.ASAM = 1;			// Auto start sampling.

	AD1CSSL = 0;					// No scanned inputs.
	AD1CON2 = 0;		 			// AVdd-AVss, int every conversion, MUXA only.
	AD1CON3 = 0x0202;				// 2 Tad auto-sample, Tad = 13*Tcy.//ok70
}

/********************************************************************
 * Function:	Init_Regs()											*
 * Description:	Inicializa los registros.							*
 ********************************************************************/
void Init_Regs (void)
{
	unsigned int size_dacdds = sizeof(array_dacdds);
	unsigned int size_level = sizeof(array_level);
	unsigned int value_to_read_dacdds = 0;
	unsigned int value_to_read_level = 0;
	unsigned int counter = 0;
	unsigned int i = 0;
	unsigned int j = 0;

	USBOutHandle = 0;	// Initialize the variable holding the handle for the last transmission.
	USBInHandle = 0;

	USBDeviceInit();	// Initializes USB module SFRs and firmware variables to known states.

	Sem.Lect_Gal = 0;
	Sem.Lect_Man = 0;
	Sem.Flag_Barrido = 0;
	Sem.Time_out = 0;
	Sem.Sube_Baja = 0;
	Sem.Primera_Galva = 0;
	Sem.Tecla = 0;
	Sem.Temper = 0;

	// Lectura de valores en memoria.
	Num_Serie = *(int *)(NVM_PROGRAM_PAGE + 4);
	if (Num_Serie > 0xFFFFFF)
	{
		Num_Serie = 0;
	}

	Volumen = *(int *)(NVM_PROGRAM_PAGE + 8);
	if (Volumen > 10)
	{
		Volumen = VOL_FAB;
	}
	Control_Volumen(Volumen);

	Dac_Gal = *(int *)(NVM_PROGRAM_PAGE + 12);
	if (Dac_Gal >= 255)
	{
		Dac_Gal = DAC_GAL_FAB;
	}

	Dac_Stim = *(int *)(NVM_PROGRAM_PAGE + 16);
	if (Dac_Stim >= 255)
	{
		Dac_Stim = DAC_STIM_FAB;
	}

	Dac_RF = *(int *)(NVM_PROGRAM_PAGE + 20);
	if (Dac_RF >= 255)
	{
		Dac_RF = DAC_RF_FAB;
	}

	Dac_Bias = *(int *)(NVM_PROGRAM_PAGE + 24);
	if (Dac_Bias >= 255)
	{
		Dac_Bias = DAC_BIAS_FAB;
	}

	Temperatura_Alarma = *(int *)(NVM_PROGRAM_PAGE + 28);
	if (Temperatura_Alarma >= 255)
	{
		Temperatura_Alarma = TEMPERATURA_ALARMA;
	}

	arrays_saved = *(int *)(NVM_PROGRAM_PAGE + 32);
	int_stats = INTDisableInterrupts();
	counter = 0;
	if(arrays_saved == ARRAYS_SAVED)
	{
		for(i = 0; i < 9; i++)
		{
			for(j = 0; j < 21; j++)
			{
				if(j == 0 || j == 4 || j == 8 || j == 12 || j == 16 || j == 20)
				{
					value_to_read_dacdds = *(unsigned int *) (NVM_PROGRAM_PAGE + 0x40 + counter);
					value_to_read_level = *(unsigned int *) (NVM_PROGRAM_PAGE + 0x40 + (80 * 4) + counter);
					counter += 4;
					if(j == 20)
					{
						value_to_read_dacdds <<= 24;
						value_to_read_level <<= 24;
					}
				}

				array_dacdds[i][j] = (value_to_read_dacdds & 0xFF000000) >> 24;
				value_to_read_dacdds <<= 8;
				array_level[i][j] = (value_to_read_level & 0xFF000000) >> 24;
				value_to_read_level <<= 8;
			}
		}
	}
	else
	{
		memcpy(array_level, array_level_fab, size_level);
		memcpy(array_dacdds, array_dacdds_fab, size_dacdds);
	}
	DelayMs(100);
	INTRestoreInterrupts(int_stats);

	Hay_App = *(int *)(NVM_PROGRAM_PAGE);
	if (Hay_App != SI && arrays_saved != ARRAYS_SAVED)
	{
		Hay_App = SI;
		Grabar_Flash();
	}
}

/********************************************************************
 * Function:	Grabar_Flash()										*
 * Definition:	Graba valores a memorizar.							*
 ********************************************************************/
void Grabar_Flash(void)
{
	unsigned int value_to_write_dacdds = 0;
	unsigned int value_to_write_level = 0;
	unsigned int counter = 0;
	unsigned int i = 0;
	unsigned int j = 0;

	int_stats = INTDisableInterrupts();
	NVMErasePage((void *)NVM_PROGRAM_PAGE);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE), Hay_App);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 4), Num_Serie);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 8), Volumen);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 12), Dac_Gal);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 16), Dac_Stim);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 20), Dac_RF);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 24), Dac_Bias);
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 28), Temperatura_Alarma);
	counter = 0;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 21; j++)
		{
			value_to_write_dacdds |= array_dacdds[i][j];
			value_to_write_level |= array_level[i][j];
			if(j == 4-1 || j == 8-1 || j == 12-1 || j == 16-1 || j == 20-1 || j == 21-1)
			{
				NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x40 + counter), value_to_write_dacdds);
				NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x40 + (80 * 4) + counter), value_to_write_level);
				counter += 4;
			}
			value_to_write_dacdds <<= 8;
			value_to_write_level <<= 8;

		}
	}
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 32), ARRAYS_SAVED);

//	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 108), Suma_Bytes);

	DelayMs(100);
	INTRestoreInterrupts(int_stats);
}

/********************************************************************
 * Function:	Config_Timer1()										*
 * Definition:	Configura el timer 1 (tick general).				*
 ********************************************************************/
void Config_Timer1(void)
{
	tick_Led = 0;			// Inicio contador led run.
	tick_Timeout = 0;		// Inicio contador timeout coms.
	tick_Manac = 0;			// Inicio contador lectura aplicadores.
	tick_Tecla = 0;			// Inicio contador lectura tecla.
	#ifdef DEBUG_MARKO
		tick_level = 0;
		tick_mitjana = 0;
	#endif // DEBUG_MARKO
	tick_pushbutton_calibra = 0;
	tick_calibra = 0;
	Sem_Lect_RF = 0;

	// Timer 1 configurado para que produzca una interrupción cada 50 ms (Scheduler).
	ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_4);				// Enable Timer1 interrupt, interrupt priority 4.
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
 * Function:	Config_Timer2()										*
 * Definition:	Configura el timer 2 para estimulación.				*
 ********************************************************************/
void Config_Timer2(void)
{
	Contador_Stim = 0;		// Reseteo contador.
	Aux_Contador_Stim = 0;
	Contador_Tim = 0;
	Modular = 0;
	Cont_Mod = 0;

	switch (Senyal_Stim)
	{
		case 1:	T2_TICK = 400;	break;	// 150 us
		case 2:	T2_TICK = 1025;	break;	// 400 us
		case 3:	T2_TICK = 1275;	break;	// 500 us
		case 4:	T2_TICK = 1150;	break;	// 450 us
		case 5:	T2_TICK = 1025;	break;	// 400 us
		case 6:	T2_TICK = 1025;	break;	// 400 us
		default:T2_TICK = 400;	break;
	}

	// Timer 2 configurado para que produzca una interrupción cada 150 us (stim).
	ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_1);				// Enable Timer2 interrupt, interrupt priority 1.
	OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_32, T2_TICK);	// Enable Timer2, internal clk, prescaler 1:32; PR2.
	// Ahora mismo voy a 80MH
	// Un tick del Timer2 = (1/80M)*32 = 2,5MHz (0.4us). Su interrupción es cada 375 => 375/2,5MHz = 150 us
}

/********************************************************************
 * Function:	Config_Timer3()										*
 * Definition:	Configura el timer 3 para galvánica.				*
 ********************************************************************/
void Config_Timer3(void)
{
	tickCambio = 0;

	// Timer 3 configurado para que produzca una interrupción cada 10 ms.
	ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_2);				// Enable Timer3 interrupt, interrupt priority 2.
	OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, T3_TICK);	// Enable Timer3, internal clk, prescaler 1:256; PR3.
	// Ahora mismo voy a 80MHz
	// un tick del Timer1 = (1/80M)*256 = 312K5Hz (3.2us). Su interrupción es cada 3125 => 3125/312K5Hz = 10 ms
}

/********************************************************************
 * Function:	Stop_Timer3()										*
 * Definition:	Para el timer 3.									*
 ********************************************************************/
void Stop_Timer3(void)
{
	T3CONbits.TON = 0;		// Paro Timer3.
	DisableIntT3;			// Paro interrupción Timer3.
	mT3ClearIntFlag();		// Clear the interrupt flag.
	tickCambio = 0;
}

/********************************************************************
 * Function:	Config_Timer4()										*
 * Definition:	Configura el timer 4.								*
 ********************************************************************/
void Config_Timer4(void)
{
	tick_T4 = 0;
	Subiendo = 0;
	Sem.Flag_Barrido = 0;
	Frecuencia_Barrido = 100;	// Siempre empiezo por 1 MHz.

	// Timer 4 configurado para que produzca una interrupción cada 50 ms.
	ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_3);				// Enable Timer4 interrupt, interrupt priority 3.
	OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_64, T4_TICK);	// Enable Timer4, internal clk, prescaler 1:64; PR4.
	// Ahora mismo voy a 80MHz
	// un tick del Timer4 = (1/80M)*64 = 1250KHz (0.8us). Su interrupción es cada 62500 => 62500/1250KHz = 50 ms
}

/********************************************************************
 * Function:	Stop_Timer4()										*
 * Definition:	Para el timer 4.									*
 ********************************************************************/
void Stop_Timer4(void)
{
	T4CONbits.TON = 0;		// Paro Timer4.
	DisableIntT4;			// Paro interrupción Timer4.
	mT4ClearIntFlag();		// Clear the interrupt flag.
	Sem.Flag_Barrido = 0;
	tick_T4 = 0;
	Subiendo = 0;
}

/********************************************************************
 * Function:	Config_Timer5()										*
 * Definition:	Configura el timer 5 para estimulación.				*
 ********************************************************************/
void Config_Timer5(void)
{
	Contador_Lift = 0;		// Reseteo contador.
	Modular = 0;
	Bajando = 0;
	Cont_Mod = 0;

	// Timer 5 configurado para que produzca una interrupción cada 25 us (electrolifting).
	ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_5);				// Enable Timer5 interrupt, interrupt priority 5.
	OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_16, T5_TICK);	// Enable Timer5, internal clk, prescaler 1:32; PR5.
	// Ahora mismo voy a 80MH
	// Un tick del Timer5 = (1/80M)*16 = 5MHz (0.2us). Su interrupción es cada 125 => 125/5MHz = 25 us
}

/********************************************************************
 * Function:	Timer1Handler()										*
 * Definition:	ISR del Timer1. Cada 5 ms.							*
 ********************************************************************/
void __ISR(_TIMER_1_VECTOR, ipl4) Timer1Handler(void)
{
	mT1ClearIntFlag();			// Clear the interrupt flag.

	#ifdef DEBUG_MARKO
		tick_level++;
		tick_mitjana++;
	#endif // DEBUG_MARKO

	if (Estoy_Test == CALIBRA_RF)
	{
		tick_pushbutton_calibra++;
		tick_calibra++;
	}

	/* WIP */
	Sem_Lect_RF = 1; /* NEW */
	/* WIP */

	tick_Led++;
	if (tick_Led >= 200)			// Blink de 1 seg. (se puede poner fuera con semáforo).
	{
		tick_Led = 0;
		LED_RUN = ~LED_RUN; 	//todo VAAAA
		Sem.Temper = 1;
		/* WIP */
		// Sem_Lect_RF = 1; /* ORIGINAL */
		/* WIP */
		Segundos++;
	}

	tick_Manac++;
	if (tick_Manac >= 40)		// Cada 200 ms.
	{
		tick_Manac = 0;
		Sem.Lect_Man = 1;
	}

	tick_Timeout++;
	if (tick_Timeout >= 820)		// Si más de 4 seg sin comunicación -> timeout.//5? y mirar pitar?!?! TODO
	{
		 tick_Timeout = 0;
		 Sem.Time_out = 1;
	}

	tick_Tecla++;
	Sem.Tecla = 1;				// Miro cada 50 ms.

	/**/
	switch (Pitar)
	{
		case 1:
		case 2:
		case 3:
			BEEP = ~BEEP;
			Pitar++;
		break;
		case 4:
			BEEP = 0;
			Pitar = 0;
		break;
		default:	;	break;
	}
	/**/
}

/********************************************************************
 * Function:	Timer2Handler()										*
 * Definition:	ISR del Timer2. Cada 150 us.						*
 ********************************************************************/
void __ISR(_TIMER_2_VECTOR, ipl1) Timer2Handler(void)		// Cada 150 us para estimulación.
{
	mT2ClearIntFlag();		// Clear the interrupt flag.

	Contador_Stim++;		// Cuento pulsos.
	Contador_Tim++;

	switch (Senyal_Stim)
	{
		case 1:
			if (Contador_Stim == 1)
			{
				if (Contador_Tim < 40000) PULSE_P = 1;	// Pulso positivo ON.	__
			}											//						  |
			if (Contador_Stim == 2) PULSE_P = 0;		// Pulso positivo OFF.	  |__

			if (Sem_Stim == 1)
			{											//						__
				PULSE_N = 0;							// Pulso negativo OFF.	  |__
				Sem_Stim = 2;
			}

			if (Sem_Stim != 2)
			{
				if (Contador_Stim == Mitad_Freq_Stim)
				{
					if (Contador_Tim < 40000) PULSE_N = 1;	// Pulso negativo ON.
					Sem_Stim = 1;
				}
			}
			else Sem_Stim = 0;

			if (Contador_Stim >= Freq_Stim) Contador_Stim = 0;	// 250 -> 25 hz ; 75 -> 82.5 hz.

			Freq_Min++;
			if (Freq_Min >= 134)
			{
				if (Sem.Sube_Baja == 0)
				{
					Freq_Stim++;
					if (Freq_Stim >= 250) Sem.Sube_Baja = 1;
				}
				else
				{
					Freq_Stim--;
					if (Freq_Stim <= 75) Sem.Sube_Baja = 0;
				}
				Freq_Min = 0;
				Mitad_Freq_Stim = Freq_Stim / 2;
			}

			if (Contador_Tim >= 55500) Contador_Tim = 0;
		break;
		case 2:
			switch (Contador_Stim)
			{
				case 1:		PULSE_P = 1;	break;			// Pulso positivo ON.	__
				case 2:		PULSE_P = 0;	break;			// Pulso positivo OFF.	  |__
				case 351:	PULSE_N = 1;	break;			// Pulso negativo ON.	__
				case 352:	PULSE_N = 0;	break;			// Pulso negativo OFF.	  |__
				default:				;	break;
			}
			if (Contador_Stim >= 700) Contador_Stim = 0;	// 3.5Hz
		break;
		case 3:
			if (Contador_Stim == 1) PULSE_P = 1;			// Pulso positivo ON.	__
			if (Contador_Stim == 2) PULSE_P = 0;			// Pulso positivo OFF.	  |__
			if (Contador_Stim >= 20) Contador_Stim = 0;		// 100Hz
		break;
		case 4:
			switch (Contador_Stim)
			{
				case 1:		if (Contador_Tim < 21500) PULSE_P = 1;	break;	// Pulso positivo ON.	__
				case 2:		PULSE_P = 0;							break;	// Pulso positivo OFF.	  |__
				case 54:	if (Contador_Tim < 21500) PULSE_N = 1;	break;	// Pulso negativo ON.	__
				case 55:	PULSE_N = 0;							break;	// Pulso negativo OFF.	  |__
				default:				;							break;
			}
			if (Contador_Stim >= 110) Contador_Stim = 0;	// 20Hz
			if (Contador_Tim >= 28000) Contador_Tim = 0;
		break;
		case 5:
			switch (Contador_Stim)
			{
				case 1:		PULSE_P = 1;	break;			// Pulso positivo ON.	__
				case 2:		PULSE_P = 0;	break;			// Pulso positivo OFF.	  |__
				case 351:	PULSE_N = 1;	break;			// Pulso negativo ON.	__
				case 352:	PULSE_N = 0;	break;			// Pulso negativo OFF.	  |__
				default:				;	break;
			}
			if (Contador_Stim >= 700) Contador_Stim = 0;	// 3.5Hz
		break;
		case 6:
			switch (Contador_Stim)
			{
				case 1:		PULSE_P = 1;	break;			// Pulso positivo ON.	__
				case 2:		PULSE_P = 0;	break;			// Pulso positivo OFF.	  |__
				case 137:	PULSE_N = 1;	break;			// Pulso negativo ON.	__
				case 138:	PULSE_N = 0;	break;			// Pulso negativo OFF.	  |__
				default:				;	break;
			}
			if (Contador_Stim >= 275) Contador_Stim = 0;	// 9Hz
		break;
		default:		;		break;
	}
}

/********************************************************************
 * Function:	Timer3Handler()										*
 * Definition:	ISR del Timer3. Cada 10 ms.							*
 ********************************************************************/
void __ISR(_TIMER_3_VECTOR, ipl2) Timer3Handler(void)
{
	mT3ClearIntFlag();		// Clear the interrupt flag.

	tickCambio++;

	if (tickCambio >= 200)
	{
		tickCambio = 0;
//		ON_GP = ON_POL;		// Positivo ON.			// Como haciamos 1seg a on 1seg a off, aqui lo poniamos a on
//		Aux_Galva = 0;
//		Carga_TLC5620(DACGAL | 10,1);
	}

	if (Aux_Galva < Target_Galva)
	{
		Aux_Galva = Aux_Galva + Pasos_Galva;
		Carga_TLC5620(DACGAL | Aux_Galva,1);
	}
//	else
//	{
//		if (tickCambio == 100)
//		{
//			ON_GP = OFF_POL;		// Positivo OFF.	// Aqui se ponia a off.
//			Carga_TLC5620(DACGAL | 10,1);
//			Aux_Galva = 0;
//		}
//	}

	tick_Galva++;
	if (tick_Galva >= 10)		// Lectura cada 100ms
	{
		tick_Galva = 0;
		Sem.Lect_Gal = 1;
	}
}

/********************************************************************
 * Function:	Timer4Handler()										*
 * Definition:	ISR del Timer4. Cada 50 ms.							*
 ********************************************************************/
void __ISR(_TIMER_4_VECTOR, ipl3) Timer4Handler(void)
{
	mT4ClearIntFlag();		// Clear the interrupt flag.

	tick_T4++;
	if (tick_T4 >= 3)		// 150ms
	{
		tick_T4 = 0;
		Sem.Flag_Barrido = 1;
	}
}

/********************************************************************
 * Function:	Timer5Handler()										*
 * Definition:	ISR del Timer5. Cada 25 us.							*
 ********************************************************************/
void __ISR(_TIMER_5_VECTOR, ipl5) Timer5Handler(void)		// Cada 25 us para electrolifting.
{
	mT5ClearIntFlag();		// Clear the interrupt flag.

	Contador_Lift++;		// Cuento pulsos.
	switch (Contador_Lift)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			PULSE_P = 1;		// Pulso positivo ON.	___
		break;					//						   |
		case 7:					//						   |
		case 8:					//						   |
		case 9:					//						   |
		case 10:				//						   |
			PULSE_P = 0;		// Pulso positivo OFF.	   |___
		break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			PULSE_N = 1;		// Pulso negativo ON.	___
		break;					//						   |
		case 17:				//						   |
		case 18:				//						   |
		case 19:				// Pulso negativo OFF.	   |___
			PULSE_N = 0;
		break;
		case 20:
		default:
			Contador_Lift = 0;
		break;
	}

	Bajando++;
	if (Bajando >= 2)
	{
		Bajando = 0;
		Cont_Mod++;
		Carga_TLC5620(REFDACB | (Vector_Seno[Cont_Mod])/2,2);// TODO verificar corregir para poner 80 hz portadora (Hacer fuera????)
		if (Cont_Mod >= 255)
		{
			Cont_Mod = 0;
			if (Contador_Lift > 0) Contador_Lift--;
		}
	}

/*	if (Cont_Mod >= 5)			// Para hacer la portadora de 80 Hz
	{
		Cont_Mod = 0;
		Carga_TLC5620(REFDACB | (Modular*2),2);

		if (Bajando == 0)
		{
			Modular++;
			if (Modular > 50)
			{
				Bajando = 1;
				Modular--;
			}
		}
		else
		{
			if (Modular > 0) Modular--;
			else
			{
				Bajando = 0;
				Modular++;
			}
		}
	}
	*/
}

//	Exception handler:
  static enum {
  	EXCEP_IRQ = 0,			// interrupt
  	EXCEP_AdEL = 4,			// address error exception (load or ifetch)
  	EXCEP_AdES,				// address error exception (store)
  	EXCEP_IBE,				// bus error (ifetch)
  	EXCEP_DBE,				// bus error (load/store)
  	EXCEP_Sys,				// syscall
  	EXCEP_Bp,				// breakpoint
  	EXCEP_RI,				// reserved instruction
  	EXCEP_CpU,				// coprocessor unusable
  	EXCEP_Overflow,			// arithmetic overflow
  	EXCEP_Trap,				// trap (possible divide by zero)
  	EXCEP_IS1 = 16,			// implementation specfic 1
  	EXCEP_CEU,				// CorExtend Unuseable
  	EXCEP_C2E				// coprocessor 2
  } _excep_code;

  static unsigned int _epc_code;
  static unsigned int _excep_addr;

  // this function overrides the normal _weak_ generic handler
  void _general_exception_handler(void)
  {
  	asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
  	asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));

  	_excep_code = (_excep_code & 0x0000007C) >> 2;
  	 	while (1) {
  		// Examine _excep_code to identify the type of exception
  		// Examine _excep_addr to find the address that caused the exception
  		Nop();
  		Nop();
  		Nop();
   	}
  }//	End of exception handler

/****************************** EOF *********************************/
