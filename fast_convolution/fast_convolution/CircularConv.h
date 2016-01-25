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
    
    CircularConvolution(int buffer_size): buffer_size(buffer_size),FFT(buffer_size, false), iFFT(buffer_size, true) {
        block_size = buffer_size / 2;
        dot_product = new std::vector<std::complex<float>> (buffer_size);
        cconv_result = new std::vector<std::complex<float>> (buffer_size);
        real_conv = new float (block_size);
        ifft_scalar = 1.0 / buffer_size;
        
    }
    
    void outputDotProduct() {
        for (int i = 0; i < buffer_size; i++) {
            std::cout << "dot_product: " << dot_product->operator[](i) << std::endl;
        }
    }

    
    void calFFT(std::vector<std::complex<float>>* time_domain, std::vector<std::complex<float>>*freq_domain) {
        
        FFT.transform(time_domain->data(), freq_domain->data());
//        for (int i = 0; i < buffer_size; i++) {
//            std::cout << "fft: " << freq_domain->operator[](i) << std::endl;
//        }
        
        
    }
    
//    void calIFFT(std::vector<std::complex<float>>* freq_domain, std::vector<std::complex<float>>* time_domain) {
//        
//        iFFT.transform(freq_domain->data(), time_domain->data());
//    }
    
    std::vector<std::complex<float>>* getDotProduct() {
        return dot_product;
    }
    
    float* getConvResult() {
        for (int i = 0; i < block_size; i++) {
            //std::cout << "i+block: " << i + block_size << std::endl;
            //std::cout << "cconv!: " << ifft_scalar << std::endl;
            real_conv[i] = ifft_scalar * (cconv_result->operator[](i+block_size).real());
            //std::cout << "real_conv1: " << real_conv[i] << std::endl;
        }
        
        return real_conv;
    }
    
    
    void outputRealConv() {
        for (int i = 0; i < block_size; i++) {
            std::cout << "real_conv: " << real_conv[i] << std::endl;
        }
    }
    
    void outputConv() {
        for (int i = 0; i < buffer_size; i++) {
            std::cout << cconv_result->operator[](i) << std::endl;
        }
    }
    void calCircularConv(std::vector<std::complex<float>>* dot_product) {
        iFFT.transform(dot_product->data(), cconv_result->data());
        
    }
    
    
    void calDotProduct(std::vector<std::complex<float>>* cpx_input_fft, std::vector<std::complex<float>>* cpx_ir_fft, int buffer_size) {
     
        for (int sample = 0; sample < buffer_size; sample ++) {
                
                dot_product->operator[](sample) = cpx_input_fft->operator[](sample) * cpx_ir_fft->operator[](sample);
        }
    }
    
};

#endif /* defined(__fast_convolution__CircularConv__) */
