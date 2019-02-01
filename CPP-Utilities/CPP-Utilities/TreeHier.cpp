//
//  DirHier.cpp
//  CPP-Utilities
//
//  Created by tridiak on 22/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "TreeHier.hpp"
#include "FlagUtilities.hpp"
#include <iostream>
#include "Debug.hpp"

namespace TreeHierarchy {

//-------------------------
#pragma mark - Constructors

#ifdef HierNodeDebug

uint64_t HierNode::nodeID = 0;

#endif

HierNode::HierNode(const std::string& p) : HierNode(PathClass(p)) {
	DebugPretty
};

HierNode::HierNode(const PathClass& p) {
	DebugPretty
	
	path = p;
	reservedPtr = nullptr;
	flags = 0;
	parent = nullptr;
	firstChild = nullptr;
	prev = nullptr;
	next = nullptr;

#ifdef InclUserValues
	userPtr = nullptr;
	user64 = 0;
#endif

#ifdef HierNodeDebug
	ID = ++nodeID;
	DebugPrint("Node created :" + std::to_string(ID), true);
#endif
};

HierNode::HierNode(const HierNode* node) {
	DebugPretty
	
	if (!node) { throw HierNodeEx(); }
	path = node->path;
	reservedPtr = node->reservedPtr;
	flags = node->flags;
	parent = nullptr;
	firstChild = nullptr;
	prev = nullptr;
	next = nullptr;

#ifdef InclUserValues
	userPtr = node->userPtr;
	user64 = node->user64;
#endif

#ifdef HierNodeDebug
	ID = ++nodeID;
	DebugPrint("Node created :" + std::to_string(ID), true);
#endif
};

/*
Deletes all children recursively (using deleted node destructors).
Siblings are also deleted. Detach if you do not want that to happen.
*/
HierNode::~HierNode() {
	DebugPretty
	
	if (firstChild) {
		firstChild->SiblingDelete();
	}
	
#ifdef HierNodeDebug
	DebugPrint("Node deleted :" + std::to_string(ID), true);
#endif
};

void DeleteNode(HierNodeRef node) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	
	if (node) {
		node->GoItAlone();
		delete node;
	}
};

// Subclasses must override this otherwise bad things will happen
// during gathering operation.
HierNodeRef HierNode::CreateNode(const PathClass& path) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	
	return new HierNode(path);
};

//-----------------------------
#pragma mark Flags

void HierNode::UserSetFlag(uint8_t f) {
	if (f >= 8 && f <= 15) { FlagUtils::SetFlagBit(f, flags); }
};

void HierNode::UserClearFlag(uint8_t f) {
	if (f >= 8 && f <= 15) { FlagUtils::ClearFlagBit(f, flags); }
};

bool HierNode::IsFlagSet(uint8_t f) {
	return FlagUtils::IsFlagBitSet(f, flags);
};

uint16_t HierNode::UserFlags() const {
	return flags & 0xFF00;
};

//---------------------------------
#pragma mark Gathering

// Children subclass override
const std::vector<PathClass> HierNode::Children() {
#if HierNodeDebug == 2
	DebugPretty
#endif
	return std::vector<PathClass>();
};

bool HierNode::GatherChildren() {
	DebugPrint("Gather children start\n");
	
	if (firstChild) { return false; }
	
	const std::vector<PathClass> children = Children();
	
	for (auto path : children) {
		HierNodeRef n = CreateNode(path); // new HierNode(path);
		AddChild(n);
		if (n->GatherChildren_R()) { return true; }
	}
	
	DebugPrint("Gather children end\n");
	return false;
};

bool HierNode::GatherChildren_R() {
#if HierNodeDebug == 2
	DebugPretty
#endif
	const std::vector<PathClass> children = Children();
#if HierNodeDebug == 2
	DebugPrint("Gathering children: " + path.Path() + "\n");
#endif
	
	for (auto path : children) {
		HierNodeRef n = CreateNode(path); // new HierNode(path);
		AddChild(n);
		if (n->GatherChildren_R()) { return true; }
	}
	
	return false;
};

// Proc method

bool HierNode::GatherChildren(HierNodeChildrenProc proc, void* userCtx) {
	DebugPrint("Gather children start\n");
	if (!proc) { throw HierNodeNilProcEx(); }
	
	// Cannot have any children
	if (firstChild) { return false; }
	const std::vector<PathClass> children = proc(path, userCtx);
	
	for (auto path : children) {
		HierNodeRef n = CreateNode(path); // new HierNode(path);
		AddChild(n);
		if (n->GatherChildren_R(proc, userCtx)) { return true; }
	}
	
	DebugPrint("Gather children end\n");
	return false;
};

