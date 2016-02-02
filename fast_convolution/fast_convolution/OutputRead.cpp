//
//  OutputRead.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 2/1/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#include "OutputRead.h"

circularBuffer<float>* OutputRead::getOutputBuffer() {
    return output_buffer;
}

void OutputRead::printOutputResult() {
    std::cout << "OUTPUT RESULT" << std::endl;
    output_buffer->print();
    std::cout << "////////////" << std::endl;
    
}


float OutputRead::writeResultToChannel() {
    float sample;
    sample = output_buffer->readFunction(0);
    output_buffer->moveReadPtr();
    return sample;
    
}

int OutputRead::getOutputRptr() {
    return output_rptr;
}

void OutputRead::updateOutputWptr() {
    output_wptr = output_wptr + 1;
}

void OutputRead::updateOutputRptr(int total_irs) {
    //std::cout << "before_output_rtpr: " << output_rptr<<std::endl;
    output_rptr = output_rptr + 1;
    output_rptr = output_rptr % total_irs;
    //std::cout << "OUTPUT_PTR: " << output_rptr << std::endl;
    
}

