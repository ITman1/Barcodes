///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Galois.cpp
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
 * @file Galois.cpp
 *
 * @brief Defines Galois Field GF(2^8).
 *
 * @author Masayuki Miyazaki
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 *
 * @see http://sourceforge.jp/projects/reedsolomon/
 */

#include "Galois.h"

namespace barcodes {

Galois::Galois(int polynomial) : POLYNOMIAL(polynomial) {
	initGaloisTable();
}

void Galois::initGaloisTable() {
	expTbl.resize(MAX_MSG_LENGTH * 2);
	logTbl.resize(MAX_MSG_LENGTH);

	int d = 1;
	for(int i = 0; i < 255; i++) {
		expTbl[i] = expTbl[255 + i] = d;
		logTbl[d] = i;
		d <<= 1;
		if((d & 0x100) != 0) {
			d = (d ^ POLYNOMIAL) & 0xff;
		}
	}
}

int Galois::toExp(int a) const {
	return expTbl[a];
}

int Galois::toLog(int a) const {
	return logTbl[a];
}

int Galois::toPos(int length, int a) const {
	return length - 1 - logTbl[a];
}

int Galois::mul(int a, int b) const {
	return (a == 0 || b == 0)? 0 : expTbl[logTbl[a] + logTbl[b]];
}

int Galois::mulExp(int a, int b) const {
	return (a == 0)? 0 : expTbl[logTbl[a] + b];
}

int Galois::div(int a, int b) const {
	return (a == 0)? 0 : expTbl[logTbl[a] - logTbl[b] + 255];
}

int Galois::divExp(int a, int b) const {
	return (a == 0)? 0 : expTbl[logTbl[a] - b + 255];
}

int Galois::inv(int a) const {
	return expTbl[255 - logTbl[a]];
}

void Galois::mulPoly(IntArray &seki, IntArray &a, IntArray &b) const {
	fill(seki.begin(),seki.end(), 0);
	for (unsigned int ia = 0; ia < a.size(); ia++) {
		if(a[ia] != 0) {
			int loga = logTbl[a[ia]];
			int ib2 = min(b.size(), seki.size() - ia);
			for(int ib = 0; ib < ib2; ib++) {
				if(b[ib] != 0) {
					seki[ia + ib] ^= expTbl[loga + logTbl[b[ib]]];
				}
			}
		}
	}
}

bool Galois::calcSyndrome(IntArray &data, int length, IntArray &syn) const {
	int hasErr = 0;
	for (unsigned int i = 0; i < syn.size();  i++) {
		int wk = 0;
		for(int idx = 0; idx < length; idx++) {
			 wk = data[idx] ^ ((wk == 0)? 0 : expTbl[logTbl[wk] + i]);
		}
		syn[i] = wk;
		hasErr |= wk;
	}
	return hasErr == 0;
}

} /* namespace barcodes */
