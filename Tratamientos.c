/********************************************************************
 * FileName:		Tratamientos.c									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Configura los diferentes tratamientos del Mesotreat.			*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/09/16		Versión inicial.				*
 *																	*
 ********************************************************************/
#include "main.h"
/* matrixes will be: [9][21]
*  [9] because there are 9 different arrays:
*		[0] 1 MHz facial
*		[1] 1 MHz corporal
*		[2] 1 MHz especific
*		[3] 3 MHz facial
*		[4] 3 MHz corporal
*		[5] 3 MHz especific
*		[6] barrido facial
*		[7] barrido corporal
*		[8] barrido especific
*  and then there are [21] for each because there are 21 steps [0, 20] (from 0%, 5%, 10%... to 100%) */
unsigned char array_dacdds[9][21] =
{
	{ARRAY_DACDDS},
	{ARRAY_DACDDS},
	{ARRAY_DACDDS},
	{ARRAY_DACDDS},
	{ARRAY_DACDDS},
	{ARRAY_DACDDS},
	{ARRAY_DACDDS},
	{ARRAY_DACDDS},
	{ARRAY_DACDDS}
};
unsigned char array_level[9][21] =
{
	{ARRAY_LEVEL},
	{ARRAY_LEVEL},
	{ARRAY_LEVEL},
	{ARRAY_LEVEL},
	{ARRAY_LEVEL},
	{ARRAY_LEVEL},
	{ARRAY_LEVEL},
	{ARRAY_LEVEL},
	{ARRAY_LEVEL}
};
unsigned char array_dacdds_fab[9][21] =
{
	/*{ARRAY_DACDDS},*/{255,46,49,43,45,47,43,50,43,43,43,43,43,43,43,43,43,43,43,43,44},	/* 1MHz Facial */
	/*{ARRAY_DACDDS},*/{255,46,44,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,44},	/* 1MHz Corporal */
	/*{ARRAY_DACDDS},*/{255,48,43,45,48,43,46,43,47,43,48,43,48,43,47,43,43,46,43,43,44},	/* 1MHz Especific */
	/*{ARRAY_DACDDS},*/{255,55,50,51,48,47,43,44,43,44,44,43,44,43,43,43,43,43,43,43,60},	/* 3MHz Facial */
	/*{ARRAY_DACDDS},*/{255,45,44,44,43,44,44,43,44,43,43,43,43,43,43,43,43,43,43,43,43},	/* 3MHz Corporal */
	/*{ARRAY_DACDDS},*/{255,46,51,54,58,48,43,44,44,44,44,43,44,44,43,44,44,44,43,43,65},	/* 3MHz Especific */
	/*{ARRAY_DACDDS}*/ {255,50,49,47,46,47,43,47,43,43,43,43,43,43,43,43,43,43,43,43,52},	/* barrido Facial */
	/*{ARRAY_DACDDS}*/ {255,45,44,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43},	/* barrido Corporal */
	/*{ARRAY_DACDDS}*/ {255,47,47,49,53,45,44,43,45,43,46,43,46,43,45,43,43,45,43,43,54}	/* barrido Especific */
};
unsigned char array_level_fab[9][21] =
{
	/*{ARRAY_LEVEL},*/{255,231,229,228,226,224,223,221,220,218,215,213,207,203,188,170,170,170,170,170,171},	/* 1MHz Facial */
	/*{ARRAY_LEVEL},*/{255,218,218,218,218,218,216,214,208,201,182,175,174,173,172,172,172,171,171,170,171},	/* 1MHz Corporal */
	/*{ARRAY_LEVEL},*/{255,231,230,228,226,225,224,224,223,223,222,222,221,221,220,220,219,218,218,217,216},	/* 1MHz Especific */
	/*{ARRAY_LEVEL},*/{255,236,235,234,234,233,232,229,227,224,222,221,219,218,216,214,212,210,208,206,201},	/* 3MHz Facial */
	/*{ARRAY_LEVEL},*/{255,232,229,225,223,220,219,219,218,218,217,216,215,214,213,212,211,209,208,207,206},	/* 3MHz Corporal */
	/*{ARRAY_LEVEL},*/{255,236,234,233,232,232,232,231,230,229,228,227,226,225,224,223,222,221,221,220,216},	/* 3MHz Especific */
	/*{ARRAY_LEVEL},*/{255,233,232,231,230,228,227,225,223,221,218,217,213,210,202,192,191,190,189,188,186},	/* barrido Facial */
	/*{ARRAY_LEVEL}*/ {255,225,223,221,220,219,217,216,213,209,199,195,194,193,192,192,191,190,189,188,188},	/* barrido Corporal */
	/*{ARRAY_LEVEL}*/ {255,233,232,230,229,228,228,227,226,226,225,224,223,223,222,221,220,219,219,218,216}		/* barrido Especific */
};

const float array_anrf_1mhz_facial[21] = {ARRAY_ANRF_1MHZ_FACIAL};
const float array_anrf_3mhz_facial[21] = {ARRAY_ANRF_3MHZ_FACIAL};
const float array_anrf_1mhz_corporal[21] = {ARRAY_ANRF_1MHZ_CORPORAL};
const float array_anrf_3mhz_corporal[21] = {ARRAY_ANRF_3MHZ_CORPORAL};
const float array_anrf_1mhz_especific[21] = {ARRAY_ANRF_1MHZ_ESPECIFIC};
const float array_anrf_3mhz_especific[21] = {ARRAY_ANRF_3MHZ_ESPECIFIC};



unsigned char handle_value = 0xFF;
volatile rf_frequencies_t freq_value = RF_freq_undef;
unsigned char dacdds_value = 0xFF;
unsigned char refdacdds_value = 0xFF;
unsigned char level_value = 0xFF;
unsigned char index_percentage_value = 0xFF;
calibration_process_t calibration_status = CALIBRATION_NO_STATE;
calibration_values_t calibration_search_result = CALIBRATION_VALUE_UNDER;

extern float voltage_anrf;
extern volatile unsigned int tick_warmup;
unsigned char warmup_started = 0;
unsigned char last_level_value = LEVEL_MIN + 5;
extern signed int dacdds_drift_correction;
extern signed int level_drift_correction;

















extern USB_HANDLE USBOutHandle;	// USB handle.  Must be initialized to 0 at startup.
extern USB_HANDLE USBInHandle;	// USB handle.  Must be initialized to 0 at startup.

extern unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
extern machine_states_t Estado_Maquina;
extern machine_states_t Mi_Estado_Maquina;
extern unsigned char Valor_Stim;
extern unsigned char Valor_Galva;
extern unsigned char Valor_Lift;
extern unsigned char Valor_RF;
extern unsigned char Aplicador;
extern unsigned char Salida_Uno;
extern unsigned char Salida_Dos;
extern unsigned char Salida_Tres;
extern unsigned char Estado_Aplicador;
extern unsigned int Num_Serie;
extern unsigned char Retorno_Galva;
extern unsigned char Alarma;	//todonova?¿
extern unsigned char Frecuencia;
extern unsigned char Cont_Galva;
extern unsigned char Estado_Galva;
extern unsigned int Vector_Galva[10];// = {0,0,0,0,0,0,0,0,0,0};
extern unsigned int Media_Galva ;
extern unsigned char Volumen;
extern unsigned int Frecuencia_Resulta;
extern volatile unsigned char Pitar;
extern unsigned char Standby;
extern volatile unsigned char Senyal_Stim;
extern unsigned char Alarma_Temp;
extern unsigned char Apagada_RF;
extern unsigned int Lect_RF;
/**/
extern unsigned char Offset_RF;
extern unsigned char No_Apli;
extern float Temperatura;
extern unsigned char Temperatura_Alarma;
extern unsigned char Dac_Gal;
extern unsigned char Dac_Stim;
extern unsigned char Dac_RF;
extern unsigned char Dac_Bias;
extern unsigned char Reducir;			//sem
extern unsigned char Cont_Reducir;
extern unsigned char Sombra_Apli;
extern unsigned char Connected;
extern volatile unsigned char Target_Galva;
extern volatile float Pasos_Galva;
extern volatile unsigned char Aux_Galva;

