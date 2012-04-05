/*
 * Galois.h
 *
 *  Created on: 5.4.2012
 *      Author: Scotty
 */

#ifndef BARCODES_GALOIS_H_
#define BARCODES_GALOIS_H_

#include <vector>
#include <algorithm>

namespace barcodes {
using namespace std;

typedef vector<int> IntArray;

class Galois {
private:
	static const Galois INSTANCE;
	static const int MAX_MSG_LENGTH = 255;
	IntArray expTbl;
	IntArray logTbl;

	Galois();
	void initGaloisTable();
public:
	static const int POLYNOMIAL = 0x1d;

	static const Galois& getInstance();

	int toExp(int a) const;
	int toLog(int a) const;
	int toPos(int length, int a) const;
	int mul(int a, int b) const;
	int mulExp(int a, int b) const;
	int div(int a, int b) const;
	int divExp(int a, int b) const;
	int inv(int a) const;
	void mulPoly(IntArray &seki, IntArray &a, IntArray &b) const;
	bool calcSyndrome(IntArray &data, int length, IntArray &syn) const;
};

} /* namespace barcodes */
#endif /* BARCODES_GALOIS_H_ */
