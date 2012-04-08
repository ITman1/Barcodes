///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Galois.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines Galois Field GF(2^8). Ported Masayuki Miyazaki solution
//             written in java into C++.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Project:    Reed-Solomon
// File:       Galois.java
// Author:     Masayuki Miyazaki
// URL:        http://sourceforge.jp/projects/reedsolomon/
//
// Brief:      Galois Field GF(2^8)
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Galois.h
 *
 * @brief Defines Galois Field GF(2^8).
 *
 * @author Masayuki Miyazaki
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 *
 * @see http://sourceforge.jp/projects/reedsolomon/
 */

#ifndef BARCODES_GALOIS_H_
#define BARCODES_GALOIS_H_

#include <vector>
#include <algorithm>

namespace barcodes {
using namespace std;

typedef vector<int> IntArray;

class Galois {
protected:
	static const int MAX_MSG_LENGTH = 255;
	IntArray expTbl;
	IntArray logTbl;

	void initGaloisTable();
public:
	const int POLYNOMIAL;

	Galois(int polynomial);

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