bool HierNode::GatherChildren_R(HierNodeChildrenProc proc, void* userCtx) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	const std::vector<PathClass> children = proc(path, userCtx);
#if HierNodeDebug == 2
	DebugPrint("Gathering children: " + path.Path() + "\n");
#endif
	
	for (auto path : children) {
		HierNodeRef n = CreateNode(path); // new HierNode(path);
		AddChild(n);
		if (n->GatherChildren_R(proc, userCtx)) { return true; }
	}
	
	return false;
};

// Block method

bool HierNode::GatherChildren(HierNodeChildrenBlock block) {
	DebugPrint("Gather children start\n");
	if (!block) { throw HierNodeNilProcEx(); }
	
	// Cannot have any children
	if (firstChild) { return false; }
	const std::vector<PathClass> children = block(path);
	
	for (auto path : children) {
		HierNodeRef n = CreateNode(path); // new HierNode(path);
		AddChild(n);
		if (n->GatherChildren_R(block)) { return true; }
	}
	
	DebugPrint("Gather children end\n");
	return false;
};

bool HierNode::GatherChildren_R(HierNodeChildrenBlock block) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	const std::vector<PathClass> children = block(path);
#if HierNodeDebug == 2
	DebugPrint("Gathering children: " + path.Path() + "\n");
#endif
	
	for (auto path : children) {
		HierNodeRef n = CreateNode(path); // new HierNode(path);
		AddChild(n);
		if (n->GatherChildren_R(block)) { return true; }
	}
	
	return false;
};

//----

void HierNode::AddChild(HierNodeRef newChild) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	newChild->parent = this;
	if (firstChild) {
		firstChild->AddSibling(newChild, INT64_MAX);
	}
	else {
		firstChild = newChild;
	}
};

/***********************************************
where = 0: return false

All cases, the newSibling is detached from its existing hierarchy.

where < 0 & prev = nil:
	prev = newSibling; newSibling->next = this; Change parent's first child if present

where > 0 & next = nil:
	next = newSibling; newSibling->prev = this;

where < 0
	D = newSibling
		A				[B]				C
		next:B			next:C			next:nil
		prev:nil		prev:A			prev:B
 
	Insert D at -1 -> after A & before B
		nodePos = A
		afterNode = B
 
		A				D			[B]				C
		next:D			next:B		next:C			next:nil
		prev:nil		prev:A		prev:D			prev:B

where > 0
 	D = newSibling
	Insert D at 1 -> after B & before C
		nodePos = C
		beforeNode = B
 
		A				[B]				D			C
		next:D			next:D			next:C		next:nil
		prev:nil		prev:D			prev:B		prev:D
*/
bool HierNode::AddSibling(HierNodeRef newSibling, int64_t where) {
#if HierNodeDebug == 2
	DebugPretty
	printf("\tAdding node (%p). Delta %lld\n", newSibling, where);
#endif
	if (!newSibling || where == 0) { return false; }
	HierNodeRef nodePos = nullptr;
	
	// Detach if attached
	newSibling->GoItAlone();
	
	newSibling->parent = parent;
	// Node has no previous siblings and delta is before
	if (where < 0 && prev == nullptr) {
#if HierNodeDebug == 2
	printf("\tBefore this node (%p)\n", this);
#endif
		newSibling->next = this;
		prev = newSibling;
		if (parent) {
			parent->firstChild = newSibling;
		}
		return true;
	}
	// Node has no next siblings and delta is after
	if (where > 0 && next == nullptr) {
#if HierNodeDebug == 2
	printf("\tAfter this node (%p)\n", this);
#endif
		newSibling->prev = this;
		next = newSibling;
		return true;
	}
	
	// Get node where sibling will be placed at
	try {
		nodePos = (*this)[where];
	}
	catch (HierNodeBoundsEx& ex) { // First or last sibling
		if (where < 0) { nodePos = FirstSibling(); }
		else { nodePos = LastSibling(); }
	};
	
	if (where < 0) { // before

		HierNodeRef afterNode = nodePos->next;
		newSibling->next = afterNode;
		newSibling->prev = nodePos;
		if (afterNode) {
			afterNode->prev = newSibling;
		}
		nodePos->next = newSibling;
		if (newSibling->prev == nullptr && newSibling->parent) {
			// If prev is nil, then it will be the parent's new first child
			newSibling->parent = newSibling;
		}
#if HierNodeDebug == 2
		printf("\tBefore node (%p) and after node (%p)\n", afterNode, nodePos);
#endif
	}
	else { // after
	
		HierNodeRef beforeNode = nodePos->prev;
		newSibling->prev = beforeNode;
		newSibling->next = nodePos;
		if (beforeNode) {
			beforeNode->next = newSibling;
		}
		nodePos->prev = newSibling;
#if HierNodeDebug == 2
		printf("\tBefore node (%p) and after node (%p)\n", nodePos, beforeNode);
#endif
	}
	
	return true;
};

