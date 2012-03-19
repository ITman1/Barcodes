#ifndef BARCODES_BARCODE_H_
#define BARCODES_BARCODE_H_

#include <string>

#include "../Image.h"

namespace barcodes {
	using namespace std;
	using namespace cv;

	struct DetectedMark {
	public:
		vector<Point> points;
		double match;
		int flags;
	};

	class Barcode {
	public:
		Barcode() {}
		virtual ~Barcode() {}

		virtual bool loadFromFile(string filename);
		virtual bool loadFromImage(Image &image);

		virtual void detect(Image &image, vector<DetectedMark> &detectedMarks, int flags = 0);
		virtual void read(Image &image, ByteArray &data, int flags = 0);

		virtual Image *createImage();
	};
}

#endif
