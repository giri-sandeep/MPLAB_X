#include <xc.h>

#if defined (__32MX460F512L__) || defined (__32MX360F512L__) || defined (__32MX795F512L__) || defined (__32MX450F256L__) || defined (__32MX470F512L__)

// DEVCFG3
// USERID = No Setting
#pragma config FSRSSEL      = PRIORITY_7       // Shadow Register Set Priority Select (SRS Priority 7)
#pragma config PMDL1WAY     = ON               // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY      = ON               // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO     = ON               // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO    = ON               // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV     = DIV_2            // PLL Input Divider (2x Divider)
#pragma config FPLLMUL      = MUL_20           // PLL Multiplier (20x Multiplier)           // #pragma config FPLLMUL  = MUL_15        // PLL Multiplier
#pragma config UPLLIDIV     = DIV_2            // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN       = ON               // USB PLL Enable (Enabled)
#pragma config FPLLODIV     = DIV_1            // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC        = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN      = OFF              // Secondary Oscillator Enable (Disabled)
#pragma config IESO         = OFF              // Internal/External Switch Over (Disabled)
#pragma config POSCMOD      = HS               // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC     = OFF              // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV       = DIV_2            // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM        = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS        = PS128              // Watchdog Timer Postscaler (1:1)
#pragma config WINDIS       = OFF              // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN       = OFF              // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG        = OFF              // Background Debugger Enable (Debugger is Disabled)
#pragma config JTAGEN       = OFF              // JTAG Disable (JTAG Port Disabled)
#pragma config ICESEL       = ICS_PGx2         // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
#pragma config PWP          = OFF              // Program Flash Write Protect (Disable)
#pragma config BWP          = OFF              // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP           = OFF              // Code Protect (Protection Disabled)

#endif
//  The following is used by the main application
#define SYS_FREQ		(80000000)

#include <plib.h>

// IOPORT bit masks can be found in ports.h
#define CONFIG          (CND_ON)
#define PINS            (CND7_ENABLE) //Only RD7 will generate change notice interrupt and RD6 will be used as input pin only with pull up
#define PULLUPS         (CND7_PULLUP_ENABLE | CND6_PULLUP_ENABLE)
#define INTERRUPT       (CHANGE_INT_ON | CHANGE_INT_PRI_2)

unsigned int dummy;

//  port_io application code
int main(void)
{
    unsigned int last_sw_state = 1;

    // Configure the device for maximum performance, but do not change the PBDIV clock divisor.
    // Given the options, this function will change the program Flash wait states,
    // RAM wait state and enable prefetch cache, but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above.
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // configure IOPORTS PORTD.RD0, RD1 as outputs
    // could also use mPORTDSetPinsDigitalOut(BIT_6 | BIT_7);
    PORTSetPinsDigitalOut(IOPORT_D, BIT_0 | BIT_1);

    // initialize the port pins states = output low
    PORTClearBits(IOPORT_D, BIT_0 | BIT_1);

    // PORTD.RD6, RD7 as inputs
    // could also use mPORTDSetPinsDigitalIn(BIT_6 | BIT_7);
    PORTSetPinsDigitalIn(IOPORT_D, BIT_6 | BIT_7);

    // configure the Change Notice Feature
    // Note: It is recommended to disable vectored interrupts prior to
    // configuring the change notice module, (if they are enabled).
    // The user must read one or more IOPORTs to clear any IO pin
    // change notice mismatch condition, then clear the change notice
    // interrupt flag before re-enabling the vector interrupts.

    // Enable change notice, enable discrete pins and weak pullups
    mCNDOpen(CONFIG, PINS, PULLUPS);

    // Read the port to clear any mismatch on change notice pins
    dummy = mPORTDRead();

    // Clear change notice interrupt flag
    ConfigIntCND(INTERRUPT);

    // Ok now to enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

    //Initialize the DB_UTILS IO channel
    DBINIT();

    // Display a message
    DBPRINTF("Welcome to the PIC32 PORT input/output example. \n");
    DBPRINTF("The build date and time is ... (" __DATE__ "," __TIME__ ")\n");
    DBPRINTF("Press SW1 to toggle LED1, press SW2 to toggle LED2 \n");


    // loop here polling for SW1, SW2 is handled by Change Notice Interrupt
    while(1)
    {
        if(PORTDbits.RD6 == 0)					// 0 = switch is pressed
        {
            PORTSetBits(IOPORT_D, BIT_0);			// RED LED = on (same as LATDSET = 0x0001)
            if(last_sw_state == 1)					// display a message only when switch changes state
            {
                DBPRINTF("Switch SW1 has been pressed. \n");
                last_sw_state = 0;
            }
            int i=0;
            for(i; i<1000000 ;i++){}
        }

        else										// 1 = switch is not pressed
        {
            PORTClearBits(IOPORT_D, BIT_0);			// RED LED = off (same as LATDCLR = 0x0001)
            if(last_sw_state == 0)                 // display a message only when switch changes state
            {
                DBPRINTF("Switch SW1 has been released. \n");
                last_sw_state = 1;
            }
        }
    };
}
/******************************************************************************
*	Change Notice Interrupt Service Routine
*
*   Note: Switch debouncing is not performed.
*   Code comes here if SW2 (CN16) PORTD.RD7 is pressed or released.
*   The user must read the IOPORT to clear the IO pin change notice mismatch
*   condition first, then clear the change notice interrupt flag.
******************************************************************************/
void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void)
{
    // Step #1 - always clear the mismatch condition first
    dummy = PORTReadBits(IOPORT_D, BIT_7);

    // Step #2 - then clear the interrupt flag
    mCNDClearIntFlag();

    PORTToggleBits(IOPORT_D, BIT_1);
    int i=0;
    for(i; i<1000000 ;i++){}
}
