//
//  ATextFile.cpp
//  CPP-Utilities
//
//  Created by tridiak on 12/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "ATextFile.hpp"
#include "Debug.hpp"

ATextFile::ATextFile() {
	DebugPretty
	
	textLF = NewLine::unix;
};

ATextFile::ATextFile(int desc, NewLine lf) : ABinaryFile(desc) {
	DebugPretty
	
	textLF = lf;
	RetrieveLines();
};

ATextFile::ATextFile(const std::string& path, NewLine lf) : ABinaryFile(path) {
	DebugPretty
	
	textLF = lf;
	RetrieveLines();
};

ATextFile::ATextFile(void* memoryBlock, uint64_t memSize, NewLine lf) : ABinaryFile(memoryBlock, memSize) {
	DebugPretty
	
	textLF = lf;
	RetrieveLines();
};

ATextFile::ATextFile(const ATextFile& obj) : ABinaryFile(obj) {
	DebugPretty
	
	textLF = obj.textLF;
	lines = obj.lines;
};

ATextFile ATextFile::operator=(const ATextFile& obj) {
	DebugPretty
	
	ABinaryFile::operator=(obj);
	textLF = obj.textLF;
	lines = obj.lines;
	
	return *this;
};

ATextFile::ATextFile(ATextFile&& ref) : ABinaryFile(std::move(ref)) {
	DebugPretty
	
	textLF = ref.textLF;
	std::swap(lines, ref.lines);
};

ATextFile ATextFile::operator=(ATextFile&& ref) {
	DebugPretty
	
	ABinaryFile::operator=(static_cast<const ABinaryFile&&>(std::move(ref)) );
	textLF = ref.textLF;
	std::swap(lines, ref.lines);
	
	return *this;
};

void ATextFile::Refresh() {
	RetrieveLines();
};

//----------------------------
#pragma mark -

bool ATextFile::IsLineFeed(uint64_t& idx) {
	char c = ABinaryFile::operator[](idx);
	switch (textLF) {
		case NewLine::classicMac:
			return c == 13;
			break;
		case NewLine::unix:
			return c == 10;
			break;
		case NewLine::windows:
			if (c != 13) { return false;}
			if (idx == Size() - 1) { return false; }
			char c = ABinaryFile::operator[](idx + 1);
			// If this position is a new line, we need to increment the index by one
			// otherwise RetrieveLines() will assume the LF character is
			// part of the text file.
			if (c == 10) {
				idx++;
				return true;
			}
			return false;
			break;
	};
};

void ATextFile::RetrieveLines() {
	DebugPretty
	
	lines.clear();
	std::string s;
	char c;
	bool lastNotLF = true;
	for (uint64_t idx = 0; idx < Size(); idx++) {
		c = ABinaryFile::operator[](idx);
		if (IsLineFeed(idx)) {
			lines.push_back(s);
			lastNotLF = false;
			s = "";
		}
		else {
			s.append(&c, 1);
			lastNotLF = true;
		}
	}
	if (lastNotLF) {
		lines.push_back(s);
	}
};

uint64_t ATextFile::LineCount() const {
	return lines.size();
};

std::string ATextFile::operator[](uint64_t line) const {
#ifdef DebugTextDetailed
	DebugPretty
#endif
	
	if (line >= LineCount()) { throw ATFException("Out of range"); }
	return lines[line];
};

SST::StringArray ATextFile::AllLines() const {
#ifdef DebugTextDetailed
	DebugPretty
#endif
	
	return lines;
};

char* ATextFile::CString_F(uint64_t line) const {
	DebugPretty
	
	if (line >= LineCount()) { return nullptr; }
	size_t len = lines[line].size();
	char* ptr = (char*)malloc(len + 1);
	if (ptr) {
		bzero(ptr, len + 1);
		memcpy(ptr, lines[line].c_str(), len);
	}
	return ptr;
};

//----------------------------
#pragma mark - ABigTextFile

