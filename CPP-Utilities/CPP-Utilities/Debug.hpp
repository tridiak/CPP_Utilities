//
//  Debug.hpp
//  CPP-Utilities
//
//  Created by tridiak on 7/12/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef Debug_hpp
#define Debug_hpp

#include <stdio.h>
#include <string>

// Remove if you want debug output.
// #define CPPDebug 1

template <class T>
void DebugPrintNum(T v, bool NL = false);

void DebugPrint(const std::string& s, bool NL = false);

#ifdef CPPDebug
	#define DebugPrintFmt(args...) printf(args)
#else
	#define DebugPrintFmt(args...)
#endif


#define DebugPretty DebugPrint(__PRETTY_FUNCTION__, true);

#endif /* Debug_hpp */
