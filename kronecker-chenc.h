#ifndef _KRONECKER_CHENC_H_
#define _KRONECKER_CHENC_H_

#include "Arduino.h"


/**
 * @brief Encodes bits using kronecker encoding for BPSK modulation. The length of symbol vectors is
 * L1 = L2 = L3 = L4 = 2 and scheme 2 is used.
 * 
 * @param packet byte to encode.
 * 
 * @return pointer to array of size 2
 */
uint8_t* encode_kronecker_tmpd4s2(uint8_t packet);

/**
 * @brief Kronecker product between the 2 last bits of 2 bytes. The 2 last bits are in position 0x01 and 0x02.
 * The multiplications is MATLAB-like where elements of the first byte are multiplied with the elements of the second byte.
 * 
 * @param first_side byte whose bits will be multiplied.
 * @param second_side byte to be multiplicated with the other byte's bits.
 * 
 * @return byte with the last 4 bits set to the product result.
 */
uint8_t _kron2(uint8_t first_side, uint8_t second_side);

/**
 * @brief Kronecker product between the bits of the first byte and the bits of the second byte. 
 * The multiplications is MATLAB-like where elements of the first byte are multiplied with the elements of the second byte.
 * 
 * @attention The product is only defined for bytes with "size" 1, 2, 4 or 8 (number of bits to multiply).
 * 
 * @param first_side byte whose bits will be multiplied.
 * @param second_side byte to be multiplicated with the other byte's bits.
 * @param size_first the number of bits to multiply with the second byte, minimum 1. The count starts from the right.
 * @param size_second byte to be multiplicated with the other byte's bits, minimum 1. The count starts from the right.
 * @param buffer array where the kronecker product's result will be stored. Its size is given by $$ceil(size_first * size_second / 8)$$.
 * 
 * @return bool indicating if the product was successful.
 */
bool _kron_product(uint8_t first_side, uint8_t second_side, size_t size_first, size_t size_second, uint8_t *buffer);

/**
 * @brief Applies "reshape" as well as Kolda and Bader unfold. Following the originating paper with RoD, it is equivalent to a reshape(vec, 2, 2, 2, 2)
 * and the subsequent unfolding.
 * 
 * @attention It is only defined for 1st, 2nd, 3rd and 4th mode, and it only works with "vectors" of size 16x1.
 * 
 * @param first_byte first 8 bits of the "vector" to unfold.
 * @param second_byte last 8 bits of the "vector" to unfold.
 * @param mode unfolding mode. It must be in {1, 2, 3, 4}.
 * @param buffer pointer to the array where the result will be stored. Its size must be atleast 2. The values represent a 2x8 matrix.
 * 
 * @return nothing.
 */
void unfold(uint8_t first_byte, uint8_t second_byte, uint8_t mode, uint8_t *buffer);

/**
 * @brief Counts number of bits set to 1.
 * 
 * @param b byte whose 1-bits will be counted.
 * 
 * @link https://forum.arduino.cc/t/counting-1-in-a-byte/547960/13
 * 
 * @return the number of bits set to 1.
 */
uint8_t bitcount(uint8_t b);

/**
 * @brief sums the value of a "byte vector" where 0==-1 and 1==1.
 * 
 * @param byte "byte vector" whose bits will be summed.
 * 
 * @return the sum of the "byte vector"
 */
double sum_byte_vector_to_double(uint8_t byte);

/**
 * @brief Applies the tensor power method detector explained in the original paper.
 * 
 * @attention The 2x8 matrix is represented as A_n in the original paper.
 * 
 * @param row_1 "first row" of a 2x8 matrix.
 * @param row_2 "second row" of a 2x8 matrix.
 * @param max_it maximum number of iterations. It stops the method earlier.
 * @param tolerance acceptable tolerance error. It stops the method earlier.
 * @param u pointer to an array where the results will be saved. Its size must be atleast 2. They are u_n in the original paper.
 * 
 * @return nothing
 */
void powermethod_hosvd_tmpd4s2(uint8_t row_1, uint8_t row_2, uint8_t max_it, double tolerance, double *u);

/**
 * @brief Decodes kronecker-product encoded data using rank one detector.
 * 
 * @attention The implementation is only for BPSK and length of symbol vectors L1 = L2 = L3 = L4 = 2 with scheme 2.
 * 
 * @param first_recv first 8 symbols received.
 * @param second_recv last 8 symbols received.
 * @param prior_knowledge prior knowledge/training data used to decode the data.
 * 
 * @return decoded byte.
 */
uint8_t rank_one_detector_tmpd4s2(uint8_t first_recv, uint8_t second_recv, uint8_t prior_knowledge);

#endif // _KRONECKER_CHENC_H_