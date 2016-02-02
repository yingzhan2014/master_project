//
//  CircularConv.cpp
//  fast_convolution
//
//  Created by Ying Zhan on 1/22/16.
//  Copyright (c) 2016 Ying Zhan. All rights reserved.
//

#include "CircularConv.h"

CircularConvolution::CircularConvolution(int buffer_size): buffer_size(buffer_size),FFT(buffer_size, false), iFFT(buffer_size, true) {
    block_size = buffer_size / 2;
    dot_product = new std::vector<std::complex<float>> (buffer_size);
    cconv_result = new std::vector<std::complex<float>> (buffer_size);
    real_conv = new float (block_size);
    ifft_scalar = 1.0 / buffer_size;
    
}



void CircularConvolution::calFFT(std::vector<std::complex<float>>* time_domain, std::vector<std::complex<float>>*freq_domain) {
    
    FFT.transform(time_domain->data(), freq_domain->data());
    
}


void CircularConvolution::calDotProduct(std::vector<std::complex<float>>* cpx_input_fft, std::vector<std::complex<float>>* cpx_ir_fft, int buffer_size) {
    
    for (int sample = 0; sample < buffer_size; sample ++) {
        
        dot_product->operator[](sample) = cpx_input_fft->operator[](sample) * cpx_ir_fft->operator[](sample);
    }
}


std::vector<std::complex<float>>* CircularConvolution::getDotProduct() {
    return dot_product;
}




void CircularConvolution::calCircularConv(std::vector<std::complex<float>>* dot_product) {
    iFFT.transform(dot_product->data(), cconv_result->data());
    
}

float* CircularConvolution::getConvResult() {
    for (int i = 0; i < block_size; i++) {
        real_conv[i] = ifft_scalar * (cconv_result->operator[](i+block_size).real());
        //std::cout << "real_conv1: " << real_conv[i] << std::endl;
    }
    
    return real_conv;
}




void CircularConvolution::outputRealConv() {
    for (int i = 0; i < block_size; i++) {
        std::cout << "real_conv: " << real_conv[i] << std::endl;
    }
}

void CircularConvolution::outputConv() {
    for (int i = 0; i < buffer_size; i++) {
        std::cout << cconv_result->operator[](i) << std::endl;
    }
}

void CircularConvolution::outputDotProduct() {
    for (int i = 0; i < buffer_size; i++) {
        std::cout << "dot_product: " << dot_product->operator[](i) << std::endl;
    }
}


