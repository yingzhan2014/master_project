//
//  IrPartition.h
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#ifndef __fast_convolution__IrPartition__
#define __fast_convolution__IrPartition__

#include <iostream>
#include "vector"
#include "cmath"
#include "complex"
//#include "/Users/cherry1991/Desktop/fast_convolution/fast_convolution/kiss_fft/kissfft.hh"


class IrPartition {
    
private:
    std::vector<float>* ir;
    int ir_len;
    int block_size;
    int buffer_size;
    int total_ir_blocks;

    std::vector<std::complex<float>>** ir_blocks;
    std::vector<std::complex<float>>** ir_blocks_fft;
    
public:
    IrPartition(std::vector<float>&h, int block_size);
    //~IrPartition();
    void partitionIr();
    int getTotalNumBlocks();
    void outputIrBlockFFT(int block_idx) {
        
         std::vector<std::complex<float>>* ir_fft;
        ir_fft = ir_blocks_fft[block_idx];
        //std::cout << "block_idx_ir_fft: " << block_idx << std::endl;
//        for (int i = 0; i < buffer_size; i++) {
//            std::cout << "this is ir_blocks_fft: " << ir_fft->operator[](i) << std::endl;
//        }

        
    }
    
    
    std::vector<std::complex<float>>* getIrBlock(int block_idx) {
        
        std::vector<std::complex<float>>* ir;
        ir = ir_blocks[block_idx];
        //std::cout<< "block_idx_ir:" << block_idx << std::endl;
//        for (int i = 0; i < block_size; i++) {
//            
//            std::cout << "this is ir_blocks: " << ir->operator[](i) << std::endl;
//        }
        return ir_blocks[block_idx];
    }
    
    std::vector<std::complex<float>>* getIrBlockFFT(int block_idx) {
        
       
                return ir_blocks_fft[block_idx];
    }
    
};



#endif /* defined(__fast_convolution__IrPartition__) */