/*
ABigTextFile::ABigTextFile() {
	textLF = LineFeed::unix;
	lastIsLF = false;
	maxLinesHeld = 0;
	doNotUpdate = false;
};
*/

ABigTextFile::ABigTextFile(int desc, uint16_t blockSz, uint64_t maxBlks, uint16_t maxLines, ATextFile::NewLine lf)
		: ABigBinaryFile(desc, blockSz, maxBlks) {
	DebugPretty
	
	textLF = lf;
	lastIsLF = false;
	maxLinesHeld = maxLines;
	doNotUpdate = false;
	
	RetrieveLinePositions();
};

ABigTextFile::ABigTextFile(const std::string& path, uint16_t blockSz, uint64_t maxBlks, uint16_t maxLines, ATextFile::NewLine lf)
 		: ABigBinaryFile(path, blockSz, maxBlks) {
	DebugPretty
	
	textLF = lf;
	lastIsLF = false;
	maxLinesHeld = maxLines;
	doNotUpdate = false;
	
	RetrieveLinePositions();
};

ABigTextFile::ABigTextFile(const ABigTextFile& obj) : ABigBinaryFile(obj) {
	// Not copied as ABigBinaryFile will reload the original file and the
	// file may have changed since last load.
	DebugPretty
	
//	lineFeedPositions = obj.lineFeedPositions;
	textLF = obj.textLF;
	lastIsLF = obj.lastIsLF;
	// See above.
//	lineCache = obj.lineCache;
//	lineHistory = obj.lineHistory;
	maxLinesHeld = obj.maxLinesHeld;
	doNotUpdate = false;
	
	RetrieveLinePositions();
};

ABigTextFile ABigTextFile::operator=(const ABigTextFile& obj) {
	DebugPretty
	
	ABigBinaryFile::operator=(obj);
	
	// Not copied as ABigBinaryFile will reload the original file and the
	// file may have changed since last load.
//	lineFeedPositions = obj.lineFeedPositions;
	textLF = obj.textLF;
	lastIsLF = obj.lastIsLF;
	// See above.
//	lineCache = obj.lineCache;
//	lineHistory = obj.lineHistory;
	maxLinesHeld = obj.maxLinesHeld;
	doNotUpdate = false;
	
	RetrieveLinePositions();
	
	return *this;
};

ABigTextFile::ABigTextFile(ABigTextFile&& ref) : ABigBinaryFile(std::move(ref)) {
	DebugPretty
	
	lineFeedPositions = ref.lineFeedPositions;
	textLF = ref.textLF;
	lastIsLF = ref.lastIsLF;
	lineCache = ref.lineCache;
	lineHistory = ref.lineHistory;
	maxLinesHeld = ref.maxLinesHeld;
	doNotUpdate = ref.doNotUpdate;
	
};

ABigTextFile ABigTextFile::operator=(ABigTextFile&& ref) {
	DebugPretty
	
	ABigBinaryFile::operator=(static_cast<const ABigBinaryFile&&>(std::move(ref)) );
	lineFeedPositions = ref.lineFeedPositions;
	textLF = ref.textLF;
	lastIsLF = ref.lastIsLF;
	lineCache = ref.lineCache;
	lineHistory = ref.lineHistory;
	maxLinesHeld = ref.maxLinesHeld;
	doNotUpdate = ref.doNotUpdate;
	
	return *this;
};

void ABigTextFile::RetrieveLinePositions() {
	DebugPretty
	
	lastIsLF = false;
	lineFeedPositions.clear();
	
	if (Size() == 0) { return; }
	lineFeedPositions.push_back(0);
	
	uint64_t sz;
	for (sz = 0; sz < Size(); sz++) {
		if (IsLineFeed(sz)) {
			// For windows new line, sz will be incremented by
			// an additional 1.
			lineFeedPositions.push_back(sz);
		}
	}
	
	if (textLF == ATextFile::NewLine::windows) {
		sz = Size() - 2;
		lastIsLF = IsLineFeed(sz);
	}
	else {
		sz = Size() - 1;
		lastIsLF = IsLineFeed(sz);
	}
	
};


