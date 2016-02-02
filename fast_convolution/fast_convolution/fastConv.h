//
//  fastConv.h
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#ifndef __fast_convolution__fastConv__
#define __fast_convolution__fastConv__

#include <iostream>
#include "IrPartition.h"
#include "InputRead.h"
#include "CircularConv.h"
#include "OverlapSave.h"
#include "OutputRead.h"




class fastConv {
private:
    
    int block_size;
    int buffer_size;
    int num_ir_blocks;
    
    std::vector<float> h;
    CircularConvolution& cconv;
    IrPartition& partitioned_ir;
    OverlapSave& ols;
    InputRead& x;
    OutputRead& y;

public:
    
    fastConv(int buffer_size, CircularConvolution& cconv, IrPartition& partitioned_ir, OverlapSave& ols, InputRead& x, OutputRead& y):buffer_size(buffer_size), block_size(buffer_size / 2), cconv(cconv),partitioned_ir(partitioned_ir), ols(ols), x(x), y(y){
        
        partitioned_ir.partitionIr();
        
        for (int ir_block_idx = 0; ir_block_idx < partitioned_ir.getTotalNumBlocks(); ir_block_idx++) {
            
            cconv.calFFT(partitioned_ir.getIrBlock(ir_block_idx), partitioned_ir.getIrBlockFFT(ir_block_idx));
            
        }
        
    }
    
    void process();
    
    
        };


#endif /* defined(__fast_convolution__fastConv__) */
