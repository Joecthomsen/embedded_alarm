/*
 * File:   deviceConfiguration.c
 * Author: Joect
 *
 * Created on April 12, 2023, 7:49 PM
 */


#include <xc.h>
#include <stdbool.h>
#include "definitions.h"
#include "mcc_generated_files/memory/flash.h"

bool deviceRegistered(){
    if(FLASH_ReadWord16(DEVICE_ID_ADDRESS) == 0xFFFF){
        return 0;
    }
    return 1;
}

