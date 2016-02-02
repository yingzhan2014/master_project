//
//  main.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 11/30/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.

#include "fastConv.h"
//#include "OutputRead.h"

int buffer_size = 8;
int block_size = buffer_size / 2;
std::vector<float> h {1,1,2,1,4,3,1,0,2,3,1,6,7,0,2,4,5,3};
CircularConvolution cconv(buffer_size);
IrPartition partitioned_ir(h, block_size);
OverlapSave ols(partitioned_ir.getTotalNumBlocks(), block_size);
InputRead x(buffer_size);
OutputRead y(block_size);


fastConv fast_conv(buffer_size,cconv,partitioned_ir,ols,x, y);


void processBlock(int num_samples, float* input) {
  
    float* channel_out;
    
    channel_out = new float[num_samples];
    
    for (int i = 0; i < num_samples; i++) {
            
        // read juce audiobuffer input to my own input ringbuffer.
        x.readInput(input, num_samples, i);
            
        if (x.isInputBufferFull(x.getInputBuffer())) {
                
            std::cout << "input is full! "<<std::endl;
               
            y.getOutputBuffer()->setWritePtr();
            fast_conv.process();
            y.getOutputBuffer()->setReadPtr();

        }
            
        // write my own output ringbuffer to juce.
        channel_out[i] = y.writeResultToChannel();
        //std::cout << "CHANNEL OUTPUT: "<<channel_out[i]<<std::endl;
    
    }
    
}



int main() {
    
    float* input;
    int input_size[6] = {3,2,6,2,9,15};
    for (int i = 0; i < 6; i++) {
        
        input = new float [input_size[i]];

        for (int j = 0; j < input_size[i]; j ++) {
            input[j] = (j + 1) * (i + 1);
            if (i == 5) {
                input[j] = 0;
            }
        }
        
        processBlock(input_size[i], input);
        
    }
    
}