//---------------------------------------------
#pragma mark Deletion

// Recursively delete all children
void HierNode::DeleteAllChildren() {
#ifdef HierNodeDebug
	DebugPretty
#endif
	if (!firstChild) { return; }
	Delete_R(firstChild);
//	delete firstChild; Done in Delete_R()
	firstChild = nullptr;
};

void HierNode::Delete_R(HierNodeRef node) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	HierNodeRef ref = node->next;
	// Delete next sibling if it exists
	if (ref) {
		ref->DeleteAllChildren();
		Delete_R(ref);
	}
	delete node;
};

// Recursively delete this and all its siblings.
// Calls DeleteAllChildren().
void HierNode::SiblingDelete() {
#ifdef HierNodeDebug
	DebugPretty
#endif
	if (parent) {
		parent->DetachChildren();
	}
	std::vector<HierNodeRef> nodes = SplitSiblings(this);
	
	for (auto node : nodes) {
		delete node;
	}
};


// Detach children from parent.
// Caller is responsible for deletion by calling SiblingDelete().
HierNodeRef HierNode::DetachChildren() {
	DebugPretty
	
	if (!firstChild) { return nullptr; }
	
	HierNodeRef ptr = firstChild;
	while (ptr) {
		ptr->parent = nullptr;
		ptr = ptr->next;
	}
	
	ptr = firstChild;
	firstChild = nullptr;
	return ptr;
};

// static method
std::vector<HierNodeRef> HierNode::SplitSiblings(HierNodeRef node) {
	DebugPretty
	
	std::vector<HierNodeRef> array;
	if (!node) { return array; }
	if (node->parent) { throw HierNodeEx(); }
	
	HierNodeRef n = node->FirstSibling();
	HierNodeRef n2 = n;
	while (n) {
		if (n->parent) { throw HierNodeEx(); }
		array.push_back(n);
		n = n->next;
	}
	
	while (n2) {
		n = n2->next;
		n2->prev = nullptr;
		n2->next = nullptr;
		n2 = n;
	}
	
	return array;
};

HierNodeRef HierNode::GoItAlone() {
	DebugPretty
	
	// Check node is not parent's first child
	if (parent && parent->firstChild == this) {
		parent->firstChild = next;
	}
	
	// Detach from parent and siblings
	parent = nullptr;
	if (prev) { prev->next = next; }
	if (next) { next->prev = prev; }
	
	prev = nullptr;
	next = nullptr;
	
	return this;
};

//------------------------
#pragma mark Counts

// Count of immediate children
uint64_t HierNode::ChildCount() const {
	DebugPretty
	
	uint64_t ct=0;
	HierNodeRef ptr = firstChild;
	while (ptr) {
		if (CountThis()) { ct++; }
		ptr = ptr->next;
	}
	return ct;
};

// Count of siblings
uint64_t HierNode::SiblingCount() const {
	DebugPretty
	
	uint64_t ct=0;
	HierNodeRef ref = FirstSibling();
	while (ref) {
		if (CountThis()) { ct++; }
		ref = ref->next;
	};
	
	return ct;
};

// Count of all children
uint64_t HierNode::DeepChildCount() const {
	DebugPretty
	
	HierNodeRef ref = FirstSibling();
	uint64_t count=0;
	ref->DeepChildCount_R(count);
	
	return count;
};

void HierNode::DeepChildCount_R(uint64_t& count) const {
#if HierNodeDebug == 2
	DebugPretty
	printf("\tCount so far = %llu\n", count);
#endif
	if (CountThis()) { count++; }
	if (next) {
		next->DeepChildCount_R(count);
	}
	if (firstChild) {
		firstChild->DeepChildCount_R(count);
	}
};

