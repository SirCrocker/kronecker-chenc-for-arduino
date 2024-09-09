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
void encode_kronecker();

void powermethod_hosvd();
void rank_one_detector();
void unfold();


#endif // _KRONECKER_CHENC_H_