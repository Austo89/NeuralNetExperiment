/* 
 * File:   RBFDataPoint.cpp
 * Author: Austo89
 * 
 * Created on October 5, 2015, 6:21 PM
 */

#include "RBFDataPoint.h"

RBFDataPoint::RBFDataPoint(int in_inputSize) {
    inputSize = in_inputSize;
}

RBFDataPoint::RBFDataPoint(const RBFDataPoint& orig) {
}

RBFDataPoint::~RBFDataPoint() {
}

RBFDataPoint::setData(double in[]){
    for (int i = 0; i < inputSize; i++){
        x[i] = in[i];
    }
}

