
#ifndef SWITCH1_H
#define SWITCH1_H

#include <stdbool.h>
#include <xc.h>

static inline bool SWITCH1_IsPressed() {return (_RB14 == true);}

#endif	/* SWITCH1_H */