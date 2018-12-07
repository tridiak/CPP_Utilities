//
//  PosNeg.cpp
//  PosNegUtility
//
//  Created by tridiak on 20/03/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "PosNeg.hpp"


const PNPositive CPosNeg::positive = PNPositive();
const PNNegative CPosNeg::negative = PNNegative();
const PNZero CPosNeg::pnzero = PNZero();

const CPosNeg& CPosNeg::Create(long v) {
	if (v > 0) { return CPosNeg::positive; }
	if (v < 0) { return CPosNeg::negative; }
	return CPosNeg::pnzero;
};

const CPosNeg& PNPositive::operator--(int) const {
	return pnzero;
};

const CPosNeg& PNNegative::operator++(int) const {
	return pnzero;
};

const CPosNeg& operator!(const CPosNeg& pn) {
	// Error this line
//	return pn.N() ? CPosNeg::positive : (pn.P() ? CPosNeg::negative : CPosNeg::pnzero);
	if (pn.N()) { return CPosNeg::positive; }
	if (pn.P()) { return CPosNeg::negative; }
	return CPosNeg::pnzero;
};

long long operator*(long long value, const CPosNeg& pn) {
	return value * pn.V();
};

double operator*(double value, const CPosNeg& pn) {
	return value * pn.V();
};

long long operator/(long long value, const CPosNeg& pn) {
	if (pn.Z()) { throw CPosNeg::nonsensicalEx(); }
	return value / pn.V();
};

double operator/(double value, const CPosNeg& pn) {
	if (pn.Z()) { throw CPosNeg::nonsensicalEx(); }
	return value / pn.V();
};
