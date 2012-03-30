/*
 * Vector2D.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace barcodes {
using namespace cv;

template<typename T>
class Vector2D_ {
public:
	T dx;
	T dy;

	Vector2D_() : dx(0), dy(0) {}

	Vector2D_(Point_<T> vec) {
		dx = vec.x;
		dy = vec.y;
	}

	Vector2D_(Point_<T> p1, Point_<T> p2) {
		dx = p2.x - p1.x;
		dy = p2.y - p1.y;
	}

	Vector2D_(T dx, T dy) {
		this->dx = dx;
		this->dy = dy;
	}

	double size() {
		return sqrt(dx * dx + dy * dy);
	}

	double angle() {
		return fastAtan2(dy, dx);
	}

	Vector2D_<T> resize(double newSize) {
		double size = this->size();
		dx *= newSize / size;
		dy *= newSize / size;

		return *this;
	}

	Vector2D_<T> rotate(double angle) {
		Mat rotMat = getRotationMatrix2D(Point2f(0, 0), angle, 1.0);
		vector<Point_<T> > pointVec;
		pointVec.push_back(Point_<T>(dx, dy));
		transform(pointVec, pointVec, rotMat);
		dx = pointVec[0].x;
		dy = pointVec[0].y;

		return *this;
	}

	Vector2D_<T> operator+ (const Vector2D_<T>& plusVec) const {
		Vector2D_<T> vec;
		vec.dx = this->dx + plusVec.dx;
		vec.dy = this->dy + plusVec.dy;

		return vec;
	}

	Vector2D_<T> operator* (const T &multConst) const {
		Vector2D_<T> vec;
		vec.dx = this->dx * multConst;
		vec.dy = this->dy * multConst;

		return vec;
	}

	Vector2D_<T> operator* (const Vector2D_<T> &multVec) const {
		Vector2D_<T> vec;
		vec.dx = this->dx * multVec.dx;
		vec.dy = this->dy * multVec.dy;

		return vec;
	}

	Point_<T> operator+ (const Point_<T> &plusPoint) const {
		Point_<T> point;
		point.x = this->dx + plusPoint.x;
		point.y = this->dy + plusPoint.y;

		return point;
	}

	operator Point_<T>() {
		return Point_<T>(dx, dy);
	}
};

typedef Vector2D_<int> Vector2D;
typedef Vector2D_<float> Vector2Df;
}

#endif /* VECTOR2D_H_ */