void HierNode::SiblingCount(uint64_t& beforeMe, uint64_t& afterMe) const {
	DebugPretty
	
	beforeMe = 0;
	afterMe = 0;
	
	HierNodeRef node = prev;
	while (node) {
		beforeMe++;
		node = node->prev;
	}
	
	node = next;
	while (node) {
		afterMe++;
		node = node->next;
	}
};

// Used by child count methods. Return true if you want the node to be part of the count.
// Default is true.
bool HierNode::CountThis() const {
#if HierNodeDebug == 2
	DebugPretty
#endif
	return true;
};

std::vector<uint64_t> HierNode::WidthForRow() {
	DebugPretty
	
	__block std::vector<uint64_t> widthForRow;
	widthForRow.resize(GreatestDepth(), 0);
	
	HierNodeRef ptr = FirstSibling();
	while (ptr) {
		Iterate(^int(HierNodeRef node, int64_t depth) {
			widthForRow[depth] += 1;
			return 0;
		}, IterationMode::breadthFirst);
		ptr = ptr->next;
	}
	
	return widthForRow;
};

uint64_t HierNode::WidestRow(uint64_t& count) {
	DebugPretty
	
	uint64_t row = 0;
	count = 0;
	std::vector<uint64_t> widthForRow = WidthForRow();
	for (size_t t=0; t < widthForRow.size(); t++) {
		if (widthForRow[t] > count) {
			row = t;
			count = widthForRow[t];
		}
	}
	
	return row;
};

uint64_t HierNode::GreatestDepth() {
#ifdef HierNodeDebug
	DebugPretty
#endif
	__block uint64_t crtdepth = 0;
	
	Iterate(^int(HierNodeRef node, int64_t depth) {
		if (depth > crtdepth) { crtdepth = depth; }
		return 0;
	}, IterationMode::depthFirst);
	
	return crtdepth;
};

//-------------------------------------
#pragma mark Siblings

HierNodeRef HierNode::NextSibling() const {
	return next;
};

HierNodeRef HierNode::PrevSibling() const {
	return prev;
};

HierNodeRef HierNode::FirstSibling() const {
	HierNodeRef ref = const_cast<HierNodeRef>(this);
	while (ref->prev) {
		ref = ref->prev;
	};
	
	return ref;
};

HierNodeRef HierNode::LastSibling() const {
	HierNodeRef ref = const_cast<HierNodeRef>(this);
	while (ref->next) {
		ref = ref->next;
	};
	
	return ref;
};


// Get SIBLING at index idx RELATIVE to this node.
// Will throw an exception if idx exceeds sibling range.
HierNodeRef HierNode::operator[](int64_t idx) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (idx == 0) { return this; }
	int64_t index = 0;
	int dx = idx < 0 ? -1 : 1;
	bool back = idx < 0;
	HierNodeRef ref = back ? this->prev : this->next;
	index += dx;
	while (ref) {
		if (index == idx) { return ref; }
		ref = back ? ref->prev : ref->next;
		index += dx;
	}
	throw HierNodeBoundsEx();
};


// Can return nil
HierNodeRef HierNode::FirstChild() const {
	return firstChild;
};


std::vector<HierNodeRef> HierNode::AllAtDepthDown(int64_t depth) {
#ifdef HierNodeDebug
	DebugPretty
#endif
	__block std::vector<HierNodeRef> array;
	if (depth < 0) { return array; }
	if (depth == 0) {
		array.push_back(this);
		return array;
	}
	
	Iterate(^int(HierNodeRef node, int64_t dpt) {
		if (dpt == depth) {
			array.push_back(node);
		}
		return 0;
	}, IterationMode::depthFirst);
	
	return array;
};

// Find root and start from there.
std::vector<HierNodeRef> HierNode::AllAtDepth(int64_t depth) {
#ifdef HierNodeDebug
	DebugPretty
#endif
	if (depth == 0) { return SiblingArray(false); }
	
	// Always <= 0
	int64_t delta = 0;
	HierNodeRef root = GetRoot(delta);
	int64_t trueDepth = depth - delta;
	
	// Above root node.
	if (trueDepth < 0) { return std::vector<HierNodeRef>(); }
	
	std::vector<HierNodeRef> siblings = root->SiblingArray(false);
	// Root node row. Return all siblings.
	if (trueDepth == 0) { return siblings; }
	
	// Get all nodes at depth N for all siblings and join the arrays together.
	std::vector<HierNodeRef> nodes, allNodes;
	for (auto node : siblings) {
		nodes.clear();
		nodes = node->AllAtDepthDown(trueDepth);
		
		allNodes.insert(allNodes.end(), nodes.begin(), nodes.end());
	}
	
	return allNodes;
};

