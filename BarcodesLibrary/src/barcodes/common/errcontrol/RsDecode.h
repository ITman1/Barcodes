/*
 * RsDecode.h
 *
 *  Created on: 5.4.2012
 *      Author: Scotty
 */

#ifndef BARCODES_RSDECODE_H_
#define BARCODES_RSDECODE_H_

#include "Galois.h"

namespace barcodes {

class RsDecode {
private:
	static const Galois &galois;
	int npar;

	int chienSearch(IntArray &pos, int n, int jisu, IntArray &sigma);
	void doForney(IntArray &data, int length, int jisu, IntArray &pos, IntArray &sigma, IntArray &omega);
public:
	static const int RS_PERM_ERROR = -1;
	static const int RS_CORRECT_ERROR = -2;

	RsDecode(int npar);

	int calcSigmaMBM(IntArray &sigma, IntArray &omega, IntArray &syn);
	int decode(IntArray &data, int length, bool noCorrect);
	int decode(IntArray &data, int length);
	int decode(IntArray &data);
};

} /* namespace barcodes */
#endif /* BARCODES_RSDECODE_H_ */
