/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include "kronecker-chenc.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

struct ErrorKr {
    size_t num_errors;
    size_t num_totals;
};

int main()
{
    std::ofstream outputFile("results.txt");

    // Check if the file was opened successfully
    if (!outputFile.is_open()) {
        // Write to the file
        std::cout << "Unable to open file.\n";
        return 1;
    }
    
    outputFile << "NUM FLIPPED;" << "NUM ERRORS;" << "TOTAL CHECKS" << std::endl;
    
    #define TRAINING_SYMBOLS 0b10101010
    
    ErrorKr errors_array[17] = {0};
    
    for (uint16_t error_mask = 0; error_mask <= UINT16_MAX; ++error_mask) {
        
        uint8_t num_flipped = bitcount(error_mask) + bitcount(error_mask >> 8);
        
        std::vector<uint8_t> checked_vals;  
        for (uint8_t msg = 0; msg <= UINT8_MAX; msg++) {
            
            if ( std::find(checked_vals.begin(), checked_vals.end(), msg | TRAINING_SYMBOLS) != checked_vals.end() ) {
                if (msg == UINT8_MAX) {
                    break;
                }
                continue;
                
            } else {
                checked_vals.emplace_back(msg | TRAINING_SYMBOLS);
            }
            
            uint8_t *encoded = {0};
            encoded = encode_kronecker_tpmd4s2(msg | TRAINING_SYMBOLS);
            encoded[0] ^= (uint8_t)(error_mask >> 8);
            encoded[1] ^= (uint8_t)error_mask;
            uint8_t decoded = rank_one_detector_tpmd4s2(encoded[0], encoded[1], TRAINING_SYMBOLS);
            
            errors_array[num_flipped].num_totals++;
            
            if (decoded != (msg | TRAINING_SYMBOLS)) {
                errors_array[num_flipped].num_errors++;
            }
            
            if (msg == UINT8_MAX) {
                break;
            }
        }
        
        if (error_mask == UINT16_MAX) {
            break;
        }
    }
    
    for (int i = 0; i < 16; i++) {
        outputFile << i+1 << ";" << +errors_array[i].num_errors << ";" << +errors_array[i].num_totals << std::endl;
    }
    
    outputFile.close();

    return 0;
}