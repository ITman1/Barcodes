///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       ReedSolomon.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines class which ensures Reed-Solomon correction.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file ReedSolomon.h
 *
 * @brief Defines class which ensures Reed-Solomon correction.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
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
	int nParity;      /**< Number of parity bits */
	RsDecode decoder; /**< Reed-Solomon Decoder */
public:
	/**
	 * Constructs object for Reed-Solomon error correction.
	 *
	 * @param nParity Number of parity bits inside data.
	 * @param galois Reference to Galois field.
	 */
	ReedSolomon(int nParity, const Galois &galois) : nParity(nParity), decoder(RsDecode(nParity, galois)) {}

	/**
	 * Corrects one block of data.
	 *
	 * @param data Data to be corrected.
	 * @return Returns true on success, else false.
	 */
	bool correct(vector<int> &data);

	/**
	 * Corrects vector of blocks of data.
	 *
	 * @param data Vector of blocks to be corrected.
	 * @return Returns true on success, else false.
	 */
	bool correct(vector<vector<int> > &data);
};

} /* namespace barcodes */
#endif /* REEDSOLOMON_H_ */
