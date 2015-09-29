/* 
 * File:   main.cpp
 * Author: Austo89
 *
 * Created on September 28, 2015, 7:12 AM
 */

#include <cstdlib>
#include "DataGenerator.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    DataGenerator tom = DataGenerator(3);
    tom.generateData(10);
    
    return 0;
}

