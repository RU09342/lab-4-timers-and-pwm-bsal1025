 * Software Debouncing
 *
 *  Created on: Oct 11, 2017
 *  Last Updated on: Oct 11, 2017
 *  Author: Brandon Salamone
 */

Libraries Used:
 msp430.h is the only library used in these programs.

Dependencies:
The msp430.h library does not have any dependencies.

User Interface:
Each program allows a user to test whether or not the buttons in each board are properly debounced. On startup, one LED will blink while another
will be off. The one that is off is an indicator light to show when the button is pressed. This is to show whether or not the button is properly debounced. 
The other LED blinks at a constant rate. A user will be able to tell if the switch is properly debounced if the indicator light turns off every time the 
button is released. If the light stays on longer than it is supposed to, that is a very good indicator that the switch is not properly debounced.   

Common Functionality Among All Processors:
All processors contain these elements in order to create a software PWM:
 1. The watchdog timer is cleared
 2. The P1DIR and/or PxDIR are set such that two LEDs will be outputs
 3. The PxOUT registers, when used, are set such that the resistor associated with the button used in the code will be a pull-up resistor
 4. The PxREN, PxIE, PxIES, and PxIFG registers are always set to one of the pins corresponding to a button on each board
 5. The Tx0CCTL0 registers are always set to CCIE to enable the timer interrupts in every program
 6. The Tx0CCR0 registers are always set to 800 so that the period of the signals will always be 800 
 7. All processor programs have a line of code that tells the processor to enter low power mode with interrupt
 8. Each program contains two ISRs. The first is a button interrupt that turns on an LED and then clears the interrupt flag. The second ISR 
    is a timer interrupt that blinks another LED at a rate controlled by registers in the main function. 

Key Differences Between Processors:
 1. MSP430FR2311, MSP430FR6989, and MSP430FR5994 all have an extra line of code that disables the default high-impedance mode. The MSP430F5529 and
    MSP430G2553 processors do not need this line of code. 

 2. Different Processors have different LED pin assignments. Below the LED pin assignments will be shown for each processor.
    msp430F5529: P1.0 and P4.7
    msp430FR2311: P1.0 and P2.0
    msp430FR5994: P1.0 and P1.1 *
    msp430FR5989: P1.0 and P9.7
    msp430G2553: P1.0 and P1.6 *
    
    * For these processors, the hexadecimal values pertaining to each pin can be combined into one number, since they have P1 in common.
      For example in the msp430FR5994 processor it would be: P1DIR = BIT0 | BIT1 = 0x03 

 3. Unlike the other processors which use Timer A, the MSP430FR2311 processor uses Timer B. This difference causes only slight adjustments to 
    the code to make it work.
