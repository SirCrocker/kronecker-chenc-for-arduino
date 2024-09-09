#include "kronecker-chenc.h"

void encode_kronecker() {
    // We will use only bits in a column vector, so we can do the following.
    uint8_t data_one;
    uint8_t data_two;

    uint8_t num_bits_to_kron_d1 = 0;
    uint8_t num_bits_to_kron_d2 = 0;
    size_t total_bits = num_bits_to_kron_d1 * num_bits_to_kron_d2;
    
    size_t buffer_size = total_bits / 8 + 1;
    uint8_t buffer[buffer_size|] = {0};

    for (size_t buffer_ind = 0; buffer_ind < buffer_size; buffer_ind++) {
        
        for (uint8_t ind = 0; ind < num_bits_to_kron; ind++) { // Esta logica está mala, se sobrepasa si es que total_bits > 8

            uint8_t data_one_element_mask = ((data_one >> ind) & 0x01) ? 0xFF : 0x00;
            uint8_t masked_data = !(data_two ^ data_one_element_mask); // "Multiplication" for kronecker encoding

            buffer[buffer_ind] <<= num_bits_to_kron_d2; // Shift so we can save data
            buffer[buffer_ind] |= (masked_data & (0xFF >> (8 - num_bits_to_kron_d2) )); // Save only the new values  

        }
    }
}