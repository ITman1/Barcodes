/*
 * ReedSolomon.h
 *
 *  Created on: 3.4.2012
 *      Author: Scotty
 */

#ifndef REEDSOLOMON_H_
#define REEDSOLOMON_H_

#include <opencv2/core/core.hpp>

#include "RsDecode.h"

namespace barcodes {
using namespace cv;
using namespace std;

class ReedSolomon {
private:
	int nParity;
	RsDecode decoder;
public:
	ReedSolomon(int nParity) : nParity(nParity), decoder(RsDecode(nParity)) {}
	bool correct(vector<int> &data);
	bool correct(vector<vector<int> > &data);
};

} /* namespace barcodes */
#endif /* REEDSOLOMON_H_ */
