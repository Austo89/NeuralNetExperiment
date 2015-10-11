/* 
 * File:   DataGenerator.h
 * Author: Austo89
 *
 * Created on September 28, 2015, 7:13 AM
 */

#include <vector>

#ifndef DATAGENERATOR_H
#define	DATAGENERATOR_H

using namespace std;

class DataGenerator {
    int inputNum;
public:
    DataGenerator(int in_inputNum);
    DataGenerator(const DataGenerator& orig);
    virtual ~DataGenerator();
    void generateData(int dataNum);
    static void normalizeData(vector<vector<float>>& data);
private:

};

#endif	/* DATAGENERATOR_H */

