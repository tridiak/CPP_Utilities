//
//  Miscellaneous.hpp
//  CPP-Utilities
//
//  Created by tridiak on 3/12/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef Miscellaneous_hpp
#define Miscellaneous_hpp

#include <stdio.h>
#include <sys/time.h>

namespace MiscStuff {

// timespec uses nanoseconds for second fraction whereas
// timeval uses microseconds.

// Convert timespec to timeval
struct timeval TStoTV(struct timespec ts);

// Convert timeval to timespec
struct timespec TVtoTS(struct timeval tv);

// Get timeval as an rvalue
struct timeval GetTV(void);

//< 0 if ts1 < ts2, > 0 if ts1 > ts2. 0 otherwise
long TimeSpecCompare(const struct timespec ts1, const struct timespec ts2);

}; // namespace

#endif /* Miscellaneous_hpp */