extern volatile struct u_sem
{
	unsigned char Lect_Gal:1;		// Semáforo de lectura de galvánica.
	unsigned char Lect_Man:1;		// Semáforo de lectura de aplicadores.
	unsigned char Flag_Barrido:1;	// Semáforo de barrido RF.
	unsigned char Time_out:1;		// Semáforo timeout coms.
	unsigned char Sube_Baja:1;		// Semáforo para aumentar o disminuir frecuencia en pulsos.
	unsigned char Primera_Galva:1;	// Semáforo de galvánica.
	unsigned char Tecla:1;			// Semáforo lectura tecla.
	unsigned char Temper:1;			// Semáforo para leer temperatura.
} Sem;

extern union unio_rel
{
	unsigned int Byte;
	struct u_rel
	{
		unsigned char Rel3:1;		//
		unsigned char Rel2:1;		//
		unsigned char Rel1:1;		//
		unsigned char TstCh2:1;		//
		unsigned char TstCh1:1;		//
		unsigned char OnCh3:1;		//
		unsigned char OnCh2:1;		//
		unsigned char OnCh1:1;		//
	} Bit;
} Rele;
union unio_rel Rele;

unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
unsigned char ToSendDataBuffer[64] TX_DATA_BUFFER_ADDRESS;
unsigned char rxLenght = 0;		// Longuitud en bytes de la trama recibida.

float Save_Temperatura = 0;
unsigned int Aux_Env_Rec = 0;
unsigned char Ant_Estado_Maquina = READY;
unsigned char Ant_Valor_Stim = 0;
unsigned char Ant_Valor_Lift = 0;
unsigned char Ant_Valor_Galva = 0;
unsigned char Ant_Valor_RF = 0;
unsigned char last_rf_value = 0;
unsigned char Ant_Frecuencia = 0;
unsigned char Ant_Senyal_Stim = 0;
unsigned char Estado_Stim = OFF;
unsigned char Estado_Lift = OFF;
unsigned char Estado_RF = OFF;

unsigned char Estoy_Test = 0;
unsigned char Cont_Test = 0;
unsigned char Dec = 0;
unsigned char Cen = 0;
unsigned char Mil = 0;
unsigned char Dmil = 0;
unsigned char Rst_Done = 0;
unsigned char intermig = 0;
//const char Version_Soft[] = "v1.0.0";
//const char Version_Soft[] = "v1.0.5";
//const char Version_Soft[] = "v1.0.6";
//const char Version_Soft[] = "v1.1.0";
//const char Version_Soft[] = "v1.2.0";
const char Version_Soft[] = "v1.3.1";
unsigned char Ajust_RefDacDDS = 114;		// per poder modificarlo desde ajustos amb boto temperatura
//unsigned char Segunda = 0;
unsigned char AuxDac_RF = 230;

/********************************************************************
 * Function:	(Error_Trama)Analizar_Trama()						*
 * Definition:	Rutina para analizar trama recibida.				*
 ********************************************************************/
