///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       GridSampler.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines GridSampler class which serves for sampling the bit
//             matrix and retrieving the bit array.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file GridSampler.h
 *
 * @brief Defines GridSampler class which serves for sampling the bit
 *        matrix and retrieving the bit array.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef GRIDSAMPLER_H_
#define GRIDSAMPLER_H_

#include <opencv2/core/core.hpp>

#include "BitMatrix.h"

namespace barcodes {
using namespace cv;

/**
 * GridSampler class implements grid reading mechanism which reads codewords
 * from the bit matrix and pushed them into bit array.
 */
class GridSampler {
public:
	/**
	 * Enumeration of all possible directions by which can be sampled.
	 */
	typedef enum {
		BOTTOM = 0x01,//!< BOTTOM
		LEFT   = 0x02,//!< LEFT
		RIGHT  = 0x04,//!< RIGHT
		TOP    = 0x08 //!< TOP
	} Direction;

	/**
	 * Enumeration of all possible directions by which to sample if first direction failed.
	 */
	typedef enum {
		BOTTOM2 = BOTTOM << 4,
		TOP2    = TOP    << 4,
		LEFT2   = LEFT   << 4,
		RIGHT2  = RIGHT  << 4
	} Direction2;

	/**
	 * Enumeration which groups possible sample directions and directions
	 * by which to sample if the end of the grid is exceeded.
	 */
	typedef enum {
		RIGHT_TOP      = RIGHT  | TOP2,   //!< RIGHT_TOP
		RIGHT_BOTTOM   = RIGHT  | BOTTOM2,//!< RIGHT_BOTTOM
		LEFT_TOP       = LEFT   | TOP2,   //!< LEFT_TOP
		LEFT_BOTTOM    = LEFT   | BOTTOM2,//!< LEFT_BOTTOM
		TOP_RIGHT      = TOP    | RIGHT2, //!< TOP_RIGHT
		TOP_LEFT       = TOP    | LEFT2,  //!< TOP_LEFT
		BOTTOM_RIGHT   = BOTTOM | RIGHT2, //!< BOTTOM_RIGHT
		BOTTOM_LEFT    = BOTTOM | LEFT2   //!< BOTTOM_LEFT
	} FlowDirection;

	/**
	 * Constructs grid sampler.
	 *
	 * @param gridSize Size of the sampling grid.
	 * @param initBitsFlow Flow of the bits inside sample grid.
	 * @param initSampleFlow Flow of the sample grid.
	 * @param bitsMirror Specifies whether should be the first direction for bit sampling
	 *        mirrored when the end is exceeded or returned to the start position.
	 * @param sampleMirror Specifies whether should be the first direction for grid sampling
	 *        mirrored when the end is exceeded or returned to the start position.
	 */
	GridSampler(Size gridSize, FlowDirection initBitsFlow, FlowDirection initSampleFlow, bool bitsMirror, bool sampleMirror)
		: gridSize(gridSize), initBitsFlow(initBitsFlow), initSampleFlow(initSampleFlow),
		  bitsMirror(bitsMirror), sampleMirror(sampleMirror) {}
	virtual ~GridSampler() {}

	/**
	 * Samples the bit matrix and returns sampled bit array.
	 *
	 * @param code Bit matrix to be sampled.
	 * @param result Result bit array.
	 * @param mask Mask due which can be omitted some values.
	 */
	void sample(BitMatrix &code, BitArray &result, BitMatrix *mask = 0);
protected:
	/**
	 * Size of the sampling grid.
	 */
	Size gridSize;

	/**
	 * Flow of the bits inside sample grid.
	 */
	FlowDirection initBitsFlow;

	/**
	 * Flow of the sample grid.
	 */
	FlowDirection initSampleFlow;

	/**
	 * Specifies whether should be the first direction for bit sampling
	 * mirrored when the end is exceeded or returned to the start position.
	 */
	bool bitsMirror;

	/**
	 * Specifies whether should be the first direction for grid sampling
	 * mirrored when the end is exceeded or returned to the start position.
	 */
	bool sampleMirror;

	/**
	 * Returns the position from which should be started to sample.
	 *
	 * @param _gridSize Size of the bit matrix from which position should be returned.
	 * @param flowDirection Flow of the sampling.
	 * @return Point from which should be started.
	 */
	Point getStartPosition(Size _gridSize, FlowDirection flowDirection);
};

} /* namespace barcodes */
#endif /* GRIDSAMPLER_H_ */
