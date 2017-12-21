/********************************************************************
 * FileName:		Tratamientos.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia Projet 64 S.L.							*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librería para Tratamientos.c.									*
 *																	*
 * Change History:			Versión 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		08/09/16		Versión inicial.				*
 *																	*
 ********************************************************************/

/* will initialize with 255 because it is the most secure, will not auto-destruct */
#define MARGIN_PERCENTAGE 0.99 /* 1% expressed ready for using with multiplication and division operation */
#define DACDDS_MAX (255-42)
#define DACDDS_MIN (0+42)
#define LEVEL_MAX 255
#define LEVEL_MIN (255-70)
#define LEVEL_STARTING_VALUE (255-20)
#define REFDACDDS_VALUE 127
#define DACDDS_HALF_VALUE 127


#define VOLTAGE_TO_WAIT_ANRF 20.0f
#define NUMBER_OF_CORRECT_VALUES_BEFORE_SAVING 5
#define MOVING_AVERAGE_NUM_OF_SAMPLES 8 /* size of data array */

#define ARRAY_REFDACDDS 255 /* 0% */, REFDACDDS_VALUE /* 5% */, REFDACDDS_VALUE /* 10% */, REFDACDDS_VALUE /* 15% */, REFDACDDS_VALUE /* 20% */, REFDACDDS_VALUE /* 25% */, REFDACDDS_VALUE /* 30% */, REFDACDDS_VALUE /* 35% */, REFDACDDS_VALUE /* 40% */, REFDACDDS_VALUE /* 45% */, REFDACDDS_VALUE /* 50% */, REFDACDDS_VALUE /* 55% */, REFDACDDS_VALUE /* 60% */, REFDACDDS_VALUE /* 65% */, REFDACDDS_VALUE /* 70% */, REFDACDDS_VALUE /* 75% */, REFDACDDS_VALUE /* 80% */, REFDACDDS_VALUE /* 85% */, REFDACDDS_VALUE /* 90% */, REFDACDDS_VALUE /* 95% */, REFDACDDS_VALUE /* 100% */
#define ARRAY_DACDDS 255 /* 0% */, DACDDS_HALF_VALUE /* 5% */, DACDDS_HALF_VALUE /* 10% */, DACDDS_HALF_VALUE /* 15% */, DACDDS_HALF_VALUE /* 20% */, DACDDS_HALF_VALUE /* 25% */, DACDDS_HALF_VALUE /* 30% */, DACDDS_HALF_VALUE /* 35% */, DACDDS_HALF_VALUE /* 40% */, DACDDS_HALF_VALUE /* 45% */, DACDDS_HALF_VALUE /* 50% */, DACDDS_HALF_VALUE /* 55% */, DACDDS_HALF_VALUE /* 60% */, DACDDS_HALF_VALUE /* 65% */, DACDDS_HALF_VALUE /* 70% */, DACDDS_HALF_VALUE /* 75% */, DACDDS_HALF_VALUE /* 80% */, DACDDS_HALF_VALUE /* 85% */, DACDDS_HALF_VALUE /* 90% */, DACDDS_HALF_VALUE /* 95% */, DACDDS_HALF_VALUE /* 100% */

