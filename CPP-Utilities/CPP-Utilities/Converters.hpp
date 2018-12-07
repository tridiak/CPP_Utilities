//
//  Converters.hpp
//  CPP-Utilities
//
//  Created by tridiak on 1/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef Converters_hpp
#define Converters_hpp

#include <stdio.h>
#include <functional>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <exception>


//-------------------------------
#pragma mark - Template Functions

//------------------------------------
/// Convert Set to Array
template <class T> std::vector<T> SetToArray(const std::set<T>& S) {
	std::vector<T> V; V.clear();
	for (auto itr = S.begin(); itr != S.end(); itr++) {
		V.push_back(*itr);
	}
	return V;
};

//------------------------------------
/// Convert Array to Set
template <class T> std::set<T> ArrayToSet(const std::vector<T>& V) {
	std::set<T> S; S.clear();
	for (auto itr = V.begin(); itr != V.end(); itr++) {
		S.insert(*itr);
	}
	return S;
};

//------------------------------------

template <class T,class U>
	using ArrayPair = std::pair<std::vector<T>, std::vector<U> >;

/// Retrieve key and value arrays from dictionary (map)
template <class T,class U> ArrayPair<T,U> MapToArrays(const std::map<T,U>& M) {
	std::pair<std::vector<T>, std::vector<U> > P;
	for (auto itr = M.begin(); itr != M.end(); itr++) {
		P.first.push_back(itr->first);
		P.second.push_back(itr->second);
	}
	return P;
};

//------------------------------------
/// Used by ArraysToMap() function.
struct VectorSizeMismatchEx : std::exception {
	const char* _Nonnull what() const noexcept {return "Size of Vectors are not equal";}
};

/// Create dictionary (map) from two arrays. If array sizes are not equal, VectorSizeMismatchEx will be thrown.
template <class T,class U> std::map<T,U> ArraysToMap(const std::vector<T>& K, const std::vector<U>& V) {
	std::map<T,U> M; M.clear();
	if (K.size() != V.size()) {throw VectorSizeMismatchEx();}
	if (K.size() != V.size() || K.empty()) {return M;}
	
	auto itr1 = K.begin();
	auto itr2 = V.begin();
	for (; itr1 != K.end(); itr1++, itr2++) {
		M[*itr1] = *itr2;
	}
	return M;
};

#endif /* Converters_hpp */
