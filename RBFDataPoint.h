/* 
 * File:   RBFDataPoint.h
 * Author: Austo89
 *
 * Created on October 5, 2015, 6:21 PM
 */

#ifndef RBFDATAPOINT_H
#define	RBFDATAPOINT_H

class RBFDataPoint {
    int x[];
    int inputSize;
public:
    RBFDataPoint(int in_inputSize);
    RBFDataPoint(const RBFDataPoint& orig);
    virtual ~RBFDataPoint();
private:

};

#endif	/* RBFDATAPOINT_H */

