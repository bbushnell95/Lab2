
#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
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

char newString[4] = "aaaa";

int state = Enter;
int firstStar = 0;            //used to detect when a user enters an '*' 
int passwordCount = 0;
int matchFlag = 0;
char* newPassword = "\0";     //is this how you initialize a blank string?
char passwordArray[4][5];    //need to check how to make an array of strings
                              //should be a 4 element array of 5 characters each including \0
int firstNum = 0;
char* buildString();
int stringCompare(char* newString);
void printWhat(const char* string);
int testPassword(char* newPassword);
int comparePassword(char* oldPass);

int main(void){
    
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();
    initKeypad();
    
    int i = 0;
    keypadFlag = 1;
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
                newPassword = buildString();
                if(testPassword(newPassword) == 0){
                    // no errors]
                    printWhat(valid);
                }
                else{
                    // error in input - '*' or '#'
                    printWhat(invalid);
                }
                
                state = Enter;
                break;
                
            case ValidateExisting:
                newPassword = buildString();
                i = 0;
                //matchFlag = stringCompare(newPassword);
                while(i < 4){
                    if((comparePassword(passwordArray[i])) == 0){
                        // found a match
                        matchFlag = 1;
                        break;
                    }
                    i++;
                }
                i = 0;
                if(matchFlag == 1){
                    printWhat(good);
                }
                else{
                    printWhat(bad);
                }
                matchFlag = 0;
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
                        printCharLCD(keystroke);
                        if(firstStar == 0){
                            firstStar = 1;
                        }
                        else{
                            firstStar = 0;                  //will need to set this down in other places too
                            state = ProgramMode;
                        }
                    }
                    else if(keystroke != '#'){
                        state = ValidateExisting;
                        firstNum = keystroke;
                    }
                    else{
                        state = Enter;
                        firstStar = 0;
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

void printWhat(const char* string){
    
                    clearLCD();
                    moveCursorLCD(1,1);
                    printStringLCD(string);
                    delayMs(8000);
                    delayMs(8000);
}


int testPassword(char* newPassword){
    
    int digitCount = 0;
    int exitCondition = 0;
    
    while(digitCount < 4){
        if(newPassword[digitCount] == '#' || newPassword[digitCount] == '*'){
            exitCondition = 1;
            digitCount = 0;
            break;                          //this needs to leave the state not just the while loop
        }
        digitCount++;  
    }
    digitCount = 0;
                
    if(passwordCount == 4){
        passwordCount = 3;
    }
    if(exitCondition != 1){
        strcpy(passwordArray[passwordCount], newPassword);
        passwordCount++;
    }
    
    return exitCondition;
    
}

/*
 *This function is used to assemble a string of 4 characters from the keypad.
 * once it has assembled the characters into a string it will return
 */



char* buildString (){
    int i;
    
    
    if(state == ValidateExisting){
        // first number needs to be captured
        newString[0] = firstNum;
        printCharLCD(newString[0]);
        i = 1; 
    }
    else if(state == ValidateNew){
        // no first number to capture
        i = 0;   
    }
    while(i < 4){
        if(keypadFlag == 1){
            CN1 = OFF;
            CN2 = OFF;
            CN3 = OFF;
            keypadFlag = 0;
            newString[i]= scanKeypad();
            printCharLCD(newString[i]);
            CN1 = ON;
            CN2 = ON;
            CN3 = ON; 
            i++;
        }
    }
    return newString;
}

//compare one password from the array with the new password
int comparePassword(char* oldPass){
    int i = 0;
    int result = 0;
    
    for(i=0; i<4; ++i){
        if (newPassword[i] != oldPass[i]){
            result = 1;
            break;
        }
    }
    return result;
}