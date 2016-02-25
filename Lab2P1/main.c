


#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"
#include "keypad.h"


volatile int keypadFlag = 0;


int main(void){
    
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();
    initKeypad();
    
    char keystroke = -1;
    
    while(1){
        
        enableODC(1);
        
        if(keypadFlag == 1){
            keypadFlag = 0;
            keystroke = scanKeypad();
            
            if(keystroke != -1){
                printCharLCD(keystroke);    
            }
        }
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){
    
    PORTD;
    PORTF;
    PORTG;
    
    IFS1bits.CNDIF=0;
    IFS1bits.CNFIF=0;
    IFS1bits.CNGIF=0;
    
    keypadFlag = 1;
}