HierNodeRef HierNode::GetRoot(int64_t& delta) {
#ifdef HierNodeDebug
	DebugPretty
#endif
	delta = 0;
	HierNodeRef root = this;
	while (root->parent) {
		delta--;
		root = root->parent;
	}
	root = root->FirstSibling();
	
	return root;
};

//------------------------------------
#pragma mark Iterate

int HierNode::Iterate(HierNodeItrProc proc, void* userCtx, IterationMode mode) {
#ifdef HierNodeDebug
	DebugPretty
#endif
	if (!proc) { return rtnBadParam; }
	if (!firstChild) { return 0; }
	
	switch (mode) {
		case IterationMode::depthFirst:
			return Iterate_Depth_R(proc, userCtx, 0);
			break;
		case IterationMode::depthFirstEnd:
			return LastSibling()->Iterate_Depth_End_R(proc, userCtx, 0);
			break;
		case IterationMode::breadthFirst:
			return Iterate_Breadth_R(proc, userCtx, 0);
			break;
		case IterationMode::breadthFirstEnd:
			return LastSibling()->Iterate_Breadth_End_R(proc, userCtx, 0);
			break;
		default: exit(1);
	}
};

// Depth first iteration
int HierNode::Iterate_Depth_R(HierNodeItrProc proc, void* userCtx, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (proc(this, userCtx, depth)) { return rtnAllStop; }
	
	if (firstChild) {
		if (firstChild->Iterate_Depth_R(proc, userCtx, depth+1)) { return rtnAllStop; }
	}
	
	if (next) {
		if (next->Iterate_Depth_R(proc, userCtx, depth)) { return -rtnAllStop; }
	}
	
	return 0;
};

// Depth first iteration from last sibling
int HierNode::Iterate_Depth_End_R(HierNodeItrProc proc, void* userCtx, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif

	if (proc(this, userCtx, depth)) { return rtnAllStop; }
	
	if (firstChild) {
		if (firstChild->LastSibling()->Iterate_Depth_End_R(proc, userCtx, depth+1)) { return rtnAllStop; }
	}
	
	if (prev) {
		if (prev->Iterate_Depth_End_R(proc, userCtx, depth+1)) { return rtnAllStop; }
	}
	
	return 0;
};

// Breadth first iteration
int HierNode::Iterate_Breadth_R(HierNodeItrProc proc, void* userCtx, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	
	if (proc(this, userCtx, depth)) { return -1; }
	
	if (next) {
		if (next->Iterate_Breadth_R(proc, userCtx, depth)) { return rtnAllStop; }
	}
	
	HierNodeRef node = firstChild;
	while (node) {
		if (node->Iterate_Breadth_R(proc, userCtx, depth+1)) { return rtnAllStop; }
		node = node->next;
	}
	
	return 0;
};

// Breadth first iteration from last sibling
int HierNode::Iterate_Breadth_End_R(HierNodeItrProc proc, void* userCtx, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	
	if (proc(this, userCtx, depth)) { return rtnAllStop; }
	
	if (prev) {
		if (prev->Iterate_Breadth_R(proc, userCtx, depth)) { return rtnAllStop; }
	}
	
	HierNodeRef node = LastSibling();
	while (node) {
		if (node->Iterate_Breadth_R(proc, userCtx, depth+1)) { return rtnAllStop; }
		node = node->prev;
	}
	
	return 0;
};

//---------------------------------
// Blocks

int HierNode::Iterate(HierNodeItrBlock block, IterationMode mode) {
#ifdef HierNodeDebug
	DebugPretty
#endif
	if (!block) { return 1; }
	if (!firstChild) { return 0; }
	
	switch (mode) {
		case IterationMode::depthFirst:
			return Iterate_Depth_R(block, 0);
			break;
		case IterationMode::depthFirstEnd:
			return LastSibling()->Iterate_Depth_End_R(block, 0);
			break;
		case IterationMode::breadthFirst:
			return Iterate_Breadth_R(block, 0);
			break;
		case IterationMode::breadthFirstEnd:
			return LastSibling()->Iterate_Breadth_End_R(block, 0);
			break;
		default: exit(1);
	}
};

