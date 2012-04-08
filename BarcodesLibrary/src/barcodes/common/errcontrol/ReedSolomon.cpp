///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       ReedSolomon.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of class which ensures Reed-Solomon correction.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file ReedSolomon.cpp
 *
 * @brief Defines members of class which ensures Reed-Solomon correction.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <cstdlib>

#include "ReedSolomon.h"

namespace barcodes {

/**
 * Corrects one block of data.
 *
 * @param data Data to be corrected.
 * @return Returns true on success, else false.
 */
bool ReedSolomon::correct(vector<int> &data) {
	return decoder.decode(data) >= 0;
}

/**
 * Corrects vector of blocks of data.
 *
 * @param data Vector of blocks to be corrected.
 * @return Returns true on success, else false.
 */
bool ReedSolomon::correct(vector<vector<int> > &data) {

	vector<vector<int> >::iterator iter;
	bool res = true;
	for (iter = data.begin(); iter != data.end(); iter++) {
		if (!correct(*iter)) res = false;
	}

	return res;
}

} /* namespace barcodes */
