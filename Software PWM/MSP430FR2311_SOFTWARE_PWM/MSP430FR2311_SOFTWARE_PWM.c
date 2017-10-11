#include <msp430.h>

int brightness[10] = {0, 10, 15, 25, 50, 100, 150, 250, 400, 511}; // Array for logarithmic brightness
unsigned int i = 0;                                                // Variable to step through array
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
            P1DIR |= BIT0;                            // P1.0 set as an output
            P2DIR |= BIT0;                            // P2.0 set as an output

            P1DIR &= ~BIT1;                           //Sets button P1.1 as an input
            P1REN |=  BIT1;                           //Enables the pull-up/down resistor
            P1OUT |=  BIT1;                           //Set the resistor to be a pull-up resistor

            TB0CTL |= TBIE;                           // Timer B interrupt enabled
            TB0CCR0 = 512-1;                          // PWM Period
            TB0CCTL0 = CCIE;                          // CCR0 Interrupt enabled
            TB0CCTL1 = CCIE;                          // CCR1 Interrupt enabled
            TB0CCR1 = 256;                            // CCR1 PWM duty cycle
            TB0CTL = TBSSEL_2 + MC_1 + TBCLR;         // SMCLK, up mode, clear TBR

            P1IE |=BIT1;                              // Enable the interrupt on Port 1.1
            P1IES |=BIT1;                             // Set as falling edge
            P1IFG &=~(BIT1);                          // Clear interrupt flag
            PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode

__bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0 with interrupt

}

// Button 1.1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void)
       {
           if(P1IES & BIT1)                           // If edge is low and button is pressed
           {
               P2OUT |= BIT0;                         // Turn on LED on P2.0
               TB0CCR1 = brightness[i];               // Calls index i in brightness array
               i++;                                   // Steps through brightness array
               if(i>9)                                // If the index goes beyond array boundary
               {
               i=0;                                   // Go back to the start of the array
               }
               P1IES &= ~BIT1;                        // Flip edge to high
           }
           else
           {
               P2OUT &= ~BIT0;                        // Turn off LED on P2.0
               P1IES |= BIT1;                         // Flip edge back to low
           }
           P1IFG &= ~(BIT1);                          // Clear the interrupt flag
           }
// Timer B0 Interrupt Service Routine
#pragma vector = TIMER0_B0_VECTOR
    __interrupt void TimerB (void)
    {
        P1OUT |= BIT0;                                // Turn on LED on P1.0
    }

// Timer B1 Interrupt Service Routine
#pragma vector = TIMER0_B1_VECTOR
    __interrupt void TimerB1 (void)
    {
        switch(TB0IV)
        {
        case 2:
            P1OUT &= ~BIT0;                           // Turn off LED on P.10
        }

    }