unsigned char Analizar_Trama(void)
{
	unsigned char Cont = 0;
	unsigned char Suma_Recep = 0;

	rxLenght = ReceivedDataBuffer[0];
	for (Cont=0; Cont < (rxLenght - 1); Cont++)
	{
		Suma_Recep = Suma_Recep^ReceivedDataBuffer[Cont];
	}

	if (Suma_Recep == ReceivedDataBuffer[rxLenght - 1])
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

/********************************************************************
 * Function:	Procesar_Ordenes(Ordre)								*
 * Definition:	Rutina para procesar órden de INTAB.				*
 ********************************************************************/
void Procesar_Ordenes(unsigned char Ordre)
{
	switch (Ordre)
	{
		case TRAMA_STATUS:	//2
			if (Estado_Maquina != Ant_Estado_Maquina)
			{
				Ant_Estado_Maquina = Estado_Maquina;

				if (Estado_Maquina == ACTIVE)	// Máquina funcionando.
				{
					if (Valor_Galva != 0)
					{
						Enciende_Galva(Valor_Galva);
						Ant_Valor_Galva = Valor_Galva;
					}
					if (Valor_Stim != 0)
					{
						Enciende_Stim(Valor_Stim);
						Ant_Valor_Stim = Valor_Stim;
					}
					if (Valor_Lift != 0)
					{
						Enciende_Lift(Valor_Lift);
						Ant_Valor_Lift = Valor_Lift;
					}
					if (Valor_RF != RF_value_0)
					{
						Enciende_RF(Valor_RF, Frecuencia);
						Ant_Valor_RF = Valor_RF;
					}
				}
				if ((Estado_Maquina == READY)||(Estado_Maquina == STANDBY)||(Estado_Maquina == 3))	// Máquina parada.
				{
					Apaga_Galva();
					Apaga_Stim();
					Apaga_Lift();
					Apaga_RF();
				}
			}

			if (Estado_Maquina == ACTIVE)
			{
				if (Sombra_Apli == 2)	// Para encender rf después de desconectar aplicador. (Realmente la aplicación debería parar el tratamiento).
				{
					Sombra_Apli = 0;
					if ((Valor_RF != RF_value_0)&&(Estado_RF == OFF))
					{
						Estado_RF = O_N;
						Ant_Valor_RF = 99;
					}
				}

				if (Estado_Galva == O_N)
				{
					if (Ant_Valor_Galva != Valor_Galva)
					{
						Ant_Valor_Galva = Valor_Galva;
						Enciende_Galva(Valor_Galva);
					}
				}
				else
				{
					if (Ant_Valor_Galva != Valor_Galva)
					{
						if (Valor_Galva != 0)
						{
							Enciende_Galva(Valor_Galva);
						}
						else
						{
							Apaga_Galva();
						}
						Ant_Valor_Galva = Valor_Galva;
					}
				}

				if (Estado_Stim == O_N)
				{
					if (Ant_Valor_Stim != Valor_Stim)
					{
						Ant_Valor_Stim = Valor_Stim;
						Enciende_Stim(Valor_Stim);
					}

					if (Ant_Senyal_Stim != Senyal_Stim)
					{
						Ant_Senyal_Stim = Senyal_Stim;
						Enciende_Stim(Valor_Stim);
					}
				}
				else
				{
					if (Ant_Valor_Stim != Valor_Stim)
					{
						if (Valor_Stim != 0)
						{
							Enciende_Stim(Valor_Stim);
						}
						else
						{
							Apaga_Stim();
						}
						Ant_Valor_Stim = Valor_Stim;
					}
				}

				if (Estado_Lift == O_N)
				{
					if (Ant_Valor_Lift != Valor_Lift)
					{
						Ant_Valor_Lift = Valor_Lift;
						Enciende_Lift(Valor_Lift);
					}
				}
				else
				{
					if (Ant_Valor_Lift != Valor_Lift)
					{
						if (Valor_Lift != 0)
						{
							Enciende_Lift(Valor_Lift);
						}
						else
						{
							Apaga_Lift();
						}
						Ant_Valor_Lift = Valor_Lift;
					}
				}

				if (Estado_RF == O_N)
				{
					if ((Ant_Valor_RF != Valor_RF)||(Ant_Frecuencia != Frecuencia))
					{
						Ant_Valor_RF = Valor_RF;
						Ant_Frecuencia = Frecuencia;
						Enciende_RF(Valor_RF,Frecuencia);				// Cuando reduzco porque está al aire, el valor recibido es diferente y encendía,
					}													// con lo que reiniciaba el contador de detección.
				}
				else
				{
					if (Ant_Valor_RF != Valor_RF)
					{
						if (Valor_RF != 0)
						{
							Enciende_RF(Valor_RF,Frecuencia);
						}
						else
						{
							Apaga_RF();
						}
						Ant_Valor_RF = Valor_RF;
					}
				}
			}
		break;
		case REC_NSERIE:	//51
			Num_Serie = 0;
			Dmil = ReceivedDataBuffer[2] - 0x30;
			Mil = ReceivedDataBuffer[3] - 0x30;
			Cen = ReceivedDataBuffer[4] - 0x30;
			Dec = ReceivedDataBuffer[5] - 0x30;
			Aux_Env_Rec = ReceivedDataBuffer[6] - 0x30;
			if (Dmil > 9)
			{
				Num_Serie = 99999;
			}
			else
			{
				Num_Serie = (Dmil*10000) + (Mil*1000) + (Cen*100) + (Dec*10) + Aux_Env_Rec;
				Grabar_Flash();
			}
		break;
		case CONT_BUZZER:	//5
			Pitar = ReceivedDataBuffer[2];	// TODO podría ser número de pitos!
			Volumen = ReceivedDataBuffer[3];
			Control_Volumen(Volumen);
		break;
		case NIVEL_BUZZER:	//6
			Volumen = ReceivedDataBuffer[2];
			Control_Volumen(Volumen);
			Grabar_Flash();
		break;
		case SET_ALARM_TEMP://27
			Temperatura_Alarma = ReceivedDataBuffer[2];
		break;
		case SET_GAL:		//31
			Dac_Gal = ReceivedDataBuffer[2];
			Cont_Test = 0;
			Estoy_Test = GALVANIC_TEST;
			/* commented just to avoid problems with Rös's, because they are setting settings here and then trying it on the main screen */
			// Enciende_Galva(10);
		break;
		case SET_STIM:		//36
			Dac_Stim = ReceivedDataBuffer[2];
			Cont_Test = 0;
			Estoy_Test = STIMULATION_TEST;
			/* commented just to avoid problems with Rös's, because they are setting settings here and then trying it on the main screen */
			// Enciende_Stim(10);
		break;
		case SET_RF:		//41
			Dac_RF = ReceivedDataBuffer[2];
			AuxDac_RF = ReceivedDataBuffer[2];
			Estoy_Test = RF_TEST;
			//	Encender_RF(10);
			// TODO ENCIEDN rf??!??! TEST?!?!?
		break;
		case SET_BIAS:		//44
			Dac_Bias = ReceivedDataBuffer[2];
			Estoy_Test = RF_TEST;
			//	Encender_RF(10);
			// TODO ENCIEDN rf??!??! TEST?!?!?
		break;
		case REC_CAL:		//48
			Estoy_Test = NO_TEST;
			//Parar todo ??!!!
			Grabar_Flash();
		break;
		case RST_FAB:		//88
			Rst_Done = 0;
			Volumen = VOL_FAB;
			Dac_Gal = DAC_GAL_FAB;
			Dac_Stim = DAC_STIM_FAB;
			Dac_RF = DAC_RF_FAB;
			Dac_Bias = DAC_BIAS_FAB;
			Temperatura_Alarma = TEMPERATURA_ALARMA;
			Grabar_Flash();
			Rst_Done = 1;
		break;
		default:	Nop();			break;
	}
}

static unsigned char index = MOVING_AVERAGE_NUM_OF_SAMPLES; /* has to start at the end because of how loop works, first increments index and then writes value */
static float array[MOVING_AVERAGE_NUM_OF_SAMPLES] = {0}; /* data array */

/********************************************************************
 * Function:	voltage_moving_average(input)							*
 * Definition:	Rutina para tratar órdenes en modo calibración		*
 ********************************************************************/
void voltage_moving_average(float new_input_value, float * moving_average)
{
	/* TODO CHECK BECAUSE HAS STRANGE BEHAVIOR: VALUES IN ARRAY SEEM CORRECT BUT CALCULATIONS DO NOT */
	// float new_value = 0.0f;
	// float old_value = 0.0f;
	float average = 0.0f;
	unsigned char i = 0;
	/* increment index and wrap around MOVING_AVERAGE_NUM_OF_SAMPLES, for ring buffering */
	index = (index + 1) % MOVING_AVERAGE_NUM_OF_SAMPLES;

	/* input new value into ring */
	array[index] = new_input_value;

	// /* add new value to moving average and eliminate oldest */
	// new_value = array[index] / MOVING_AVERAGE_NUM_OF_SAMPLES;
	// old_value = array[(index + 1) % MOVINfG_AVERAGE_NUM_OF_SAMPLES] / MOVING_AVERAGE_NUM_OF_SAMPLES;
	// (*moving_average) = (float) ((*moving_average) + (new_value) - (old_value));

	for(i = 0; i < MOVING_AVERAGE_NUM_OF_SAMPLES; i++)
	{
		average = (float) average + array[i];
	}
	average = (float) (average/((float)(MOVING_AVERAGE_NUM_OF_SAMPLES * 1.0f)));

	(*moving_average) = (float) average;
}

/********************************************************************
 * Function:	calibration_process(input)							*
 * Definition:	Rutina para tratar órdenes en modo calibración		*
 ********************************************************************/
float voltage = 0.0f;
unsigned char value_correct_counter = 0;
void calibration_process(unsigned char input)
{
	unsigned int i = 0;
	switch((calibration_process_t)input)
	{
		case CALIBRATION_STARTING_RF:
			HS_FAN = 1;				// Enciendo ventilador radiador.
			Mirar_Aplicador();
			Rele.Bit.Rel3 = 1;
			Control_TPIC();
			Aplicador = handle_value;
			Estado_Maquina = ACTIVE;
			Estado_RF = O_N;		// Indico que la RF está en marcha.
			Valor_RF = RF_value_5;
			Frecuencia = freq_value;
			refdacdds_value = REFDACDDS_VALUE;
			dacdds_value = DACDDS_MIN;
			level_value = LEVEL_STARTING_VALUE;
			/* Enciende_RF */
				Carga_TLC5620(REFDACBIAS | 200,3);
				Carga_TLC5620(DACBIAS | Dac_Bias,3);	// 80
				Carga_TLC5620(REFDACDDS | REFDACDDS_VALUE, 3);
				Carga_TLC5620(DACDDS | DACDDS_MIN, 3);
				Carga_TLC5620(LEVEL | LEVEL_STARTING_VALUE, 1);
				Stop_Timer4();
				Set_Freq_AD9834(freq_value*100000);	// Pongo frecuencia.
				Apagada_RF = 0;
			/* Enciende_RF */
			warmup_started = 0;
			calibration_status = CALIBRATION_RF_RUNNING;
		break;
		case CALIBRATION_RF_RUNNING:
			Carga_TLC5620(REFDACDDS | refdacdds_value, 3);
			Carga_TLC5620(DACDDS | dacdds_value, 3);
			Carga_TLC5620(LEVEL | level_value, 1);
			calibration_process(CALIBRATION_RF_READ_VOLTAGE);
		break;
		case CALIBRATION_SAVE_SERIAL:
			if((ReceivedDataBuffer[6] - 0x30) == FACIAL)
			{
				handle_value = FACIAL;
			}
			else if ((ReceivedDataBuffer[6] - 0x30) == CORPORAL)
			{
				handle_value = CORPORAL;
			}
			else if ((ReceivedDataBuffer[6] - 0x30) == ESPE)
			{
				handle_value = ESPE;
			}
			if (((ReceivedDataBuffer[3] - 0x30)*100 + (ReceivedDataBuffer[4] - 0x30)*10 + (ReceivedDataBuffer[5] - 0x30)) == 10)
			{
				freq_value = RF_freq_1mhz;
			}
			else if ((ReceivedDataBuffer[3] - 0x30)*100 + (ReceivedDataBuffer[4] - 0x30)*10 + (ReceivedDataBuffer[5] - 0x30) == 30)
			{
				freq_value = RF_freq_3mhz;
			}
			else if ((ReceivedDataBuffer[3] - 0x30)*100 + (ReceivedDataBuffer[4] - 0x30)*10 + (ReceivedDataBuffer[5] - 0x30) == 0xBA)
			{
				freq_value = RF_freq_sweep;
			}
		break;
		case CALIBRATION_SAVE_VALUES:
		case CALIBRATION_READ_TEMP:
			/* TODO should be a more or less automatic process with which the machine adjusts itself linearly between a max and a min */
			if (index_percentage_value < RF_calibrate || (input == CALIBRATION_SAVE_VALUES && calibration_status == CALIBRATION_FOUND_VALUE))
			{
				/* SAVE VALUE TO TABLE for accessing when starting RF */
				if(handle_value == CORPORAL)
				{
					if(freq_value == RF_freq_1mhz)
					{
						array_dacdds[RF_arrays_1mhz_corporal][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_1mhz_corporal][index_percentage_value] = level_value;
					}
					else if(freq_value == RF_freq_3mhz)
					{
						array_dacdds[RF_arrays_3mhz_corporal][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_3mhz_corporal][index_percentage_value] = level_value;
					}
					else if (freq_value == RF_freq_sweep)
					{
						array_dacdds[RF_arrays_ba_corporal][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_ba_corporal][index_percentage_value] = level_value;
					}
				}
				else if (handle_value == FACIAL)
				{
					if(freq_value == RF_freq_1mhz)
					{
						array_dacdds[RF_arrays_1mhz_facial][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_1mhz_facial][index_percentage_value] = level_value;
					}
					else if(freq_value == RF_freq_3mhz)
					{
						array_dacdds[RF_arrays_3mhz_facial][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_3mhz_facial][index_percentage_value] = level_value;
					}
					else if (freq_value == RF_freq_sweep)
					{
						array_dacdds[RF_arrays_ba_facial][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_ba_facial][index_percentage_value] = level_value;
					}
				}
				else if (handle_value == ESPE)
				{
					if(freq_value == RF_freq_1mhz)
					{
						array_dacdds[RF_arrays_1mhz_especific][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_1mhz_especific][index_percentage_value] = level_value;
					}
					else if(freq_value == RF_freq_3mhz)
					{
						array_dacdds[RF_arrays_3mhz_especific][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_3mhz_especific][index_percentage_value] = level_value;
					}
					else if (freq_value == RF_freq_sweep)
					{
						array_dacdds[RF_arrays_ba_especific][index_percentage_value] = dacdds_value;
						array_level[RF_arrays_ba_especific][index_percentage_value] = level_value;
					}
				}
			}
			//else if (index_percentage_value == RF_calibrate)
			else
			{
				/* SEARCH FOR DACDDS AND LEVEL VALUES KNOWING DESIRED VOLTAGES */
				/* when searching ascending */
				// index_percentage_value = RF_value_5;
				/* when searching descending */
				tick_warmup = 0;
				index_percentage_value = RF_value_100;
				calibration_status = CALIBRATION_WAITING_WARMUP;
			}
		break;
		case CALIBRATION_SET_GAL:
			/* save ReceivedDataBuffer[2] as refdacdds value */
			if(ReceivedDataBuffer[2] != REFDACDDS_VALUE)
			{
				refdacdds_value = REFDACDDS_VALUE;
			}
			else
			{
				refdacdds_value = ReceivedDataBuffer[2];
			}
		break;
		case CALIBRATION_SET_STIM:
			/* save ReceivedDataBuffer[2] as dacdds value */
			if(ReceivedDataBuffer[2] <= DACDDS_MIN)
			{
				dacdds_value = DACDDS_MIN;
			}
			else
			{
				dacdds_value = ReceivedDataBuffer[2];
			}
		break;
		case CALIBRATION_SET_RF:
			/* save ReceivedDataBuffer[2] as level value */
			if(ReceivedDataBuffer[2] <= LEVEL_MIN)
			{
				level_value = LEVEL_MIN;
			}
			else
			{
				level_value = ReceivedDataBuffer[2];
			}
		break;
		case CALIBRATION_SET_BIAS:
			/* save ReceivedDataBuffer[2] as % value */
			if(index_percentage_value > RF_calibrate)
			{
				index_percentage_value = RF_calibrate;
			}
			else
			{
				index_percentage_value = ReceivedDataBuffer[2];
			}
		break;
		case CALIBRATION_START_SEARCH:
			refdacdds_value = REFDACDDS_VALUE;
			dacdds_value = DACDDS_MIN;
			// /* ORIGINAL */ level_value = LEVEL_MAX;

			/* when searching ascending */
			// level_value = last_level_value + 5;

			/* when searching descending */
			level_value = last_level_value - 5;

			Carga_TLC5620(REFDACDDS | refdacdds_value, 3);
			Carga_TLC5620(DACDDS | dacdds_value, 3);
			Carga_TLC5620(LEVEL | level_value, 1);

			calibration_status = CALIBRATION_SEARCHING_VALUE;
		break;
		case CALIBRATION_FOUND_VALUE:
			/* count number of correct voltages before saving value for DACDDS and LEVEL */
			if(value_correct_counter >= NUMBER_OF_CORRECT_VALUES_BEFORE_SAVING)
			{
				value_correct_counter = 0;
				calibration_process(CALIBRATION_SAVE_VALUES);

				last_level_value = level_value;

				/* wrap around 20 to not overflow any array */
				/* when searching ascending */
				// index_percentage_value++;
				// if(index_percentage_value == RF_calibrate)
				/* when searching descending */
				index_percentage_value--;
				if(index_percentage_value == RF_value_0)
				{
					calibration_status = CALIBRATION_ALL_VALUES_FOUND_FOR_SERIES;
				}
				else if (index_percentage_value > RF_value_100)
				{
					index_percentage_value = RF_value_100;
				}
				else
				{
					/* when searching ascending */
					// calibration_status = CALIBRATION_WAITING_ANRF_FALL_DOWN;
					/* when searching descending */
					calibration_status = CALIBRATION_START_SEARCH;
				}
			}
			else
			{
				value_correct_counter++;
				calibration_status = CALIBRATION_CHECKING_VALUE;
			}
		break;
		case CALIBRATION_WAITING_ANRF_FALL_DOWN:
			refdacdds_value = REFDACDDS_VALUE;
			dacdds_value = DACDDS_MIN;
			level_value = LEVEL_MAX;

			Carga_TLC5620(REFDACDDS | refdacdds_value, 3);
			Carga_TLC5620(DACDDS | dacdds_value, 3);
			Carga_TLC5620(LEVEL | level_value, 1);
			calibration_process(CALIBRATION_RF_READ_VOLTAGE);
			if(voltage_anrf < VOLTAGE_TO_WAIT_ANRF)
			{
				calibration_status = CALIBRATION_START_SEARCH;
			}
		break;
		case CALIBRATION_SEARCHING_VALUE:
			dacdds_value++;
			if(dacdds_value >= DACDDS_MAX)
			{
				dacdds_value = DACDDS_MIN;
				/* when searching ascending */
				// level_value--;
				// if(level_value <= LEVEL_MIN)
				// {
				// 	level_value = LEVEL_MAX;
				// 	calibration_status = CALIBRATION_VALUE_NOT_REACHABLE;
				// }
				/* when searching descending */
				level_value++;
				if(level_value <= LEVEL_MIN || level_value >= LEVEL_MAX)
				{
					level_value = LEVEL_MIN;
					calibration_status = CALIBRATION_VALUE_NOT_REACHABLE;
				}
			}

			Carga_TLC5620(REFDACDDS | refdacdds_value, 3);
			Carga_TLC5620(DACDDS | dacdds_value, 3);
			Carga_TLC5620(LEVEL | level_value, 1);
			calibration_status = CALIBRATION_CHECKING_VALUE;
		break;
		case CALIBRATION_CHECKING_VALUE:
			calibration_process(CALIBRATION_RF_READ_VOLTAGE);
			calibration_search_result = (calibration_values_t) is_voltage_correct(voltage_anrf, index_percentage_value, handle_value, freq_value, NULL);
			/* when searching ascending */
			// if((dacdds_value == (DACDDS_MIN + 1)) && (calibration_search_result == CALIBRATION_VALUE_UNDER))
			/* when searching descending */
			if((dacdds_value == (DACDDS_MIN + 1)) && (calibration_search_result == CALIBRATION_VALUE_OVER))
			{
				/* acceleration because if at max power (DACDDS_MIN) at that LEVEL we already are under desired value, means we can fastly increment LEVEL */
				dacdds_value = DACDDS_MIN;
				/* when searching ascending */
				// level_value--;
				// if(level_value <= LEVEL_MIN)
				// {
				// 	level_value = LEVEL_MAX;
				// 	calibration_status = CALIBRATION_VALUE_NOT_REACHABLE;
				// }
				/* when searching descending */
				level_value++;
				if(level_value <= LEVEL_MIN || level_value >= LEVEL_MAX)
				{
					level_value = LEVEL_MIN;
					calibration_status = CALIBRATION_VALUE_NOT_REACHABLE;
				}
				calibration_status = CALIBRATION_SEARCHING_VALUE;
			}
			/* when searching ascending */
			// else if(calibration_search_result == CALIBRATION_VALUE_OVER)
			/* when searching descending */
			else if(calibration_search_result == CALIBRATION_VALUE_UNDER)
			{
				/* when searching descending */
				level_value--;
				dacdds_value = DACDDS_MIN + 1;
				if(level_value <= LEVEL_MIN || level_value >= LEVEL_MAX)
				{
					level_value = LEVEL_MIN;
					calibration_status = CALIBRATION_VALUE_NOT_REACHABLE;
				}
				calibration_status = CALIBRATION_SEARCHING_VALUE;
			}
			else if(calibration_search_result == CALIBRATION_VALUE_OK)
			{
				/* found value was OK */
				calibration_status = CALIBRATION_FOUND_VALUE;
			}
			else if((value_correct_counter > 0) && (calibration_search_result != CALIBRATION_VALUE_OK))
			{
				calibration_status = CALIBRATION_SEARCHING_VALUE;
			}
			else
			{
				/* value was not correct */
				calibration_status = CALIBRATION_SEARCHING_VALUE;
			}
		break;
		case CALIBRATION_RF_READ_VOLTAGE:
			if(freq_value == RF_freq_1mhz)
			{
				voltage_moving_average(read_voltage_anrf_1mhz(Lectura_RF()), &voltage_anrf);
			}
			else if(freq_value == RF_freq_3mhz)
			{
				voltage_moving_average(read_voltage_anrf_3mhz(Lectura_RF()), &voltage_anrf);
			}
		break;
		case CALIBRATION_VALUE_NOT_REACHABLE:
			/* TODO: what happens in this case? just a breakpoint for now */
			Nop();
		break;
		case CALIBRATION_ALL_VALUES_FOUND_FOR_SERIES:
			if(freq_value == RF_freq_3mhz && handle_value == CORPORAL)
			{
				freq_value = RF_freq_3mhz;
				handle_value = FACIAL;
				Set_Freq_AD9834(freq_value*100000);	// Pongo frecuencia.
				index_percentage_value = RF_value_100;
				calibration_status = CALIBRATION_START_SEARCH;
			}
			else if(freq_value == RF_freq_3mhz && handle_value == FACIAL)
			{
				freq_value = RF_freq_3mhz;
				handle_value = ESPE;
				Set_Freq_AD9834(freq_value*100000);	// Pongo frecuencia.
				index_percentage_value = RF_value_100;
				calibration_status = CALIBRATION_START_SEARCH;
			}
			else if(freq_value == RF_freq_3mhz && handle_value == ESPE)
			{
				freq_value = RF_freq_1mhz;
				handle_value = CORPORAL;
				Set_Freq_AD9834(freq_value*100000);	// Pongo frecuencia.
				index_percentage_value = RF_value_100;
				calibration_status = CALIBRATION_START_SEARCH;
			}
			else if(freq_value == RF_freq_1mhz && handle_value == CORPORAL)
			{
				freq_value = RF_freq_1mhz;
				handle_value = FACIAL;
				Set_Freq_AD9834(freq_value*100000);	// Pongo frecuencia.
				index_percentage_value = RF_value_100;
				calibration_status = CALIBRATION_START_SEARCH;
			}
			else if(freq_value == RF_freq_1mhz && handle_value == FACIAL)
			{
				freq_value = RF_freq_1mhz;
				handle_value = ESPE;
				Set_Freq_AD9834(freq_value*100000);	// Pongo frecuencia.
				index_percentage_value = RF_value_100;
				calibration_status = CALIBRATION_START_SEARCH;
			}
			else if(freq_value == RF_freq_1mhz && handle_value == ESPE)
			{
				calibration_status = CALIBRATION_STOP_RF;

				for(i = 0; i < 21; i++)
				{
					array_dacdds[RF_arrays_ba_facial][i] = (array_dacdds[RF_arrays_1mhz_facial][i] + array_dacdds[RF_arrays_3mhz_facial][i])/2;
					array_level[RF_arrays_ba_facial][i] = (array_level[RF_arrays_1mhz_facial][i] + array_level[RF_arrays_3mhz_facial][i])/2;
					array_dacdds[RF_arrays_ba_corporal][i] = (array_dacdds[RF_arrays_1mhz_corporal][i] + array_dacdds[RF_arrays_3mhz_corporal][i])/2;
					array_level[RF_arrays_ba_corporal][i] = (array_level[RF_arrays_1mhz_corporal][i] + array_level[RF_arrays_3mhz_corporal][i])/2;
					array_dacdds[RF_arrays_ba_especific][i] = (array_dacdds[RF_arrays_1mhz_especific][i] + array_dacdds[RF_arrays_3mhz_especific][i])/2;
					array_level[RF_arrays_ba_especific][i] = (array_level[RF_arrays_1mhz_especific][i] + array_level[RF_arrays_3mhz_especific][i])/2;
				}
			}
		break;
		case CALIBRATION_STOP_RF:
			Apaga_RF();
			calibration_status = CALIBRATION_NO_STATE;
		break;
		case CALIBRATION_WAITING_WARMUP:
			if(warmup_started == 0)
			{
				HS_FAN = 1;				// Enciendo ventilador radiador.
				handle_value = CORPORAL;
				freq_value = RF_freq_3mhz; /* forcing 3 MHz for warmup */
				Mirar_Aplicador();
				Rele.Bit.Rel3 = 1;
				Control_TPIC();
				Aplicador = handle_value;
				Estado_Maquina = ACTIVE;
				Estado_RF = O_N;		// Indico que la RF está en marcha.
				Valor_RF = RF_value_5;
				Frecuencia = freq_value;
				refdacdds_value = REFDACDDS_VALUE;
				dacdds_value = DACDDS_MIN;
				level_value = (255-60); /* forcing relatively high value for warmup */
				/* Enciende_RF */
					Carga_TLC5620(REFDACBIAS | 200,3);
					Carga_TLC5620(DACBIAS | Dac_Bias,3);	// 80
					Carga_TLC5620(REFDACDDS | REFDACDDS_VALUE, 3);
					Carga_TLC5620(DACDDS | DACDDS_MIN, 3);
					Carga_TLC5620(LEVEL | level_value, 1);
					Stop_Timer4();
					Set_Freq_AD9834(freq_value*100000);	// Pongo frecuencia.
					Apagada_RF = 0;
				/* Enciende_RF */

				tick_warmup = 0;
				warmup_started = 1;
			}
			/* tick_warmup++ every 250ms, so 2 minutes = 120s -> 120/0.250 = 480 clicks */
			if(warmup_started == 1 && tick_warmup > WARMUP_TIME_TICKS)
			{
				tick_warmup = 0;
				calibration_status = CALIBRATION_START_SEARCH;
				index_percentage_value = RF_value_100;
				freq_value = RF_freq_3mhz;
				handle_value = CORPORAL;
			}
			else if(warmup_started == 1 && tick_warmup < WARMUP_TIME_TICKS)
			{
				/* will check for not surpassing max voltage, to avoid damages */
				calibration_process(CALIBRATION_RF_READ_VOLTAGE);
				if(is_voltage_correct(voltage_anrf, index_percentage_value, handle_value, freq_value, NULL) == CALIBRATION_VALUE_OVER)
				{
					if((dacdds_drift_correction >= (-DACDDS_DRIFT_CORRECTION_MAX)) && (dacdds_drift_correction < DACDDS_DRIFT_CORRECTION_MAX))
					{
						dacdds_drift_correction++;
						dacdds_value++;
					}
					else
					{
						dacdds_drift_correction = DACDDS_DRIFT_CORRECTION_MAX;
						dacdds_value = DACDDS_MIN;
						if((level_drift_correction >= (-LEVEL_DRIFT_CORRECTION_MAX)) && (level_drift_correction < LEVEL_DRIFT_CORRECTION_MAX))
						{
							level_drift_correction++;
							level_value++;
							dacdds_drift_correction = 0;
						}
						else
						{
							level_drift_correction = LEVEL_DRIFT_CORRECTION_MAX;
							level_value = LEVEL_MAX;
						}
					}

					Carga_TLC5620(REFDACDDS | REFDACDDS_VALUE, 3);
					Carga_TLC5620(DACDDS | dacdds_value, 3);
					Carga_TLC5620(LEVEL | level_value, 1);
				}
			}
		break;
		case CALIBRATION_NO_STATE:
		default:
		break;
	}

	// if(input != CALIBRATION_REFRESH_INTAB)
	// {
	// 	Mirar_Aplicador();
	// 	Rele.Bit.Rel1 = 1;
	// 	Rele.Bit.Rel2 = 1;
	// 	Rele.Bit.Rel3 = 1;
	// 	Control_TPIC();
	// 	Estado_Maquina = ACTIVE;
	// 	Estado_RF = O_N;
	// 	Valor_RF = RF_value_5;
	// 	Frecuencia = freq_value;
	// 	Enciende_RF(1, freq_value);
	// 	HS_FAN = 1;				// Enciendo ventilador radiador.
	// 	Nop();
	// }
}

/********************************************************************
 * Function:	read_voltage_anrf_FREQ(adc_value)					*
 * Definition:	Rutina para leer la tensión RMS que hay en ANRF		*
 ********************************************************************/
float read_voltage_anrf_1mhz(unsigned int adc_value)
{
	// /* ORIGINAL */ return (0.1042f * adc_value + 12.422f)*1.0f;
	return (0.1042f * adc_value + (12.422f + 1.0f))*1.0f; /* correction because I was always going ~1Vrms over setpoint */
}
float read_voltage_anrf_3mhz(unsigned int adc_value)
{
	// /* ORIGINAL */ return (0.1237f * adc_value + 13.133f)*1.0f;
	return (0.1237f * adc_value + (13.133f + 1.0f))*1.0f; /* correction because I was always going ~1Vrms over setpoint */
}

/********************************************************************
 * Function:	is_voltage_correct(voltage, index, handle, frequency)*
 * Definition:	Rutina para evaluar si una tension esta dentro del	*
 				margen para un indice dado							*
 ********************************************************************/
unsigned char is_voltage_correct(float voltage, unsigned char index, unsigned char handle, unsigned char frequency, float * voltage_read)
{
	float min = 0.0f;
	float max = 0.0f;
	calibration_values_t ret_value = CALIBRATION_VALUE_UNDER;

	if(handle == FACIAL)
	{
		if(frequency == 10)
		{
			min = array_anrf_1mhz_facial[index] * MARGIN_PERCENTAGE;
			max = array_anrf_1mhz_facial[index] / MARGIN_PERCENTAGE;
		}
		else if(frequency == 30)
		{
			min = array_anrf_3mhz_facial[index] * MARGIN_PERCENTAGE;
			max = array_anrf_3mhz_facial[index] / MARGIN_PERCENTAGE;
		}
	}
	else if(handle == CORPORAL)
	{
		if(frequency == 10)
		{
			min = array_anrf_1mhz_corporal[index] * MARGIN_PERCENTAGE;
			max = array_anrf_1mhz_corporal[index] / MARGIN_PERCENTAGE;
		}
		else if(frequency == 30)
		{
			min = array_anrf_3mhz_corporal[index] * MARGIN_PERCENTAGE;
			max = array_anrf_3mhz_corporal[index] / MARGIN_PERCENTAGE;
		}
	}
	else if(handle == ESPE)
	{
		if(frequency == 10)
		{
			min = array_anrf_1mhz_especific[index] * MARGIN_PERCENTAGE;
			max = array_anrf_1mhz_especific[index] / MARGIN_PERCENTAGE;
		}
		else if(frequency == 30)
		{
			min = array_anrf_3mhz_especific[index] * MARGIN_PERCENTAGE;
			max = array_anrf_3mhz_especific[index] / MARGIN_PERCENTAGE;
		}
	}

	if(voltage_read != NULL)
	{
		*voltage_read = max * MARGIN_PERCENTAGE; /* return voltage outside to be able to know when exceeding limits */
	}

	if(voltage > max)
	{
		ret_value = CALIBRATION_VALUE_OVER;
	}
	else if(voltage < min)
	{
		ret_value = CALIBRATION_VALUE_UNDER;
	}
	else if((voltage >= min) && (voltage <= max))
	{
		ret_value = CALIBRATION_VALUE_OK;
	}
	return ret_value;
}

/********************************************************************
 * Function:	Responder(Orden_Emb)								*
 * Definition:	Rutina para responder órden de embedded.			*
 ********************************************************************/
void Responder(unsigned char Orden_Emb)
{
	unsigned char Bytes_To_Send = 0;

	switch (Orden_Emb)
	{
		case TRAMA_STATUS:

			// Alarmas TODO
			Alarma = 0;
			if (Alarma_Temp == 1)	// Alarma temperatura radiador RF.
			{
				Alarma = 1;
				Estado_Maquina = READY;
			}

			// if (Sobre_Corriente == 1)
			// {
			// 	Buf_Envio[11] = Buf_Envio[11] + 4;			// Alarma CC ó sobre corriente.
			// }

			// if ((Alarma_Temp == 1)||(Sobre_Corriente == 1))
			// {
			// 	Estado_Maquina = READY;		// Paro yo el tratamiento y lo notifico.
			// }

			if ((Valor_Stim == 0)&&(Valor_Galva == 0)&&(Valor_Lift == 0)&&(Valor_RF == RF_value_0))	// Si no hay ningún tratamiento.
			{
				if (Mi_Estado_Maquina != STANDBY)
				{
					Estado_Maquina = READY;								// Si no estoy en Standby.
				}
			}
			else if ((Estado_Maquina == STANDBY)&&(Mi_Estado_Maquina != STANDBY))
			{
				Estado_Maquina = READY;
			}

			if (Retorno_Galva > Valor_Galva)
			{
				Retorno_Galva = Valor_Galva;					// Nunca retorno más de consigna.
			}
			if ((Estado_Maquina == READY)||(Estado_Galva == OFF))
			{
				Retorno_Galva = 0;			// Para no enviar siempre mismo nivel cuando no leo.
			}

//			Segunda++;
//			if ((Connected == 1)&&(Segunda >= 2))
//			{
//				Segunda = 2;
				if (Standby == 1)
				{
					Estado_Maquina = STANDBY;
				}
//			}

			Bytes_To_Send = 13;
			ToSendDataBuffer[2] = Estado_Maquina;
			ToSendDataBuffer[3] = Valor_Stim;
			ToSendDataBuffer[4] = Retorno_Galva;
			ToSendDataBuffer[5] = Valor_Lift;
			ToSendDataBuffer[6] = Valor_RF;
			ToSendDataBuffer[7] = Frecuencia;
			ToSendDataBuffer[8] = Alarma;
			ToSendDataBuffer[9] = Senyal_Stim;
			ToSendDataBuffer[10] = Estado_Aplicador;
			if (Temperatura >= 0xFF)
			{
				Temperatura = 0xFF;
			}
			Aux_Env_Rec = Temperatura;
			ToSendDataBuffer[11] = Aux_Env_Rec;
		break;
		case TRAMA_VERSION:	// OK
			Bytes_To_Send = 9;
			ToSendDataBuffer[2] = Version_Soft[0];	//v
			ToSendDataBuffer[3] = Version_Soft[1];	//0
			ToSendDataBuffer[4] = Version_Soft[2];	//.
			ToSendDataBuffer[5] = Version_Soft[3];	//0
			ToSendDataBuffer[6] = Version_Soft[4];	//.
			ToSendDataBuffer[7] = Version_Soft[5];	//1
		break;
		case READ_NSERIE:	//50
		case REC_NSERIE:	//51
			Aux_Env_Rec = Num_Serie;
			if (Aux_Env_Rec >= 100000)
			{
				Aux_Env_Rec = 99999;
			}
			Dec = 0;
			Cen = 0;
			Mil = 0;
			Dmil = 0;
			while (Aux_Env_Rec >= 10)
			{
				Aux_Env_Rec = Aux_Env_Rec - 10;
				Dec++;
				if (Dec >= 10)
				{
					Dec = Dec - 10;
					Cen++;
					if (Cen >= 10)
					{
						Cen = Cen - 10;
						Mil++;
						if (Mil >= 10)
						{
							Mil = Mil - 10;
							Dmil++;
						}
					}
				}
			}

			Bytes_To_Send = 8;
			ToSendDataBuffer[2] = Dmil;
			ToSendDataBuffer[3] = Mil;
			ToSendDataBuffer[4] = Cen;
			ToSendDataBuffer[5] = Dec;
			ToSendDataBuffer[6] = Aux_Env_Rec;
		break;
		case CONT_BUZZER:	//5
	//		Bytes_To_Send = 5;
	//		ToSendDataBuffer[2] = Volumen;// PitoS??
	//		ToSendDataBuffer[3] = Volumen;
	//	break;
		case NIVEL_BUZZER:	//6
			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Volumen;
		break;
		case READ_TEMP:		//25
			if (Temperatura >= 0xFF) Temperatura = 0xFF;
			Aux_Env_Rec = Temperatura;

			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Aux_Env_Rec;
		break;
		case READ_ALARM_TEMP://26
			if (Temperatura_Alarma >= 0xFF) Temperatura_Alarma = 0xFF;
			Aux_Env_Rec = Temperatura_Alarma;

			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Aux_Env_Rec;
		break;
		case READ_GAL:		//30
			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Dac_Gal;
		break;
		case READ_STIM:		//35
			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Dac_Stim;
		break;
		case READ_RF:		//40
			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Dac_RF;
		break;
		case READ_BIAS:		//43
			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Dac_Bias;
		break;
		case RST_FAB:
			Bytes_To_Send = 4;
			ToSendDataBuffer[2] = Rst_Done;
		break;
		default:		;		break;
	}

	ToSendDataBuffer[0] = Bytes_To_Send;	// Bytes_To_Send.
	ToSendDataBuffer[1] = Orden_Emb;		// Orden a responder.
}

/********************************************************************
 * Function:	Enciende_Stim(V_Stim)								*
 * Definition:	Rutina para poner en marcha estimulación			*
 ********************************************************************/
void Enciende_Stim(unsigned char V_Stim)
{
	Apaga_Lift();	// Sólo va uno de los dos tratamientos.

	PULSE_N = 0;	// No hay pulso.
	PULSE_P = 0;	// No hay pulso.
//	TEST1 = 0;		// Desactivo lectura de pulso
//	TEST2 = 0;		// Desactivo lectura de pulso

	Rele.Bit.TstCh1 = OFF;	// Deshabilito test
	Rele.Bit.TstCh2 = OFF;	// Deshabilito test

//	V_Stim = V_Stim * 20;	// TODO OJO
	/**/
	V_Stim = V_Stim * 10;	// TODO OJO
	/**/

	if ((Senyal_Stim == 4)||(Senyal_Stim == 5))
	{
		Carga_TLC5620(REFDACA,2);	// Canal A
		Carga_TLC5620(REFDACB,2);	// Canal B
	}
	else
	{
		Carga_TLC5620(REFDACA | Dac_Stim,2);	// Canal A 	//110
		Carga_TLC5620(REFDACB | Dac_Stim,2);	// Canal B	//110
	}
	Carga_TLC5620(DACA | V_Stim,2);
	Carga_TLC5620(DACB | V_Stim,2);

	Rele.Bit.OnCh1 = O_N;			// Activo salida 1.
	Rele.Bit.OnCh2 = O_N;			// Activo salida 2.
	Rele.Bit.OnCh3 = OFF;			// Desactivo salida 3.

	Control_TPIC();
	Estado_Stim = O_N;
	Config_Timer2();		// Configura timer de estimulación (interrupción).
}

/********************************************************************
 * Function:	Apaga_Stim()										*
 * Definition:	Rutina para parar estimulación.						*
 ********************************************************************/
void Apaga_Stim(void)
{
	if (Estoy_Test != 2)
	{
		CloseTimer2();		// Paro timer asociado.
		PULSE_N = 0;
		PULSE_P = 0;

		Carga_TLC5620(REFDACA,2);
		Carga_TLC5620(DACA,2);		// Pongo a cero nivel salida canal A.
		Carga_TLC5620(REFDACB,2);
		Carga_TLC5620(DACB,2);		// Pongo a cero nivel salida canal B.

		Rele.Bit.OnCh1 = OFF;		// Desactivo salida 1.
		Rele.Bit.OnCh2 = OFF;		// Desactivo salida 2.
		Rele.Bit.OnCh3 = OFF;		// Desactivo salida 3.

		Control_TPIC();
		Estado_Stim = OFF;

		Valor_Stim = 0;
	}
}

/********************************************************************
 * Function:	Enciende_Galva(V_Galva)								*
 * Definition:	Rutina para poner en marcha la galvánica.			*
 ********************************************************************/
void Enciende_Galva(unsigned char V_Galva)
{
	Sem.Primera_Galva = 0;
	Media_Galva = 0;
	for (Cont_Galva=0;Cont_Galva<10;Cont_Galva++) Vector_Galva[Cont_Galva] = 0;
	Cont_Galva = 0;

	Carga_TLC5620(REFDAC | Dac_Gal,1);		// 150

	/**/
//	V_Galva = V_Galva * 10;
	/**/
/*
	Target_Galva = V_Galva * 10;
	Pasos_Galva = (Target_Galva-10) / 10;
// Fa rampa sempre que canviem valor
	Aux_Galva = 0;
//	Carga_TLC5620(DACGAL | V_Galva,1);
	Carga_TLC5620(DACGAL | 10,1);
*/
	if (Estado_Galva == OFF) // Sol fa rampa si encenc la galva la primera vegada
	{
		Target_Galva = V_Galva * 10;
		Pasos_Galva = (Target_Galva-10) / 10;
		Aux_Galva = 0;
		Carga_TLC5620(DACGAL | 10,1);
	}
	else
	{
		V_Galva = V_Galva * 10;
		Aux_Galva = Target_Galva;
		Carga_TLC5620(DACGAL | V_Galva,1);
	}

	Estado_Galva = O_N;		// Indico que la galvánica está en marcha.

	ON_GP = OFF_POL;			// Positivo ON.
	ON_GN = ON_POL;
	Config_Timer3();		// Enciendo timer polaridad salida.

	if (Estoy_Test == 1)
	{
		if (Salida_Uno != 0)
		{
			Aplicador = Salida_Uno;
		}
		else if (Salida_Dos != 0)
		{
			Aplicador = Salida_Dos;
		}
		else if (Salida_Tres != 0)
		{
			Aplicador = Salida_Tres;
		}
	}
	/**/
	if (V_Galva == 0)
	{
		Apaga_Galva();	// Para que detecte si estoy tirando rf y galva y bajo galva a 0
	}
	/**/
}

/********************************************************************
 * Function:	Apaga_Galva()										*
 * Definition:	Rutina para parar galvánica.						*
 ********************************************************************/
void Apaga_Galva(void)
{
	if (Estoy_Test != 1)
	{
		CloseTimer3();			// Paro timer asociado.

		Carga_TLC5620(REFDAC,1);// Pongo a cero nivel salida.
		Carga_TLC5620(DACGAL,1);// Pongo a cero nivel salida.

		ON_GP = OFF_POL;		// Desconecto polaridad.
		ON_GN = OFF_POL;
		Estado_Galva = OFF;		// Indico que galvánica parada.

		Sem.Primera_Galva = 0;
		Media_Galva = 0;
		for (Cont_Galva=0;Cont_Galva<10;Cont_Galva++)
		{
			Vector_Galva[Cont_Galva] = 0;
		}
		Cont_Galva = 0;
		Valor_Galva = 0;
		Aux_Galva = 0;
	}
}

/********************************************************************
 * Function:	Enciende_Lift(V_Lift)								*
 * Definition:	Rutina para poner en marcha el lifting.				*
 ********************************************************************/
void Enciende_Lift(unsigned char V_Lift)
{
	Apaga_Stim();			// Sólo va uno de los dos tratamientos.
	/**/
	CloseTimer5();				// Paro timer asociado.
	/**/
	PULSE_N = 0;			// No hay pulso.
	PULSE_P = 0;			// No hay pulso.
//	TEST1 = 0;				// Desactivo lectura de pulso
//	TEST2 = 0;				// Desactivo lectura de pulso

	Rele.Bit.TstCh1 = OFF;	// Deshabilito test
	Rele.Bit.TstCh2 = OFF;	// Deshabilito test

//	V_Lift = V_Lift * 4;	// Así tengo el doble de recorrido para el dac.	// TODO OJO
	/**/
	V_Lift = V_Lift * 2;	// Así tengo el doble de recorrido para el dac.
	/**/

	Carga_TLC5620(REFDACB | 0,2);	// Canal B.	Tengo que ir de 0 a 100 (60Vpp).
	Carga_TLC5620(DACB | V_Lift,2);
	Rele.Bit.OnCh1 = OFF;			// Desactivo salida 1
	Rele.Bit.OnCh2 = OFF;			// Desactivo salida 2.
	Rele.Bit.OnCh3 = O_N;			// Activo salida 3.

	Control_TPIC();
	Estado_Lift = O_N;
	Config_Timer5();		// Configura timer de electrolifting (interrupción).
}

/********************************************************************
 * Function:	Apaga_Lift()										*
 * Definition:	Rutina para parar lifting.							*
 ********************************************************************/
void Apaga_Lift(void)
{
	CloseTimer5();				// Paro timer asociado.
	PULSE_N = 0;
	PULSE_P = 0;

	Carga_TLC5620(REFDACB,2);
	Carga_TLC5620(DACB,2);		// Pongo a cero nivel salida.

	Rele.Bit.OnCh1 = OFF;		// Desactivo salida 1.
	Rele.Bit.OnCh2 = OFF;		// Desactivo salida 2.
	Rele.Bit.OnCh3 = OFF;		// Desactivo salida 3.

	Control_TPIC();
	Estado_Lift = OFF;

	Valor_Lift = 0;
}

/********************************************************************
 * Function:	Enciende_RF(V_RF,F_RF)								*
 * Definition:	Rutina para poner en marcha la RF.					*
 ********************************************************************/
void Enciende_RF(unsigned char V_RF, unsigned char F_RF)
{
	if (No_Apli == 0)		// Si detecto aplicador...
	{
		if (((F_RF >= 10)&&(F_RF <= 30))||(F_RF == 0xBA))	// 1MHz, 3MHz o barrido
		{
			/**/
			if (Reducir == 0)
			{
				Cont_Reducir = 0;
			}
			/**/
			Carga_TLC5620(REFDACBIAS | 200,3);
			Carga_TLC5620(DACBIAS | Dac_Bias,3);	// 80

			// V_RF = 170 - (V_RF * 2.5);		 // TODO OJO

			if (Aplicador == CORPORAL)
			{
				if (F_RF == 10)
				{
					dacdds_value = array_dacdds[RF_arrays_1mhz_corporal][V_RF];
					level_value = array_level[RF_arrays_1mhz_corporal][V_RF];
				}
				if (F_RF == 30)
				{
					dacdds_value = array_dacdds[RF_arrays_3mhz_corporal][V_RF];
					level_value = array_level[RF_arrays_3mhz_corporal][V_RF];
				}
				if (F_RF == 0xBA)
				{
					dacdds_value = array_dacdds[RF_arrays_ba_corporal][V_RF];
					level_value = array_level[RF_arrays_ba_corporal][V_RF];
				}
				// if (V_RF == 0)
				// {
				// 	Nop();
				// }
				// else
				// {
				// 	V_RF = AuxDac_RF - V_RF;//230 - V_RF;
				// }
			}
			else if (Aplicador == FACIAL)
			{
				if (F_RF == 10)
				{
					dacdds_value = array_dacdds[RF_arrays_1mhz_facial][V_RF];
					level_value = array_level[RF_arrays_1mhz_facial][V_RF];
				}
				if (F_RF == 30)
				{
					dacdds_value = array_dacdds[RF_arrays_3mhz_facial][V_RF];
					level_value = array_level[RF_arrays_3mhz_facial][V_RF];
				}
				if (F_RF == 0xBA)
				{
					dacdds_value = array_dacdds[RF_arrays_ba_facial][V_RF];
					level_value = array_level[RF_arrays_ba_facial][V_RF];
				}
				// if (V_RF == 0)
				// {
				// 	Nop();
				// }
				// else
				// {
				// 	V_RF = 148 - V_RF;
				// }
			}
			else if (Aplicador == ESPE)
			{
				if (F_RF == 10)
				{
					dacdds_value = array_dacdds[RF_arrays_1mhz_especific][V_RF];
					level_value = array_level[RF_arrays_1mhz_especific][V_RF];
				}
				if (F_RF == 30)
				{
					dacdds_value = array_dacdds[RF_arrays_3mhz_especific][V_RF];
					level_value = array_level[RF_arrays_3mhz_especific][V_RF];
				}
				if (F_RF == 0xBA)
				{
					dacdds_value = array_dacdds[RF_arrays_ba_especific][V_RF];
					level_value = array_level[RF_arrays_ba_especific][V_RF];
				}
			}

			Carga_TLC5620(REFDACDDS | REFDACDDS_VALUE, 3);	// 1 MHz
			Carga_TLC5620(DACDDS | dacdds_value, 3);
			Carga_TLC5620(LEVEL | level_value, 1);


			/*
			if (Aplicador == CORPORAL)
			{
				Carga_TLC5620(REFDACDDS | 114,3);
				if (V_RF == 0)
				{
					Nop();
				}
				else
				{
					V_RF = 230 - V_RF;
				}
			}
			else
			{
				Carga_TLC5620(REFDACDDS | 195,3);
				if (V_RF == 0)
				{
					Nop();
				}
				else
				{
					V_RF = 148 - V_RF;
				}
			}
			*/
			/**/
			Offset_RF = 170;	// todo mirar bien
			/**/

			// Carga_TLC5620(DACDDS | V_RF,3);
			// Carga_TLC5620(DACDDS | dacdds_value,3);
			// Carga_TLC5620(LEVEL | level_value,1);


			if (F_RF == 0xBA)		// Hacer barrido. Empiezo siempre por 1 Mhz.
			{
				if (T4CONbits.TON == 0)			// Activo barrido sólo si no lo estaba haciendo de antes.
				{
					Config_Timer4();			// Enciendo timer barrido RF.
					Set_Freq_AD9834(1000000);	// Pongo frecuencia.
				}
			}
			else
			{
				Stop_Timer4();
				Frecuencia_Resulta = F_RF*100000;
				Set_Freq_AD9834(Frecuencia_Resulta);	// Pongo frecuencia.
			}
			Estado_RF = O_N;		// Indico que la RF está en marcha.
			Apagada_RF = 0;
			HS_FAN = 1;				// Enciendo ventilador radiador.
		}
	}
	if (V_RF == 0)
	{
		Apaga_RF();	// Si el valor de rf es 0, apaga.
	}
}

/********************************************************************
 * Function:	Apaga_RF()											*
 * Definition:	Rutina para parar RF.								*
 ********************************************************************/
void Apaga_RF(void)
{
	Stop_Timer4();
	Carga_TLC5620(REFDACBIAS,3);
	Carga_TLC5620(DACBIAS,3);		// Pone a 0 el nivel.
	Carga_TLC5620(REFDACDDS,3);
	Carga_TLC5620(DACDDS,3);		// Pone a 0 el nivel.
	Suspend_AD9834();				// Pone a 0 el nivel del DDS.

	//control leds y reles
	Estado_RF = OFF;		// Indico que la RF está apagada.
	Valor_RF = 0;
	Lect_RF = 0;
}

/****************************** EOF *********************************/
