///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       RsDecode.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines ReedSolomon Decoder. Ported Masayuki
//             Miyazaki solution written in java into C++.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Project:    Reed-Solomon
// File:       RsDecode.java
// Author:     Masayuki Miyazaki
// URL:        http://sourceforge.jp/projects/reedsolomon/
//
// Brief:      ReedSolomon Decoder
///////////////////////////////////////////////////////////////////////////////

/**
 * @file RsDecode.h
 *
 * @brief Defines ReedSolomon Decoder.
 *
 * @author Masayuki Miyazaki
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 *
 * @see http://sourceforge.jp/projects/reedsolomon/
 */

#ifndef BARCODES_RSDECODE_H_
#define BARCODES_RSDECODE_H_

#include "Galois.h"

namespace barcodes {

class RsDecode {
private:
	const Galois &galois;
	int npar;

	/**
	 * Calculates Error Location(s)
	 * (Chien Search Algorithm)
	 */
	int chienSearch(IntArray &pos, int n, int jisu, IntArray &sigma);

	/**
	 * Calculates Error Magnitude(s) and Corrects Error(s)
	 * (Forney Algorithm)
	 */
	void doForney(IntArray &data, int length, int jisu, IntArray &pos, IntArray &sigma, IntArray &omega);
public:
	static const int RS_PERM_ERROR = -1;
	static const int RS_CORRECT_ERROR = -2;

	RsDecode(int npar, const Galois &galois) : galois(galois), npar(npar) {}

	/**
	 * Calculates Sigma(z), Omega(z) from Syndrome
	 * (Modified Berlekamp-Massey Algorithm)
	 */
	int calcSigmaMBM(IntArray &sigma, IntArray &omega, IntArray &syn);

	/**
	 * Decoding ReedSolomon Code
	 *
	 * @param data Input data
	 * @param length Data length (with parity)
	 * @param noCorrect Error check only
	 * @return    0: has no error
	 *          > 0: number of corrected
	 *          < 0: fail
	 */
	int decode(IntArray &data, int length, bool noCorrect);
	int decode(IntArray &data, int length);
	int decode(IntArray &data);
};

} /* namespace barcodes */
#endif /* BARCODES_RSDECODE_H_ */
