/*
 * RsDecode.cpp
 *
 *  Created on: 5.4.2012
 *      Author: Scotty
 */

#include "RsDecode.h"

namespace barcodes {

	const Galois &RsDecode::galois = Galois::getInstance();

	RsDecode::RsDecode(int npar) {
		this->npar = npar;
	}

	int RsDecode::calcSigmaMBM(IntArray &sigma, IntArray &omega, IntArray &syn) {
		IntArray sg0(npar);
		IntArray sg1(npar);
		sg0[1] = 1;
		sg1[0] = 1;
		int jisu0 = 1;
		int jisu1 = 0;
		int m = -1;

		for(int n = 0; n < npar; n++) {
			int d = syn[n];
			for(int i = 1; i <= jisu1; i++) {
				d ^= galois.mul(sg1[i], syn[n - i]);
			}
			if(d != 0) {
				int logd = galois.toLog(d);
				IntArray wk(npar);
				for(int i = 0; i <= n; i++) {
					wk[i] = sg1[i] ^ galois.mulExp(sg0[i], logd);
				}
				int js = n - m;
				if(js > jisu1) {
					m = n - jisu1;
					jisu1 = js;
					if(jisu1 > npar / 2) {
						return -1;
					}
					for(int i = 0; i <= jisu0; i++) {
						sg0[i] = galois.divExp(sg1[i], logd);
					}
					jisu0 = jisu1;
				}
				sg1 = wk;
			}
			copy(sg0.begin(), sg0.begin() + min((int)(sg0.size() - 1), jisu0), sg0.begin() + 1);
			sg0[0] = 0;
			jisu0++;
		}
		galois.mulPoly(omega, sg1, syn);
		copy(sg1.begin(), sg1.begin() + min(sg1.size(), sigma.size()), sigma.begin());
		return jisu1;
	}

	int RsDecode::chienSearch(IntArray &pos, int n, int jisu, IntArray &sigma) {

		int last = sigma[1];

		if(jisu == 1) {
			if(galois.toLog(last) >= n) {
				return RS_CORRECT_ERROR;
			}
			pos[0] = last;
			return 0;
		}

		int posIdx = jisu - 1;
		for(int i = 0; i < n; i++) {
			int z = 255 - i;
			int wk = 1;
			for(int j = 1; j <= jisu; j++) {
				wk ^= galois.mulExp(sigma[j], (z * j) % 255);
			}
			if(wk == 0) {
				int pv = galois.toExp(i);
				last ^=  pv;
				pos[posIdx--] = pv;
				if(posIdx == 0) {
					if(galois.toLog(last) >= n) {
						return RS_CORRECT_ERROR;
					}
					pos[0] = last;
					return 0;
				}
			}
		}

		return RS_CORRECT_ERROR;
	}

	void RsDecode::doForney(IntArray &data, int length, int jisu, IntArray &pos, IntArray &sigma, IntArray &omega) {
		for(int i = 0; i < jisu; i++) {
			int ps = pos[i];
			int zlog = 255 - galois.toLog(ps);

			int ov = omega[0];
			for(int j = 1; j < jisu; j++) {
				ov ^= galois.mulExp(omega[j], (zlog * j) % 255);
			}

			int dv = sigma[1];
			for(int j = 2; j < jisu; j += 2) {
				dv ^= galois.mulExp(sigma[j + 1], (zlog * j) % 255);
			}

			data[galois.toPos(length, ps)] ^= galois.mul(ps, galois.div(ov, dv));
		}
	}

	int RsDecode::decode(IntArray &data, int length, bool noCorrect) {
		if(length < npar || length > 255) {
			return RS_PERM_ERROR;
		}

		IntArray syn(npar);
		if(galois.calcSyndrome(data, length, syn)) {
			return 0;
		}

		IntArray sigma(npar / 2 + 2);
		IntArray omega(npar / 2 + 1);
		int jisu = calcSigmaMBM(sigma, omega, syn);
		if(jisu <= 0) {
			return RS_CORRECT_ERROR;
		}

		IntArray pos(jisu);
		int r = chienSearch(pos, length, jisu, sigma);
		if(r < 0) {
			return r;
		}
		if(!noCorrect) {
			doForney(data, length, jisu, pos, sigma, omega);
		}
		return jisu;
	}

	int RsDecode::decode(IntArray &data, int length) {
		return decode(data, length, false);
	}

	int RsDecode::decode(IntArray &data) {
		return decode(data, data.size(), false);
	}

} /* namespace barcodes */
