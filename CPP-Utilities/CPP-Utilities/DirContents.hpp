//
//  DirContents.hpp
//  CPP-Utilities
//
//  Created by tridiak on 27/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef DirContents_hpp
#define DirContents_hpp

#include <stdio.h>
#include "PathClass.hpp"
#include <algorithm>
#include <exception>

namespace DirContent {

// Return Value
//	0 : Continue
//	-1 : All stop
// Path will be a full path, not just the name.
typedef int (*IterateProc)(const PathClass& path, void* userCtx);
typedef int (^IterateBlock)(const PathClass& path);

// exceptions
struct DirContentsEx : std::exception {
	std::string reason;
	DirContentsEx(const std::string& s) : reason(s) {}
	const char* what() const _NOEXCEPT { return reason.c_str(); }
};

//-----------------------------------
/*
Get all filing system objects in a directory except for . & .. .
Must call Gather() to retrieve contents.
*/
class DirContents {
	PathClass dirPath;
	SST::StringArray names;
	
public:
	DirContents(const std::string& path);
	DirContents(const PathClass& path);
	
	// Gather all contents again.
	// . & .. are not included.
	// Sort() is called after gathering.
	void Gather();
	
	// Copy of all items as full path
	PathClassArray Items() const;
	// Copy of all items as name
	SST::StringArray Names() const;
	
	// If false, operator[] returns a name.
	// If true, operator[] returns a path.
	bool nameOrPathFlag;
	
	// If idx is >= count, a DirContentsEx will be thrown.
	// if nameOrPathFlag is false, PathClass will just contain the name even if it is not a legitimate path.
	PathClass operator[](uint64_t idx) const;
	
	// Return Values
	//	0 : No issues
	//	-1 : All stop flagged
	//	> 0 : posix error
	int Iterate(IterateProc proc, void* userCtx);
	int Iterate(IterateBlock block);
	
	// Override if you want to sort names array.
	// Default is standard alphabetical sort.
	// This is called inside Gather().
	virtual void Sort();
};

}; // namespace

#endif /* DirContents_hpp */
