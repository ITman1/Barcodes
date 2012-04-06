///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Vector2D.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the Vector2D_ template which encapsulates possible
//             operations with 2D vector defined by dx and dy differences.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Vector2D.h
 *
 * @brief Defines the Vector2D_ template which encapsulates possible
 *        operations with 2D vector defined by dx and dy differences.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace barcodes {
using namespace cv;

/**
 * Template for creation 2D vectors which are defined as a difference of two points.
 */
template<typename T>
class Vector2D_ {
public:
	T dx;	/**< Difference on X axis. */
	T dy;	/**< Difference on Y axis. */

	Vector2D_() : dx(0), dy(0) {}

	/**
	 * Constructs 2D vector from one point. Coordinates of this point will be used
	 * as a vector dimensions.
	 *
	 * @param vec Point that stores the vector.
	 */
	Vector2D_(Point_<T> vec) {
		dx = vec.x;
		dy = vec.y;
	}

	/**
	 * Constructs 2D vector from two point. Vector is calculated from the difference
	 * of these points.
	 *
	 * @param p1 First point where vector starts.
	 * @param p2 Second point where vector ends.
	 */
	Vector2D_(Point_<T> p1, Point_<T> p2) {
		dx = p2.x - p1.x;
		dy = p2.y - p1.y;
	}

	/**
	 * Constructs 2D vector from specified vector dy, dy differences.
	 *
	 * @param dx Difference on the first dimension (x axis).
	 * @param dy Difference on the second dimension (y axis).
	 */
	Vector2D_(T dx, T dy) {
		this->dx = dx;
		this->dy = dy;
	}

	/**
	 * Returns size of the vector.
	 *
	 * @return Size of the vector.
	 */
	double size() {
		return sqrt(dx * dx + dy * dy);
	}

	/**
	 * Returns angle under which lies this vector.
	 *
	 * @return Angle under which lies this vector.
	 */
	double angle() {
		return fastAtan2(dy, dx);
	}

	/**
	 * Resizes the vector.
	 *
	 * @param newSize New size of the vector.
	 * @return Vector with a new size.
	 */
	Vector2D_<T> resize(double newSize) {
		double size = this->size();
		dx *= newSize / size;
		dy *= newSize / size;

		return *this;
	}

	/**
	 * Rotates the vector.
	 *
	 * @param angle The angle about which should be vector rotated.
	 * @return Rotated vector.
	 */
	Vector2D_<T> rotate(double angle) {
		Mat rotMat = getRotationMatrix2D(Point2f(0, 0), angle, 1.0);
		vector<Point_<T> > pointVec;
		pointVec.push_back(Point_<T>(dx, dy));
		transform(pointVec, pointVec, rotMat);
		dx = pointVec[0].x;
		dy = pointVec[0].y;

		return *this;
	}

	/**
	 * Adds a vector to this one and returns the result.
	 *
	 * @param plusVec The vector to be added to this one.
	 * @return Result vector.
	 */
	Vector2D_<T> operator+ (const Vector2D_<T>& plusVec) const {
		Vector2D_<T> vec;
		vec.dx = this->dx + plusVec.dx;
		vec.dy = this->dy + plusVec.dy;

		return vec;
	}

	/**
	 * Multiplies this vector by constant.
	 *
	 * @param multConst The constant which multiplies this vector.
	 * @return Result vector.
	 */
	Vector2D_<T> operator* (const T &multConst) const {
		Vector2D_<T> vec;
		vec.dx = this->dx * multConst;
		vec.dy = this->dy * multConst;

		return vec;
	}

	/**
	 * Multiplies this vector by another vector.
	 *
	 * @param multVec The vector which multiplies this vector.
	 * @return Result vector.
	 */
	Vector2D_<T> operator* (const Vector2D_<T> &multVec) const {
		Vector2D_<T> vec;
		vec.dx = this->dx * multVec.dx;
		vec.dy = this->dy * multVec.dy;

		return vec;
	}

	/**
	 * Adds this vector to a point and returns result point.
	 *
	 * @param plusPoint The point which be offset by this vector.
	 * @return Result point.
	 */
	Point_<T> operator+ (const Point_<T> &plusPoint) const {
		Point_<T> point;
		point.x = this->dx + plusPoint.x;
		point.y = this->dy + plusPoint.y;

		return point;
	}

	/**
	 * Converts vector to the point.
	 */
	operator Point_<T>() {
		return Point_<T>(dx, dy);
	}
};

/**
 * Short name for Vector2D_<int> template definition.
 */
typedef Vector2D_<int> Vector2D;

/**
 * Short name for Vector2D_<float> template definition.
 */
typedef Vector2D_<float> Vector2Df;
}

#endif /* VECTOR2D_H_ */
