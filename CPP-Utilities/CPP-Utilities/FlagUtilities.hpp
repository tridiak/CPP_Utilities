//
//  FlagUtilities.hpp
//  CPP-Utilities
//
//  Created by tridiak on 28/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef FlagUtilities_hpp
#define FlagUtilities_hpp

#include <stdio.h>
#include <stdint.h>

namespace FlagUtils {

template<class T> void SetFlagBit(uint8_t bit, T& flags) {
	if (bit < sizeof(T) * 8) {
		flags |= 1 << bit;
	}
};

template<class T> void ClearFlagBit(uint8_t bit, T& flags) {
	if (bit < sizeof(T) * 8) {
		T m = ~(1 << bit);
		flags &= m;
	}
};

template<class T> bool IsFlagBitSet(uint8_t bit, T& flags) {
	if (bit < sizeof(T) * 8) {
		return (flags & (1 << bit)) > 0;
	}
	
	return false;
};

template<class T> void SetFlags(const T toSet, T& flags) {
	flags |= toSet;
};

template<class T> void ClearFlags(const T toClear, T& flags) {
	flags &= ~toClear;
};

template<class T> bool AnySet(const T value, const T& flags) {
	return (flags & value) > 0;
};

template<class T> bool AllSet(const T value, const T& flags) {
	if (value == 0) { return false; }
	return (flags & value) == value;
};

} // namespace

#endif /* FlagUtilities_hpp */
