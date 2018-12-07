//
//  Debug.cpp
//  CPP-Utilities
//
//  Created by tridiak on 7/12/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "Debug.hpp"
#include <iostream>

#ifdef CPPDebug
	#define PRINT(MSG) std::cout << MSG;
#else
	#define PRINT(MSG)
#endif


template <class T>
void DebugPrintNum(T v, bool NL) {
	PRINT(std::to_string(v));
	if (NL) {PRINT("\n");}
};

void DebugPrint(const std::string& s, bool NL) {
	PRINT(s);
	if (NL) {PRINT("\n");}
};

