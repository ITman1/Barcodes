#ifndef DATASEGMENT_H_
#define DATASEGMENT_H_

#include <vector>
#include <opencv2/core/core.hpp>

namespace barcodes {
	using namespace std;
	using namespace cv;
	typedef vector<uint8_t> ByteArray;

	class DataSegment {
	public:
		ByteArray data;
		int mode;
		int flags;
		size_t remainderBits;
	};
}

#endif
