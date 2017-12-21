/********************************************************************
 * FileName:		Actualizacion.h									*
 * Dependencies:													*
 * Processor:		PIC32MX564F128H									*
 * Company:			Ingenia											*
 *																	*
 ********************************************************************
 * File Description:												*
 *																	*
 * 	- Librer�a para Actualizacion.c.								*
 *																	*
 * Change History:			Versi�n 0.0								*
 *																	*
 * Name				Date            Changes							*
 * -----			-----			--------						*
 * J. Tejedor		21/07/16		Versi�n inicial.				*
 *																	*
 ********************************************************************/

#define REC_FLASHED 				0
#define REC_NOT_FOUND 				1
#define REC_FOUND_BUT_NOT_FLASHED 	2
#define DATA_RECORD 				0
#define END_OF_FILE_RECORD 			1
#define EXT_SEG_ADRS_RECORD 		2
#define EXT_LIN_ADRS_RECORD 		4
#define DEV_CONFIG_REG_BASE_ADDRESS	0x9FC02FF0
#define DEV_CONFIG_REG_END_ADDRESS	0x9FC02FFF
#define FLASH_PAGE_SIZE 			0x1000	

#define APP_FLASH_BASE_ADDRESS		0x9D006000//0x9D005000
#define APP_FLASH_END_ADDRESS		0x9D01EFFF//(0x9D000000+BMXPFMSZ-1-0x1000)	// BMXPFMSZ = 0x20000. Resto 0x1000 de NVM.

/********************* Declaraci�n de funciones *********************/
void ConvertAsciiToHex(UINT8* asciiRec, UINT8* hexRec);
void WriteHexRecord2Flash(UINT8* HexRecord);
void EraseFlash(void);
void JumpToApp(void);

/****************************** EOF *********************************/