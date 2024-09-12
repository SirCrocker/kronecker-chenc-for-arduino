#include "kronecker-chenc.h"

void encode_kronecker_tmpd4s2(uint8_t packet) {
    // We will use only bits in a column vector, so we can do the following.
    // For TMPD-4 SCH 2 we need 16 bits for an 8 bit packet
    // 7,8 - 5,6 - 3,4 - 1,2
    
    static uint8_t buffer[2] = {0};
    
    uint8_t first_batch = _kron2(packet, packet >> 2);
    uint8_t second_batch = _kron2(packet >> 4, packet >> 6);

    _kron_product(first_batch, second_batch, 4, 4, buffer);

}


uint8_t _kron2(uint8_t first_side, uint8_t second_side) {
    // As in MATLAB we multiply elements of first_side with second_size
    static uint8_t n_bits = 2;
    uint8_t result = 0;

    for (int8_t ind = n_bits-1; ind >= 0; ind--) {

        uint8_t data_one_element_mask = ((first_side >> ind) & 0x01) ? 0xFF : 0x00;
        uint8_t masked_data = ~(second_side ^ data_one_element_mask); // "Multiplication" for kronecker product
        result <<= n_bits; // Shift so we can save data
        result |= (masked_data & ( 0xFF >> (8-n_bits) )); // Save only the new values  

    }

    return result;
}

bool _kron_product(uint8_t first_side, uint8_t second_side, size_t size_first, size_t size_second, uint8_t *buffer) {
    // As in MATLAB we multiply elements of first_side with second_size
    uint8_t buffer_ind = 0;
    uint8_t saved_number = 0;
    
    // Valid number of bits to multiply
    if (size_first > 8 || size_first == 0 || size_second > 8 || size_second == 0) {
        return false;
    }

    // It is a factor of 8
    if ((size_first % 8 != 0) || (size_second % 8 != 0)) {
        return false;
    }

    // We multiply, assuming the buffer has the correct size
    for (int8_t ind = size_first-1; ind >= 0; ind--) {

        uint8_t data_one_element_mask = ((first_side >> ind) & 0x01) ? 0xFF : 0x00; // Each bit of first has to be multiplied by each of second
        uint8_t masked_data = ~(second_side ^ data_one_element_mask); // "Multiplication" for kronecker product
        
        // So we dont overwrite data
        if (saved_number >= 8)
        {
            buffer_ind++;
            saved_number %= 8;
        }
        
        // We save data
        buffer[buffer_ind] <<= size_second; // Shift so we can save data
        buffer[buffer_ind] |= (masked_data & ( 0xFF >> (8-size_second) )); // Save only the new values  

        // To keep in control the writing of data
        saved_number += (8 - size_second);

    }
    
    return true;

}