//
//  CircularConv.h
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#ifndef __fast_convolution__CircularConv__
#define __fast_convolution__CircularConv__

#include <iostream>

#include <iostream>
#include "vector"
#include "complex"
#include "/Users/cherry1991/Desktop/fast_convolution/fast_convolution/kiss_fft/kissfft.hh"

class CircularConvolution {
private:
    std::vector<std::complex<float>>* dot_product;
    std::vector<std::complex<float>>* cconv_result;
    float* real_conv;
    int buffer_size;
    int block_size;
    kissfft<float> FFT;
    kissfft<float> iFFT;
    float ifft_scalar;
public:
    
    CircularConvolution(int buffer_size);
    
    void calFFT(std::vector<std::complex<float>>* time_domain, std::vector<std::complex<float>>*freq_domain);
    
    void calDotProduct(std::vector<std::complex<float>>* cpx_input_fft, std::vector<std::complex<float>>* cpx_ir_fft, int buffer_size);
    
    std::vector<std::complex<float>>* getDotProduct();
    
    void calCircularConv(std::vector<std::complex<float>>* dot_product);
    
    float* getConvResult();
    
    // these functions for debugging
    void outputDotProduct();
    void outputRealConv();
    void outputConv();
    
};

#endif /* defined(__fast_convolution__CircularConv__) */
