//
//  FileUtil.cpp
//  CPP-Utilities
//
//  Created by tridiak on 27/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "FileUtil.hpp"
#include <sys/stat.h>

namespace FileUtil {

bool IsADir(const std::string& path) {
	return IsADir(path.c_str());
};

bool IsADir(const char* path) {
	struct stat s;
	int res = stat(path, &s);
	if (res) { return false; }
	
	return ((s.st_mode & S_IFMT) == S_IFDIR);
};

};
