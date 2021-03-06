///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       detection.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Runs series of detection process above images.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file detection.cpp
 *
 * @brief Runs series of detection process above images.
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

                QrBarcode::detect(image, detectedMarks);

                std::cout << "Found finder patterns " << string(ent->d_name) << ": " << detectedMarks.size() << endl;

                cvtColor(image, image, CV_GRAY2RGB);
                vector<vector<Point> > drawVec;
                for (unsigned int i = 0; i < detectedMarks.size(); i++) {
                    drawVec.clear(); drawVec.push_back(detectedMarks[i].points);
                    drawContours(image, drawVec, -1, CV_RGB(0,255,0), 2, 8);
                }

                imwrite(string("out/detection/") + string(ent->d_name), image);
          }
      }
      closedir (dir);
    }
    return 0;
}