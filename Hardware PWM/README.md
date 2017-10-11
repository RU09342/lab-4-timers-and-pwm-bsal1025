 * Hardware PWM
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
Each program allows the user to push a button to control the duty cycle and hence the brightness of an LED. On startup, two LEDs will light up.
One of them is an indicator light to show when the button is pressed. This is to show whether or not the button is properly debounced. The other 
LED takes in the PWM signal. Initially, the duty cycle is 50%. Upon a button press, the program begins to cycle through a brightness array of 
integers that will update the duty cycle every time a button is pressed. This means that on the first button press, the duty cycle will go from
50% to 0%. After that, the program runs through the entire array on consecutive button presses until 100% duty cycle is reached, after which the
program resets to 0% duty cycle.    

Common Functionality Among All Processors:
All processors contain these elements in order to create a hardware PWM:
 1. The watchdog timer is cleared
 2. Every program contains a brightness array that allows for logarithmic brightness. The values in the array allow the user to visualize the 
    incremental changes in brightness as the LED gets progressively brighter
 3. The P1DIR and/or PxDIR are set such that two LEDs will be outputs
 4. PxSEL registers in 5529 and 2553 processors are always set to an LED. PxSEL0 registers in other processors are set the same way.
 5. PxSEL1 registers in certain processors are set such that the LED is negated (examples shown in code).
 6. The PxOUT registers, when used, are set such that the resistor associated with the button used in the code will be a pull-up resistor
 7. The PxREN, PxIE, PxIES, and PxIFG registers are always set to one of the pins corresponding to a button on each board
 8. The Tx0CCTL0 registers are always set to CCIE to enable the timer interrupts in every program
 9. The Tx0CCTL1 registers are always set to OUTMOD_7 to set the output mode to reset/set in every program
 10. The Tx0CCR0 registers are always set to 512-1 so that the period of the signals will always be 512
 11. The Tx0CCR1 registers are always initialized to 256 to give an initial duty cycle of 50%  
 12. All processor programs have a line of code that tells the processor to enter low power mode with interrupt
 13. Each program contains an ISR. The first performs multiple important tasks. First, the indicator light is turned on. Next, the brightness 
    array code runs. Then, the interrupt edge select is flipped to be high. Once the button is released, the LED is turned off and the interrupt 
    edge select goes back to low before the cycle starts all over again.  

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
 4. The msp430F5529 and msp430g2553 processors use PxSEL as a register, while all other processors use the PxSEL0 and PxSEL1 registers.