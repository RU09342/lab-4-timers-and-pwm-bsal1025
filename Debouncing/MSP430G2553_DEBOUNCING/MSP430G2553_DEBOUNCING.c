#include <msp430.h>
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

            P1DIR |= BIT0+BIT6;                     // sets the direction of pin 1 to output
            P1OUT &= ~(BIT0+BIT6);                  // sets LED as output

            P1DIR &= ~BIT3;                         // Sets P1.3 as an input
            P1REN |=  BIT3;                         // Enables the pull-up/down resistor
            P1OUT |=  BIT3;                         // Set the resistor to be a pull-up resistor


            TA0CCTL0 = CCIE;                        // CCR0 interrupt enabled
            TA0CCR0 = 800;                          // ACLK runs at 10 Hz
            TA0CTL = TASSEL_1 + MC_1;               // ACLK, UP Mode

            P1IE |=BIT3;                            // Enable the interrupt on Port 1.3
            P1IES |=BIT3;                           // Set as falling edge
            P1IFG &=~(BIT3);                        // Clear interrupt flag

__bis_SR_register(LPM0_bits + GIE);                 // Enter LPM0 and general interrupt enable
__no_operation();                                   // For debugger
}

//Button 1.1 Interrupt
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
   {
        P1OUT ^= BIT0;                              // Blinks red LED
        P1IFG &= ~(BIT3);                           // Clears the flag bit to break out of the loop

   }
//Timer interrupt
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
    {
        P1OUT ^= BIT6;                              // Test LED
       __bis_SR_register(LPM0_bits + GIE);          // Enables interrupts from button disable
    }


