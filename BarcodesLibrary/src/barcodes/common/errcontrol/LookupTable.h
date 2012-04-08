///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       LookupTable.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines LookupTable which is simple map extended about correction
//             methods.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file LookupTable.h
 *
 * @brief Defines LookupTable which is simple map extended about correction
 *        methods.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef LOOKUPTABLE_H_
#define LOOKUPTABLE_H_

#include <map>
#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace std;
using namespace cv;

/**
 * Class of the lookup table which serves for searching the decoded numbers.
 * Searching can be done by map method find() or search with the error
 * correction with correctEncoded().
 */
template <class T1, class T2>
class LookupTable: public map<T1, T2> {
public:
	typedef LookupTable<T1, T2> Base;
	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;

	/**
	 * Constructs lookup table from the array.
	 *
	 * @param first Pointer to the first member pair.
	 * @param last Pointer to the last member pair.
	 */
	LookupTable(pair<T1, T2> *first, pair<T1, T2> *last)
		: map<T1, T2>(first, last) {}
	virtual ~LookupTable() {}

	/**
	 * Corrects the encoded number if it is possible.
	 *
	 * @param encoded Encoded number/key to correct.
	 * @param correctLimit Number of bits which can be corrected.
	 * @return True on success, false on fail.
	 */
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
