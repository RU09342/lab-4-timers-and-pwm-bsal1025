#include <msp430.h>
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

            P1DIR |= BIT0;                          // sets the direction of pin 1 to output
            P1OUT &= ~BIT0;                         // sets LED as output
            P9DIR |= BIT7;                          // sets the direction of pin 9 to output
            P9OUT &= ~BIT7;                         // sets output high

            P1DIR &= ~(BIT1);                       // Sets P1.1 as an input
            P1REN |=  BIT1;                         // Enables the pull-up/down resistor
            P1OUT |=  BIT1;                         // Set the resistor to be a pull-up resistor


            TA0CCTL0 = CCIE;                        // CCR0 interrupt enabled
            TA0CCR0 = 800;                          // ACLK runs at 10 Hz
            TA0CTL = TASSEL_1 + MC_1;               // ACLK, UP Mode

            P1IE |=BIT1;                            // Enable the interrupt on Port 1.1
            P1IES |=BIT1;                           // Set as falling edge
            P1IFG &=~(BIT1);                        // clear interrupt flag

            PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                                    // Required for Code to Work Properly

__bis_SR_register(LPM0_bits + GIE);                 // Enter LPM0 and general interrupt enable
__no_operation();                                   // For debugger
}

//Button 1.1 Interrupt
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
   {
        P1OUT ^= BIT0;                              // Blinks red LED
        P1IFG &= ~(BIT1);                           // Clears the flag bit to break out of the loop
   }
//Timer interrupt
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
    {
        P9OUT ^= BIT7;                              // Test LED
       __bis_SR_register(LPM0_bits + GIE);          // Enables interrupts from button disable
    }


