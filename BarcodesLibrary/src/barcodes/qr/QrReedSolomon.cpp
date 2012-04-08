///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrReedSolomon.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members for GaloisQR and QrReedSolomon classes.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrReedSolomon.cpp
 *
 * @brief Defines members for GaloisQR and QrReedSolomon classes.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <cstdlib>

#include "QrReedSolomon.h"

namespace barcodes {

const GaloisQR GaloisQR::INSTANCE;

const GaloisQR& GaloisQR::getInstance() {
	return INSTANCE;
}

} /* namespace barcodes */
