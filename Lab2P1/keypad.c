#include <xc.h>
#include "keypad.h"
#include "timer.h"

#define TRIS_COLUMN1 TRISGbits.TRISG0
#define TRIS_COLUMN2 TRISFbits.TRISF1
#define TRIS_COLUMN3 TRISDbits.TRISD12
#define OUTPUT 0
#define INPUT 1
#define ENABLE 1
#define COLUMN1 PORTGbits.RG0
#define COLUMN2 PORTFbits.RF1
#define COLUMN3 PORTDbits.RD12
#define ROW1 PORTAbits.RA6
#define ROW2 PORTAbits.RA7
#define ROW3 PORTGbits.RG1
#define ROW4 PORTFbits.RF0

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    TRIS_COLUMN1 = OUTPUT;      //enable as outputs
    TRIS_COLUMN2 = OUTPUT;
    TRIS_COLUMN3 = OUTPUT; 
    CNPUGbits.CNPUG0 = ENABLE;       //enable pull up resistors
    CNPUFbits.CNPUF1 = ENABLE;
    CNPUDbits.CNPUD12 = ENABLE;
    
    ODCAbits.ODCA6 = ENABLE;
    ODCAbits.ODCA7 = ENABLE;
    ODCGbits.ODCG1 = ENABLE;
    ODCFbits.ODCF0 = ENABLE;
    
    

}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void){
    char key = -1;
    
    
    
    
    return key;
}
