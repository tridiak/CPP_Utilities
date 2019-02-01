//
//  Usage.cpp
//  CPP_Test
//
//  Created by tridiak on 7/12/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "Usage.hpp"
#include "StringStuff.hpp"
#include "Converters.hpp"
#include "MultiString.hpp"
#include "PathClass.hpp"
#include "ABinaryFile.hpp"
#include "ATextFile.hpp"
#include "PosNeg.hpp"
#include "TreeHier.hpp"
#include "DirContents.hpp"
#include "FileUtil.hpp"
#include <iostream>

#pragma mark String Stuff

void StringStuffUsage() {
	std::string aString = "Big [red] jelly bean";
	
	// String Between
	long openPos, closePos;
	
	std::cout << aString << " -> " << SST::RPStringBetween(aString, "", "", openPos, closePos) << "\n";
	std::cout << aString << " -> " << SST::RPStringBetween(aString, "[", "", openPos, closePos) << "\n";
	std::cout << aString << " -> " << SST::RPStringBetween(aString, "", "]", openPos, closePos) << "\n";
	std::cout << aString << " -> " << SST::RPStringBetween(aString, "[", "]", openPos, closePos) << "\n";
	std::cout << aString << " -> " << SST::RPStringBetween(aString, "*", "", openPos, closePos) << "\n";
	std::cout << aString << " -> " << SST::RPStringBetween(aString, "Big [red] jelly", "jelly bean", openPos, closePos) << "\n";
	
	// Remove from set
	
	std::string s = aString;
	SST::RPStringRemoveFromSet(s, "e", false);
	std::cout << aString << " -> " << s << "\n";
	s = aString;
	SST::RPStringRemoveFromSet(s, "e", true);
	std::cout << aString << " -> " << s << "\n";
	
	// String after last character
	std::cout << aString << " -> " << SST::RPStrFindFromBack(aString, ' ') << "\n";
	// String before last character
	std::cout << aString << " -> " << SST::RPStrBeforeMarker(aString, ' ') << "\n";
	
	// String divider
	std::string b, a;
	SST::RPStringBeforeAndAfter(aString, "[red]", b, a);
	std::cout << aString << " -> " << "Before: " << b << " ;After: " << a << "\n";
	
	// Integer to string with suffix
	long V = 12345678;
	std::cout << SST::RPXByteString(V) << "\n";
	std::cout << SST::RPXByteString1000(V) << "\n";
	
	// String with suffix to integer
	std::string ary[] = {"1.23GB", "1.23 GB", "1.23 GiB", "1.23", "1.2.3"};
	for (int t=0; t < 5; t++) {
		std::cout << ary[t] << " -> " << std::to_string(SST::RPStringToBytes(ary[t])) << "\n";
	}
	
	// Reduce characters
	a = "A111BBBBCDD11E";
	b = a;
	SST::RPReduceConsecutiveChars(a);
	SST::RPReduceConsecutiveChars(a, '1');
	std::cout << a << "\n";
	std::cout << b << "\n";
};

//-----------------------------------------
#pragma mark Converters

void ConvertersUsage() {
	std::vector<char> keys = {'a','b','c','d','e'};
	std::vector<int> values = {12, 24, 36, 48, 92};
	
	printf("Arrays to Map\n");
	std::map<char,int> map = ArraysToMap(keys, values);
	
	for (auto itr : map) {
		std::cout << itr.first << ":" << itr.second << "\n";
	}
	
	ArrayPair<char,int> vectors = MapToArrays(map);
	
	printf("Map to Array\n");
	for (auto v : vectors.first) {
		std::cout << v << "\n";
	}
	for (auto v : vectors.second) {
		std::cout << v << "\n";
	}
	
};

//-----------------------------------------
#pragma mark MultiString

void MSUsage() {
	MultiString ms(SST::StringArray({"A", "a", "B", "b", "C", "D"}));
	SST::RPOutputSequence(std::cout, ms.Set().begin(), ms.Set().end(), "\t" );
	printf("\n");
	
	ms.SetCaseInsen(true);
	
	printf("MS contains 'c': %s\n", ms == "c" ? "T" : "F");
	
	ms.SetCaseInsen(false);
	
	printf("MS contains 'c': %s\n", ms == "c" ? "T" : "F");
	
};

//-----------------------------------------
#pragma mark Path Class

#define Write(S) std::cout << S << "\n";
#define WriteN(S) std::cout << S;
#define WT printf("\t");

