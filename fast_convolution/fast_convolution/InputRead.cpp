//
//  InputRead.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#include "InputRead.h"

InputRead::InputRead(int buffer_size):buffer_size(buffer_size) {
    
    input_buffer = new circularBuffer<float>(buffer_size);
    block_size = buffer_size / 2;
    block_idx = 0;
    cpx_input = new std::vector<std::complex<float>>(buffer_size);
    cpx_input_fft = new std::vector<std::complex<float>>(buffer_size);
}


std::vector<std::complex<float>>* InputRead::getCpxInput() {
    return cpx_input;
}

std::vector<std::complex<float>>* InputRead::getCpxInputFFT() {
    return cpx_input_fft;
}

circularBuffer<float>* InputRead::getInputBuffer() {
    return input_buffer;
    
}


void InputRead::readInput(float* input,int input_size, int sample_idx) {
    
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
    
}

void InputRead::updateBlockIdx() {
    block_idx = block_idx + 1;
}

void InputRead::clearBlockIdx() {
    block_idx = 0;
}

int InputRead::getBlockIdx() {
    //std::cout << "BLOCK_IDX: " << block_idx << std::endl;
    return block_idx;
}

bool InputRead::isInputBufferFull(circularBuffer<float>* input_buffer) {
    if (input_buffer->getWritePtr() % block_size == 0) {
        input_buffer->setReadPtr();
        input_buffer->setWritePtr();
        
        return true;
    }
    return false;
}

void InputRead::writeCpxInput(circularBuffer<float>* input_buffer) {
    
    for (int i = 0; i<buffer_size; i++) {
        cpx_input->operator[](i) = input_buffer->readFunction(i);
        std::cout << "input: " << input_buffer->readFunction(i) << std::endl;
        //std::cout << "cpx_input" << cpx_input->operator[](i) << std::endl;
        
    }
    
}

void InputRead::outputCpxFFT() {
    for (int i = 0; i < buffer_size; i++) {
        std::cout << "cpx_fft: " << cpx_input_fft->operator[](i) << std::endl;
    }
    
}