#define ARRAY_LEVEL 255 /* 0% */, 255 /* 5% */, 255 /* 10% */, 255 /* 15% */, 255 /* 20% */, 255 /* 25% */, 255 /* 30% */, 255 /* 35% */, 255 /* 40% */, 255 /* 45% */, 255 /* 50% */, 255 /* 55% */, 255 /* 60% */, 255 /* 65% */, 255 /* 70% */, 255 /* 75% */, 255 /* 80% */, 255 /* 85% */, 255 /* 90% */, 255 /* 95% */, 255 /* 100% */
#define ARRAY_ANRF_1MHZ_FACIAL 0.0f /* 0% */, 43.3f /* 5% */, 47.4f /* 10% */, 51.6f /* 15% */, 55.7f /* 20% */, 59.8f /* 25% */, 63.2f /* 30% */, 66.6f /* 35% */, 70.0f /* 40% */, 73.4f /* 45% */, 76.8f /* 50% */, 78.0f /* 55% */, 79.2f /* 60% */, 80.5f /* 65% */, 81.7f /* 70% */, 82.9f /* 75% */, 83.3f /* 80% */, 83.7f /* 85% */, 84.1f /* 90% */, 84.5f /* 95% */, 84.9f/* 100% */
#define ARRAY_ANRF_3MHZ_FACIAL 0.0f /* 0% */, 24.7f /* 5% */, 27.5f /* 10% */, 30.3f /* 15% */, 33.0f /* 20% */, 35.8f /* 25% */, 39.4f /* 30% */, 43.0f /* 35% */, 46.6f /* 40% */, 50.2f /* 45% */, 53.8f /* 50% */, 56.7f /* 55% */, 59.6f /* 60% */, 62.6f /* 65% */, 65.5f /* 70% */, 68.4f /* 75% */, 70.9f /* 80% */, 73.5f /* 85% */, 76.0f /* 90% */, 78.6f /* 95% */, 81.1f/* 100% */
#define ARRAY_ANRF_1MHZ_CORPORAL 0.0f /* 0% */, 74.4f /* 5% */, 75.8f /* 10% */, 77.1f /* 15% */, 78.5f /* 20% */, 79.8f /* 25% */, 80.2f /* 30% */, 80.6f /* 35% */, 81.0f /* 40% */, 81.4f /* 45% */, 81.8f /* 50% */, 82.1f /* 55% */, 82.4f /* 60% */, 82.8f /* 65% */, 83.1f /* 70% */, 83.4f /* 75% */, 83.8f /* 80% */, 84.1f /* 85% */, 84.5f /* 90% */, 84.8f /* 95% */, 85.2f/* 100% */
#define ARRAY_ANRF_3MHZ_CORPORAL 0.0f /* 0% */, 41.8f /* 5% */, 44.7f /* 10% */, 47.6f /* 15% */, 50.5f /* 20% */, 53.4f /* 25% */, 55.3f /* 30% */, 57.2f /* 35% */, 59.0f /* 40% */, 60.9f /* 45% */, 62.8f /* 50% */, 64.1f /* 55% */, 65.4f /* 60% */, 66.8f /* 65% */, 68.1f /* 70% */, 69.4f /* 75% */, 70.6f /* 80% */, 71.8f /* 85% */, 72.9f /* 90% */, 74.1f /* 95% */, 75.3f /* 100% */

typedef enum
{
	RF_arrays_1mhz_facial = 0, // 1 MHz facial
	RF_arrays_1mhz_corporal = 1, // 1 MHz corporal
	RF_arrays_3mhz_facial = 2, // 3 MHz facial
	RF_arrays_3mhz_corporal = 3, // 3 MHz corporal
	RF_arrays_ba_facial = 4, // barrido facial
	RF_arrays_ba_corporal = 5 // barrido corporal
} RF_arrays_indexes_t;


/********************* Declaración de funciones *********************/
unsigned char Analizar_Trama(void);							// Rutina para analizar trama recibida.
void Procesar_Ordenes(unsigned char Ordre);					// Rutina para procesar órden de INTAB.
void Enciende_Stim(unsigned char V_Stim);					// Rutina para poner en marcha la estimulación.
void Apaga_Stim(void);										// Rutina para apagar estimulación.
void Enciende_Galva(unsigned char V_Galva);					// Rutina para poner en marcha la galvánica.
void Apaga_Galva(void);										// Rutina para parar galvánica.
void Enciende_Lift(unsigned char V_Lift);					// Rutina para poner en marcha el lifting.
void Apaga_Lift(void);										// Rutina para apagar el lifting
void Enciende_RF(unsigned char V_RF, unsigned char F_RF);	// Rutina para poner en marcha la RF.
void Apaga_RF(void);										// Rutina para apagar la RF.
void Responder(unsigned char Orden_Emb);					// Contesta las tramas de la INTAB.
void calibration_process(unsigned char input);				// Rutina para procesar órdenes en calibración de RF.
float read_voltage_anrf_1mhz(unsigned int adc_value);		// Rutina para leer la tensión RMS que hay en ANRF.
float read_voltage_anrf_3mhz(unsigned int adc_value);		// Rutina para leer la tensión RMS que hay en ANRF.
unsigned char is_voltage_correct(float voltage, unsigned char index, unsigned char handle, unsigned char frequency);
void voltage_moving_average(float new_input_value, float * moving_average);

/****************************** EOF *********************************/
