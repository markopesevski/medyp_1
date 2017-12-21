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
/* need 21 because there are 21 steps (from 0%, 5%, 10%... to 100%) */
unsigned char array_knowns_1mhz_facial[21] = {0};
unsigned char array_refdacdds_1mhz_facial[21] = {ARRAY_REFDACDDS};
unsigned char array_dacdds_1mhz_facial[21] = {ARRAY_DACDDS};
unsigned char array_level_1mhz_facial[21] = {ARRAY_LEVEL};
unsigned int array_lect_rf_1mhz_facial[21] = {0};
float array_anrf_1mhz_facial[21] = {ARRAY_ANRF_1MHZ_FACIAL};

unsigned char array_knowns_3mhz_facial[21] = {0};
unsigned char array_refdacdds_3mhz_facial[21] = {ARRAY_REFDACDDS};
unsigned char array_dacdds_3mhz_facial[21] = {ARRAY_DACDDS};
unsigned char array_level_3mhz_facial[21] = {ARRAY_LEVEL};
unsigned int array_lect_rf_3mhz_facial[21] = {0};
float array_anrf_3mhz_facial[21] = {ARRAY_ANRF_1MHZ_CORPORAL};

unsigned char array_knowns_ba_facial[21] = {0};
unsigned char array_refdacdds_ba_facial[21] = {ARRAY_REFDACDDS};
unsigned char array_dacdds_ba_facial[21] = {ARRAY_DACDDS};
unsigned char array_level_ba_facial[21] = {ARRAY_LEVEL};
unsigned int array_lect_rf_ba_facial[21] = {0};
float array_anrf_ba_facial[21] = {0.0f};

unsigned char array_knowns_1mhz_corporal[21] = {0};
unsigned char array_refdacdds_1mhz_corporal[21] = {ARRAY_REFDACDDS};
unsigned char array_dacdds_1mhz_corporal[21] = {ARRAY_DACDDS};
unsigned char array_level_1mhz_corporal[21] = {ARRAY_LEVEL};
unsigned int array_lect_rf_1mhz_corporal[21] = {0};
float array_anrf_1mhz_corporal[21] = {ARRAY_ANRF_3MHZ_FACIAL};

unsigned char array_knowns_3mhz_corporal[21] = {0};
unsigned char array_refdacdds_3mhz_corporal[21] = {ARRAY_REFDACDDS};
unsigned char array_dacdds_3mhz_corporal[21] = {ARRAY_DACDDS};
unsigned char array_level_3mhz_corporal[21] = {ARRAY_LEVEL};
unsigned int array_lect_rf_3mhz_corporal[21] = {0};
float array_anrf_3mhz_corporal[21] = {ARRAY_ANRF_3MHZ_CORPORAL};

unsigned char array_knowns_ba_corporal[21] = {0};
unsigned char array_refdacdds_ba_corporal[21] = {ARRAY_REFDACDDS};
unsigned char array_dacdds_ba_corporal[21] = {ARRAY_DACDDS};
unsigned char array_level_ba_corporal[21] = {ARRAY_LEVEL};
unsigned int array_lect_rf_ba_corporal[21] = {0};
float array_anrf_ba_corporal[21] = {0.0f};

unsigned char handle_value = 0xFF;
unsigned char freq_value = 0xFF;
unsigned char dacdds_value = 0xFF;
unsigned char refdacdds_value = 0xFF;
unsigned char level_value = 0xFF;
unsigned char index_percentage_value = 0xFF;
calibration_process_t calibration_status = CALIBRATION_NO_STATE;

extern float voltage_anrf;



















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
const char Version_Soft[] = "v1.0.6";
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
			Enciende_Galva(10);
		break;
		case SET_STIM:		//36
			Dac_Stim = ReceivedDataBuffer[2];
			Cont_Test = 0;
			Estoy_Test = STIMULATION_TEST;
			Enciende_Stim(10);
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

