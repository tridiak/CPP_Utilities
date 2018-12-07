//
//  ABinaryFile.cpp
//  CPP-Utilities
//
//  Created by tridiak on 6/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "ABinaryFile.hpp"
#include "Debug.hpp"
#include <errno.h>
#include <sys/stat.h>
#include <algorithm>
#include <unistd.h>

ABinaryFile::ABinaryFile() {
	blob = nullptr;
	size = 0;
};

ABinaryFile::ABinaryFile(void* memoryBlock, uint64_t memSize) {
	DebugPretty
	
	blob = malloc(memSize);
	memcpy(blob, memoryBlock, memSize);
	size = memSize;
};

ABinaryFile::ABinaryFile(int desc) {
	DebugPretty
	
	struct stat s;
	bzero(&s, sizeof(s));
	
	int res = fstat(desc, &s);
	if (res == -1) {
		throw ABinaryFileEx(std::string("Open error ") + std::to_string(errno));
	}
	if ((s.st_mode & S_IFMT) != S_IFREG) {
		throw ABinaryFileEx(std::string("Not a file"));
	}
	
	FILE* F = fdopen(desc, "r");
	if (!F) {
		throw ABinaryFileEx(std::string("Open error ") + std::to_string(errno));
	}
	
	blob = malloc(s.st_size);
	size_t itemsRead = 0;
	if (blob) {
		size = s.st_size;
		itemsRead = fread(blob, size, 1, F);
	}
	
	fclose(F);
	if (itemsRead == 0) {
		throw ABinaryFileEx(std::string("Read error ") + std::to_string(errno));
	}

#ifdef DebugBinaryDetailed
	printf("\t%llu bytes loaded\n", size);
#endif
};

ABinaryFile::ABinaryFile(const std::string& path) {
	DebugPretty
	
	struct stat s;
	bzero(&s, sizeof(s));
	
	int res = stat(path.c_str(), &s);
	if (res == -1) {
		throw ABinaryFileEx(std::string("Open error ") + std::to_string(errno));
	}
	if ((s.st_mode & S_IFMT) != S_IFREG) {
		throw ABinaryFileEx(std::string("Not a file"));
	}
	
	FILE* F = fopen(path.c_str(), "r");
	if (!F) {
		throw ABinaryFileEx(std::string("Open error ") + std::to_string(errno));
	}
	
	blob = malloc(s.st_size);
	size_t itemsRead = 0;
	if (blob) {
		size = s.st_size;
		 itemsRead = fread(blob, size, 1, F);
	}
	
	fclose(F);
	if (itemsRead == 0) {
		throw ABinaryFileEx(std::string("Read error ") + std::to_string(errno));
	}
	
#ifdef DebugBinaryDetailed
	printf("\t%llu bytes loaded\n", size);
#endif
};


ABinaryFile::ABinaryFile(const ABinaryFile& obj) {
	DebugPretty
	
	size = obj.size;
	if (size) {
		blob = malloc(size);
		memcpy(blob, obj.blob, size);
	}
	else { blob = nullptr; }
};

void Swap(ABinaryFile& A, ABinaryFile& B) {
	std::swap(A.blob, B.blob);
	std::swap(A.size, B.size);
	
#ifdef DebugBinaryDetailed
	printf("Swapping %p & %p\n", &A, &B);
#endif
};

ABinaryFile ABinaryFile::operator=(ABinaryFile obj) {
	DebugPretty
	
	if (this == &obj) { return *this; }
	Swap(*this, obj);
	return *this;
};


ABinaryFile::ABinaryFile(ABinaryFile&& ref) : ABinaryFile() {
	DebugPretty
	
	Swap(*this, ref);
};

ABinaryFile ABinaryFile::operator=(ABinaryFile&& ref) {
	DebugPretty
	
	assert(this != &ref);
	if (blob) { free(blob); }
	
	blob = ref.blob;
	size = ref.size;
	ref.blob = nullptr;
	ref.size = 0;
	
	return *this;
}

ABinaryFile::~ABinaryFile() {
	DebugPretty
	
	if (blob) { free(blob); }
};

//------------------

uint64_t ABinaryFile::Size() const {
	return size;
};

uint8_t ABinaryFile::operator[](uint64_t pos) const {
	if (pos >= size) { throw ABinaryFileEx("Out of range"); }

#if DebugBinaryDetailed == 2
	int v = ((uint8_t*)blob)[pos];
	printf("Retrieving byte %d at index %llu\n", v, pos);
#endif

	return ((uint8_t*)blob)[pos];
};

