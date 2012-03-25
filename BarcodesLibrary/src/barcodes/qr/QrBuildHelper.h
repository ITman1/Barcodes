/*
 * BuildHelper.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef QRBUILDHELPER_H_
#define QRBUILDHELPER_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;

class QrBuildHelper {
public:
	QrBuildHelper() {}
	virtual ~QrBuildHelper() {}

	static Mat buildQrMark(int size);
};

} /* namespace barcodes */
#endif /* QRBUILDHELPER_H_ */
