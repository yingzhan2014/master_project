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
#include "IrPartition.h"
#include "circularBuffer.h"


// this class stores infos of convolution results.

class OverlapSave {
private:
    int output_wptr = 0;
    int output_rptr = 0;
    int ols_ptr = 0;
    //int ols_rptr = 0;
    int block_size;
    int buffer_size;
    circularBuffer<float>** overlap_save_result;
    
    void updateOlsResult(int output_rptr) {
        overlap_save_result[output_rptr]->writeFunction(0);
        overlap_save_result[output_rptr]->moveWritePtr();
    }
    
    
public:
    
    OverlapSave(int total_ir_blocks, int block_size):block_size(block_size){
        
        overlap_save_result = new circularBuffer<float>*[total_ir_blocks];
        buffer_size = 2 * block_size;
        for (int i = 0; i <total_ir_blocks; i++) {
            overlap_save_result[i] = new circularBuffer<float>(block_size);
            
        }
        
    }
    void outputOLS(int total_irs) {
        for (int i = 0; i < total_irs; i++) {
            //std::cout << "num_ir_block: " << i <<  std::endl;
            overlap_save_result[i]->print();
        }
    }
    void updateWptr(int total_ir_blocks) {
        output_wptr = output_wptr + 1;
        output_wptr = output_wptr % total_ir_blocks;
    }
    
    void saveAndAddResult(float* cconv) {
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
    
    void updateOlsPtr(int total_irs) {
        
        ols_ptr = ols_ptr + 1;
        ols_ptr = ols_ptr % total_irs;
        //std::cout << "ols_ptr: " << ols_ptr << std::endl;
        
    }
    
    bool outputIsNotFull(int block_idx, int input_size) {
        if ( (block_idx ) * block_size < input_size) {
            return true;
        }
        return false;
    }
    
    void writeResult(float* channel_out, int num_input_buffers, int input_size) {
        for (int i = 0; i < block_size; i++) {
            if (i + num_input_buffers * block_size < input_size) {
                channel_out[i + num_input_buffers * block_size] = overlap_save_result[output_rptr]->readFunction(0);
                //std::cout << "channle_idx: " << i + num_input_buffers * block_size << std::endl;
                //std::cout << "channel_out: " << channel_out[i + num_input_buffers * block_size] << std::endl;
                updateOlsResult(output_rptr);
                overlap_save_result[output_rptr]->moveReadPtr();

            }
            else {
                break;
            }
        }
        
    }
    
    void updateOutputRptr(int total_irs) {
        //std::cout << "before_output_rtpr: " << output_rptr<<std::endl;
        output_rptr = output_rptr + 1;
        output_rptr = output_rptr % total_irs;
        std::cout << "OUTPUT_PTR: " << output_rptr << std::endl;
        
    }
    
   
    void setOlsPtr(int ptr) {
        ols_ptr = ptr;
    }
    int getOutputRptr() {
        return output_rptr;
    }
    void setOutputWptr(int ptr) {
        output_wptr = ptr;
    }
    int getOutputWptr() {
        return output_wptr;
    }
};


#endif /* defined(__fast_convolution__OverlapSave__) */
