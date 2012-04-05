/*
 * ReedSolomon.cpp
 *
 *  Created on: 3.4.2012
 *      Author: Scotty
 */

#include <cstdlib>

#include "ReedSolomon.h"

namespace barcodes {

bool ReedSolomon::correct(vector<int> &data) {
	return decoder.decode(data) >= 0;
}

bool ReedSolomon::correct(vector<vector<int> > &data) {

	vector<vector<int> >::iterator iter;
	bool res = true;
	for (iter = data.begin(); iter != data.end(); iter++) {
		if (!correct(*iter)) res = false;
	}

	return res;
}

} /* namespace barcodes */
