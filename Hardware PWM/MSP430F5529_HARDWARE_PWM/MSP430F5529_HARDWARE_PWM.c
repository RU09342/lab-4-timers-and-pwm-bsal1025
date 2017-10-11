#include <msp430.h>

int brightness[10] = {0, 150, 250, 350, 450, 550, 650, 750, 850, 1000}; // Logarithmic Brightness Array
unsigned int i = 0;                                                     // Variable to step through array
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
            P1DIR |= BIT2+BIT3;                       // P1.2 and P1.3 output
            P1SEL |= BIT2;                            // P1.2 options select

            P1DIR &= ~BIT1;                           //Sets P1.1 as an input
            P1REN |=  BIT1;                           //Enables the pull-up/down resistor
            P1OUT |=  BIT1;                           //Set the resistor to be a pull-up resistor
            P1IE |=BIT1;                              //Enable the interrupt on Port 1.1
            P1IES |=BIT1;                             //Set as falling edge
            P1IFG &=~(BIT1);                          //Clear interrupt flag

            TA0CCR0 = 512-1;                         // PWM Period
            TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
            TA0CCR1 = 256;                            // CCR1 PWM duty cycle
            TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR



__bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0 with interrupt

}

// Button 1.1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void)
       {
          if(P1IES & BIT1)                            // If edge is low and button is pressed
          {
              P1OUT |= BIT3;                          // Turn on LED on P1.3
              TA0CCR1 = brightness[i];                // Load value of array to TA0CCR1
              i++;                                    // Increment to the next array value
              if(i>9)                                 // If the index is about to go out of bounds
              {
                 i=0;                                 // Go back to the beginning of the array
              }
              P1IES &= ~BIT1;                         // Flip edge to high
          }
          else
          {
             P1OUT &= ~BIT3;                          // Turn off LED on P1.3
             P1IES |= BIT1;                           // Flip edge back to low
          }
           P1IFG &= ~(BIT1);                          // Clear the interrupt flag
        }



