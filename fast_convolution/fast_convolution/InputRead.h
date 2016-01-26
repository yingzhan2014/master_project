//
//  InputRead.h
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#ifndef __fast_convolution__InputRead__
#define __fast_convolution__InputRead__

#include <iostream>
#include "complex"
#include "circularBuffer.h"
#include "vector"


class InputRead {
    
private:
      //float* input;
      //float* zeropadded;
      //int zeropadded_input_len;
      int block_size;
      int buffer_size;
      int block_idx;
      circularBuffer<float>* input_buffer;
      std::vector<std::complex<float>>* cpx_input;
      std::vector<std::complex<float>>* cpx_input_fft;
    
    
public:
    InputRead(int buffer_size);
    std::vector<std::complex<float>>* getCpxInput();
    std::vector<std::complex<float>>* getCpxInputFFT();
    
    circularBuffer<float>* getInputBuffer();
    void readInput(float* input,int input_size, int sample_idx);
    void updateBlockIdx();
    void clearBlockIdx();
    int getBlockIdx();
    bool isInputBufferFull(circularBuffer<float>* input_buffer);
    void writeCpxInput(circularBuffer<float>* input_buffer);
    
    void outputCpxFFT();
};



#endif /* defined(__fast_convolution__InputRead__) */