//----------------------------
#pragma mark -

// Purge cache
void ABigTextFile::Purge() {
	DebugPretty
	
	lineCache.clear();
	lineHistory.clear();
};

// Calls Purge() & RetrieveLinePositions()
void ABigTextFile::Refresh() {
	DebugPretty
	
	Purge();
	RetrieveLinePositions();
};

uint64_t ABigTextFile::LineCount() const {
	if (Size() == 0) { return 0; }
	return lastIsLF ? lineFeedPositions.size() + 1 : lineFeedPositions.size();
};

bool ABigTextFile::IsLineFeed(uint64_t& idx) {
#if DebugTextDetailed == 2
	DebugPretty
#endif

	char c = ABigBinaryFile::operator[](idx);
	switch (textLF) {
		case ATextFile::NewLine::classicMac:
			return c == 13;
			break;
		case ATextFile::NewLine::unix:
			return c == 10;
			break;
		case ATextFile::NewLine::windows:
			if (c != 13) { return false;}
			if (idx == Size() - 1) { return false; }
			char c = ABigBinaryFile::operator[](idx + 1);
			// If this position is a new line, we need to increment the index by one
			// otherwise RetrieveLinePositions() will assume the LF character is
			// part of the text file.
			if (c == 10) {
				idx++;
				return true;
			}
			return false;
			break;
	};
};

void ABigTextFile::AddToHistory(uint64_t line, std::string& s) {
#ifdef DebugTextDetailed
	DebugPretty
	DebugPrintFmt("Adding %s to history\n", s.c_str());
#endif
	
	if (doNotUpdate) {
#ifdef DebugTextDetailed
		DebugPrint("\tDo not update flagged\n");
#endif
		return;
	}
	if (lineHistory.size() == maxLinesHeld) {
#if DebugTextDetailed == 2
		DebugPrint("\tRemoving old line\n");
#endif
		uint64_t old = lineHistory[0];
		lineHistory.erase(lineHistory.begin());
		lineCache.erase(old);
	//	printf("-");
	}
	
	lineHistory.push_back(line);
	lineCache[line] = s;
//	printf("+");
};

std::string ABigTextFile::operator[](uint64_t line) {
#if DebugTextDetailed == 2
	DebugPretty
	printf("Retrieving line %llu\n", line);
#endif
	
	if (line >= LineCount()) { throw ATextFile::ATFException("No such line"); }
	
	auto litr = lineCache.find(line);
	if (litr != lineCache.end()) {
#if DebugTextDetailed == 2
		printf("\tFrom cache\n");
#endif
		return litr->second;
	}
	
	int64_t pos = IsWindows() ? -2 : -1;
	if (line > 0) {
		pos = lineFeedPositions[line];
	}
	
	uint64_t nextLF = 0;
	if (line == LineCount() - 1) {
		nextLF = Size();
		if (lastIsLF) { nextLF -= (IsWindows() ? 2 : 1); }
	}
	else {
		nextLF = lineFeedPositions[line+1];
	}
	
	uint64_t actualPos = IsWindows() ? pos + 2 : pos + 1;
	std::string s;
	for (uint64_t idx = actualPos; idx < nextLF; idx++) {
		s += ABigBinaryFile::operator[](idx);
	}
	
	AddToHistory(line, s);
	
	return s;
};

SST::StringArray ABigTextFile::AllLines() {
	DebugPretty
	
	SST::StringArray ary;
	doNotUpdate = true;
	for (uint64_t line = 0; line < LineCount(); line++) {
		ary.push_back((*this)[line]);
	}
	doNotUpdate = false;
	
	return ary;
};

// Caller must call free()
char* ABigTextFile::CString_F(uint64_t line) {
	DebugPretty
	
	std::string s = (*this)[line];
	size_t len = s.size();
	char* ptr = (char*)malloc(len+1);
	bzero(ptr, len+1);
	memcpy(ptr, s.c_str(), len);
	
	return ptr;
};

