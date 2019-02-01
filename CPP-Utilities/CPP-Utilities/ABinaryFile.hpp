//
//  ABinaryFile.hpp
//  CPP-Utilities
//
//  Created by tridiak on 6/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef ABinaryFile_hpp
#define ABinaryFile_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <exception>
#include <set>
#include <map>

// Define if you want detailed information during calls.
// Note: CPPDebug has to be defined also.
// #define DebugBinaryDetailed 1
	// Set DebugBinaryDetailed to 2 if you want craps load of output.

//---------------------------------------------
// Class that loads a file into memory.
// If is all-or-nothing. It will never write back to the original file.

class ABinaryFile {
	void* blob;
	uint64_t size;
	
	friend void Swap(ABinaryFile& A, ABinaryFile& B);
protected:
	// For use by subclasses
	ABinaryFile(void* memoryBlock, uint64_t memSize);
public:
	// Keeping this so other class copy constructors will be happy
	ABinaryFile();
	
	// Can throw ABinaryFileEx
	ABinaryFile(int desc);
	
	// Can throw ABinaryFileEx
	ABinaryFile(const std::string& path);
	
	// Copy
	ABinaryFile(const ABinaryFile& obj);
	virtual ABinaryFile operator=(ABinaryFile obj);
	
	// Move
	ABinaryFile(ABinaryFile&& ref);
	virtual ABinaryFile operator=(ABinaryFile&& obj);
	
	~ABinaryFile();
	//------------------
	
	// Can return 0
	uint64_t Size() const;
	
	// Can throw ABinaryFileEx
	uint8_t operator[](uint64_t pos) const;
	
	/// Returns 16 bit integer from BYTE position.
	/// If position+2 exceeds size, an exception will be thrown
	uint16_t B16(uint64_t bytePos) const;
	
	/// Returns 32 bit integer from BYTE position.
	/// If position+4 exceeds size, an exception will be thrown
	uint32_t B32(uint64_t bytePos) const;
	
	/// Returns 64 bit integer from BYTE position.
	/// If position+8 exceeds size, an exception will be thrown
	uint64_t B64(uint64_t bytePos) const;
	
	// Pointer to loaded data.
	virtual const void* Blob() const;
	
	//----------------------------------------
	// Also used by ABigBinaryFile
	struct ABinaryFileEx : std::exception {
		std::string reason;
		ABinaryFileEx(const std::string& reason) : reason(reason) {}
		const char* what() const _NOEXCEPT { return reason.c_str(); }
	};

	struct FileAccessEx : ABinaryFileEx {
		FileAccessEx() : ABinaryFileEx("Backing file is missing") {}
		FileAccessEx(const std::string& reason): ABinaryFileEx(reason) {}
	};
	
	// Future
	// operator[] with signed integer. If -ve, the byte is read from the end of the data block.
	
};

//------------------------------------------------
#pragma mark -

/*
 Class to access very big files.
 The class loads a maximum of N blocks which are of size blockSize.
 The blockSize range is 256 - 65535.
 Max block count range is 1 to UINT_MAX, though a small number is detrimental to performance.
 A very large number will result in the system throwing a fit.
 The constructors allocate a memory block whose size = maxBlock x blockSize.
 This is stored in blockArray.

 The file is opened during construction, if it fails and exception will be thrown.
 
 If the file becomes inaccessible or it is closed behind the class's back, an exception will be thrown.
*/

class ABigBinaryFile {
	// File data size
	uint64_t dataSize;
	
	// Either-Or
	int fileDesc;
	std::string path;
	// Remains open until destructor is called.
	FILE* file;
	
	// Nunber of bytes per block. Minimum is 256.
	uint16_t blockSize;
	// Number of blocks = dataSize / blockSize, rounded up.
	int64_t blockCount;
	
	// Usually user set. Cannot be zero or >= blockCount
	uint64_t maxBlocks;
	
	// size = maxBlocks.
	// Array of calloc'ed pointers.
	// blockArray pointer is only created in the constructors
	// and deleted in the destructor.
	char* *blockArray;
	
	// key is blockNumber, value is blockArray index
	std::map<int64_t, uint64_t> blockNumAddrMap;
	
	// oldest in front
	std::vector<uint64_t> blkNumberHistory;
	
