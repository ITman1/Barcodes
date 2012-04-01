/*
 * LookupTable.h
 *
 *  Created on: 31.3.2012
 *      Author: Scotty
 */

#ifndef LOOKUPTABLE_H_
#define LOOKUPTABLE_H_

#include <map>
#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace std;
using namespace cv;

template <class T1, class T2>
class LookupTable: public map<T1, T2> {
public:
	typedef LookupTable<T1, T2> Base;
	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;

	LookupTable(pair<T1, T2> *first, pair<T1, T2> *last)
		: map<T1, T2>(first, last) {}
	virtual ~LookupTable() {}

	bool correctEncoded(T1 &encoded, int correctLimit) const {
		const_iterator iter;

		T1 _encoded = 0;
		int minDiff = correctLimit + 1;
		for (iter = Base::begin(); iter != Base::end(); iter++) {
			int diff = 0;
			for (unsigned int i = 0; i < 8 * sizeof(T1); i++) {
				diff += ((iter->first >> i) & 0x01) ^ ((encoded >> i) & 0x01);
			}
			if (diff < minDiff) {
				minDiff = diff;
				_encoded = iter->first;
			}
		}

		if (minDiff < correctLimit + 1) {
			encoded = _encoded;
			return true;
		} else {
			return false;
		}
	}
};

} /* namespace barcodes */
#endif /* LOOKUPTABLE_H_ */