void PathClassUsage() {
	PathClass A("/A/path/to/file.txt");
	Write(A.Path());
	
	Write("Appending Dir Marker");
	A.AppendDirMarker();
	Write(A.Path());
	
	Write("Removing Dir Marker");
	A.RemoveDirMarker();
	Write(A.Path());
	
	Write("Removing last component");
	A--;
	Write(A.Path());
	
	Write("Appending Path Component 'file.txt'");
	A.AddPath(std::string("file.txt"));
	Write(A.Path());
	
	Write("Removing suffix");
	A.RemoveSuffix();
	Write(A.Path());
	
	Write("Appending suffix 'txt'");
	A.AddSuffix("txt");
	Write(A.Path());
	
	Write("Writing last component");
	Write(A.LastComponent());
	
	PathClass B = A;
	Write("Swap dir marker");
	B.SwapDirMarkers('.');
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	Write("Convert Dir Marker");
	B = A;
	B.ConvertDirMarkers('.');
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	Write("Change Dir Marker");
	B = A;
	B.ChangeDirMarker('.');
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	Write("Prepend Path 'root'");
	B = A;
	B.PrependPath(std::string("root"));
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	Write("Prepend Path 'root/'");
	B = A;
	B.PrependPath(std::string("root/"));
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	Write("Prepend Path '/root'");
	B = A;
	B.PrependPath(std::string("/root"));
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	Write("Prepend Path '/root/'");
	B = A;
	B.PrependPath(std::string("/root/"));
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	Write("Add path component (+=) dummy.png");
	B = A;
	B += std::string("dummy.png");
	WriteN(A.Path()); WriteN(" -> "); Write(B.Path());
	
	SST::StringArray parts = A.Components();
	SST::RPOutputSequence(std::cout, parts.begin(), parts.end(), ",");
};

//-----------------------------------------
#pragma mark Binary File

void BinaryFileUsage() {
	try {
		// Uncomment CPPDebug in Debug.hpp if you want to see constructor/destructor calls.
		
		// *******************
		// Enter your own file path here
		const char* filePath = "/Users/tridiak/Programming/Active_Projects/DandD/FeatNameList.txt";
		
		ABinaryFile abf(filePath);
		
		for (int t=0; t < 8; t++) {
			printf("%X ", (int)abf[t]);
		}
		printf("\n");
		
		for (int t=0; t < 8; t+=2) {
			printf("%X ", abf.B16(t));
		}
		printf("\n");
		
		for (int t=0; t < 8; t+=4) {
			printf("%X ", abf.B32(t));
		}
		printf("\n");
		
		for (int t=0; t < 8; t+=8) {
			printf("%llX ", abf.B64(t));
		}
		printf("\n");
		
		
		ABinaryFile abf2 = abf;
		ABinaryFile abf3 = std::move(abf);
		
		//-------------------------------------
		// Uncomment DebugBinaryDetailed in ABinaryFile.hpp to see block accesses.
		ABigBinaryFile big(filePath, 256, 4);
		
		printf("Sequential Read\n");
		for (int t=0; t < 3000; t++) {
			printf("%X ", (int)big[t]);
		}
		printf("\n");
		
		printf("Random Read\n");
		for (int t=0; t < 3000; t++) {
			long r = arc4random() % big.Size();
			printf("%X ", (int)big[r]);
		}
		printf("\n");
		
		printf("Access beyond data size\n");
		printf("%X ", (int)big[INT64_MAX]);
	}
	catch (ABinaryFile::ABinaryFileEx& ex) {
		printf("Exception : %s\n", ex.what());
	};
};

//-----------------------------------------
#pragma mark Text File