/// Returns 16 bit integer from BYTE position.
/// If position+2 exceeds size, an exception will be thrown
uint16_t ABinaryFile::B16(uint64_t bytePos) const {
	if (bytePos+1 >= size) { throw ABinaryFileEx("Out of range"); }
	uint16_t* ptr = (uint16_t*)&((uint8_t*)blob)[bytePos];

#if DebugBinaryDetailed == 2
	printf("Retrieving UInt16 %u at index %llu\n", *ptr, bytePos);
#endif

	return *ptr;
};


/// Returns 32 bit integer from BYTE position.
/// If position+4 exceeds size, an exception will be thrown
uint32_t ABinaryFile::B32(uint64_t bytePos) const {
	if (bytePos+3 >= size) { throw ABinaryFileEx("Out of range"); }
	uint32_t* ptr = (uint32_t*)&((uint8_t*)blob)[bytePos];

#if DebugBinaryDetailed == 2
	printf("Retrieving UInt32 %lu at index %llu\n", *ptr, bytePos);
#endif

	return *ptr;
};


/// Returns 64 bit integer from BYTE position.
/// If position+8 exceeds size, an exception will be thrown
uint64_t ABinaryFile::B64(uint64_t bytePos) const {
	if (bytePos+7 >= size) { throw ABinaryFileEx("Out of range"); }
	uint64_t* ptr = (uint64_t*)&((uint8_t*)blob)[bytePos];

#if DebugBinaryDetailed == 2
	printf("Retrieving UInt64 %llu at index %llu\n", *ptr, bytePos);
#endif

	return *ptr;
};

const void* ABinaryFile::Blob() const {
	return blob;
};

//---------------------------------------------------------------
#pragma mark - ABigBinaryFile

/*
ABigBinaryFile::ABigBinaryFile() {
	dataSize = 0;
	fileDesc = -1;
	path.clear();
	blockSize = 0;
	blockCount = 0;
	maxBlocks = 0;
	lastCheck.tv_nsec = 0;
	lastCheck.tv_sec = LONG_MIN;
	zeroBlocks = false;
	
	blockArray = nullptr;
};
*/

void ABigBinaryFile::BlockErase() {
	if (blockArray) {
		// Does nothing in this implementation.
	}
};

//----

void ABigBinaryFile::ZeroSection(uint64_t section) {
	DebugPrint(__PRETTY_FUNCTION__ + std::to_string(section), true);
	
	if (zeroBlocks) {
		// Last block's size is not necessarily equal to block size
		if (section == blockCount - 1) {
			bzero(blockArray[section], LastBlockSize());
		}
		else {
			bzero(blockArray[section], blockSize);
		}
	}
};

//----

void ABigBinaryFile::DataReset() {
	DebugPretty
	
	currentPtr = nullptr;
	currBlockNum = -1;
	
//	BlockErase();
	for (uint64_t t = 0; t < maxBlocks; t++) {
		freeSections[t] = false;
		ZeroSection(t);
	}
	
	blockNumAddrMap.clear();
	blkNumberHistory.clear();
};

//----
// Check if file still exists, is accessible and whether or not it has changed.
void ABigBinaryFile::FileCheck() {
	DebugPretty
	
	struct stat s;
	bzero(&s, sizeof(s));
	
	if (fileDesc > STDERR_FILENO) {
		
		int res = fstat(fileDesc, &s);
		if (res == -1) {
			throw ABinaryFile::ABinaryFileEx(std::string("File check failure: ") + std::to_string(errno));
		}
	}
	else { // path based
		int res = stat(path.c_str(), &s);
		if (res == -1) {
			throw ABinaryFile::ABinaryFileEx(std::string("File check failure: ") + std::to_string(errno));
		}
	}
	
	//--
	if ((s.st_mode & S_IFMT) != S_IFREG) {
		throw ABinaryFile::ABinaryFileEx(std::string("Not a file"));
	}
	
	if (lastCheck.tv_sec == LONG_MIN) {
		lastCheck = s.st_mtimespec;
	}
	else {
		if (lastCheck.tv_sec < s.st_mtimespec.tv_sec
				|| (lastCheck.tv_sec == s.st_mtimespec.tv_sec && lastCheck.tv_nsec < s.st_mtimespec.tv_nsec)) {
			// modification date change
			DataReset();
		}
	}
	
	dataSize = s.st_size;
	blockCount = dataSize > 0 ? (dataSize - 1) / blockSize + 1 : 0;

#ifdef DebugBinaryDetailed
	if (fileDesc > STDERR_FILENO) {
		printf("Checked file with descriptor %d. Data size = %llu. Block count == %llu\n", fileDesc, dataSize, blockCount);
	}
	else {
		printf("Checked file at path %s. Data size = %llu. Block count == %llu\n", path.c_str(), dataSize, blockCount);
	}
#endif
};

