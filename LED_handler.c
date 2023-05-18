
#include "LED_handler.h"

void turnOnRedLED(){
    _LATB11 = 1;
}
void turnOnYellowLED(){
    _LATB12 = 1;
}
void turnOnGreenLED(){
    _LATB13 = 1;
}
void turnOnBlueLED(){
    _LATB10 = 1;
}
void turnOffRedLED(){
    _LATB11 = 0;
}
void turnOffYellowLED(){
    _LATB12 = 0;
}
void turnOffGreenLED(){
    _LATB13 = 0;
}
void turnOffBlueLED(){
    _LATB10 = 0;
}
void initStatusLED(){
    
    _TRISB13 = 0;
    _TRISB12 = 0;
    _TRISB11 = 0;
    _TRISB10 = 0;
    
    turnOffRedLED();
    turnOffYellowLED();
    turnOffGreenLED();
    turnOffBlueLED();
    
}