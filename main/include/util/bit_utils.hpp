#ifndef BIT_UTILS_HPP
#define BIT_UTILS_HPP

#include <cstdint>

/*
    Parameter instruction:      A 32-bit unsigned integer representing machine code.
    Parameter start:            An integer value representing which bit to start from.
    Parameter end:              An integer value representing which bit to end with.
    
    Returns:                    An unsigned 32-bit integer with lower bits of range [start, end]

    Description:                Bit extraction function. [Range of bits] (inclusive) -> Range of bits starting from 0 bits (Return value).
*/
uint32_t extractBits(uint32_t instruction, int start, int end){
    uint32_t rightShiftedNumber = instruction >> start; // Shifts the bits of the instruction by the amount of the starting position.
                                                        // This brings the starting position bit down to the least-most significant bit.

    uint32_t mask = 1;
    uint32_t width = (end - start + 1);
    uint32_t bitMask = (mask << width) - 1;     // Ex. Width 5 -> (0b1 shift left 5: 0b100000). 0b100000 - 0b1 = 0b011111
                                                // Bitwise-AND of the five bits and 0b011111 preserves the five least-significant bits
    return (rightShiftedNumber & bitMask);      // and removes the rest (zeroes out).
}

/*
    Parameter rawValue:     An unsigned 32-bit integer.
    Parameter bits:         Integer amount of bits to read from.
    
    Returns:                A 32-bit signed integer where the bits of parameter "rawValue" is read in two's complement.
*/
int32_t twos_complement(uint32_t rawValue, int bits){
    if (rawValue & (1 << (bits - 1))){     // Check if the sign-bit (most-sig bit) is one.
        rawValue -= (1 << bits);           // If so, subtract that next power of 2.
    }

    return (static_cast<int32_t>(rawValue));
}

/*
    Parameter bits:             A 32-bit unsigned integer representing an immediate value.
    Parameter originalBits:     An integer value representing how many bits in the parameter "bits" represent the immediate value.
    
    Returns:                    A 32-bit signed integer that should represent the original value in parameter bits, either
                                left zero-extended or newly one-extended.
*/
int32_t signExtend(uint32_t bits, int originalBits){
    int shiftAmount = 32 - originalBits;
    uint32_t leftShifted = bits << shiftAmount;

    return ((static_cast<int32_t>(leftShifted)) >> shiftAmount);
}

#endif