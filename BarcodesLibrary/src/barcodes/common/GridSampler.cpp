/*
 * GridSampler.cpp
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#include "../../common/Vector2D.h"

#include "GridSampler.h"

namespace barcodes {

inline bool OUT_OF_BOUND(Size &_gridSize, Point &point) {
	return (point.x >= _gridSize.width) || (point.y >= _gridSize.height)
			|| (point.x < 0) || (point.y < 0);
}

inline void BIT_OFFSET_DIMENSION(GridSampler::Direction direction, Point &point) {
	switch (direction) {
		case GridSampler::LEFT:   point.x -= 1; break;
		case GridSampler::RIGHT:  point.x += 1; break;
		case GridSampler::TOP:    point.y -= 1; break;
		case GridSampler::BOTTOM: point.y += 1; break;
	}
}

inline void GET_OFFSET_DIMENSION(GridSampler::Direction direction, Point &point) {
	switch (direction) {
		case GridSampler::LEFT:   point.y = 0; point.x = -1; break;
		case GridSampler::RIGHT:  point.y = 0; point.x =  1; break;
		case GridSampler::TOP:    point.x = 0; point.y = -1; break;
		case GridSampler::BOTTOM: point.x = 0; point.y =  1; break;
	}
}

inline void STRIP_2ND_DIMENSION(GridSampler::Direction direction, Point &point) {
	switch (direction) {
		case GridSampler::LEFT:   point.y = 0; break;
		case GridSampler::RIGHT:  point.y = 0; break;
		case GridSampler::TOP:    point.x = 0; break;
		case GridSampler::BOTTOM: point.x = 0; break;
	}
}

inline void INIT_DIMENSION(GridSampler::Direction direction, Size &_gridSize, Point &point) {
	switch (direction) {
		case GridSampler::LEFT:   point.x = _gridSize.width - 1; break;
		case GridSampler::RIGHT:  point.x = 0; break;
		case GridSampler::TOP:    point.y = _gridSize.height - 1; break;
		case GridSampler::BOTTOM: point.y = 0; break;
	}
}

inline GridSampler::Direction REFLECT_DIRECTION(GridSampler::Direction direction) {
	switch (direction) {
		case GridSampler::LEFT:
			return GridSampler::RIGHT;
		case GridSampler::RIGHT:
			return GridSampler::LEFT;
		case GridSampler::TOP:
			return GridSampler::BOTTOM;
		default:
			return GridSampler::TOP;
	}
}

Point GridSampler::getStartPosition(Size _gridSize, FlowDirection flowDirection) {
	if ((_gridSize.width < 1) || (_gridSize.height < 1)) return Point(-1, -1);

	Direction direction1 = Direction(initBitsFlow & 0x0F);
	Direction direction2 = Direction((initBitsFlow & 0xF0) >> 4);

	if ((direction1 == direction2) || (direction1 == REFLECT_DIRECTION(direction2))) return Point(-1, -1);

	Point point = Point(-1, -1);
	INIT_DIMENSION(direction1, _gridSize, point);
	INIT_DIMENSION(direction2, _gridSize, point);

	return point;
}

// for now it is neccessary to have the same direction of sampling and bits, 2nd flow dimensions the same
void GridSampler::sample(BitMatrix &code, BitArray &result, BitMatrix *mask) {
	result.clear();

	Size codeSize = Size(code.cols, code.rows);

	Point outerStartPosition = getStartPosition(codeSize, initSampleFlow);
	if ((outerStartPosition.x == -1) || (outerStartPosition.y == -1)) return;

	Point innerStartPosition = getStartPosition(gridSize, initBitsFlow);
	if ((innerStartPosition.x == -1) || (innerStartPosition.y == -1)) return;

	Direction  bitsDirection1   = Direction(initBitsFlow & 0x0F);
	Direction  bitsDirection2   = Direction((initBitsFlow & 0xF0)   >> 4);
	Direction  sampleDirection1 = Direction(initSampleFlow & 0x0F);
	Direction  sampleDirection2 = Direction((initSampleFlow & 0xF0) >> 4);

	Point offsetDim1(0, 0);
	Point offsetDim2(0, 0);
	Point gridDim1Pos(0, 0); INIT_DIMENSION(bitsDirection1, gridSize, gridDim1Pos);

	Point currPoint = outerStartPosition + innerStartPosition;
	if (sampleDirection1 == LEFT || sampleDirection2 == LEFT) currPoint -= Point(gridSize.width - 1, 0);
	if (sampleDirection1 == TOP || sampleDirection2 == TOP) currPoint -= Point(0, gridSize.height - 1);

	while (!OUT_OF_BOUND(codeSize, currPoint)) {
		if ((!mask) || (mask->getBit(currPoint))) {
			bool bit = code.getBit(currPoint);
			result.pushBit(bit);
		}

		GET_OFFSET_DIMENSION(bitsDirection1, offsetDim1);
		BIT_OFFSET_DIMENSION(bitsDirection1, gridDim1Pos);

		if (OUT_OF_BOUND(gridSize, gridDim1Pos)) {
			if (bitsMirror) {
				bitsDirection1 = REFLECT_DIRECTION(bitsDirection1);
				offsetDim1 = Point(0, 0);
			} else {
				offsetDim1 = Vector2D(offsetDim1) * Vector2D(gridSize.width - 1, gridSize.height - 1) * -1;
			}
			INIT_DIMENSION(bitsDirection1, gridSize, gridDim1Pos);

			GET_OFFSET_DIMENSION(bitsDirection2, offsetDim2);
		} else {
			offsetDim2 = Point(0, 0);
		}

		Point offset = offsetDim1 + offsetDim2;
		Point _currPoint = currPoint + offset;

		if (OUT_OF_BOUND(codeSize, _currPoint)) {
			if (sampleMirror) {
				bitsDirection2 = REFLECT_DIRECTION(bitsDirection2);
				sampleDirection1 = REFLECT_DIRECTION(sampleDirection1);
			}

			GET_OFFSET_DIMENSION(sampleDirection2, offsetDim1);
			offset = Vector2D(_currPoint) + Vector2D(offsetDim1) * Vector2D(gridSize.width, gridSize.height);
			INIT_DIMENSION(sampleDirection1, codeSize, offset);
			offset -= currPoint;
		}

		currPoint += offset;
	}
}



} /* namespace barcodes */
