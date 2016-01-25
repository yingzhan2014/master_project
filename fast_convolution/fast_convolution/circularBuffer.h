//
//  circularBuffer.h
//  fast_convolution
//
//  Created by Ying Zhan on 11/30/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#ifndef fast_convolution_circularBuffer_h
#define fast_convolution_circularBuffer_h

#include <iostream>
#include <cstddef>
using namespace std;

template<class T>
class circularBuffer {
private:
    int readPtr;
    int writePtr;
    size_t bufferSize; // size_t: type of size
    T* a; // circular buffer
    size_t wrapFunction(int position) { // this function makes sure that you can turn position within buffersize
        //std::cout << "p : " << position << std::endl;
        while (position < 0) {
            position = (size_t)position+ bufferSize;
            //std::cout << "hah : " << position << std::endl;
        }
       // std::cout << "p: "<<position%bufferSize<< std::endl;
        return position%bufferSize;
    }
    
    
public:
    circularBuffer(size_t bufferSize) // constructor function
    :bufferSize(bufferSize), readPtr(0),writePtr(0){
        a=new T[bufferSize];
        for (int i = 0; i < bufferSize; i++) {
            a[i] = 0;
        }
        
    }
    ~circularBuffer(){ //destructor function
        delete[] a;
    }
    
    void print() {
        for (int i = 0; i < bufferSize; i++) {
            std::cout << "this is : " << a[i] << std::endl;
        }

    }
    
    void setWritePtr() {
        writePtr = 0;
    }
    
    void setReadPtr() {
        readPtr = 0;
    }
    int getWritePtr() {
        //std::cout << "writePtr: " << writePtr << std::endl;
        return writePtr;
    }
    
    int getReadPtr() {
        std::cout << "readPtr: " << readPtr << std::endl;
        return readPtr;
    }
    circularBuffer& writeFunction(T const & sample) {
        a[wrapFunction(writePtr)] = sample;
        //writePtr++;
        //std::cout << "content1: " << wrapFunction(writePtr) << std::endl;
        return *this; //this is the pointer point to itself(object of this class)
    }
    circularBuffer& writeFunction(T const& sample, int offset) {
        a[wrapFunction(writePtr + offset)] = sample;
        //writePtr++;
        //std::cout << "content2: " << wrapFunction(writePtr + offset) << std::endl;
        return *this;
    }
    
    T readFunction(int offset) {
        float temp;
        temp = a[wrapFunction(readPtr+offset)];
        
        //std::cout << "ha : " << readPtr<< std::endl;
       // std::cout << "read+offset : "<< (readPtr + offset) % bufferSize << std::endl;
        //std::cout << "ha2 : " << wrapFunction(readPtr + offset) << std::endl;

        return temp;
    }
    
    circularBuffer& moveWritePtr() {
        writePtr++;
        return *this;
    }
    
    circularBuffer& moveReadPtr(){
        readPtr++;
        return *this;
    }
    
};

#endif
