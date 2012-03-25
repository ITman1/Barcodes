/*
 * Line2D.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef LINE2D_H_
#define LINE2D_H_

#include <opencv2/core/core.hpp>

#include "Vector2D.h"

namespace barcodes {
using namespace cv;

template<typename T>
class Line2D_ {
public:
	Point_<T> pivot;
	Vector2D_<T> slopeVector;

	Line2D_() {}
	Line2D_(Point_<T> pivot, Vector2D_<T> slopeVector) : slopeVector(slopeVector), pivot(pivot) {}
	Line2D_(Point_<T> point1, Point_<T> point2) {
		slopeVector = Vector2D_<T>(point1, point2);
		pivot = point1;
	}
	virtual ~Line2D_() {}

	static bool intersection(Line2D_<T> line1, Line2D_<T> line2, Point_<T> &intersect) {
		Point_<T> x = line2.pivot - line1.pivot;
		Point_<T> d1 = line1.slopeVector;
		Point_<T> d2 = line2.slopeVector;

		float inter = d1.x * d2.y - d1.y * d2.x;

		if (fabs(inter) < 1e-8) {
		    return false;
		}

		double t1 = (x.x * d2.y - x.y * d2.x) / inter;
		intersect = line1.pivot + d1 * t1;

		return true;
	}

	bool intersection(Line2D_<T> line, Point_<T> &intersect) {
	    return intersection(*this, line, intersect);
	}
};

typedef Line2D_<int> Line2D;
typedef Line2D_<float> Line2Df;
};

#endif /* LINE2D_H_ */
