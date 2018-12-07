//
//  ATextFile.hpp
//  CPP-Utilities
//
//  Created by tridiak on 12/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef ATextFile_hpp
#define ATextFile_hpp

#include <stdio.h>
#include "ABinaryFile.hpp"
#include "StringStuff.hpp"

// Define if you want detailed information during calls.
// Note: CPPDebug has to be defined also.
// #define DebugTextDetailed 1
	// Set DebugTextDetailed to 2 if you want craps load of output.

//------
#pragma mark Text File

/*
Class which loads a text file.
It is all or nothing.
If the file cannot be loaded, an exception will be thrown.

It is read only.
*/
class ATextFile : public ABinaryFile {
public:
	enum class NewLine { classicMac, unix, windows };
	// CR 13, LF 10, CRLF 13 10

private:
	NewLine textLF;
	SST::StringArray lines;
	
	// A reference is needed so if it is the windows LF, we need to increment the index by
	// another 1.
	// See RetrieveLines()
	bool IsLineFeed(uint64_t& idx);
	
	// Parse through the loaded memory blob and retrieve all lines.
	void RetrieveLines();
public:
	
	ATextFile();
	// Throws ABinaryFileEx
	ATextFile(int desc, NewLine lf = NewLine::unix);
	// Throws ABinaryFileEx
	ATextFile(const std::string& path, NewLine lf = NewLine::unix);
	
	ATextFile(void* memoryBlock, uint64_t memSize, NewLine lf = NewLine::unix);
	
	ATextFile(const ATextFile& obj);
	ATextFile operator=(const ATextFile& obj);
	
	ATextFile(ATextFile&& ref);
	ATextFile operator=(ATextFile&& ref);
	
	//------------------
	// Clears lines array and retrieves them all again.
	// Currently redundant because the parent class never accesses the original
	// file one loaded.
	void Refresh();
	
	uint64_t LineCount() const;
	
	// Will throw ATFException if line >= lineCount
	std::string operator[](uint64_t line) const;
	
	// Copy of all lines.
	SST::StringArray AllLines() const;
	
	// If line >= line count, nil will be returned.
	// Caller must call free().
	char* CString_F(uint64_t line) const;
	
	//---------------------------------------
	struct ATFException : ABinaryFile::ABinaryFileEx {
		ATFException(std::string reason) : ABinaryFileEx(reason) {}
	};
};

//-----------------------------------
#pragma mark - Big Text File

/*
 Can't use ATFTextFile way of doing it as line count could get extraordinarily large.
 Initial creation will do a new line search of the file, gathering all new line positions.
 When a line is asked for, we get super class to load the block when it's accessed and
 then we create the line.
*/
class ABigTextFile : public ABigBinaryFile {
	// List of all line feed positions. For the case of windows LF, this will
	// be the first char.
	std::vector<uint64_t> lineFeedPositions;
	
	ATextFile::NewLine textLF;
	
	bool IsWindows() const { return textLF == ATextFile::NewLine::windows; }
	
	// If true, last character in file is the line feed character(s).
	// Line count is either lineFeedPositions count or lineFeedPositions count + 1.
	bool lastIsLF;
	
	// Key is line number, value is line.
	std::map<uint64_t, std::string> lineCache;
	// Oldest first
	std::vector<uint64_t> lineHistory;
	
	// Maximum number of lines in cache.
	uint16_t maxLinesHeld;
	
	// If true, do not add line to history.
	// Set by AllLines() so we do not waste time adding lines that will
	// be removed in a few iterations.
	bool doNotUpdate;
	
	// Cache
	void AddToHistory(uint64_t line, std::string& s);
	
	// Determine positions of all line feeds.
	void RetrieveLinePositions();
	
	// Will increment the idx by 1 if the new line is windows.
	// See RetrieveLinePositions().
	bool IsLineFeed(uint64_t& idx);
public:
	ABigTextFile() = delete;
	
	// Will throw any exception that ABigBinaryFile will throw.
	ABigTextFile(int desc, uint16_t blockSz, uint64_t maxBlks, uint16_t maxLines, ATextFile::NewLine lf = ATextFile::NewLine::unix);
	ABigTextFile(const std::string& path, uint16_t blockSz, uint64_t maxBlks, uint16_t maxLines, ATextFile::NewLine lf = ATextFile::NewLine::unix);
	
	// See ABigBinaryFile
	ABigTextFile(const ABigTextFile& obj);
	ABigTextFile operator=(const ABigTextFile& obj);
	
	ABigTextFile(ABigTextFile&& ref);
	ABigTextFile operator=(ABigTextFile&& ref);
	
	// Purge cache
	void Purge();
	
	// Calls Purge() & RetrieveLinePositions()
	void Refresh();
	
	uint64_t LineCount() const;
	
	// If line >= line count, an ATFException wil be thrown.
	// Exceptions from the parent class will not be caught.
	std::string operator[](uint64_t line);
	
	// Not advisable. Retrieval can be slow.
	// And the array could be really large.
	SST::StringArray AllLines();
	
	// Caller must call free()
	char* CString_F(uint64_t line);
	
	/*
	Future:
		Lock line (do not purge from cache or add it to a "permanent" history).
	*/
};

#endif /* ATextFile_hpp */
