///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       decoding.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Runs series of decoding process above images.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file decoding.cpp
 *
 * @brief Runs series of decoding process above images.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>
#include <string>
#include <dirent.h>

#include <barlib/barcodes/qr/QrBarcode.h>

using namespace barcodes;

int main() {
    mkdir("out");
    mkdir("out/detection");

    DetectedMarks detectedMarks;
    DIR *dir;
    struct dirent *ent;
    dir = opendir ("img/detection");
    if (dir != NULL) {
      while ((ent = readdir (dir)) != NULL) {
          if (*ent->d_name != '.') {
                Image image = Image::fromFileGrayscale(string("img/detection/") + string(ent->d_name));
				DataSegments dataSegments;
				QrBarcode::decode(image, dataSegments);

				if (dataSegments.size() > 0) {
					std::cout << "Found data segments " << string(ent->d_name) << ": " << string((const char *)&dataSegments[0].data[0], dataSegments[0].data.size()) << endl;
					std::cout << "Data segments length " << string(ent->d_name) << ": " << dataSegments.size() << endl;
					std::cout << "Data segments errors " << string(ent->d_name) << ": " << string((dataSegments.flags != 0)? "YES" : "NO") << endl;
				} else {
					std::cout << "No data segments found " << string(ent->d_name) << "!" << endl;
				}
          }
      }
      closedir (dir);
    }
    return 0;
}
