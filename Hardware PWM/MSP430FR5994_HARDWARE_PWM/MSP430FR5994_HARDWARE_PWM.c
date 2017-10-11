#include <msp430.h>

int brightness[10] = {0, 150, 250, 350, 450, 550, 650, 750, 850, 1000};  // Logarithmic Brightness Array
unsigned int i = 0;                                                      // Variable to step through array
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
            P1DIR |= BIT1+BIT0;                       // P1.0 and P1.1 output
            P1SEL0 |= BIT0;                           // P1.0 options select0
            P1SEL1 &= ~BIT0;                          // P1.0 options select1

            P5DIR &= ~BIT6;                           // Sets P5.6 as an input
            P5REN |=  BIT6;                           // Enables the pull-up/down resistor
            P5OUT |=  BIT6;                           // Set the resistor to be a pull-up resistor
            P5IE |= BIT6;                             // Enable the interrupt on Port 5.6
            P5IES |=BIT6;                             // Set as falling edge
            P5IFG &=~(BIT6);                          // Clear interrupt flag

            TA0CCR0 = 512-1;                          // PWM Period
            TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
            TA0CCR1 = 256;                            // CCR1 PWM duty cycle
            TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR

            PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                                      // Required for Code to Properly Function

__bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0 with interrupt

}
// Button 5.6 Interrupt Service Routine
#pragma vector = PORT5_VECTOR
    __interrupt void Port_5(void)
       {
          if(P5IES & BIT6)                            // If edge is low and button is pressed
          {
              P1OUT |= BIT1;                          // Turn on LED on P1.1
              TA0CCR1 = brightness[i];                // Set CCR1 to appropriate value of brightness array
              i++;                                    // Increment to step through brightness array
              if(i>9)                                 // If i goes out of bounds
              {
                 i=0;                                 // Return to the beginning of the array
              }
              P5IES &= ~BIT6;                         // Flip edge to high
          }
          else
          {
             P1OUT &= ~BIT1;                          // Turn off LED on P1.1
             P5IES |= BIT6;                           // Flip edge back to low
          }
           P5IFG &= ~(BIT6);                          // Clear the interrupt flag
          }
