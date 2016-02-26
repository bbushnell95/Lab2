
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

typedef enum stateTypeEnum{
    Enter, ProgramMode, ValidateNew, Wait, ValidateExisting
} stateType;

volatile int keypadFlag = 0;
char keystroke = -1;
const char* enter = "Enter:";
const char* setMode = "Set Mode:";
const char* good = "Good";
const char* bad = "Bad";
const char* valid = "Valid";
const char* invalid = "Invalid";

int state = Enter;
int firstStar = 0;            //used to detect when a user enters an '*' 
int digitCount = 0;
int passwordCount = 0;
int matchFlag = 0;
char* newPassword = "\0";     //is this how you initialize a blank string?
char passwordArray [3][5];    //need to check how to make an array of strings
                              //should be a 4 element array of 5 characters each including \0
char* buildString(void);
int stringCompare(char* newString);

int main(void){
    
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();
    initKeypad();
    
    while(1){
        
        switch(state){
            
            case Enter:
                clearLCD();
                printStringLCD(enter);
                moveCursorLCD(1,2);
                state = Wait;
                break;
                
            case ProgramMode:
                clearLCD();
                printStringLCD(setMode);
                moveCursorLCD(1,2);
                state = ValidateNew;
                break;

            case ValidateNew:
                newPassword= buildString();
                while(digitCount < 4){
                    if(newPassword[digitCount] == '#' || newPassword[digitCount] == '*'){
                        clearLCD();
                        moveCursorLCD(1,1);
                        printStringLCD(invalid);
                        delayMs(2000);
                        state = Enter;
                        digitCount = 0;
                        break;                          //this needs to leave the state not just the while loop
                    }
                    digitCount++;  
                }
                digitCount = 0;
                
                if(passwordCount == 4){
                    passwordCount = 0;
                }
                passwordArray[passwordCount] = newPassword;
                passwordCount++;
                clearLCD();
                moveCursorLCD(1,1);
                printStringLCD(valid);
                delayMs(2000);
                state = Enter;
                break;
                
            case ValidateExisting:
                newPassword=buildString();
                matchFlag=stringCompare(newPassword);
                
                if(matchFlag == 1){
                    clearLCD();
                    moveCursorLCD(1,1);
                    printStringLCD(good);
                    delayMs(2000);
                }
                else{
                    clearLCD();
                    moveCursorLCD(1,1);
                    printStringLCD(bad);
                    delayMs(2000);
                }
                
                state = Enter;
                break;
                
            case Wait:
                if(keypadFlag == 1){
                CN1 = OFF;
                CN2 = OFF;
                CN3 = OFF;
                keypadFlag = 0;
                keystroke = scanKeypad();
                if(keystroke == '*'){
                    if(firstStar == 0){
                        firstStar = 1;
                    }
                    else{
                        firstStar = 0;                  //will need to set this down in other places too
                        state = ProgramMode;
                    }
                }
                else{
                    state = ValidateExisting;
                }
                CN1 = ON;
                CN2 = ON;
                CN3 = ON;
                
                }   
                break;
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

/*
 *This function is used to assemble a string of 4 characters from the keypad.
 * once it has assembled the characters into a string it will return
 */

char* buildString (void){
    int i = 0;
    char* newString = "\0";
    
    while(i < 4){
        if(keypadFlag == 1){
            CN1 = OFF;
            CN2 = OFF;
            CN3 = OFF;
            keypadFlag = 0;
            newString[i]= scanKeypad();
            CN1 = ON;
            CN2 = ON;
            CN3 = ON; 
            i++;
        }
    }
    return newString;
}
/*
 *This function is used to compare a newly entered string to the existing strings
 * in the array of strings being build above.
 */
int stringCompare(char* newString){     //logic needs to be worked on..
    int match = 1;
    int arrayCounter = 0;
    int elementCounter = 0;
    char oldString = "\0";
    
    while(arrayCounter < 4)
    {
        oldString = passwordArray[arrayCounter];
        while(elementCounter < 4){
            if(newString[elementCounter] != oldString[elementCounter]){
                match =0;
                break;
            }
            elementCounter++;
        }
        arrayCounter++;
        elementCounter = 0;
    }
    
    return match;
}