// Depth first iteration
int HierNode::Iterate_Depth_R(HierNodeItrBlock block, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (block(this, depth)) { return -1; }
	
	if (firstChild) {
		if (firstChild->Iterate_Depth_R(block, depth+1)) { return -1; }
	}
	
	if (next) {
		if (next->Iterate_Depth_R(block, depth)) { return -1; }
	}
	
	return 0;
};

// Depth first iteration from last sibling
int HierNode::Iterate_Depth_End_R(HierNodeItrBlock block, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (block(this, depth)) { return -1; }
	
	if (firstChild) {
		if (firstChild->LastSibling()->Iterate_Depth_End_R(block, depth+1)) { return -1; }
	}
	
	if (prev) {
		if (prev->Iterate_Depth_End_R(block, depth)) { return -1; }
	}
	
	return 0;
};

// Breadth first iteration
int HierNode::Iterate_Breadth_R(HierNodeItrBlock block, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
//	if (block(this, depth)) { return -1; }
	
	std::vector<HierNodeRef> ary = SiblingArray();
	
	for (auto n : ary) {
		if (block(n, depth)) { return -1; }
	}
	
	for (auto n : ary) {
		if (n->firstChild) {
			if (n->firstChild->Iterate_Breadth_R(block, depth+1)) { return -1; }
		}
	}
	
//	if (firstChild) {
//		if (firstChild->Iterate_Breadth_R(block, depth+1)) { return -1; }
//	}
	
	return 0;
};

// Breadth first iteration from last sibling
int HierNode::Iterate_Breadth_End_R(HierNodeItrBlock block, int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (block(this, depth)) { return -1; }
	
	if (prev) {
		if (prev->Iterate_Breadth_End_R(block, depth)) { return -1; }
	}
	
	if (firstChild) {
		if (firstChild->LastSibling()->Iterate_Breadth_End_R(block, depth+1)) { return -1; }
	}
	
	return 0;
};

// Subclass version
int HierNode::Iterate(IterationMode mode) {
#ifdef HierNodeDebug
	DebugPretty
#endif
	
	switch (mode) {
		case IterationMode::depthFirst:
			return Iterate_Depth_R(0);
			break;
		case IterationMode::depthFirstEnd:
			return LastSibling()->Iterate_Depth_End_R(0);
			break;
		case IterationMode::breadthFirst:
			return Iterate_Breadth_R(0);
			break;
		case IterationMode::breadthFirstEnd:
			return LastSibling()->Iterate_Breadth_End_R(0);
			break;
		default: exit(1);
	}
};

// Default does nothing
int HierNode::IterationAction(int64_t depth) {
	return 0;
};

// Depth first iteration
int HierNode::Iterate_Depth_R(int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (IterationAction(depth)) { return -1; }
	
	if (firstChild) {
		if (firstChild->Iterate_Depth_R(depth+1)) { return -1; }
	}
	
	if (next) {
		if (next->Iterate_Depth_R(depth)) { return -1; }
	}
	
	return 0;
};

// Depth first iteration from last sibling
int HierNode::Iterate_Depth_End_R(int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (IterationAction(depth)) { return -1; }
	
	if (firstChild) {
		if (firstChild->LastSibling()->Iterate_Depth_End_R(depth+1)) { return -1; }
	}
	
	if (prev) {
		if (prev->Iterate_Depth_End_R(depth)) { return -1; }
	}
	
	return 0;
};

// Breadth first iteration
int HierNode::Iterate_Breadth_R(int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (IterationAction(depth)) { return -1; }
	
	if (next) {
		if (next->Iterate_Breadth_R(depth)) { return -1; }
	}
	
	if (firstChild) {
		if (firstChild->Iterate_Breadth_R(depth+1)) { return -1; }
	}
	
	return 0;
};

// Breadth first iteration from last child
int HierNode::Iterate_Breadth_End_R(int64_t depth) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (IterationAction(depth)) { return -1; }
	
	if (prev) {
		if (prev->Iterate_Breadth_End_R(depth)) { return -1; }
	}
	
	if (firstChild) {
		if (firstChild->LastSibling()->Iterate_Breadth_End_R(depth+1)) { return -1; }
	}
	
	return 0;
};


