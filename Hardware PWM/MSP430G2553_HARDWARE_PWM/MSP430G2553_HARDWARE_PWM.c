#include <msp430.h>

int brightness[10] = {0, 150, 250, 350, 450, 550, 650, 750, 850, 1000}; // Logarithmic brightness array
unsigned int i = 0;                                                     // Variable to step through array
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                // Stop WDT

            P1DIR |= BIT0+BIT6;                      // P1.0 and P1.6 output
            P1SEL |= BIT6;                           // P1.6 options select

            P1DIR &= ~BIT3;                          //Sets P1.3 as an input
            P1REN |=  BIT3;                          //Enables the pull-up/down resistor
            P1OUT |=  BIT3;                          //Set the resistor to be a pull-up resistor
            P1IE  |=  BIT3;                          //Enable the interrupt on Port 1.3
            P1IES |=  BIT3;                          //Set as falling edge
            P1IFG &= ~(BIT3);                        //Clear interrupt flag

            TA0CCR0 = 512-1;                         // PWM Period
            TA0CCTL1 = OUTMOD_7;                     // CCR1 reset/set
            TA0CCR1 = 500;                           // CCR1 PWM duty cycle

            TA0CTL = TASSEL_2 + MC_1 + TACLR;        // SMCLK, up mode, clear TAR



__bis_SR_register(LPM0_bits + GIE);                  // Enter LPM0 with interrupt

}
// Button 1.1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void)
       {
          if(P1IES & BIT3)                           // If edge is low and button is pressed
          {
              P1OUT |= BIT0;                         // Turn on LED on P1.0
              TA0CCR1 = brightness[i];               // Set CCR1 to appropriate value of brightness array
              i++;                                   // Increment to step through brightness array
              if(i>9)                                // If i goes out of bounds
              {
                 i=0;                                // Go back to the start of the array
              }
              P1IES &= ~BIT3;                        // Flip edge to high
          }
          else
          {
             P1OUT &= ~BIT0;                         // Turn off LED on P1.0
             P1IES |= BIT3;                          // Flip edge back to low
          }
           P1IFG &= ~(BIT3);                         // Clear the interrupt flag
          }
