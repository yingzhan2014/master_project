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




class fastConv {
private:

    int input_size;
    int block_size;
    int buffer_size;
    int padded_input_len;
    
    float* channel_out;
    int num_ir_blocks;
    std::vector<float> h;
    CircularConvolution& cconv;
    IrPartition& partitioned_ir;
    OverlapSave& ols;
    InputRead& x;

public:
    
    fastConv(int buffer_size, CircularConvolution& cconv, IrPartition& partitioned_ir, OverlapSave& ols, InputRead& x):buffer_size(buffer_size), block_size(buffer_size / 2), cconv(cconv),partitioned_ir(partitioned_ir), ols(ols), x(x){
        
        
        
        channel_out = new float[input_size];
        
        partitioned_ir.partitionIr();
        
        for (int ir_block_idx = 0; ir_block_idx < partitioned_ir.getTotalNumBlocks(); ir_block_idx++) {
            
            cconv.calFFT(partitioned_ir.getIrBlock(ir_block_idx), partitioned_ir.getIrBlockFFT(ir_block_idx));
            
        }
        
    }
    
    
    float* process(float* channel_in, int input_size) {
        
        padded_input_len = ceil( (float(input_size) / block_size) ) * block_size + block_size;
        
        for (int i = 0; i < padded_input_len; i++) {
            x.readInput(channel_in, input_size, i);
            
            if (x.isInputBufferFull(x.getInputBuffer())) {
                x.writeCpxInput(x.getInputBuffer());
                cconv.calFFT(x.getCpxInput(), x.getCpxInputFFT());
                for (int num_ir_block = 0; num_ir_block < partitioned_ir.getTotalNumBlocks(); num_ir_block++) {
                    //std::cout << "num_ir_block: " << num_ir_block << std::endl;
                    cconv.calDotProduct(partitioned_ir.getIrBlockFFT(num_ir_block), x.getCpxInputFFT(), buffer_size);
                    cconv.calCircularConv(cconv.getDotProduct());
                    float* real_conv;
                    real_conv = cconv.getConvResult();
                    // here need to save and add conv result at the same time
                    ols.saveAndAddResult(real_conv);
                    ols.updateOlsPtr(partitioned_ir.getTotalNumBlocks());
                    
                }
                ols.updateOlsPtr(partitioned_ir.getTotalNumBlocks());
                
                if (ols.outputIsNotFull(x.getBlockIdx(),input_size)) {
                    ols.writeResult(channel_out, x.getBlockIdx(), input_size);
                    ols.updateOutputRptr(partitioned_ir.getTotalNumBlocks());
                    
                }
                else{
                    x.clearBlockIdx();
                    break;
                }
                
                x.updateBlockIdx();
                
            }
        }
        
        ols.setOlsPtr(ols.getOutputRptr());
        
        for (int k = 0; k < input_size; k++) {
            std::cout << "OUTPUT: "<< channel_out[k] << std::endl;
        }
        
        ols.outputOLS(partitioned_ir.getTotalNumBlocks());
        return channel_out;
    }
    
    };


#endif /* defined(__fast_convolution__fastConv__) */
