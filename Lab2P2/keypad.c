#include <xc.h>
#include "keypad.h"
#include "timer.h"

#define TRIS_COLUMN1 TRISGbits.TRISG0
#define TRIS_COLUMN2 TRISFbits.TRISF1
#define TRIS_COLUMN3 TRISDbits.TRISD12

#define TRIS_ROW1 TRISDbits.TRISD2 //TRISG1 // change to RD2
#define TRIS_ROW2 TRISDbits.TRISD3 // // change to RD3
#define TRIS_ROW3 TRISDbits.TRISD0   //D13 // change to RD0
#define TRIS_ROW4 TRISDbits.TRISD1 //  D7 //// change to RD1

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

#define ROW1 PORTDbits.RD2 // pin 17
#define ROW2 PORTDbits.RD3 // pin 18
#define ROW3 PORTDbits.RD0 // pin 19
#define ROW4 PORTDbits.RD1 // pin 20

#define PULLUP1 CNPUGbits.CNPUG0
#define PULLUP2 CNPUFbits.CNPUF1
#define PULLUP3 CNPUDbits.CNPUD12

#define ODC1 ODCDbits.ODCD2  //G1 // change to RD2
#define ODC2 ODCDbits.ODCD3  //F0 // change to RD3
#define ODC3 ODCDbits.ODCD0  //13 // change to RD0
#define ODC4 ODCDbits.ODCD1//  D7 // change to RD1

#define LATODC1 LATDbits.LATD2 //G1 // change to RD2
#define LATODC2 LATDbits.LATD3 //F0 // change to RD3
#define LATODC3 LATDbits.LATD0  // 13 // change to RD0
#define LATODC4 LATDbits.LATD1   //D7 // change to RD1

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    TRIS_COLUMN1 = INPUT;      //enable as outputs
    TRIS_COLUMN2 = INPUT;
    TRIS_COLUMN3 = INPUT; 
    
    //PULLUP1 = ENABLE;       //enable pull up resistors
    //PULLUP2 = ENABLE;
    //PULLUP3 = ENABLE;
    
    CNCONDbits.ON=1;
    CNCONFbits.ON=1;
    CNCONGbits.ON=1;
    
    IFS1bits.CNDIF=0;
    IFS1bits.CNFIF=0;
    IFS1bits.CNGIF=0;
    
    IEC1bits.CNDIE=1;
    IEC1bits.CNFIE=1;
    IEC1bits.CNGIE=1;

    CN1=ENABLE;
    CN2=ENABLE;
    CN3=ENABLE;

    IEC1bits.CNDIE=1;
    IEC1bits.CNFIE=1;
    IEC1bits.CNGIE=1;

    IPC8bits.CNIP=7;
    
    ODC1 = ENABLE;          //enable ODC 
    ODC2 = ENABLE;
    ODC3 = ENABLE;
    ODC4 = ENABLE;
    
    LATODC1=OPEN;
    LATODC2=OPEN;
    LATODC3=OPEN;
    LATODC4=OPEN;
    
    TRIS_ROW1=OUTPUT;
    TRIS_ROW2=OUTPUT;
    TRIS_ROW3=OUTPUT;
    TRIS_ROW4=OUTPUT;
    
    
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void){      //close 1 open the rest
    char key = -1;
     LATODC1=OPEN;
     LATODC2=OPEN;
     LATODC3=OPEN;
     LATODC4=OPEN;
    
     while(1){
        LATODC1=CLOSED;
        LATODC2=OPEN;
        LATODC3=OPEN;
        LATODC4=OPEN;
        
        //enableODC(1);

        delayMs(5);
        if(COLUMN1 == 0){
            key = '1';
            //TRISDbits.TRISD0=0;
            //LATDbits.LATD0=!LATDbits.LATD0;
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
       delayMs(5);
        
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
       delayMs(5);
        
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

       delayMs(5);
        if(COLUMN1 == 0){
            key = '*';
            //TRISDbits.TRISD1=0;
            //LATDbits.LATD1=!LATDbits.LATD1;
            break;
        }
        else if(COLUMN2 == 0){
            key = '0';
            //TRISDbits.TRISD1=0;
            //LATDbits.LATD1=!LATDbits.LATD1;
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
