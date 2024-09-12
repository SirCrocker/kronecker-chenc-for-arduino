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
void encode_kronecker_tmpd4s2(uint8_t packet);

/**
 * @brief Kronecker product of 2 bytes with 2 useful bits each in LSP
 */
uint8_t _kron2(uint8_t first_side, uint8_t second_side);

/**
 * @brief Kronecker product for bytes with "length" 1, 2, 4 or 8
 */
bool _kron_product(uint8_t first_side, uint8_t second_side, size_t size_first, size_t size_second, uint8_t *buffer) {

void powermethod_hosvd();
void rank_one_detector();
void unfold();


#endif // _KRONECKER_CHENC_H_