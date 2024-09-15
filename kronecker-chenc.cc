#include "kronecker-chenc.h"

uint8_t* encode_kronecker_tmpd4s2(uint8_t packet) {
    // We will use only bits in a column vector, so we can do the following.
    // For TMPD-4 SCH 2 we need 16 bits for an 8 bit packet
    // 7,8 - 5,6 - 3,4 - 1,2
    
    static uint8_t buffer[2] = {0};
    
    uint8_t first_batch = _kron2(packet, packet >> 2);
    uint8_t second_batch = _kron2(packet >> 4, packet >> 6);

    _kron_product(first_batch, second_batch, 4, 4, buffer);

    return buffer;
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

void unfold(uint8_t first_byte, uint8_t second_byte, uint8_t mode, uint8_t *buffer) {
    // Assuming the coming data is in "reshape form" (2, 2, 2, 2). We still represent it as a 2 bytes
    // The "out data" is always a 2x8 matrix, so we can reorder the data such as the first byte is the first row and likewise for the second.
    // Hardcoded unfolding, we need to be quick in the receiver 
    uint8_t out_first = 0;
    uint8_t out_second = 0;
    uint8_t mask_1;
    uint8_t mask_2;

    switch (mode)
    {
    case 1:
        mask_1 = 0x80;
        mask_2 = 0x40;

        for (uint8_t i = 0; i < 4; i++)
        {
            out_first  <<= 1;
            out_second <<= 1;
            
            out_first  |= (first_byte & mask_1) >> 7;
            out_second |= (first_byte & mask_2) >> 6;
            
            first_byte <<= 2;
        }
        
        for (uint8_t i = 0; i < 4; i++)
        {
            out_first  <<= 1;
            out_second <<= 1;
            
            out_first  |= (second_byte & mask_1) >> 7;
            out_second |= (second_byte & mask_2) >> 6;
            
            second_byte <<= 2;
        }

        break;
    
    case 2:
        mask_1 = 0xC0;
        mask_2 = 0x30;
        
        for (uint8_t i = 0; i < 2; i++)
        {
            out_first  <<= 2;
            out_second <<= 2;
            
            out_first  |= (first_byte & mask_1) >> 6;
            out_second |= (first_byte & mask_2) >> 4;
            
            first_byte <<= 4;
        }
        
        for (uint8_t i = 0; i < 2; i++)
        {
            out_first  <<= 2;
            out_second <<= 2;
            
            out_first  |= (second_byte & mask_1) >> 6;
            out_second |= (second_byte & mask_2) >> 4;
            
            second_byte <<= 4;
        }

        break;

    case 3:
        mask_1 = 0xF0;
        mask_2 = 0x0F;

        out_first  |= (first_byte & mask_1) >> 4;
        out_second |= (first_byte & mask_2);
        
        out_first  <<= 4;
        out_second <<= 4;

        out_first  |= (second_byte & mask_1) >> 4;
        out_second |= (second_byte & mask_2);

        break;

    case 4:
        out_first = first_byte;
        out_second = second_byte;
        break;

    default:
        break;
    }

    buffer[0] = out_first;
    buffer[1] = out_second;

}

void powermethod_hosvd_tmpd4s2(uint8_t row_1, uint8_t row_2, uint8_t max_it, double tolerance, double *u) {

    uint8_t random_vector = (17 * (int8_t)row_2 + 7) % 251; // "Random" ~ Linear Congruential Generator

    // We still deal in bits
    uint8_t u_first_row = ~(random_vector ^ row_1);
    uint8_t u_second_row = ~(random_vector ^ row_2);

    double H_by_Ht[2][2];
    // double u[2];
    
    // Lets go to doubles
    u[0] = sum_byte_vector_to_double(u_first_row)  + 0.01;
    u[1] = sum_byte_vector_to_double(u_second_row) + 0.01;

    H_by_Ht[0][0] = 8; // sum_byte_vector_to_double(~(u_first_row ^ u_first_row)); !
    H_by_Ht[0][1] = sum_byte_vector_to_double(~(u_first_row ^ u_second_row));
    H_by_Ht[1][0] = H_by_Ht[0][1];
    H_by_Ht[1][1] = 8; // sum_byte_vector_to_double(~(u_second_row ^ u_second_row)); !

    /*
    ! Since we have only -1s and 1s, when we multiply by itself everything adds up to 8.
    */

    double u_old[2];
    double u_norm;

    for(uint8_t iter = 0; iter < max_it; iter++) {     
        u_old[0] = u[0]; 
        u_old[1] = u[1];
        u[0] = H_by_Ht[0][0] * u_old[0] + H_by_Ht[0][1] * u_old[1];
        u[1] = H_by_Ht[1][0] * u_old[0] + H_by_Ht[1][1] * u_old[1];
        
        u_norm = sqrt(u[0]*u[0] + u[1]*u[1]);
        
        u[0] = u[0] / u_norm;
        u[1] = u[1] / u_norm;
    
        u_norm = u[0] - u_old[0]; // why dont we just reuse this
        double temp = u[1] - u_old[1];

        double error = u_norm * u_norm + temp * temp;
        if (error < tolerance) {
            break;
        }
    }

    return;

}

double sum_byte_vector_to_double(uint8_t byte) {
    uint8_t num_1s = bitcount(byte);
    return num_1s - (8 - num_1s);
}

uint8_t bitcount(uint8_t b)
{
    // Magic
     b = b - ((b >> 1) & 0x55);
     b = (b & 0x33) + ((b >> 2) & 0x33);
     return (((b + (b >> 4)) & 0x0F) * 0x01);
}

uint8_t rank_one_detector_tmpd4s2(uint8_t first_recv, uint8_t second_recv, uint8_t prior_knowledge) {

    uint8_t buffer[2];
    double buffer_d[2];
    double factor;
    uint8_t output = 0;
    uint8_t factor_mask = 0x80;

    for (uint8_t mode = 1; mode < 5; mode++) {
        output <<= 2;
        unfold(first_recv, second_recv, mode, buffer);
        powermethod_hosvd_tmpd4s2(buffer[0], buffer[1], 3, 1e-6, buffer_d);
        factor = buffer_d[0] / (prior_knowledge & factor_mask ? 1: -1);
        output |= (buffer_d[0]/factor > 0) ? 0x02 : 0x00;
        output |= (buffer_d[1]/factor > 0) ? 0x01 : 0x00;
        factor_mask >>= 2;
    }
    return output;

}