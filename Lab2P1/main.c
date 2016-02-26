
#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"
#include "keypad.h"

#define CN1 CNENGbits.CNIEG0
#define CN2 CNENFbits.CNIEF1
#define CN3 CNENDbits.CNIED12
#define ON 1
#define OFF 0


volatile int keypadFlag = 0;
char keystroke = -1;

int main(void){
    
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();
    initKeypad();

    int pressCount = 0;
    moveCursorLCD(1,1);
    while(1){
        
        
        if(keypadFlag == 1){
            
            CN1 = OFF;
            CN2 = OFF;
            CN3 = OFF;
            pressCount++;
            keypadFlag = 0;
            keystroke = scanKeypad();
            
            if(keystroke != -1){
                if(pressCount == 16){
                    moveCursorLCD(1,2);
                }
                else if(pressCount == 32){
                    moveCursorLCD(1,1);
                    pressCount = 0;
                }
            }
                printCharLCD(keystroke);    
            
            CN1 = ON;
            CN2 = ON;
            CN3 = ON;
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