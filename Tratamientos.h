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
///* Original */#define MARGIN_PERCENTAGE 0.99 /* 1% expressed ready for using with multiplication and division operation */
#define MARGIN_PERCENTAGE 0.98 /* 2% expressed ready for using with multiplication and division operation */
#define DACDDS_MAX (255-42)
#define DACDDS_MIN (0+42)
#define LEVEL_MAX 255
#define LEVEL_MIN (255-85)
#define LEVEL_STARTING_VALUE (255-20)
#define REFDACDDS_VALUE 127
#define DACDDS_HALF_VALUE 127

//#define SYS_FREQ 80000000UL
//#define T1_TICK 6250
#define T1_PRESCALER 64
#define TIMER1_MS ((1000/((SYS_FREQ/T1_PRESCALER)/T1_TICK)))
#define CALIBRATION_ACTION_MS 50
#define CALIBRATION_ACTION_TICKS (CALIBRATION_ACTION_MS/TIMER1_MS)
#define WARMUP_TIME_MS 120*1000
#define WARMUP_TIME_TICKS (WARMUP_TIME_MS/CALIBRATION_ACTION_MS)

#define LEVEL_DRIFT_CORRECTION_MAX 15
#define DACDDS_DRIFT_CORRECTION_MAX 40

#define VOLTAGE_TO_WAIT_ANRF 20.0f
#define NUMBER_OF_CORRECT_VALUES_BEFORE_SAVING 5
#define MOVING_AVERAGE_NUM_OF_SAMPLES 8 /* size of data array */

#define ARRAY_REFDACDDS 255 /* 0% */, REFDACDDS_VALUE /* 5% */, REFDACDDS_VALUE /* 10% */, REFDACDDS_VALUE /* 15% */, REFDACDDS_VALUE /* 20% */, REFDACDDS_VALUE /* 25% */, REFDACDDS_VALUE /* 30% */, REFDACDDS_VALUE /* 35% */, REFDACDDS_VALUE /* 40% */, REFDACDDS_VALUE /* 45% */, REFDACDDS_VALUE /* 50% */, REFDACDDS_VALUE /* 55% */, REFDACDDS_VALUE /* 60% */, REFDACDDS_VALUE /* 65% */, REFDACDDS_VALUE /* 70% */, REFDACDDS_VALUE /* 75% */, REFDACDDS_VALUE /* 80% */, REFDACDDS_VALUE /* 85% */, REFDACDDS_VALUE /* 90% */, REFDACDDS_VALUE /* 95% */, REFDACDDS_VALUE /* 100% */
#define ARRAY_DACDDS 255 /* 0% */, DACDDS_HALF_VALUE /* 5% */, DACDDS_HALF_VALUE /* 10% */, DACDDS_HALF_VALUE /* 15% */, DACDDS_HALF_VALUE /* 20% */, DACDDS_HALF_VALUE /* 25% */, DACDDS_HALF_VALUE /* 30% */, DACDDS_HALF_VALUE /* 35% */, DACDDS_HALF_VALUE /* 40% */, DACDDS_HALF_VALUE /* 45% */, DACDDS_HALF_VALUE /* 50% */, DACDDS_HALF_VALUE /* 55% */, DACDDS_HALF_VALUE /* 60% */, DACDDS_HALF_VALUE /* 65% */, DACDDS_HALF_VALUE /* 70% */, DACDDS_HALF_VALUE /* 75% */, DACDDS_HALF_VALUE /* 80% */, DACDDS_HALF_VALUE /* 85% */, DACDDS_HALF_VALUE /* 90% */, DACDDS_HALF_VALUE /* 95% */, DACDDS_HALF_VALUE /* 100% */

