//
//  ThreadValue.hpp
//  CPP-Utilities
//
//  Created by tridiak on 20/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef ThreadValue_hpp
#define ThreadValue_hpp

#include <stdio.h>
#include <map>
#include <pthread.h>


uint64_t ThreadID();

//------------------------------------
#pragma mark -

// Template thread object. Any type.
template<class T> class ThreadValue {
	//T value;
	std::map<uint64_t, T> threadObject;
	pthread_mutex_t threadObjectLock;
public:
	ThreadValue() {
		if (pthread_mutex_init(&threadObjectLock, nullptr)) {
			printf("ThreadValue : could not create mutex\n");
			exit(1);
		}
	};
	
	ThreadValue(const ThreadValue& obj) = delete;
	ThreadValue operator=(const ThreadValue& obj) = delete;
	~ThreadValue() {
		pthread_mutex_destroy(&threadObjectLock);
	};
	
	// Returns false only if the lock could not be locked.
	bool SetThreadValue(const T& V) {
		uint64_t id = ThreadID();
		if (pthread_mutex_lock(&threadObjectLock)) { return false; }
		threadObject[id] = V;
	
		assert(pthread_mutex_unlock(&threadObjectLock));
	
		return true;
	};
	
	// Will return nullptr if lock could not be locked.
	T GetThreadValue() {
		uint64_t id = ThreadID();
		T v{};
	
		if (pthread_mutex_lock(&threadObjectLock)) { return nullptr; }
		auto itr = threadObject.find(id);
		if (itr != threadObject.end()) { v = itr->second; }
		
		assert(pthread_mutex_unlock(&threadObjectLock));
		
		return v;
	};
	
	// Will return false if lock could not be locked.
	bool GetThreadValue(T& value) {
		uint64_t id = ThreadID();
		
		if (pthread_mutex_lock(&threadObjectLock)) { return false; }
		auto itr = threadObject.find(id);
		if (itr != threadObject.end()) { value = itr->second; }
		
		assert(pthread_mutex_unlock(&threadObjectLock));
		
		return true;
	};
	
	
};

//---------------------------------------------

// Pointer thread object.
// Uses ThreadValue<void*>

// Returns false only if the lock could not be locked.
bool SetThreadValue(void* value);

// Will return nullptr if lock could not be locked.
void* GetThreadValue();

// Will return false if lock could not be locked.
bool GetThreadValue(void*& V);


#endif /* ThreadValue_hpp */