//----

ABigBinaryFile::ABigBinaryFile(int desc, uint16_t blockSz, uint64_t maxBlks) {
	DebugPrintFmt("%p ", this);DebugPretty
	
	if (desc <= STDERR_FILENO) { throw ABinaryFile::ABinaryFileEx("Invalid file descriptor"); }
	if (maxBlks == 0) { throw ABinaryFile::ABinaryFileEx("Zero maximum blocks"); }
	
	dataSize = 0;
	fileDesc = desc;
	path.clear();
	blockSize = blockSz;
	blockCount = 0;
	maxBlocks = maxBlks;
	file = nullptr;
	currentPtr = nullptr;
	currBlockNum = -1;
	lastCheck.tv_nsec = 0;
	lastCheck.tv_sec = LONG_MIN;
	
	blockArray = (char**)calloc(maxBlks, blockSize);
	if (!blockArray) { throw ABinaryFile::ABinaryFileEx("Block array memory failure"); }
	for (uint64_t t = 0; t < maxBlks; t++) {
		freeSections.push_back(false);
	}
	
	FileCheck();
	OpenFile();
};

//----

ABigBinaryFile::ABigBinaryFile(const std::string& path, uint16_t blockSz, uint64_t maxBlks) {
	DebugPrintFmt("%p ", this);DebugPretty
	
	if (maxBlks == 0) { throw ABinaryFile::ABinaryFileEx("Zero maximum blocks"); }
	dataSize = 0;
	fileDesc = -1;
	this->path = path;
	blockSize = blockSz;
	blockCount = 0;
	maxBlocks = maxBlks;
	file = nullptr;
	currentPtr = nullptr;
	currBlockNum = -1;
	lastCheck.tv_nsec = 0;
	lastCheck.tv_sec = LONG_MIN;
	
	blockArray = (char**)calloc(maxBlks, blockSize);
	if (!blockArray) { throw ABinaryFile::ABinaryFileEx("Block array memory failure"); }
	for (uint64_t t = 0; t < maxBlks; t++) {
		freeSections.push_back(false);
		blockArray[t] = (char*)calloc(1, blockSize);
		if (!blockArray[t]) {throw ABinaryFile::ABinaryFileEx("Block array item memory failure"); }
	}
	
	FileCheck();
	OpenFile();
};

// Copy constructor
ABigBinaryFile::ABigBinaryFile(const ABigBinaryFile& obj) {
	DebugPrintFmt("%p ", this);DebugPretty
	
	dataSize = obj.dataSize;
	fileDesc = obj.fileDesc;
	path = obj.path;
	blockSize = obj.blockSize;
	blockCount = obj.blockCount;
	maxBlocks = obj.maxBlocks;
	file = nullptr;
	currentPtr = nullptr;
	currBlockNum = -1;
	lastCheck = obj.lastCheck;
	
	blockArray = (char**)calloc(maxBlocks, blockSize);
	if (!blockArray) { throw ABinaryFile::ABinaryFileEx("Block array memory failure"); }
	for (uint64_t t = 0; t < maxBlocks; t++) {
		freeSections.push_back(false);
		blockArray[t] = (char*)calloc(1, blockSize);
		if (!blockArray[t]) {throw ABinaryFile::ABinaryFileEx("Block array item memory failure"); }
	}
	
	FileCheck();
	OpenFile();
};

// Assignment operator
ABigBinaryFile ABigBinaryFile::operator=(const ABigBinaryFile& obj) {
	DebugPrintFmt("%p ", this);DebugPretty
	
	dataSize = obj.dataSize;
	fileDesc = obj.fileDesc;
	path = obj.path;
	blockSize = obj.blockSize;
	blockCount = obj.blockCount;
	maxBlocks = obj.maxBlocks;
	file = nullptr;
	currentPtr = nullptr;
	currBlockNum = -1;
	lastCheck = obj.lastCheck;
	
	blockArray = (char**)calloc(maxBlocks, blockSize);
	if (!blockArray) { throw ABinaryFile::ABinaryFileEx("Block array memory failure"); }
	for (uint64_t t = 0; t < maxBlocks; t++) {
		freeSections.push_back(false);
		blockArray[t] = (char*)calloc(1, blockSize);
		if (!blockArray[t]) {throw ABinaryFile::ABinaryFileEx("Block array item memory failure"); }
	}
	
	FileCheck();
	OpenFile();
	
	return *this;
};

