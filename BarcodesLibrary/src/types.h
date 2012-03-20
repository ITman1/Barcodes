/*
 * types.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef BARCODES_TYPES_H_
#define BARCODES_TYPES_H_

#include <opencv2/core/core.hpp>
#include <vector>
#include <cstdarg>

#ifdef TARGET_DEBUG
	#ifdef ANDROID
		#include <android/log.h>
		#define DEBUG_TAG "QrBarcode.cpp"

		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(string("sdcard/") + string(filename), image)
		#define DEBUG_PRINT(tag, format, ...)	__android_log_print(ANDROID_LOG_DEBUG, string(tag).c_str(), string(format).c_str(), ## __VA_ARGS__)
	#else
		#include <cstdio>

		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(filename, image)
		#define DEBUG_PRINT(tag, format, ...) printf((string(tag) + string(": ") + string(format) + string("\n")).c_str(), ## __VA_ARGS__)
	#endif

#else
	#define DEBUG_WRITE_IMAGE(filename, image)
	#define DEBUG_PRINT(tag, text, args...)
#endif

using namespace std;

namespace barcodes {
	using namespace cv;

	typedef vector<uchar> ByteArray;

	template<typename T>
	class Vector2D_ {
	public:
		T dx;
		T dy;

		Vector2D_() : dx(0), dy(0) {}

		Vector2D_(Point_<T> p1, Point_<T> p2) {
			dx = p2.x - p1.x;
			dy = p2.y - p1.y;
		}

		double size() {
			return sqrt(dx * dx + dy * dy);
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

	template <class T>
	string toStr(const T &value) {
		stringstream ss;
		ss << value;
		return ss.str();
	}
};

#endif /* TYPES_H_ */
