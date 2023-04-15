  
#ifndef SETTINGS_H
#define	SETTINGS_H

#include <xc.h> // include processor files - each processor file is guarded.  

int * getStoredAlarmPeriod();
void setAlarmPeriod(int startTime, int endtime);

#endif	/* SETTINGS_H */

