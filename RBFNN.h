/* 
 * File:   RBFNN.h
 * Author: Austo89
 *
 * Created on September 28, 2015, 7:18 AM
 */
#include <vector>
#include <math.h>

using namespace std;

#ifndef RBFNN_H
#define	RBFNN_H



class RBFNN {
    int inputSize;
    int K;
    double gamma;
    vector<double> mus;
    vector<double> weights;
public:
    RBFNN(int in_K, int in_inputSize);
    RBFNN(const RBFNN& orig);
    virtual ~RBFNN();
    void trainNetwork(vector<vector<double>> data);
    double functionApproximation();
private:

};

#endif	/* RBFNN_H */

