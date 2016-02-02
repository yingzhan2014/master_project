//
//  fastConv.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#include "fastConv.h"

void fastConv::process() {
    
    x.writeCpxInput(x.getInputBuffer());
    cconv.calFFT(x.getCpxInput(), x.getCpxInputFFT());
    
    for (int num_ir_block = 0; num_ir_block < partitioned_ir.getTotalNumBlocks(); num_ir_block++) {
        
        // cal dot product between input and one block of ir
        cconv.calDotProduct(partitioned_ir.getIrBlockFFT(num_ir_block), x.getCpxInputFFT(), buffer_size);
        
        // cal ifft of dot product
        cconv.calCircularConv(cconv.getDotProduct());
        
        // here need to save and add conv result to overlapsave ring buffer
        ols.saveAndAddResult(cconv.getConvResult());
        
        //update overlapsave pointer
        ols.updateOlsPtr(partitioned_ir.getTotalNumBlocks());
        
    }
    
    ols.updateOlsPtr(partitioned_ir.getTotalNumBlocks());
    
    // write conv result into my own output ringbuffer
    ols.writeResult(y.getOutputBuffer(),y.getOutputRptr());
    y.updateOutputWptr();
    y.updateOutputRptr(partitioned_ir.getTotalNumBlocks());
    
    
    // cout some results to debug
    y.printOutputResult();
    ols.outputOLS(partitioned_ir.getTotalNumBlocks());
    
}





