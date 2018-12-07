//
//  ThreadValue.cpp
//  CPP-Utilities
//
//  Created by tridiak on 20/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "ThreadValue.hpp"

static ThreadValue<void*> voidPtrValue;

uint64_t ThreadID() {
	uint64_t v;
	pthread_threadid_np(pthread_self(), &v);
	
	return v;
};

bool SetThreadValue(void* value) {
	return voidPtrValue.SetThreadValue(value);
};

void* GetThreadValue() {
	return voidPtrValue.GetThreadValue();
};

bool GetThreadValue(void*& V) {
	return voidPtrValue.GetThreadValue(V);
};



