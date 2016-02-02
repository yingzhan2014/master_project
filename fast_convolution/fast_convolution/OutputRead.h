//
//  OutputRead.h
//  fast_convolution
//
//  Created by Ying Zhan on 2/1/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#ifndef __fast_convolution__OutputRead__
#define __fast_convolution__OutputRead__

#include <iostream>
#include "circularBuffer.h"

class OutputRead {
private:
    circularBuffer<float>* output_buffer;
    int block_size;
    int output_rptr;
    int output_wptr;
public:
    OutputRead(int block_size):block_size(block_size) {
        output_buffer = new circularBuffer<float>(block_size);
        output_wptr = 0;
        output_rptr = 0;
    }
    
    float writeResultToChannel();
    circularBuffer<float>* getOutputBuffer();
    void updateOutputRptr(int total_irs);
    void updateOutputWptr();
    void printOutputResult();
    int getOutputRptr();
 
    
};

#endif /* defined(__fast_convolution__OutputRead__) */