// Move constructor
ABigBinaryFile::ABigBinaryFile(ABigBinaryFile&& ref) {
	DebugPrintFmt("%p ", this);DebugPretty
	
	dataSize = ref.dataSize;
	fileDesc = ref.fileDesc;
	
	ref.fileDesc = -1;
	path = ref.path;
	ref.path.clear();
	file = ref.file;
	ref.file = nullptr;
	
	blockSize = ref.blockSize;
	blockCount = ref.blockCount;
	maxBlocks = ref.maxBlocks;
	blockArray = ref.blockArray;
	ref.blockArray = nullptr;
	
	blockNumAddrMap = ref.blockNumAddrMap;
	blkNumberHistory = ref.blkNumberHistory;
	freeSections = ref.freeSections;
	
	lastCheck = ref.lastCheck;
	
	currentPtr = ref.currentPtr;
	currBlockNum = ref.currBlockNum;
	lastCheck = ref.lastCheck;
	
	
};

// Move assignment operator
ABigBinaryFile& ABigBinaryFile::operator=(ABigBinaryFile&& ref) {
	DebugPrintFmt("%p ", this);DebugPretty
	
	dataSize = ref.dataSize;
	fileDesc = ref.fileDesc;
	
	ref.fileDesc = -1;
	path = ref.path;
	ref.path.clear();
	file = ref.file;
	ref.file = nullptr;
	
	blockSize = ref.blockSize;
	blockCount = ref.blockCount;
	maxBlocks = ref.maxBlocks;
	blockArray = ref.blockArray;
	ref.blockArray = nullptr;
	
	blockNumAddrMap = ref.blockNumAddrMap;
	blkNumberHistory = ref.blkNumberHistory;
	freeSections = ref.freeSections;
	
	lastCheck = ref.lastCheck;
	
	currentPtr = ref.currentPtr;
	currBlockNum = ref.currBlockNum;
	lastCheck = ref.lastCheck;
	
	return *this;
};

//----

ABigBinaryFile::~ABigBinaryFile() {
	DebugPrintFmt("%p ", this);
	DebugPretty
	
	if (file) { fclose(file); }
	BlockErase();
	if (blockArray) {
		for (uint64_t t=0; t < maxBlocks; t++) {
			free(blockArray[t]);
		}
		free(blockArray);
	}
};

//-----------------------
#pragma mark -

void ABigBinaryFile::OpenFile() {
	DebugPretty
	
	if (file) { return; }
	if (fileDesc > STDERR_FILENO) {
		file = fdopen(fileDesc, "r");
	}
	else {
		file = fopen(path.c_str(), "r");
	}
	if (!file) {
		throw ABinaryFile::ABinaryFileEx("Could not open data file");
	}
};

//----

// Assumes caller has checked against blockCount
void ABigBinaryFile::LoadBlock(uint64_t blkNum) {
	DebugPretty
	
	if (blockNumAddrMap.find(blkNum) != blockNumAddrMap.end()) { return; }

#ifdef DebugBinaryDetailed
	printf("Loading block %llu\n", blkNum);
#endif

	if (blockNumAddrMap.size() == maxBlocks) {
		uint64_t oldest = blkNumberHistory[0];
		
#ifdef DebugBinaryDetailed
		printf("Purging block %llu\n", oldest);
#endif
		
		auto itr = blockNumAddrMap.find(oldest);
		assert(itr != blockNumAddrMap.end());
		uint64_t bidx = itr->second;
		ZeroSection(bidx);
		
		freeSections[bidx] = false;
		blockNumAddrMap.erase(itr);
		blkNumberHistory.erase(blkNumberHistory.begin());
	}
	
	auto itr = std::find(freeSections.begin(), freeSections.end(), false);
	assert(itr != freeSections.end());
	
	uint64_t idx = itr - freeSections.begin();
	fpos_t pos = blkNum * blockSize;
	int res = fsetpos(file, &pos);
	if (res) {
		throw ABinaryFile::FileAccessEx("Could not set file position. Block load failed");
	}
	size_t ct = fread(blockArray[idx], 1, blockSize, file);
	
	if (ct < blockSize && blkNum != blockCount - 1) {
		throw ABinaryFile::FileAccessEx("Could not load all data");
	}
	
	freeSections[idx] = true;
	blkNumberHistory.push_back(blkNum);
	blockNumAddrMap[blkNum] = idx;
	
#ifdef DebugBinaryDetailed
	printf("Block %llu loaded\n", blkNum);
#endif
};

//----

void ABigBinaryFile::Preload(uint64_t blockNumber) {
	DebugPretty
	
	if (blockNumber >= blockCount) { return; }
	LoadBlock(blockNumber);
};

