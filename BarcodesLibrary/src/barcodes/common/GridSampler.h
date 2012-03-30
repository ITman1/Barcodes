/*
 * GridSampler.h
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#ifndef GRIDSAMPLER_H_
#define GRIDSAMPLER_H_

#include <opencv2/core/core.hpp>

#include "BitMatrix.h"

namespace barcodes {
using namespace cv;
class GridSampler {
public:
	typedef enum {
		BOTTOM = 0x01,
		LEFT   = 0x02,
		RIGHT  = 0x04,
		TOP    = 0x08
	} Direction;

	typedef enum {
		BOTTOM2 = BOTTOM << 4,
		TOP2    = TOP    << 4,
		LEFT2   = LEFT   << 4,
		RIGHT2  = RIGHT  << 4
	} Direction2;

	typedef enum {
		RIGHT_TOP      = RIGHT  | TOP2,
		RIGHT_BOTTOM   = RIGHT  | BOTTOM2,
		LEFT_TOP       = LEFT   | TOP2,
		LEFT_BOTTOM    = LEFT   | BOTTOM2,
		TOP_RIGHT      = TOP    | RIGHT2,
		TOP_LEFT       = TOP    | LEFT2,
		BOTTOM_RIGHT   = BOTTOM | RIGHT2,
		BOTTOM_LEFT    = BOTTOM | LEFT2
	} FlowDirection;

	GridSampler(Size gridSize, FlowDirection initBitsFlow, FlowDirection initSampleFlow, bool bitsMirror, bool sampleMirror)
		: gridSize(gridSize), initBitsFlow(initBitsFlow), initSampleFlow(initSampleFlow),
		  bitsMirror(bitsMirror), sampleMirror(sampleMirror) {}
	virtual ~GridSampler() {}

	void sample(BitMatrix &code, BitArray &result, BitMatrix *mask = 0);
protected:
	Size gridSize;
	FlowDirection initBitsFlow;
	FlowDirection initSampleFlow;
	bool bitsMirror;
	bool sampleMirror;

	Point getStartPosition(Size _gridSize, FlowDirection flowDirection);
};

} /* namespace barcodes */
#endif /* GRIDSAMPLER_H_ */
