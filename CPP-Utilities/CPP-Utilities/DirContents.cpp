//
//  DirContents.cpp
//  CPP-Utilities
//
//  Created by tridiak on 27/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "DirContents.hpp"
#include <sys/stat.h>
#include <dirent.h>

namespace DirContent {

DirContents::DirContents(const std::string& path) {
	dirPath = path;
	dirPath.RemoveDirMarker();
	nameOrPathFlag = false;
};

DirContents::DirContents(const PathClass& path) {
	dirPath = path;
	dirPath.RemoveDirMarker();
	nameOrPathFlag = false;
};


// Gather all contents again.
void DirContents::Gather() {
	struct stat s;
	int res = stat(dirPath.Path().c_str(), &s);
	if (res) { throw DirContentsEx("Invalid path"); }
	
	if ((s.st_mode & S_IFMT) != S_IFDIR) {
		throw DirContentsEx("Not a directory");
	}
	
	names.clear();
	
	DIR* dptr=opendir(dirPath.Path().c_str());
	if (!dptr) { throw DirContentsEx("Could not access directory"); }
	
	dirent* d = nullptr;
	while ((d = readdir(dptr)) ) {
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) {continue;}
		names.push_back(d->d_name);
	}
	
	closedir(dptr);
	
	Sort();
};


// Copy of current items
PathClassArray DirContents::Items() const {
	PathClassArray array;
	for (auto n : names) {
		PathClass pc(dirPath.Path() + "/" + n);
		array.push_back(pc);
	}
	
	return array;
};

SST::StringArray DirContents::Names() const {
	return names;
};

// Item N
PathClass DirContents::operator[](uint64_t idx) const {
	if (idx >= names.size()) { throw DirContentsEx("Out of range"); }
	
	std::string s = names[idx];
	if (nameOrPathFlag) {
		s = dirPath.Path() + "/" + s;
	}
	
	return PathClass(s);
};


int DirContents::Iterate(IterateProc proc, void* userCtx) {
	if (!proc) { return EINVAL; }
	PathClassArray items = Items();
	int res=0;
	for (auto p : items) {
		res = proc(p, userCtx);
		if (res) { return res; }
	}
	
	return 0;
};

int DirContents::Iterate(IterateBlock block) {
	if (!block) { return EINVAL; }
	PathClassArray items = Items();
	int res=0;
	for (auto p : items) {
		res = block(p);
		if (res) { return res; }
	}
	
	return 0;
};

void DirContents::Sort() {
	std::sort(names.begin(), names.end());
};

}; // namespace
