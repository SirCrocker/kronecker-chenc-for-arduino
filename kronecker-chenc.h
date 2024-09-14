#ifndef _KRONECKER_CHENC_H_
#define _KRONECKER_CHENC_H_

#include "Arduino.h"


/**
 * @brief Encodes bits. 
 * 
 * @param first columv vector 1 of tupe uint8t
 * @param second columv vector 2 of tupe uint8t
 * @param buffer where the column vectors will be saved
 * 
 * @return blah
 */
uint8_t* encode_kronecker_tmpd4s2(uint8_t packet);

/**
 * @brief Kronecker product of 2 bytes with 2 useful bits each in LSP
 */
uint8_t _kron2(uint8_t first_side, uint8_t second_side);

/**
 * @brief Kronecker product for bytes with "length" 1, 2, 4 or 8
 */
bool _kron_product(uint8_t first_side, uint8_t second_side, size_t size_first, size_t size_second, uint8_t *buffer);

/**
 * @brief Applies Kolda and Bader unfold 
 * 
 * @return n-mode unfold of tensor tenX.
 */
void unfold(uint8_t first_byte, uint8_t second_byte, uint8_t mode, uint8_t *buffer);

/**
 * @brief counts 1 bits in a byte
 * 
 * @link https://forum.arduino.cc/t/counting-1-in-a-byte/547960/13
 */
uint8_t bitcount(uint8_t b);

/**
 * @brief transform a byte vector to a double
 */
double byte_vector_to_double(uint8_t byte);

/**
 * @brief waaaluigi
 * 
 * @attention U HAS TO BE a 2 element vector!!!
 */
void powermethod_hosvd(uint8_t row_1, uint8_t row_2, uint8_t max_it, double tolerance, double *u);

/**
 * @brief decode using rank one detector
 * 
 * @return decoded byte
 */
uint8_t rank_one_detector(uint8_t first_recv, uint8_t second_recv);

#endif // _KRONECKER_CHENC_H_