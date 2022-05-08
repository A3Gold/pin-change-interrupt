// Purpose: To alter the hex count on a 7-seg. display using a rotary encoder.
// Reference: http://darcy.rsgc.on.ca/ACES/TEI4M/2021/Tasks.html#PinChange
// Author: A. Goldman
// Date: Jan. 30, 2021
// Status: Working

#include <DDBv6AG.h>                // Includes custom library
volatile uint8_t triggered = false; // Defines triggered variable
uint8_t count = 0;                  // Defines count variable
volatile uint8_t A;                 // Defines A variable
uint8_t previousA;                  // Defines previousA variable

void setup()
{
    PCMSK0 |= (1 << PCINT0 | 1 << PCINT1);             // Activates PCIs for pins in use
    GIMSK |= 1 << PCIE0;                               // Activates Port A for PCIs
    GIFR |= 1 << PCIF0;                                // Clears flag for Port A
    AGShiftOut(MSBFIRST, sevenSegCCMSBFHexMap[count]); // Shifts out initial 0
}

ISR(PCINT0_vect)
{                        // ISR called when PC on Port A
    triggered = true;    // Triggered set to true
    A = PINA & 1 << PA0; // Stores value of A output on encoder
}

void loop()
{
    if (triggered)
    {                      // If triggered is true...
        triggered = false; // Set trigger to false
        if (A != previousA)
        { // If state of A has changed...
            // If A when ISR called equals B now...
            if (((PINA & (1 << PA1)) >> 1) == A)
                count--; // Decrease count
            else         // Otherwise...
                count++; // Increase it
            if (count > 15)
            {              // If count greater than 15...
                count = 0; // Set it back to 0
            }
            previousA = A; // Record state of A
        }
    }
    AGShiftOut(MSBFIRST, sevenSegCCMSBFHexMap[count]); // Shifts out data
}
