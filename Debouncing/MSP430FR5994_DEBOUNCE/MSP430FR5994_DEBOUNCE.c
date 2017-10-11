#include <msp430.h>
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

            P1DIR |= BIT0+BIT1;                     // Sets the direction of pin 1 to output
            P1OUT &= ~(BIT0|BIT1);                  // Sets LED as output

            P5DIR &= ~BIT6;                         // Sets P5.6 as an input
            P5REN |=  BIT6;                         // Enables the pull-up/down resistor
            P5OUT |=  BIT6;                         // Set the resistor to be a pull-up resistor

            TA0CCTL0 = CCIE;                        // CCR0 interrupt enabled
            TA0CCR0 = 800;                          // ACLK runs at 10 Hz
            TA0CTL = TASSEL_1 + MC_1;               // ACLK, UP Mode

            P5IE |=BIT6;                            // Enable the interrupt on Port 5.6
            P5IES |=BIT6;                           // Set as falling edge
            P5IFG &=~(BIT6);                        // Clear interrupt flag

            PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                                    // Required for Code to Work Properly
__bis_SR_register(LPM0_bits + GIE);                 // Enter LPM0 and general interrupt enable
__no_operation();                                   // For debugger
}

//Button 1.1 Interrupt
#pragma vector = PORT5_VECTOR
__interrupt void Port_5(void)
   {
        P1OUT ^= BIT0;                              // Blinks red LED
        P5IFG &= ~(BIT6);                           // Clears the flag bit to break out of the loop

   }
//Timer interrupt
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
    {
        P1OUT ^= BIT1;                              // Test LED
       __bis_SR_register(LPM0_bits + GIE);          // Enables interrupts from button disable
    }