	// Could use boost dynamic bit set
	std::vector<bool> freeSections;
	
	// Does nothing in this implementation as the block is
	// is one large memory allocation.
	void BlockErase();
	
	// Zero block
	void ZeroSection(uint64_t section);
	
	// Zero block after it is purged/resued.
	// Default is false.
	bool zeroBlocks;
	
	void OpenFile();
	
	// Throws FileAccessEx if there is an issue accessing the underlying file.
	void LoadBlock(uint64_t blkNum);
	
	//------------------
	struct	timespec lastCheck;
	
	void DataReset();
public:
	// Non-sensical
	ABigBinaryFile() = delete;
	
	// Throws ABinaryFileEx
	ABigBinaryFile(int desc, uint16_t blockSz, uint64_t maxBlks);
	
	// Throws ABinaryFileEx
	ABigBinaryFile(const std::string& path, uint16_t blockSz, uint64_t maxBlks);
	
	// The copy constructor will not copy loaded blocks, block history or freeSections.
	// The FILE* property will not be copied. A new one will be opened.
	// Using a copy constructor or assignment operator can be expensive in time and
	// memory.
	ABigBinaryFile(const ABigBinaryFile& obj);
	ABigBinaryFile operator=(const ABigBinaryFile& obj);
	
	// Move constructors
	ABigBinaryFile(ABigBinaryFile&& ref);
	ABigBinaryFile& operator=(ABigBinaryFile&& ref);
	
	~ABigBinaryFile();
	
	// If the block containing the index is not present, it will be loaded,
	// purging existing blocks if required.
	// If pos >= dataSize, ABinaryFileEx will be thrown.
	// This can throw FileAccessEx as it may call through to LoadBlock()
	uint8_t operator[](uint64_t pos);
private:
	// Last accessed block. Saves looking up blockNumAddrMap dictionary all the time.
	char* currentPtr;
	// If < 0, no last block
	int64_t currBlockNum;
public:
	// Do not zero blocks when loading new data.
	void SkipZeroing(bool yes) { zeroBlocks = yes; }
	
	// Returns size of file data
	uint64_t Size() const;
	
	// Block Size
	uint16_t BlockSize() const;
	
	// Number of bytes for the last block of a file.
	// This is most likely not the block size.
	// blockCount * blockSize does not necessarily equal file size.
	// = dataSize % blockSize
	uint16_t LastBlockSize() const;
	
	// Number of blocks. This can be zero.
	uint64_t BlockCount() const;
	
	//
	bool BlockIsLoaded(uint64_t blockNumber) const;
	
	// List of currently loaded blocks
	std::vector<uint64_t> LoadedBlocks() const;
	
	// Preload a block. If blockNumber >= blockCount, nothing will happen.
	// But FileAccessEx can be thrown as this calls through to LoadBlock()
	void Preload(uint64_t blockNumber);
	
	// If any parameter is invalid, an exception will be thrown.
	// If size < block size, first size bytes will be copied.
	// Return value is number of bytes copied. This can be less than
	// the block size if the the last block of the file is not block size bytes.
	uint16_t CopyToBlob(void* dest, uint16_t size, uint64_t blockNumber) const;
	
	// If blockNumber is >= block count, an exception will be thrown.
	// Pointer returned is always blockSize bytes.
	// nullptr should never be returned. If selected block is the last block,
	// call LastBlockSize() to get actual byte count.
	// Caller must call free()
	void* CopyBlock_F(uint64_t blockNumber);
	
	// A pointer to the blob is not returned because the data pointed to
	// can arbitarily change.
	
	// Attempt to copy all file data to memory.
	// It is all or nothing. Caller must call free()
	void* AllData() const;
	
	// Purge all stored blocks.
	// Force a file check. dataSize can change.
	void Reset();
	
	// Check if file still exists, is accessible and whether or not it has changed.
	// This will clear internal data if changes are detected.
	// A ABinaryFileEx exception will be thrown if there is any issues with the underlying file.
	// If it has been modified, all internal data will be cleared.
	void FileCheck();
	
	/*
	Future
	------
	LockBlock() - prevent a block from being purged.
	operator[] with signed integer. If -ve, the byte is read from the end of the data block.
	*/
	
};
#endif /* ABinaryFile_hpp */
