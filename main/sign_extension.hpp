#ifndef SIGN_EXTENSION_HPP
#define SIGN_EXTENSION_HPP

#include <cstdint>

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