//
//  IrPartition.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#include "IrPartition.h"

IrPartition::IrPartition(std::vector<float>&h, int block_size): block_size(block_size) {
    
    ir = &h;
    ir_len = int(h.size());
    buffer_size = 2 * block_size;
    total_ir_blocks = ceil( ir_len / float(block_size) );
    
    ir_blocks = new std::vector<std::complex<float>>* [total_ir_blocks];
    ir_blocks_fft = new std::vector<std::complex<float>>*[total_ir_blocks];
    
}

int IrPartition::getTotalNumBlocks() {
    return total_ir_blocks;
}



void IrPartition::partitionIr() {
    
    for (int ir_block_idx = 0; ir_block_idx < total_ir_blocks; ir_block_idx++) {
        //std::cout << "block_idx: " << ir_block_idx << std::endl;
        ir_blocks[ir_block_idx] = new std::vector<std::complex<float>>(buffer_size);
        ir_blocks_fft[ir_block_idx] = new std::vector<std::complex<float>>(buffer_size);
        
        for (int sample = 0; sample < block_size; sample ++) {
            
            if (ir_block_idx * block_size + sample < ir_len) {
                
                ir_blocks[ir_block_idx]->operator[](sample) = ir->operator[](ir_block_idx*block_size + sample);
                ir_blocks[ir_block_idx]->operator[](sample + block_size) = 0;
                
            }
            else {
                ir_blocks[ir_block_idx]->operator[](sample) = 0;
                ir_blocks[ir_block_idx]->operator[](sample + block_size) = 0;
            }
            
        }
        
//        for (int j = 0; j < buffer_size; j++) {
//            std::cout << ir_blocks[ir_block_idx]->operator[](j) << std::endl;
//        }
//        
        //FFT.transform(ir_blocks[ir_block_idx]->data(), ir_blocks_fft[ir_block_idx]->data());
        
    }
    
    
}

