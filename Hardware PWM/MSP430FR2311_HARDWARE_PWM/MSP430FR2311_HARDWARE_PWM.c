#include <msp430.h>

int brightness[10] = {0, 150, 250, 350, 450, 550, 650, 750, 850, 1000}; // Logarithmic Brightness Array
unsigned int i = 0;                                                     // Variable to step through array
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
            P2DIR |= BIT0+BIT1;                       // P2.0 and P2.1 output
            P2SEL0 |= BIT0;                           // P2.0 options select 0
            P2SEL1 &= ~BIT0;                          // P2.0 options select 1

            P1DIR &= ~BIT1;                           // Sets P1.1 as an input
            P1REN |=  BIT1;                           // Enables the pull-up/down resistor
            P1OUT |=  BIT1;                           // Set the resistor to be a pull-up resistor
            P1IE |=BIT1;                              // Enable the interrupt on Port 1.1
            P1IES |=BIT1;                             // Set as falling edge
            P1IFG &=~(BIT1);                          // Clear interrupt flag

            TB1CCR0 = 512-1;                          // PWM Period
            TB1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
            TB1CCR1 = 256;                            // CCR1 PWM duty cycle
            TB1CTL = TBSSEL_2 + MC_1 + TBCLR;         // SMCLK, up mode, clear TBR



            PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                                      // Required for Code to Properly Function

__bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0 with interrupt

}

// Button 1.1 Interrupt Service Routine
    #pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void)
       {
          if(P1IES & BIT1)                            // If edge is low and button is pressed
          {
              P2OUT |= BIT1;                          // Turn on LED on P2.1
              TB1CCR1 = brightness[i];                // Set TB1CCR1 to appropriate value of brightness array
              i++;                                    // Increment to step through brightness array
              if(i>9)                                 // If i is about to go out of bounds
              {
                 i=0;                                 // Return to the beginning of the array
              }
              P1IES &= ~BIT1;                         // Flip edge to high
          }
          else
          {
             P2OUT &= ~BIT1;                          // Turn off LED on P2.1
             P1IES |= BIT1;                           // Flip edge back to low
          }
           P1IFG &= ~(BIT1);                          // Clear the interrupt flag
          }