#define ARRAY_LEVEL 255 /* 0% */, 255 /* 5% */, 255 /* 10% */, 255 /* 15% */, 255 /* 20% */, 255 /* 25% */, 255 /* 30% */, 255 /* 35% */, 255 /* 40% */, 255 /* 45% */, 255 /* 50% */, 255 /* 55% */, 255 /* 60% */, 255 /* 65% */, 255 /* 70% */, 255 /* 75% */, 255 /* 80% */, 255 /* 85% */, 255 /* 90% */, 255 /* 95% */, 255 /* 100% */
#define ARRAY_ANRF_1MHZ_FACIAL 0.0f /* 0% */, 43.3f /* 5% */, 47.7f /* 10% */, 52.1f /* 15% */, 56.5f /* 20% */, 60.9f /* 25% */, 64.3f /* 30% */, 67.7f /* 35% */, 71.1f /* 40% */, 74.5f /* 45% */, 77.9f /* 50% */, 79.1f /* 55% */, 80.3f /* 60% */, 81.5f /* 65% */, 82.7f /* 70% */, 83.9f /* 75% */, 84.2f /* 80% */, 84.4f /* 85% */, 84.7f /* 90% */, 85.0f /* 95% */, 85.3f /* 100% */
#define ARRAY_ANRF_3MHZ_FACIAL 0.0f /* 0% */, 24.7f /* 5% */, 28.4f /* 10% */, 32.1f /* 15% */, 35.8f /* 20% */, 39.5f /* 25% */, 42.4f /* 30% */, 45.3f /* 35% */, 48.3f /* 40% */, 51.2f /* 45% */, 54.1f /* 50% */, 56.7f /* 55% */, 59.3f /* 60% */, 62.0f /* 65% */, 64.6f /* 70% */, 67.2f /* 75% */, 69.5f /* 80% */, 71.9f /* 85% */, 74.2f /* 90% */, 76.5f /* 95% */, 78.9f /* 100% */
#define ARRAY_ANRF_1MHZ_CORPORAL 0.0f /* 0% */, 74.4f /* 5% */, 74.7f /* 10% */, 75.1f /* 15% */, 75.4f /* 20% */, 75.8f /* 25% */, 77.3f /* 30% */, 78.8f /* 35% */, 80.3f /* 40% */, 81.8f /* 45% */, 83.3f /* 50% */, 83.5f /* 55% */, 83.8f /* 60% */, 84.0f /* 65% */, 84.2f /* 70% */, 84.5f /* 75% */, 84.9f /* 80% */, 85.4f /* 85% */, 85.8f /* 90% */, 86.3f /* 95% */, 86.8f /* 100% */
#define ARRAY_ANRF_3MHZ_CORPORAL 0.0f /* 0% */, 41.8f /* 5% */, 45.6f /* 10% */, 49.5f /* 15% */, 53.3f /* 20% */, 57.1f /* 25% */, 58.3f /* 30% */, 59.4f /* 35% */, 60.6f /* 40% */, 61.7f /* 45% */, 62.9f /* 50% */, 64.3f /* 55% */, 65.7f /* 60% */, 67.1f /* 65% */, 68.5f /* 70% */, 69.9f /* 75% */, 71.3f /* 80% */, 72.7f /* 85% */, 74.1f /* 90% */, 75.5f /* 95% */, 76.9f /* 100% */
#define ARRAY_ANRF_1MHZ_ESPECIFIC 0.0f /* 0% */, 43.3f /* 5% */, 47.7f /* 10% */, 52.1f /* 15% */, 56.5f /* 20% */, 60.9f /* 25% */, 62.0f /* 30% */, 63.2f /* 35% */, 64.3f /* 40% */, 65.5f /* 45% */, 66.6f /* 50% */, 67.7f /* 55% */, 68.9f /* 60% */, 70.0f /* 65% */, 71.2f /* 70% */, 72.3f /* 75% */, 73.4f /* 80% */, 74.6f /* 85% */, 75.7f /* 90% */, 76.9f /* 95% */, 78.0f /* 100% */
#define ARRAY_ANRF_3MHZ_ESPECIFIC 0.0f /* 0% */, 25.0f /* 5% */, 28.6f /* 10% */, 32.2f /* 15% */, 35.8f /* 20% */, 39.4f /* 25% */, 40.4f /* 30% */, 41.5f /* 35% */, 42.5f /* 40% */, 43.6f /* 45% */, 44.6f /* 50% */, 45.6f /* 55% */, 46.7f /* 60% */, 47.7f /* 65% */, 48.8f /* 70% */, 49.8f /* 75% */, 50.8f /* 80% */, 51.9f /* 85% */, 52.9f /* 90% */, 54.0f /* 95% */, 55.0f /* 100% */

typedef enum
{
	RF_arrays_1mhz_facial = 0, // 1 MHz facial
	RF_arrays_1mhz_corporal = 1, // 1 MHz corporal
	RF_arrays_1mhz_especific = 2, // 1 MHz especific
	RF_arrays_3mhz_facial = 3, // 3 MHz facial
	RF_arrays_3mhz_corporal = 4, // 3 MHz corporal
	RF_arrays_3mhz_especific = 5, // 3 MHz especific
	RF_arrays_ba_facial = 6, // barrido facial
	RF_arrays_ba_corporal = 7, // barrido corporal
	RF_arrays_ba_especific = 8, // barrido especific
	RF_arrays_max
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
unsigned char is_voltage_correct(float voltage, unsigned char index, unsigned char handle, unsigned char frequency, float * voltage_read);
void voltage_moving_average(float new_input_value, float * moving_average);

/****************************** EOF *********************************/
