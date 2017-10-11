#include <msp430.h>

int brightness[10] = {0, 10, 15, 25, 50, 100, 150, 250, 400, 511}; // Array for logarithmic brightness
unsigned int i = 0;                                                // Variable to step through array
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
            P1DIR |= BIT0+BIT1;                       // P1.0 and P1.1 set as an output

            P5DIR &= ~BIT6;                           // Sets button P5.6 as an input
            P5REN |=  BIT6;                           // Enables the pull-up/down resistor
            P5OUT |=  BIT6;                           // Set the resistor to be a pull-up resistor

            TA0CTL |= TAIE;                           // Timer A interrupt enabled
            TA0CCR0 = 512-1;                          // PWM Period
            TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
            TA0CCTL1 = CCIE;                          // CCR1 interrupt enabled
            TA0CCR1 = 256;                            // CCR1 PWM duty cycle
            TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR

            P5IE |=BIT6;                              // Enable the interrupt on Port 5.6
            P5IES |=BIT6;                             // Set as falling edge
            P5IFG &=~(BIT6);                          // Clear interrupt flag
            PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode

__bis_SR_register(LPM0_bits + GIE);                   // Enter LPM0 with interrupt

}

// Button 5.6 Interrupt Service Routine
#pragma vector = PORT5_VECTOR
    __interrupt void Port_5(void)
       {
           if(P5IES & BIT6)                           // If edge is low and button is pressed
           {
               P1OUT |= BIT1;                         // Turn on LED on P1.1
               TA0CCR1 = brightness[i];               // Calls index i in brightness array
               i++;                                   // Steps through brightness array
               if(i>9)                                // If the index goes beyond array boundary
               {
               i=0;                                   // Go back to the start of the array
               }
               P5IES &= ~BIT6;                        // Flip edge to high
           }
           else
           {
               P1OUT &= ~BIT1;                        // Turn off LED on P1.1
               P5IES |= BIT6;                         // Flip edge back to low
           }
           P5IFG &= ~(BIT6);                          // Clear the interrupt flag
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
            P1OUT &= ~BIT0;                           // Turn off LED on P1.0
        }

    }