//----------------------------------------
#pragma mark - Sorting

// Relink all the nodes.
void RelinkSorted(std::vector<HierNodeRef>& nodes) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	HierNodeRef node = nodes[0];
	if (node->parent) {
		node->parent->firstChild = node;
	}
	node->prev = nullptr;
	
	auto itr = nodes.begin();
	itr++;
	HierNodeRef nodeB = nullptr;
	while (itr != nodes.end()) {
		nodeB = *itr;
		node->next = nodeB;
		nodeB->prev = node;
		nodeB->next = nullptr;
		
		node = nodeB;
	}
};

// 'setSortFlag' is not implemented.
// The flags purpose was to prevent changes to the hierarchy while
// it was being iterated.
std::vector<HierNodeRef> HierNode::SiblingArray(bool setSortFlag) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	std::vector<HierNodeRef> array;
	HierNodeRef node = FirstSibling();
	while (node) {
		array.push_back(node);
		node = node->next;
	}
	
	return array;
};

std::vector<HierNodeRef> HierNode::SiblingArray() {
	return SiblingArray(false);
};

// Used by SiblingSort(HierNodeRef)
bool NodeCompareLT(HierNodeRef LHS, HierNodeRef RHS) {
	return LHS->Path() < RHS->Path();
};

// Used by SiblingSort(HierNodeRef)
struct NodeCompareS {
	bool operator()(HierNodeRef LHS, HierNodeRef RHS) const {
		return NodeCompareLT(LHS, RHS);
	};
};

// Sort siblings using Compare() method.
void HierNode::SiblingSort(HierNodeRef startNode) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (!startNode) { return; }
	std::vector<HierNodeRef> nodeArray = startNode->SiblingArray(true);
	
	if (nodeArray.size() == 1) { return; }
	if (nodeArray.size() == 2) {
		if (!NodeCompareLT(nodeArray[0], nodeArray[1]) ) {
			std::swap(nodeArray[0], nodeArray[1]);
			RelinkSorted(nodeArray);
			return;
		}
	}
	
	// 3+ elements
	std::sort(nodeArray.begin(), nodeArray.end(), NodeCompareS());
	
	RelinkSorted(nodeArray);
};

// If the callback is nil, the routine returns immediately and does nothing.
void HierNode::SiblingSort(HierNodeRef startNode, HierNodeSortProc proc, void* userCtx) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (!startNode || !proc) { return; }
	std::vector<HierNodeRef> nodeArray = startNode->SiblingArray(true);
	
	if (nodeArray.size() == 1) { return; }
	if (nodeArray.size() == 2) {
		if (!proc(nodeArray[0], nodeArray[1], userCtx) ) {
			std::swap(nodeArray[0], nodeArray[1]);
			RelinkSorted(nodeArray);
			return;
		}
	}
	
	// 3+ elements
	struct ProcCompare {
		HierNodeSortProc proc;
		void* userCtx;
		ProcCompare(HierNodeSortProc p, void* u) : proc(p), userCtx(u) {}
		bool operator()(HierNodeRef LHS, HierNodeRef RHS) const {
			return proc(LHS, RHS, userCtx);
		};
	} PC(proc, userCtx);
	
	std::sort(nodeArray.begin(), nodeArray.end(), PC);
	
	RelinkSorted(nodeArray);
	
};

// If the block is nil, the routine returns immediately and does nothing.
void HierNode::SiblingSort(HierNodeRef startNode, HierNodeSortBlock block) {
#if HierNodeDebug == 2
	DebugPretty
#endif
	if (!startNode || !block) { return; }
	std::vector<HierNodeRef> nodeArray = startNode->SiblingArray(true);
	
	if (nodeArray.size() == 1) { return; }
	if (nodeArray.size() == 2) {
		if (!block(nodeArray[0], nodeArray[1]) ) {
			std::swap(nodeArray[0], nodeArray[1]);
			RelinkSorted(nodeArray);
			return;
		}
	}
	
	// 3+ elements
	struct ProcCompare {
		HierNodeSortBlock block;
		ProcCompare(HierNodeSortBlock b) : block(b) {}
		bool operator()(HierNodeRef LHS, HierNodeRef RHS) const {
			return block(LHS, RHS);
		};
	} PC(block);
	
	std::sort(nodeArray.begin(), nodeArray.end(), PC);
	
	RelinkSorted(nodeArray);
};

} // namespace
