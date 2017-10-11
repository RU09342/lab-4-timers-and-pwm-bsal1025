#include <msp430.h>
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

            P1DIR |= BIT0;                          // Sets the direction of pin 1 to output
            P1OUT &= ~BIT0;                         // Sets LED as output
            P2DIR |= BIT0;                          // Sets the direction of pin 2 to output
            P2OUT &= ~BIT0;                         // Sets output high

            P1DIR &= ~BIT1;                         // Sets pin1.1 as an input
            P1REN |=  BIT1;                         // Enables the pull-up/down resistor
            P1OUT |=  BIT1;                         // Set the resistor to be a pull-up resistor


            TB0CCTL0 = CCIE;                        // CCR0 interrupt enabled
            TB0CCR0 = 800;                          // ACLK runs at 10 Hz
            TB0CTL = TBSSEL_1 + MC_1;               // ACLK, Up Mode

            P1IE |= BIT1;                           // Enable the interrupt on Port 1.1
            P1IES |= BIT1;                          // Set as falling edge
            P1IFG &= ~(BIT1);                       // Clear interrupt flag

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
        __bic_SR_register_on_exit(LPM0_bits + GIE); // Disables interrupts until  they are enabled (in timer)
   }
//Timer interrupt
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0_ISR(void)
    {
        P2OUT ^= BIT0;                              // Test LED
       __bis_SR_register(LPM0_bits + GIE);          // Enables interrupts from button disable
    }

