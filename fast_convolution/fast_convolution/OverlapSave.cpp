//
//  OverlapSave.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#include "OverlapSave.h"

OverlapSave::OverlapSave(int total_ir_blocks, int block_size):block_size(block_size){
    
    overlap_save_result = new circularBuffer<float>*[total_ir_blocks];
    buffer_size = 2 * block_size;
    for (int i = 0; i <total_ir_blocks; i++) {
        overlap_save_result[i] = new circularBuffer<float>(block_size);
        
    }
    
}



void OverlapSave::outputOLS(int total_irs) {
    for (int i = 0; i < total_irs; i++) {
        overlap_save_result[i]->print();
    }
}

void OverlapSave::saveAndAddResult(float* cconv) {
    //std::cout << "OLS_PTR: " << ols_ptr << std::endl;
    for (int i = 0; i<block_size; i++) {
        
        float original_sample;
        float sum_sample;
        original_sample = overlap_save_result[ols_ptr]->readFunction(0);
        
        //std::cout << "original_sample: " << original_sample << std::endl;
        overlap_save_result[ols_ptr]->moveReadPtr();
        
        sum_sample = cconv[i] + original_sample;
        //std::cout << "sum: " << sum_sample << std::endl;
        overlap_save_result[ols_ptr]->writeFunction(sum_sample);
        overlap_save_result[ols_ptr]->moveWritePtr();
        
    }
    
}

void OverlapSave::updateOlsPtr(int total_irs) {
    
    ols_ptr = ols_ptr + 1;
    ols_ptr = ols_ptr % total_irs;
    //std::cout << "ols_ptr: " << ols_ptr << std::endl;
    
}

bool OverlapSave::outputIsNotFull(int block_idx, int input_size) {
    if ( (block_idx ) * block_size < input_size) {
        return true;
    }
    return false;
}

void OverlapSave::writeResult(float* channel_out, int num_input_buffers, int input_size) {
    for (int i = 0; i < block_size; i++) {
        //if (i + num_input_buffers * block_size < input_size) {
            channel_out[i + num_input_buffers * block_size] = overlap_save_result[output_rptr]->readFunction(0);
            updateOlsResult(output_rptr);
            overlap_save_result[output_rptr]->moveReadPtr();
            
        //}
//        else {
//            break;
//        }
    }
    
}

void OverlapSave::updateOutputRptr(int total_irs) {
    //std::cout << "before_output_rtpr: " << output_rptr<<std::endl;
    output_rptr = output_rptr + 1;
    output_rptr = output_rptr % total_irs;
    std::cout << "OUTPUT_PTR: " << output_rptr << std::endl;
    
}


void OverlapSave::setOlsPtr(int ptr) {
    ols_ptr = ptr;
}
int OverlapSave::getOutputRptr() {
    return output_rptr;
}