void TextFileUsage() {
	try {
		// Uncomment CPPDebug in Debug.hpp if you want to see constructor/destructor calls.
		// Uncomment DebugTextDetailed in ATextFile.hpp to see text accesses.
		
		// *******************
		// Enter your own file path here
		const char* filePath = "/Users/tridiak/Programming/Active_Projects/DandD/FeatNameList.txt";
		
		printf("-------------------------A Text File\n");
		ATextFile atf(filePath);
		
		printf("-------------------------Sequential line read\n");
		for (int t=0; t < 10; t++) {
			Write(atf[t % 2]);
		}
		
		printf("-------------------------Random line read\n");
		for (int t=0; t < 10; t++) {
			long r = arc4random() % atf.LineCount();
			Write(atf[r]);
		}
		
		printf("-------------------------All Lines\n");
		auto ary = atf.AllLines();
		for (size_t t=0; t < ary.size(); t++) {
			Write(ary[t]);
		}
		
		//-----------------------
		// Uncomment DebugBinaryDetailed in ABinaryFile.hpp to see block accesses.
		
		printf("-------------------------A Big Text File\n");
		ABigTextFile btf(filePath, 256, 8, 8);
		
		printf("-------------------------Sequential line read\n");
		for (int t=0; t < 10; t++) {
			Write(btf[t % 2]);
		}
		
		printf("-------------------------Random line read\n");
		for (int t=0; t < 10; t++) {
			long r = arc4random() % atf.LineCount();
			Write(btf[r]);
		}
		
		printf("-------------------------All Lines\n");
		auto bary = btf.AllLines();
		for (size_t t=0; t < bary.size(); t++) {
			Write(bary[t]);
		}
	}
	catch (ABinaryFile::ABinaryFileEx& ex) {
		printf("Exception : %s\n", ex.what());
	};
};

//-----------------------------------------
#pragma mark PosNeg class

void PosNegUsage() {
	try {
		const CPosNeg& N = CPosNeg::Create(-1);
		const CPosNeg& P = CPosNeg::Create(1);
		const CPosNeg& Z = CPosNeg::Create(0);
		
		const PNPositive& A = dynamic_cast<const PNPositive&>(CPosNeg::Create(1));
		
		printf("%s\n", P.Name());
		printf("%s\n", Z.Name());
		printf("%s\n", N.Name());
		
		const CPosNeg& R = N++;
		printf("Negative++ -> %s\n", R.Name());
		
		const CPosNeg& X = N / P;
		printf("Negative / Positive -> %s\n", X.Name());
	}
	catch (CPosNeg::nonsensicalEx& ex) {
		printf("Exception: %s\n", ex.what());
	};
};

//-----------------------------------------
#pragma mark Hierarchy

using namespace TreeHierarchy;

class AHier : public HierNode {
public:
	AHier(const std::string& path) : HierNode(path) {}
	
	// MUST override method
	HierNodeRef CreateNode(const PathClass& path) {
		return new AHier(path.Path());
	};
	
	const std::vector<PathClass> Children() {
		if (!FileUtil::IsADir(Path().Path()) ) { return PathClassArray(); }
		DirContent::DirContents dc(Path());
		dc.Gather();
		return dc.Items();
	};
	
	// Let's just print file/directory names.
	int IterationAction(int64_t depth) {
		for (int t=0; t < depth; t++) { printf("  "); }
		Write(Path().LastComponent());
	
		return 0;
	};
};

void HeirarchyUsage() {
	try {
		// *************************
		// Set own path here.
		PathClass path("/Users/tridiak/Temp/A");
		
		HierNodeRef root = new AHier(path.Path());
		
		root->GatherChildren();
		
		printf("Breadth first\n");
		root->Iterate(AHier::IterationMode::breadthFirst);
		
		printf("Breadth first end\n");
		root->Iterate(AHier::IterationMode::breadthFirstEnd);
		
		printf("Depth first\n");
		root->Iterate(AHier::IterationMode::depthFirst);
		
		printf("Depth first end\n");
		root->Iterate(AHier::IterationMode::depthFirstEnd);
		
		std::vector<HierNodeRef> D1 = root->AllAtDepth(1);
		printf("Depth 1\n");
		for (auto N : D1) {
			Write(N->Path().Path());
		}
		
		std::vector<HierNodeRef> D3 = root->AllAtDepth(3);
		printf("Depth 3\n");
		for (auto N : D3) {
			Write(N->Path().Path());
		}
		
		printf("Root's children\n");
		HierNodeRef firstChild = root->FirstChild();
		std::vector<HierNodeRef> siblings = firstChild->SiblingArray();
		for (auto N : siblings) {
			Write(N->Path().Path());
		}
		
		uint64_t row=0;
		uint64_t count=0;
		row = root->WidestRow(count);
		printf("Widest Row = %llu, count = %llu\n", row, count);
		
		printf("Greatest Depth = %llu\n", root->GreatestDepth());
		
		// You now own firstChild
		firstChild->GoItAlone();
		printf("Root\n");
		
		root->Iterate(AHier::IterationMode::breadthFirst);
		printf("Ex-first child\n");
		firstChild->Iterate(AHier::IterationMode::breadthFirst);
		
	}
	catch (TreeHierarchy::HierNodeEx& ex) {
		printf("Exception: %s\n", ex.what());
	};
};


