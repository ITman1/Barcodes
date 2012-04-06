#ifndef DATASEGMENT_H_
#define DATASEGMENT_H_

#include <vector>
#include <opencv2/core/core.hpp>

namespace barcodes {
	using namespace cv;
	using namespace std;

	typedef vector<uint8_t> ByteArray;

	class DataSegment {
	public:
		ByteArray data;
		int mode;
		int flags;
		size_t remainderBits;

		DataSegment() : mode(0), flags(0), remainderBits(0) {}
	};

	class DataSegments:public vector<DataSegment> {
	public:
		static const int DATA_SEGMENTS_CORRUPTED = 0x01;
		int flags;

		DataSegments() : flags(0) {}
	};
}

#endif
