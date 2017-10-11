#include <msp430.h>

int brightness[10] = {0, 10, 15, 25, 50, 100, 150, 250, 400, 511}; // Array for logarithmic brightness
unsigned int i = 0;                                                // Variable to step through array
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
            P1DIR |= BIT0;                            // P1.0 set as an output
            P4DIR |= BIT7;                            // P4.7 set as an output

            P1DIR &= ~BIT1;                           //Sets button P1.1 as an input
            P1REN |=  BIT1;                           //Enables the pull-up/down resistor
            P1OUT |=  BIT1;                           //Set the resistor to be a pull-up resistor

            TA0CTL |= TAIE;                           // Timer A interrupt enabled
            TA0CCR0 = 512-1;                          // PWM Period
            TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
            TA0CCTL1 = CCIE;                          // CCR1 interrupt enabled
            TA0CCR1 = 256;                            // CCR1 PWM duty cycle
            TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR

            P1IE |=BIT1;                              //Enable the interrupt on Port 1.1
            P1IES |=BIT1;                             //Set as falling edge
            P1IFG &=~(BIT1);                          //Clear interrupt flag


__bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0 with interrupt

}

// Button 1.1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void)
       {
          if(P1IES & BIT1)                            // If edge is low and button is pressed
          {
            P4OUT |= BIT7;                            // Turn on LED on P4.7
            TA0CCR1 = brightness[i];                  // Calls index i in brightness array
            i++;                                      // Steps through brightness array
            if(i>9)                                   // If the index goes beyond array boundary
            {
               i=0;                                   // Go back to the start of the array
            }
           P1IES &= ~BIT1;                            // Flip edge to high
          }
          else
          {
            P4OUT &= ~BIT7;                           // Turn off LED on P4.7
            P1IES |= BIT1;                            // Flip edge back to low

          }
              P1IFG &= ~(BIT1);                       // Clear the interrupt flag
        }

// Timer A0 Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
    __interrupt void TimerA (void)
    {
        P1OUT |= BIT0;                                // Turn on LED on P1.0
    }
// Timer A1 Interrupt Service Routine
#pragma vector = TIMER0_A1_VECTOR
    __interrupt void TimerA1 (void)
    {
        switch(TA0IV)
        {
        case 2:
            P1OUT &= ~BIT0;                          // Turn off LED on P1.0
        }

    }
