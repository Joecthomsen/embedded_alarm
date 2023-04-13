

#ifndef FLASH_H
#define FLASH_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS 512U
#define FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS 64U

#define FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS (FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS * 2U)
#define FLASH_WRITE_ROW_SIZE_IN_PC_UNITS (FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS * 2U)
 
#define FLASH_UNLOCK_KEY 0x00AA0055
#define FLASH_ERASE_PAGE_MASK (~((FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS*2UL) - 1U)) 

void     FLASH_Unlock(uint32_t  key);
void     FLASH_Lock(void);

/* FLASH_ErasePage(uint32_t adddress ):  
 *                     Address must be page aligned.  
 *                     Use the FLASH_GetErasePageAddress() function
 *                     to get the page for a specific memory address. */
bool     FLASH_ErasePage(uint32_t address);

/* The address in the Read and Write word functions below must be even */
uint32_t FLASH_ReadWord24(uint32_t address);
uint16_t FLASH_ReadWord16(uint32_t address);

bool     FLASH_WriteWord24(uint32_t address, uint32_t Data);
bool     FLASH_WriteWord16(uint32_t address, uint16_t Data);

/* Program the flash one row at a time. */

/* FLASH_WriteRow24: Writes a single row of data from the location given in *data to
 *                   the flash location in address.  Since the flash is only 24 bits wide
 *                   all data in the upper 8 bits of the source will be lost .  
 *                   The address in *data must be row aligned.
 *                   returns true if successful */
bool     FLASH_WriteRow24(uint32_t address, uint32_t *data);


/* FLASH_WriteRow16: Writes a single row of data from the location in given in *data to
 *                   to the flash location in address. Each 16 bit source data 
 *                   word is stored in the lower 16 bits of each flash entry and the 
 *                   upper 8 bits of the flash is not programmed. 
 *                   The address in *data must be row aligned.
 *                   returns true if successful */
bool     FLASH_WriteRow16(uint32_t address, uint16_t *data);

uint16_t FLASH_GetErasePageOffset(uint32_t address);
uint32_t FLASH_GetErasePageAddress(uint32_t address);

#endif	/* FLASH_H */

