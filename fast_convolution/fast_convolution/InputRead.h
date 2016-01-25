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
    InputRead(int buffer_size):buffer_size(buffer_size) {
        //input = channel_in;
        //std::cout << "input: "<<input<< std::endl;
        input_buffer = new circularBuffer<float>(buffer_size);
        block_size = buffer_size / 2;
        block_idx = 0;
        //std::cout << "buffer_size: "<<buffer_size << std::endl;
        cpx_input = new std::vector<std::complex<float>>(buffer_size);
        cpx_input_fft = new std::vector<std::complex<float>>(buffer_size);
//        for (int i = 0; i < 24; i++) {
//            std::cout << input[i] << std::endl;
//        }
        //zeropadded_input = new float[zeropadded_input_len];
        
    }
    
    std::vector<std::complex<float>>* getCpxInput() {
        return cpx_input;
    }
    
    std::vector<std::complex<float>>* getCpxInputFFT() {
        return cpx_input_fft;
    }
    void outputCpxFFT() {
        for (int i = 0; i < buffer_size; i++) {
            std::cout << "cpx_fft: " << cpx_input_fft->operator[](i) << std::endl;
        }
        
    }
    circularBuffer<float>* getInputBuffer() {
        return input_buffer;
        
    }

    
    void readInput(float* input,int input_size, int sample_idx) {
        
        float sample;
        sample = input_buffer->readFunction(block_size);
        //std::cout << "sample_idx: "<<sample_idx << std::endl;
       // std::cout << "input:  " << input[sample_idx] << std::endl;
        input_buffer->moveReadPtr();
        input_buffer->writeFunction(sample);
        if (sample_idx < input_size) {
            input_buffer->writeFunction(input[sample_idx], block_size);
            input_buffer->moveWritePtr();
        }
        else {
            input_buffer->writeFunction(0, block_size);
            input_buffer->moveWritePtr();
            
        }
        
        
        
        
//        if ( input_buffer->getWritePtr()% block_size == 0 ) {
//            
//            input_buffer->setReadPtr();
//            input_buffer->setWritePtr();
//            std::cout << "block_idx"<< std::endl;
//            
//            for (int j = 0; j< buffer_size; j++) {
//                
//                cpx_input->operator[](j) = input_buffer->readFunction(j);
//                std::cout << "input: " << input_buffer->readFunction(j) << std::endl;
//                std::cout << "cpx_input" << cpx_input->operator[](j) << std::endl;
//                
//            }
//        
//        }
    }
    void updateBlockIdx() {
        block_idx = block_idx + 1;
    }
    
    void clearBlockIdx() {
        block_idx = 0;
    }
    
    int getBlockIdx() {
        std::cout << "BLOCK_IDX: " << block_idx << std::endl;
        return block_idx;
    }
    
    bool isInputBufferFull(circularBuffer<float>* input_buffer) {
        if (input_buffer->getWritePtr() % block_size == 0) {
            input_buffer->setReadPtr();
            input_buffer->setWritePtr();
            
            return true;
        }
        return false;
    }
    
    void writeCpxInput(circularBuffer<float>* input_buffer) {
        
        for (int i = 0; i<buffer_size; i++) {
            cpx_input->operator[](i) = input_buffer->readFunction(i);
            std::cout << "input: " << input_buffer->readFunction(i) << std::endl;
            //std::cout << "cpx_input" << cpx_input->operator[](i) << std::endl;
            
        }
        
        
    }
    
    


    
    
    
//    void readInput(float* zeropadded_input, int zeropadded_input_len, circularBuffer<float>* input_buffer, int block_size, std::vector<std::complex<float>>*cpx_input) {
//        
//        for (int sample_idx = 0; sample_idx < zeropadded_input_len ; sample_idx ++) {
//            
//           
//            float sample;
//            
//            
//            sample = input_buffer->readFunction(block_size);
//
//            input_buffer->moveReadPtr();
//            input_buffer->writeFunction(sample);
//            input_buffer->writeFunction(zeropadded_input[sample_idx], block_size);
//            input_buffer->moveWritePtr();
//            
//            
//            if ( input_buffer->getWritePtr()% block_size == 0 ) {
//                
//                input_buffer->setReadPtr();
//                input_buffer->setWritePtr();
//                std::cout << "block_idx"<< std::endl;
//                
//                for (int j = 0; j< 2* block_size; j++) {
//                    
//                    cpx_input->operator[](j) = input_buffer->readFunction(j);
//                    std::cout << "input: " << input_buffer->readFunction(j) << std::endl;
//                    
//                }
//            }
//        
//    }
//    }
//    
};



#endif /* defined(__fast_convolution__InputRead__) */
