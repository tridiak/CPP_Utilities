//
//  Miscellaneous.cpp
//  CPP-Utilities
//
//  Created by tridiak on 3/12/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "Miscellaneous.hpp"
#include <strings.h>

using namespace MiscStuff;

#define USEC_PER_SEC 1000000

struct timeval GetTV() {
	struct timeval tv;
	bzero(&tv, sizeof(tv));
	gettimeofday(&tv, nullptr);
	return tv;
};

struct timespec TVtoTS(struct timeval tv) {
	return {tv.tv_sec, static_cast<long>(tv.tv_usec/USEC_PER_SEC)};
};

struct timeval TStoTV(struct timespec ts) {
	return {ts.tv_sec, static_cast<__darwin_suseconds_t>(ts.tv_nsec * USEC_PER_SEC)};
};

long TimeSpecCompare(const timespec ts1, const timespec ts2) {
	if (ts1.tv_sec == ts1.tv_sec) {
		return ts1.tv_nsec - ts2.tv_nsec;
	}
	return ts1.tv_sec - ts2.tv_sec;
};
