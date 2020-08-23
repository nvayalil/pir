/*
 * File:   main.c
 * Author: Niras
 *
 * Created on 20 August 2020, 9:34 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"

#define TEST_DEVICE

#ifdef  TEST_DEVICE
#define TRIAC_On()      LATAbits.LATA1 = 1
#define TRIAC_Off()     LATAbits.LATA1 = 0
#else
#define TRIAC_On()      LATAbits.LATA1 = 0
#define TRIAC_Off()     LATAbits.LATA1 = 1
#endif
#define LED_On()        LATAbits.LATA0 = 1
#define LED_Off()       LATAbits.LATA0 = 0
#define LED_Toggle()    LATAbits.LATA0 ^= 1
/*
 *  RA0, RA1 -> GPIO o/p
 *  RA2 -> PIR i/p
 *  RA3 -> MCLR  
 */

/*
    Interrupt on change, low to high transition of PIR output
 */
void __interrupt() IsrIOC(void)
{
    /*  
        The IOCIF bit of the INTCON register reflects 
        the status of all IOCAFx bits, therefore is IOCIF ignored
     */
    if(IOCAFbits.IOCAF2 == 1)
    {
#ifdef TEST_DEVICE
        LED_Toggle();           
#endif
        TRIAC_On();             // Turn the device on
        WDTCONbits.SWDTEN = 1;  // Enable WDT for time out
    }                           // WDT is cleared on wakeup
    IOCAF = 0;                  // Clear all flags, although not expecting
                                // anything other than IOCAF2
}   

void main(void) 
{
    /* MCC initialization function */
    SYSTEM_Initialize();
    /* Enable Interrupt on Change */
    INTCONbits.IOCIE = 1;
    /* Disable WDT */
    WDTCONbits.SWDTEN = 0;
    LED_On();
    /* Enable global interrupts */
    INTCONbits.GIE = 1;
    while(1)
    {
        SLEEP();
        NOP();
        if(STATUSbits.nTO == 0)
        {
            /* Resumes operations by WDT Timeout, turn off the device*/
            WDTCONbits.SWDTEN = 0; // Disable WDT       
            TRIAC_Off();
#ifdef TEST_DEVICE
            LED_On();
#endif
        }
    }
}
