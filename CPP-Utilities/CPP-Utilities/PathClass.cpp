//
//  PathClass.cpp
//  CPP-Utilities
//
//  Created by tridiak on 4/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "PathClass.hpp"
#include <fcntl.h>
#include <limits.h>


void ZeroCheck(char m) {
	if (m == 0) { throw PathClassEx("Cannot have null (0) as a path marker"); }
};

PathClass::PathClass(char marker) {
	path.clear();
	dirMarker = marker;
	ZeroCheck(marker);
};

PathClass::PathClass(const char* path, char marker) {
	if (!path) { throw PathClassEx("null path pointer"); }
	this->path = path;
	dirMarker = marker;
	ZeroCheck(marker);
};

PathClass::PathClass(const std::string& path, char marker) {
	this->path = path;
	dirMarker = marker;
	ZeroCheck(marker);
};

PathClass::PathClass(int desc, char marker) {
	char buf[PATH_MAX];
	bzero(buf, PATH_MAX);
	if (fcntl(desc, F_GETPATH, buf) == -1) {throw PathClassEx("Invalid descriptor");}
	path = buf;
	dirMarker = marker;
	ZeroCheck(marker);
};

//-----------------------------------------------------
#pragma mark -

bool PathClass::operator==(const PathClass& obj) const {
	return *this == obj.Path();
};

bool PathClass::operator==(const std::string& P) const {
	if (path.empty() && P.empty()) { return true; }
	if (path.empty() || P.empty()) { return false; }
	std::string L = StringStuff::RPRemoveIfLastChar(path, dirMarker);
	std::string R = StringStuff::RPRemoveIfLastChar(P, dirMarker);
	
	return L == R;
};

bool PathClass::operator!=(const PathClass& obj) const {
	return !(*this == obj.Path());
};

bool PathClass::operator!=(const std::string& P) const {
	return !(*this == P);
};

bool PathClass::operator<(const PathClass& obj) const {
	return path < obj.path;
};

PathClass& PathClass::operator+=(const PathClass& obj) {
	AddPath(obj);
	return *this;
};

PathClass& PathClass::operator+=(const std::string& path) {
	AddPath(path);
	return *this;
};

PathClass operator+(PathClass& LHS, const PathClass& RHS) {
	LHS += RHS;
	return LHS;
};

PathClass operator+(PathClass& LHS, const std::string& RHS) {
	LHS += RHS;
	return LHS;
};

PathClass& PathClass::operator--(int dummy) {
	RemoveLastComponent();
	return *this;
};

std::string PathClass::operator[](unsigned int idx) {
	SST::StringArray comps = Components();
	if (idx >= comps.size()) { throw PathClassEx("Invalid path component"); }
	return comps[idx];
};

std::string PathClass::operator[](unsigned int idx) const {
	SST::StringArray comps = Components();
	if (idx >= comps.size()) { throw PathClassEx("Invalid path component"); }
	return comps[idx];
};

#pragma mark -

void PathClass::AppendDirMarker() {
	if (SST::RPLastChar(path) != dirMarker) { path.append(&dirMarker, 1); }
};

void PathClass::RemoveDirMarker() {
	path = StringStuff::RPRemoveIfLastChar(path, dirMarker);
};

void PathClass::ConvertDirMarkers(char newMarker) {
	if (newMarker == dirMarker) { return; }
	ZeroCheck(newMarker);
	
	for (size_t t=0; t < path.size(); t++) {
		if (path[t] == dirMarker) { path[t] = newMarker; }
	}
	dirMarker = newMarker;
};

void PathClass::SwapDirMarkers(char newMarker) {
	if (newMarker == dirMarker) { return; }
	ZeroCheck(newMarker);
	
	for (size_t t=0; t < path.size(); t++) {
		if (path[t] == dirMarker) { path[t] = newMarker; }
		else if (path[t] == newMarker) { path[t] = dirMarker; }
	}
	dirMarker = newMarker;
};

void PathClass::ChangeDirMarker(char newMarker) {
	ZeroCheck(newMarker);
	dirMarker = newMarker;
};

#pragma mark -

std::string PathClass::Path() const {
	return path;
};

bool PathClass::IsRelative() const {
	return path.empty() ? false : path[0] == dirMarker;
};

void PathClass::PrependPath(const PathClass& component) {
	PrependPath(component.Path());
};

void PathClass::PrependPath(const std::string& component) {
	if (component.empty()) { return; }
	else if (path.empty()) {
		path = component;
		return;
	}
	
	if (SST::RPLastChar(component) == dirMarker) {
		if (path[0] == dirMarker) { path.erase(0, 1); }
		path.insert(0, component);
	}
	else {
		if (path[0] != dirMarker) { path.insert(0, &dirMarker, 1); }
		path.insert(0, component);
	}
};

void PathClass::AddPath(const PathClass& component) {
	AddPath(component.Path());
};

void PathClass::AddPath(const std::string& component) {
	if (component.empty()) { return; }
	else if (path.empty()) {
		path = component;
		return;
	}
	
	RemoveDirMarker();
	if (component[0] != dirMarker) { path.append(&dirMarker, 1); }
	path.append(component);
	
};

std::string PathClass::LastComponent() const {
	return SST::RPStrFindFromBack(path, dirMarker);
};

void PathClass::RemoveLastComponent() {
	path = SST::RPStrBeforeMarker(path, dirMarker);
};

void PathClass::AddSuffix(const std::string& suffix) {
	path.append(".").append(suffix);
};

void PathClass::RemoveSuffix() {
	if (dirMarker == '.') { throw PathClassEx("Cannot remove suffix as directory marker is a '.'"); }
	
	size_t dotIndex = path.find_last_of('.');
	if (dotIndex == std::string::npos) { return; }
	size_t slashIndex = path.find_last_of(dirMarker);
	if (slashIndex == std::string::npos) {
		path = path.substr(0, dotIndex);
	}
	else {
		if (slashIndex > dotIndex) { return; }
		path = path.substr(0, dotIndex);
	}
};


SST::StringArray PathClass::Components() const {
	return SST::RPStringSplit(path, std::string(&dirMarker, 1));
};
