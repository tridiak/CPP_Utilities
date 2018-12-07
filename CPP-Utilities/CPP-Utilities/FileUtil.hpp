//
//  FileUtil.hpp
//  CPP-Utilities
//
//  Created by tridiak on 27/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef FileUtil_hpp
#define FileUtil_hpp

#include <stdio.h>
#include <string>

namespace FileUtil {

bool IsADir(const std::string& path);
bool IsADir(const char* path);

}; // namespace

#endif /* FileUtil_hpp */
