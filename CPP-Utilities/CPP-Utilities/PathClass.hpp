//
//  PathClass.hpp
//  CPP-Utilities
//
//  Created by tridiak on 4/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef PathClass_hpp
#define PathClass_hpp

#include <stdio.h>
#include <string>
#include <exception>
#include "StringStuff.hpp"

//--------------------------

struct PathClassEx : std::exception {
	std::string reason;
	PathClassEx(const std::string& reason) : reason(reason) {}
	const char* what() const _NOEXCEPT { return (std::string("PathClass exception: ") + reason).c_str(); }
};

//-------------------------------

/*
Wrote this for practice.
*/
class PathClass {
	std::string path;
	
	char dirMarker;
	
public:
	// Directory marker cannot be '.'. An exception will be thrown.
	// Can throw
	PathClass(char marker = '/');
	// Can throw
	PathClass(const char* path, char marker = '/');
	// Can throw
	PathClass(const std::string& path, char marker = '/');
	// Can throw
	PathClass(int desc, char marker = '/');
	
	// Equality operators
	bool operator==(const PathClass& obj) const;
	bool operator==(const std::string& path) const;
	
	// Inequality operators
	bool operator!=(const PathClass& obj) const;
	bool operator!=(const std::string& path) const;
	
	// LT
	bool operator<(const PathClass& obj) const;
	
	// Append to path. All call through to AddPath()
	PathClass& operator+=(const PathClass& obj);
	PathClass& operator+=(const std::string& path);
	friend PathClass operator+(PathClass& LHS, const PathClass& RHS);
	friend PathClass operator+(PathClass& LHS, const std::string& RHS);
	
	// Get path component idx. Will throw an exception if component idx
	// does not exist.
	std::string operator[](unsigned int idx);
	std::string operator[](unsigned int idx) const;
	
	// Returns copy of path
	std::string Path() const;
	
	// Returns true if relative path. First char is not '/'
	bool IsRelative() const;
	
	// Add dir marker if it does not exist
	void AppendDirMarker();
	
	// Remove dir marker if it exists
	void RemoveDirMarker();
	
	// Change the directory marker and convert path's markers to such.
	// Existing characters the same as the new marker in the path will
	// not be converted.
	// 'A/B/C:s:3/D'. newMarker = ':' -> 'A:B:C:s:3:D'.
	void ConvertDirMarkers(char newMarker);
	
	// Change the directory marker and convert path's markers to such.
	// Existing characters the same as the new marker will be converted to the
	// old marker.
	// 'A/B/C:s:3/D'. newMarker = ':' -> 'A:B:C/s/3:D'.
	void SwapDirMarkers(char newMarker);
	
	// Change the directory marker. The path's directory markers are not changed.
	void ChangeDirMarker(char newMarker);
	
	char DirMarker() const { return dirMarker; }
	
	// Will insert or remove '/' as necessary
	void PrependPath(const PathClass& component);
	void PrependPath(const std::string& component);
	
	// Will insert or remove '/' as necessary
	void AddPath(const PathClass& component);
	void AddPath(const std::string& component);
	
	// Everything after last '/'
	std::string LastComponent() const;
	
	// Remove last component
	void RemoveLastComponent();
	PathClass& operator--(int dummy);
	
	// Adds '.<suffix>'.
	void AddSuffix(const std::string& suffix);
	
	// Removes everything after last '.' that is after the last dirMarker character.
	// dirMarker = /
	// 'A/B/C/D.txt' -> 'A/B/C/D'
	// 'A/B/C.txt/D' -> 'A/B/C.txt/D'
	// The last '.' will be removed.
	// IMPORTANT: if directory marker is a '.', a PathClassEx exception will be thrown.
	void RemoveSuffix();
	
	// Splits path by directory marker.
	SST::StringArray Components() const;
	
};

//------------------------------------------
#pragma mark -

typedef std::vector<PathClass> PathClassArray;
typedef std::set<PathClass> PathClassSet;

#endif /* PathClass_hpp */
