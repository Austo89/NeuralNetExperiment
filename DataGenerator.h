/* 
 * File:   DataGenerator.h
 * Author: Austo89
 *
 * Created on September 28, 2015, 7:13 AM
 */

#ifndef DATAGENERATOR_H
#define	DATAGENERATOR_H

class DataGenerator {
    int inputNum;
public:
    DataGenerator(int in_inputNum);
    DataGenerator(const DataGenerator& orig);
    virtual ~DataGenerator();
    void generateData(int dataNum);
private:

};

#endif	/* DATAGENERATOR_H */