//----

// Subscript operator
uint8_t ABigBinaryFile::operator[](uint64_t pos) {
#if DebugBinaryDetailed == 2
	DebugPretty
#endif
	
	if (pos >= dataSize) { throw ABinaryFile::ABinaryFileEx("Out of range"); }
	
	uint64_t blkNum = pos / blockSize;
	uint16_t idx = pos % blockSize;

#if DebugBinaryDetailed == 2
	printf("Getting byte at %llu. Block number %llu, block index %llu\n", pos, blkNum, idx);
#endif

	// Help speed things up.
	if (blkNum == currBlockNum) {
#if DebugBinaryDetailed == 2
		int v = currentPtr[idx];
		printf("\tByte = %d\n", v);
#endif
		return currentPtr[idx];
	}
	
	LoadBlock(blkNum);
	
	currBlockNum = blkNum;
	// This should not fail
	uint64_t arrayIdx = blockNumAddrMap[blkNum];
	currentPtr = blockArray[arrayIdx];

#if DebugBinaryDetailed == 2
	int v = blockArray[arrayIdx][idx];
	printf("\tByte = %d\n", v);
#endif
	
	return blockArray[arrayIdx][idx];
};

uint64_t ABigBinaryFile::Size() const {
	return dataSize;
};

//----

// Block Size
uint16_t ABigBinaryFile::BlockSize() const {
	return blockSize;
};

//----

// Number of bytes for the last block of a file.
// This is most likely not the block size.
// blockCount * blockSize does not necessarily equal file size.
uint16_t ABigBinaryFile::LastBlockSize() const {
	DebugPretty

#if DebugBinaryDetailed == 2
	printf("Last block size is %llu\n", dataSize % blockSize);
#endif

	return dataSize % blockSize;
};

//----

// Number of blocks. This can be zero.
uint64_t ABigBinaryFile::BlockCount() const {
	return blockCount;
};

uint16_t ABigBinaryFile::CopyToBlob(void* dest, uint16_t size, uint64_t blockNumber) const {
	DebugPretty
	DebugPrintFmt("Copy block# %llu to %p. %d bytes maximum\n", blockNumber, dest, size);
	
	if (!dest || blockNumber >= blockCount) { throw ABinaryFile::ABinaryFileEx("Invalid CopyToBlob parameter"); }
	bzero(dest, size);
	
	auto itr = blockNumAddrMap.find(blockNumber);
	if (itr != blockNumAddrMap.end()) {
		// Use local store
		uint16_t maxSizeRead = size > blockSize ? blockSize : size;
		if (blockNumber == blockCount - 1) {
			if (maxSizeRead > (dataSize % blockSize)) { maxSizeRead = (dataSize % blockSize); }
		}
		
		char* ptr = blockArray[itr->second];
		memcpy(dest, ptr, maxSizeRead);
		
		return maxSizeRead;
	}
	return fread(dest, 1, size, file);
	
};

//----

void* ABigBinaryFile::CopyBlock_F(uint64_t blockNumber) {
	DebugPretty
	DebugPrint("Attempting to create a malloced copy of block %llu\n", blockNumber);
	
	if (blockNumber >= blockCount) { throw ABinaryFile::ABinaryFileEx("Bad block number"); }
	char* ptr = (char*)malloc(blockSize);
	bzero(ptr, blockSize);
	LoadBlock(blockNumber);
	
	uint64_t arrayIdx = blockNumAddrMap[blockNumber];
	if (blockNumber == blockCount - 1) {
		uint64_t sz = dataSize % blockSize;
		memcpy(ptr, blockArray[arrayIdx], sz);
	}
	else {
		memcpy(ptr, blockArray[arrayIdx], blockSize);
	}
	
	DebugPrint("\tBlock %llu copied\n", blockNumber);
	return ptr;
};

// Attempt to copy all file data to memory.
// It is all or nothing.
void* ABigBinaryFile::AllData() const {
	DebugPretty
	DebugPrint("Attempting to create a in-memory copy of the file (%%llu)\n", dataSize);
	
	char* ptr = (char*)malloc(dataSize);
	if (ptr) {
		fpos_t sz = 0;
		if (fsetpos(file, &sz)) {
			free(ptr);
			return nullptr;
		}
		if (fread(ptr, dataSize, 1, file) == 0) {
			free(ptr);
			return nullptr;
		}
	}
	return ptr;
};

// Purge all stored blocks.
// Force a file check. dataSize can change.
void ABigBinaryFile::Reset() {
	DebugPretty
	
	DataReset();
	FileCheck();
};
