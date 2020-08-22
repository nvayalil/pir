/*
 * File:   main.c
 * Author: Niras
 *
 * Created on 20 August 2020, 9:34 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"

//#define _XTAL_FREQ 32000
/*
 *  RA0 -> GPIO o/p
 *  RA1 -> CLC2 o/p
 *  RA2 -> CLC1IN2 i/p
 *  RA3 -> MCLR
 *  
 */
void Isr_IOC()
{
    CLRWDT();
    WDTCONbits.SWDTEN = 1;  // Enable WDT for time out
}

void main(void) 
{
    SYSTEM_Initialize();
    IOCAF2_SetInterruptHandler(Isr_IOC);
    INTERRUPT_GlobalInterruptEnable();
    //INTERRUPT_PeripheralInterruptEnable();
    WDTCONbits.SWDTEN = 0; // Disable WDT
    TRISAbits.TRISA0 = 0;
    while(1)
    {
        SLEEP();
        NOP();
        if(STATUSbits.nTO == 0)
        {
            /* Resume operation by WDT Timeout, turn off the device*/
            WDTCONbits.SWDTEN = 0; // Disable WDT       
            //LATAbits.LATA0 = 1;
            PORTAbits.RA0 = 1;
            NOP();
            PORTAbits.RA0 = 0;
        }
    }
}
