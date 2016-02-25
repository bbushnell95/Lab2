#include <xc.h>
#include "keypad.h"
#include "timer.h"

#define TRIS_COLUMN1 TRISGbits.TRISG0
#define TRIS_COLUMN2 TRISFbits.TRISF1
#define TRIS_COLUMN3 TRISDbits.TRISD12

#define TRIS_ROW1 TRISGbits.TRISG1
#define TRIS_ROW2 TRISFbits.TRISF0
#define TRIS_ROW3 TRISDbits.TRISD13
#define TRIS_ROW4 TRISDbits.TRISD7 

#define OUTPUT 0
#define INPUT 1
#define ENABLE 1

#define OPEN 1              //check if these are right..
#define CLOSED 0

#define COLUMN1 PORTGbits.RG0
#define COLUMN2 PORTFbits.RF1
#define COLUMN3 PORTDbits.RD12

#define CN1 CNENGbits.CNIEG0
#define CN2 CNENFbits.CNIEF1
#define CN3 CNENDbits.CNIED12

#define ROW1 PORTGbits.RG1 //RD11   
#define ROW2 PORTFbits.RF0 //RC14  
#define ROW3 PORTDbits.RD13// RC2 
#define ROW4 PORTDbits.RD7 //RC4   

#define PULLUP1 CNPUGbits.CNPUG0
#define PULLUP2 CNPUFbits.CNPUF1
#define PULLUP3 CNPUDbits.CNPUD12

#define ODC1 ODCDbits.ODCD11
#define ODC2 ODCCbits.ODCC14
#define ODC3 ODCCbits.ODCC2
#define ODC4 ODCCbits.ODCC4

#define LATODC1 LATGbits.LATG1
#define LATODC2 LATFbits.LATF0
#define LATODC3 LATDbits.LATD13
#define LATODC4 LATDbits.LATD7



/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    TRIS_COLUMN1 = INPUT;      //enable as outputs
    TRIS_COLUMN2 = INPUT;
    TRIS_COLUMN3 = INPUT; 
    
    PULLUP1 = ENABLE;       //enable pull up resistors
    PULLUP2 = ENABLE;
    PULLUP3 = ENABLE;
    
    CN1=ENABLE;
    CN2=ENABLE;
    CN3=ENABLE;
    
    CNCONDbits.ON=1;
    CNCONFbits.ON=1;
    CNCONGbits.ON=1;
    
    IFS1bits.CNDIF=0;
    IFS1bits.CNFIF=0;
    IFS1bits.CNGIF=0;
    
    IEC1bits.CNDIE=1;
    IEC1bits.CNFIE=1;
    IEC1bits.CNGIE=1;
    
    IPC8bits.CNIP=7;
    
    
    TRIS_ROW1=OUTPUT;
    TRIS_ROW2=OUTPUT;
    TRIS_ROW3=OUTPUT;
    TRIS_ROW4=OUTPUT;
    
    ODC1 = ENABLE;          //enable ODC 
    ODC2 = ENABLE;
    ODC3 = ENABLE;
    ODC4 = ENABLE;
    
    

}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void){      //close 1 open the rest
    char key = -1;
    
    while(1){
        LATODC1=CLOSED;
        LATODC2=OPEN;
        LATODC3=OPEN;
        LATODC4=OPEN;
        
        //enableODC(1);

        if(COLUMN1 == 0){
            key = '1';
            break;
        }
        else if(COLUMN2 == 0){
            key = '2';
            break;
        }
        else if(COLUMN3 == 0){
            key = '3';
            break;
        }

        LATODC1=OPEN;
        LATODC2=CLOSED;
        LATODC3=OPEN;
        LATODC4=OPEN;

        if(COLUMN1 == 0){
            key = '4';
            break;
        }
        else if(COLUMN2 == 0){
            key = '5';
            break;
        }
        else if(COLUMN3 == 0){
            key = '6';
            break;
        }

        LATODC1=OPEN;
        LATODC2=OPEN;
        LATODC3=CLOSED;
        LATODC4=OPEN;

        if(COLUMN1 == 0){
            key = '7';
            break;
        }
        else if(COLUMN2 == 0){
            key = '8';
            break;
        }
        else if(COLUMN3 == 0){
            key = '9';
            break;
        }

        LATODC1=OPEN;
        LATODC2=OPEN;
        LATODC3=OPEN;
        LATODC4=CLOSED;

        if(COLUMN1 == 0){
            key = '*';
            break;
        }
        else if(COLUMN2 == 0){
            key = '0';
            break;
        }
        else if(COLUMN3 == 0){
            key = '#';
            break;
        }
    }
    
    return key;
}

void enableODC(int ODCNum){
    
    switch(ODCNum){
            case 1:
                LATODC1 = CLOSED;
                LATODC2 = OPEN;
                LATODC3 = OPEN;
                LATODC4 = OPEN;
                break;
            case 2:
                LATODC1 = OPEN;
                LATODC2 = CLOSED;
                LATODC3 = OPEN;
                LATODC4 = OPEN;
                break;
            case 3:
                LATODC1 = OPEN;
                LATODC2 = OPEN;
                LATODC3 = CLOSED;
                LATODC4 = OPEN;
                break;
            case 4:
                LATODC1 = OPEN;
                LATODC2 = OPEN;
                LATODC3 = OPEN;
                LATODC4 = CLOSED;
                break;
    }
}
