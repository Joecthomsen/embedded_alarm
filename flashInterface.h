#ifndef FLASH_INTERFACE_H
#define	FLASH_INTERFACE_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "definitions.h"

bool deviceHasId();
uint16_t getUserId();
uint16_t getStartTime();
uint16_t getEndTime();
void setAlarmPeriod(uint16_t startTime, uint16_t endTime);
#endif	/* XC_HEADER_TEMPLATE_H */

