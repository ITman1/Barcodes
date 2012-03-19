
#include "Barcode.h"

using namespace barcodes;

bool Barcode::loadFromFile(string filename) {
	return false;
}

bool Barcode::loadFromImage(Image &image) {
	return false;
}

void Barcode::detect(Image &image, vector<DetectedMark> &detectedMarks, int flags) {
	detectedMarks.clear();
}

void Barcode::read(Image &image, ByteArray &data, int flags) {
	data.clear();
}

Image *Barcode::createImage() {
	return NULL;
}
