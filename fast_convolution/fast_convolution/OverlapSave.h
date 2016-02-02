//
//  OverlapSave.h
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#ifndef __fast_convolution__OverlapSave__
#define __fast_convolution__OverlapSave__

#include <iostream>
#include "circularBuffer.h"

// this class is designed for save and add convolution result of every block of input


class OverlapSave {
private:
    //int output_wptr = 0;
    //int output_rptr = 0;
    int ols_ptr = 0;
    int block_size;
    int buffer_size;
    //circularBuffer<float>* output;
    
    // overlap_save_result: save reuslt of convolution between blocked-input and blocked-ir.
    // two dimensions array: each row stores result of conv between blocked_input with different blocked_ir, the size of this array is ( number_of_ir_blocks * block_size )
    circularBuffer<float>** overlap_save_result;
    
    void updateOlsResult(int output_rptr) {
        overlap_save_result[output_rptr]->writeFunction(0);
        overlap_save_result[output_rptr]->moveWritePtr();
    }
    
    
public:
    
    OverlapSave(int total_ir_blocks, int block_size);
    
    // saveAndAddResult: save current block convolution result and add with the previous result.
    void saveAndAddResult(float* cconv);
    
    // outputIsNotFull: if channel_out (output array: same size with buffer_size in juce) is not
    // full, then keep writing result into channel_out, otherwide 'break';
    
    
    // writeResult into output_buffer.
    void writeResult(circularBuffer<float>* output, int output_rptr);
    
    
    //void setOlsPtr(int ptr);
    //void updateOutputRptr(int total_irs);
   // int getOutputRptr();
    void updateOlsPtr(int total_irs);
    //void updateOutputWptr();
    
    // this function is to debug
    void outputOLS(int total_irs);
    //void printOutputResult();
    

    
};


#endif /* defined(__fast_convolution__OverlapSave__) */