/********************************************************************
 * Function:	calibration_process(input)							*
 * Definition:	Rutina para tratar órdenes en modo calibración		*
 ********************************************************************/
float voltage = 0.0f;
void calibration_process(unsigned char input)
{
	switch((calibration_process_t)input)
	{
		case CALIBRATION_SAVE_SERIAL:
			if(((ReceivedDataBuffer[6] - 0x30) == FACIAL) || ((ReceivedDataBuffer[6] - 0x30) == ESPE))
			{
				handle_value = FACIAL;
			}
			else if (((ReceivedDataBuffer[6] - 0x30) == CORPORAL))
			{
				handle_value = CORPORAL;
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
			// handle_value = FACIAL;
			// freq_value = RF_freq_1mhz;
		break;
		case CALIBRATION_SAVE_VALUES:
		case CALIBRATION_READ_TEMP:
			/* TODO should be a more or less automatic process with which the machine adjusts itself linearly between a max and a min */

			if (index_percentage_value < RF_calibrate || (input == CALIBRATION_SAVE_VALUES && calibration_status == CALIBRATION_FOUND_VALUE))
			{
				/* SAVE VALUE TO TABLE for accessing when starting RF */
				if(handle_value == CORPORAL)
				{
					if(freq_value == 10)
					{
						array_dacdds_1mhz_corporal[index_percentage_value] = dacdds_value;
						array_refdacdds_1mhz_corporal[index_percentage_value] = refdacdds_value;
						array_level_1mhz_corporal[index_percentage_value] = level_value;
						array_lect_rf_1mhz_corporal[index_percentage_value] = Lectura_RF();
						// array_anrf_1mhz_corporal[index_percentage_value] = read_voltage_anrf_1mhz(array_lect_rf_1mhz_corporal[index_percentage_value]);
						array_knowns_1mhz_corporal[index_percentage_value] = 1;
					}
					else if(freq_value == 30)
					{
						array_dacdds_3mhz_corporal[index_percentage_value] = dacdds_value;
						array_refdacdds_3mhz_corporal[index_percentage_value] = refdacdds_value;
						array_level_3mhz_corporal[index_percentage_value] = level_value;
						array_lect_rf_3mhz_corporal[index_percentage_value] = Lectura_RF();
						// array_anrf_3mhz_corporal[index_percentage_value] = read_voltage_anrf_3mhz(array_lect_rf_3mhz_corporal[index_percentage_value]);
						array_knowns_3mhz_corporal[index_percentage_value] = 1;
					}
					else if (freq_value == 0xBA)
					{
						array_dacdds_ba_corporal[index_percentage_value] = dacdds_value;
						array_refdacdds_ba_corporal[index_percentage_value] = refdacdds_value;
						array_level_ba_corporal[index_percentage_value] = level_value;
						array_lect_rf_ba_corporal[index_percentage_value] = Lectura_RF();
						// array_anrf_ba_corporal[index_percentage_value] = read_voltage_anrf(array_lect_rf_ba_corporal[index_percentage_value]);
						array_knowns_ba_corporal[index_percentage_value] = 1;
					}
				}
				else if (handle_value == ESPE || handle_value == FACIAL)
				{
					if(freq_value == 10)
					{
						array_dacdds_1mhz_facial[index_percentage_value] = dacdds_value;
						array_refdacdds_1mhz_facial[index_percentage_value] = refdacdds_value;
						array_level_1mhz_facial[index_percentage_value] = level_value;
						array_lect_rf_1mhz_facial[index_percentage_value] = Lectura_RF();
						// array_anrf_1mhz_facial[index_percentage_value] = read_voltage_anrf_1mhz(array_lect_rf_1mhz_facial[index_percentage_value]);
						array_knowns_1mhz_facial[index_percentage_value] = 1;
					}
					else if(freq_value == 30)
					{
						array_dacdds_3mhz_facial[index_percentage_value] = dacdds_value;
						array_refdacdds_3mhz_facial[index_percentage_value] = refdacdds_value;
						array_level_3mhz_facial[index_percentage_value] = level_value;
						array_lect_rf_3mhz_facial[index_percentage_value] = Lectura_RF();
						// array_anrf_3mhz_facial[index_percentage_value] = read_voltage_anrf_3mhz(array_lect_rf_3mhz_facial[index_percentage_value]);
						array_knowns_3mhz_facial[index_percentage_value] = 1;
					}
					else if (freq_value == 0xBA)
					{
						array_dacdds_ba_facial[index_percentage_value] = dacdds_value;
						array_refdacdds_ba_facial[index_percentage_value] = refdacdds_value;
						array_level_ba_facial[index_percentage_value] = level_value;
						array_lect_rf_ba_facial[index_percentage_value] = Lectura_RF();
						// array_anrf_ba_facial[index_percentage_value] = read_voltage_anrf(array_lect_rf_ba_facial[index_percentage_value]);
						array_knowns_ba_facial[index_percentage_value] = 1;
					}
				}
			}
			else if (index_percentage_value == RF_calibrate)
			{
				/* SEARCH FOR DACDDS AND LEVEL VALUES KNOWING DESIRED VOLTAGES */
				index_percentage_value = RF_value_5;
				calibration_status = CALIBRATION_START_SEARCH;
				// if(handle_value == CORPORAL)
				// {
				// 	if(freq_value == 10)
				// 	{
				// 		/* only when you already know values 1 and 20 */
				// 		if((array_knowns_1mhz_corporal[1] & array_knowns_1mhz_corporal[20]) == 1)
				// 		{
				// 			index_percentage_value = 2;
				// 			calibration_status = CALIBRATION_START_SEARCH;
				// 		}
				// 	}
				// 	else if(freq_value == 30)
				// 	{
				// 		/* only when you already know values 1 and 20 */
				// 		if((array_knowns_3mhz_corporal[1] & array_knowns_3mhz_corporal[20]) == 1)
				// 		{
				// 			index_percentage_value = 2;
				// 			calibration_status = CALIBRATION_START_SEARCH;
				// 		}
				// 	}
				// }
				// else if(handle_value == ESPE || handle_value == FACIAL)
				// {
				// 	if(freq_value == 10)
				// 	{
				// 		/* only when you already know values 1 and 20 */
				// 		if((array_knowns_1mhz_facial[1] & array_knowns_1mhz_facial[20]) == 1)
				// 		{
				// 			index_percentage_value = 2;
				// 			calibration_status = CALIBRATION_START_SEARCH;
				// 		}
				// 	}
				// 	else if(freq_value == 30)
				// 	{
				// 		/* only when you already know values 1 and 20 */
				// 		if((array_knowns_3mhz_facial[1] & array_knowns_3mhz_facial[20]) == 1)
				// 		{
				// 			index_percentage_value = 2;
				// 			calibration_status = CALIBRATION_START_SEARCH;
				// 		}
				// 	}
				// }
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
			level_value = LEVEL_MAX;
			calibration_status = CALIBRATION_SEARCHING_VALUE;
		break;
		case CALIBRATION_FOUND_VALUE:
			calibration_process(CALIBRATION_SAVE_VALUES);
			/* wrap around 20 to not overflow any array */
			index_percentage_value++;
			if(index_percentage_value == RF_calibrate)
			{
				calibration_status = CALIBRATION_ALL_VALUES_FOUND_FOR_SERIES;
				index_percentage_value = RF_calibrate; /* going to leave the value at RF_calibrate because this value is what operator should be seeing on screen */
			}
			else
			{
				refdacdds_value = REFDACDDS_VALUE;
				dacdds_value = DACDDS_MAX;
				level_value = LEVEL_MAX;
				calibration_status = CALIBRATION_WAITING_ANRF_FALL_DOWN;
			}
		break;
		case CALIBRATION_WAITING_ANRF_FALL_DOWN:
			if(voltage_anrf < 20.0f)
			{
				calibration_status = CALIBRATION_START_SEARCH;
			}
		break;
		case CALIBRATION_SEARCHING_VALUE:
			dacdds_value++;
			if(dacdds_value >= DACDDS_MAX)
			{
				dacdds_value = DACDDS_MIN;
				level_value--;
				if(level_value <= LEVEL_MIN)
				{
					level_value = LEVEL_MAX;
					calibration_status = CALIBRATION_VALUE_NOT_REACHABLE;
				}
			}
		break;
		case CALIBRATION_VALUE_NOT_REACHABLE:
			/* TODO: what happens in this case? just a breakpoint for now */
			Nop();
		break;
		case CALIBRATION_ALL_VALUES_FOUND_FOR_SERIES:
			/* TODO: shut down RF to note operator you have found all values */
			calibration_status = CALIBRATION_STOP_RF;
		break;
		case CALIBRATION_STOP_RF:
			Apaga_RF();
		break;
		case CALIBRATION_READ_SERIAL:
		case CALIBRATION_READ_GAL:
		case CALIBRATION_READ_STIM:
		case CALIBRATION_READ_RF:
		case CALIBRATION_READ_BIAS:
		case CALIBRATION_READ_ALARM_TEMP:
		/* should go with all other cases and respond the actual value being calibrated */
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
	return (0.1042f * adc_value + 12.422f)*1.0f;
}
float read_voltage_anrf_3mhz(unsigned int adc_value)
{
	return (0.1237f * adc_value + 13.133f)*1.0f;
}

/********************************************************************
 * Function:	is_voltage_correct(voltage, index, handle, frequency)*
 * Definition:	Rutina para evaluar si una tension esta dentro del	*
 				margen para un indice dado							*
 ********************************************************************/
unsigned char is_voltage_correct(float voltage, unsigned char index, unsigned char handle, unsigned char frequency)
{
	float min = 0.0f;
	float max = 0.0f;
	calibration_values_t ret_value = CALIBRATION_VALUE_UNDER;

	if(handle == FACIAL || handle == ESPE)
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
					// Carga_TLC5620(REFDACDDS | Ajust_RefDacDDS,3);	// 1 MHz
					Carga_TLC5620(REFDACDDS | refdacdds_value,3);	// 1 MHz
				}
				if (F_RF == 30)
				{
					// Carga_TLC5620(REFDACDDS | Ajust_RefDacDDS,3);	// 3 MHz
					Carga_TLC5620(REFDACDDS | refdacdds_value,3);	// 3 MHz
				}
				if (F_RF == 0xBA)
				{
					// Carga_TLC5620(REFDACDDS | 119,3);	// Barrido
					Carga_TLC5620(REFDACDDS | refdacdds_value,3);	// Barrido
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
			else
			{
				if (F_RF == 10)
				{
					// Carga_TLC5620(REFDACDDS | 195,3);	// 1 MHz
					// Carga_TLC5620(REFDACDDS | refdacdds_value,3);	// 1 MHz
					Carga_TLC5620(REFDACDDS | array_refdacdds_1mhz_facial[V_RF],3);	// 1 MHz
					Carga_TLC5620(DACDDS | array_dacdds_1mhz_facial[V_RF],3);
					Carga_TLC5620(LEVEL | array_level_1mhz_facial[V_RF],1);
				}
				if (F_RF == 30)
				{
					// Carga_TLC5620(REFDACDDS | 195,3);	// 3 MHz
					// Carga_TLC5620(REFDACDDS | refdacdds_value,3);	// 3 MHz
				}
				if (F_RF == 0xBA)
				{
					// Carga_TLC5620(REFDACDDS | 195,3);	// Barrido
					Carga_TLC5620(REFDACDDS | refdacdds_value,3);	// Barrido
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
