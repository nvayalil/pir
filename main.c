/*
 * File:   main.c
 * Author: Niras
 *
 * Created on 20 August 2020, 9:34 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"

#define TRIAC_On()  LATAbits.LATA1 = 1
#define TRIAC_Off() LATAbits.LATA1 = 0
#define LED_On()    LATAbits.LATA0 = 1
#define LED_Off()   LATAbits.LATA0 = 0
/*
 *  RA0, RA1 -> GPIO o/p
 *  RA2 -> PIR i/p
 *  RA3 -> MCLR  
 */

/*
    Interrupt on change (high to low transition of PIR output
 */
void Isr_IOC()
{
    CLRWDT();
    LED_Off();
    TRIAC_On();
    WDTCONbits.SWDTEN = 1;  // Enable WDT for time out
}

void main(void) 
{
    /* MCC initialization function */
    SYSTEM_Initialize();
    /* Set IOC interrupt vector */
    IOCAF2_SetInterruptHandler(Isr_IOC);
    /* Enable global interrupts */
    INTERRUPT_GlobalInterruptEnable();
    /* Disable WDT */
    WDTCONbits.SWDTEN = 0;
    LED_On();
    while(1)
    {
        SLEEP();
        NOP();
        if(STATUSbits.nTO == 0)
        {
            /* Resumes operations by WDT Timeout, turn off the device*/
            WDTCONbits.SWDTEN = 0; // Disable WDT       
            TRIAC_Off();
            LED_On();
        }
    }
}
