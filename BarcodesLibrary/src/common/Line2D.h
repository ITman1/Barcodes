///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Line2D.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the Line2D_ template which encapsulates possible
//             operations with 2D line defined by slope vector and pivot point.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Line2D.h
 *
 * @brief Defines the Line2D_ template which encapsulates possible
 *        operations with 2D line defined by slope vector and pivot point.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef LINE2D_H_
#define LINE2D_H_

#include <opencv2/core/core.hpp>

#include "Vector2D.h"

namespace barcodes {
using namespace cv;

/**
 * Template used for creation of the 2D lines.
 */
template<typename T>
class Line2D_ {
public:
	Point_<T> pivot;			/**< Pivot point of the line. */
	Vector2D_<T> slopeVector;	/**< Slope vector which defines direction of the line */

	/**
	 * Constructs the line from pivot point and slope vector.
	 *
	 * @param pivot Pivot point of the line. Just some point which belongs to line.
	 * @param slopeVector Slope vector of the line which defines direction.
	 */
	Line2D_(Point_<T> pivot, Vector2D_<T> slopeVector) : slopeVector(slopeVector), pivot(pivot) {}

	/**
	 * Constructs the line from two points which lies on the line.
	 *
	 * @param point1 First point which lies on the line.
	 * @param point2 Second point which lies on the line.
	 */
	Line2D_(Point_<T> point1, Point_<T> point2) {
		slopeVector = Vector2D_<T>(point1, point2);
		pivot = point1;
	}
	virtual ~Line2D_() {}

	/**
	 * Calculates intersect of two lines if exists.
	 *
	 * @param line1 First line.
	 * @param line2 Second line.
	 * @param intersect Result intersect.
	 * @return True if there is intersect of two lines, otherwise false.
	 */
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

	/**
	 * Calculates intersect of this line and specified line.
	 *
	 * @param line Line with which should be calculated the intersect.
	 * @param intersect Result intersect.
	 * @return True if there is intersect of two lines, otherwise false.
	 */
	bool intersection(Line2D_<T> line, Point_<T> &intersect) {
	    return intersection(*this, line, intersect);
	}
};

/**
 * Short name for Line_<int> template definition.
 */
typedef Line2D_<int> Line2D;

/**
 * Short name for Line_<float> template definition.
 */
typedef Line2D_<float> Line2Df;
};

#endif /* LINE2D_H_ */
