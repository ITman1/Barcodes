///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrReedSolomon.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines Galois field for QR codes and QR code error correction.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrReedSolomon.h
 *
 * @brief Defines Galois field for QR codes and QR code error correction.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRREEDSOLOMON_H_
#define QRREEDSOLOMON_H_

#include <opencv2/core/core.hpp>

#include "../common/errcontrol/ReedSolomon.h"

namespace barcodes {
using namespace cv;
using namespace std;

/**
 * Defines galois field for QR code which uses generator polynomial: x^8 + x^4 + x^3 + x^2 + 1.
 */
class GaloisQR:public Galois {
private:
	static const GaloisQR INSTANCE;
public:
	GaloisQR() : Galois(0x1d) {}

	static const GaloisQR& getInstance();
};

/**
 * Class which ensures error correction of QR codes.
 */
class QrReedSolomon:public ReedSolomon {
public:
	/**
	 * Constructs correction object for specified number of parity bits.
	 *
	 * @param nParity Number of parity bits inside data to be corrected.
	 */
	QrReedSolomon(int nParity) : ReedSolomon(nParity, GaloisQR::getInstance()) {}
};

} /* namespace barcodes */
#endif /* QRREEDSOLOMON_H_ */
