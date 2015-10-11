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
    double eta;
    double gamma;
    vector<vector<double>> mus;
    vector<double> weights;
    vector<vector<int>> clusters;
public:
    RBFNN(int in_K, int in_inputSize);
    RBFNN(const RBFNN& orig);
    virtual ~RBFNN();
    void trainNetwork(vector<vector<double>> data, vector<double> ys);
    double runModel(vector<double> input);
private:
    double findClustering(vector<vector<double>> data);
    void findMus(vector<vector<double>> data);
    double adeline(vector<vector<double>> data,vector<vector<double>> phi_matrix, vector<double> ys);
    double phi(vector<double> x, vector<double> mu);
};

#endif	/* RBFNN_H */

