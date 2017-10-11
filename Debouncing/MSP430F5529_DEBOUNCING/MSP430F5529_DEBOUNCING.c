#include <msp430.h>
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

            P1DIR |= BIT0;                          // Sets the direction of pin 1 to output
            P1OUT &= ~BIT0;                         // Sets LED as output
            P4DIR |= BIT7;                          // Sets the direction of pin 1 to output
            P4OUT &= ~BIT7;                         // Sets output high

            P1DIR &= ~BIT1;                         // Sets button2, pin1.2 as an input
            P1REN |=  BIT1;                         // Enables the pull-up/down resistor
            P1OUT |=  BIT1;                         // Set the resistor to be a pull-up resistor
            P2DIR &= ~BIT1;                         // Sets button2, pin1.2 as an input
            P2REN |=  BIT1;                         // Enables the pull-up/down resistor
            P2OUT |=  BIT1;                         // Set the resistor to be a pull-up resistor

            TA1CCTL0 = CCIE;                        // CCR0 interrupt enabled
            TA1CCR0 = 800;                          // Aclk runs at 10 hz
            TA1CTL = TASSEL_1 + MC_1;               // ACLK, Up Mode

            P1IE |=BIT1;                            // Enable the interrupt on Port 1.1
            P1IES |=BIT1;                           // Set as falling edge
            P1IFG &=~(BIT1);                        // Clear interrupt flag
            P2IE |=BIT1;                            // Enable the interrupt on Port 2.1
            P2IES |=BIT1;                           // Set as falling edge
            P2IFG &=~(BIT1);                        // Clear interrupt flag
__bis_SR_register(LPM0_bits + GIE);                 // Enter LPM0 and general interrupt enable
__no_operation();                                   // For debugger
}
//Button 2.1 Interrupt
    #pragma vector = PORT2_VECTOR
    __interrupt void Port_2(void)
       {
        P1OUT ^= BIT0;                              // Blinks red LED
    P2IFG &= ~(BIT1);                               // Clears the flag bit to break out of the loop
        __bic_SR_register_on_exit(LPM0_bits + GIE); // Disables interrupts until  they are enabled (in timer)
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
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TA1_ISR(void)
    {
        P4OUT ^= BIT7;                              // Test LED
       __bis_SR_register(LPM0_bits + GIE);          // Enables interrupts from button disable
    }

