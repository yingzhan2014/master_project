//
//  main.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 11/30/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.

#include "fastConv.h"

int buffer_size = 8;
std::vector<float> h {1,1,2,1,4,3,1,0,2,3};

fastConv fast_conv(buffer_size, h);


void processBlock() {
    int num_samples = 16;
    
    float streaming_input[16] = {1,2,3,4,5,6,7,8,9,10,3,4,5,6,7,8};
    float* output;
    output = fast_conv.process(streaming_input, num_samples);
}



int main() {
    
    for (int i = 0; i < 3; i++) {
        processBlock();
    }
    
}