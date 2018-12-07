//
//  DirHier.hpp
//  CPP-Utilities
//
//  Created by tridiak on 22/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef DirHier_hpp
#define DirHier_hpp

#include <stdio.h>
#include "PathClass.hpp"
#include <vector>
#include <exception>
#include <algorithm>

// Define if you want detailed information during calls.
// Note: CPPDebug has to be defined also.
// #define HierNodeDebug 1
	// Set HierNodeDebug to 2 if you want craps load of output.

namespace TreeHierarchy {

typedef class HierNode* HierNodeRef;
typedef const class HierNode* ConstHierNodeRef;

//----------------

enum DNMessages {
	
	// Return messages
	rtnAllStop 			= -1,
	rtnContinue			= 0,
	rtnSuccess			= 0,
	rtnBadParam			= 1,
	
};

//--------------------------------
#pragma mark - typedefs

// Return true if LHS < RHS
typedef bool (*HierNodeSortProc)(const HierNodeRef LHS, const HierNodeRef RHS, void* userCtx);
typedef bool (^HierNodeSortBlock)(const HierNodeRef LHS, const HierNodeRef RHS);

// Used by Iterate() method
// Return 0 to continue, -1 for all stop.
typedef int (*HierNodeItrProc)(HierNodeRef node, void* userCtx, int64_t depth);
typedef int (^HierNodeItrBlock)(HierNodeRef node, int64_t depth);

// Return all children for passed path.t Do not delve into sub-directories.
typedef const std::vector<PathClass> (*HierNodeChildrenProc)(const PathClass& parent, void* userCtx);
typedef const std::vector<PathClass> (^HierNodeChildrenBlock)(const PathClass& parent);

//------------------------------
#pragma mark - Exceptions

// General exception
struct HierNodeEx : std::exception {
	const char* what() const _NOEXCEPT {
		return "DirNode exception";
	};
};

// Out of range exception
struct HierNodeBoundsEx : HierNodeEx {
	const char* what() const _NOEXCEPT {
		return "Out of range";
	};
};

// Node is in use (usually because of sorting or part of an iteration process).
struct HierNodeInUseEx : HierNodeEx {
	const char* what() const _NOEXCEPT {
		return "Node is in use";
	};
};

// Callback or block is not set.
struct HierNodeNilProcEx : HierNodeEx {
	const char* what() const _NOEXCEPT {
		return "nil callback or block";
	};
};

//-----------------------------------
#pragma mark -

/*
Creates a hierarchy of linked nodes.

If a node has children, it will only store a reference(pointer) to its first child.
Children are a linked list. First child previous sibling should be nil.
All children have a reference to their parent.

To gather the children of a node, call GatherChildren() with a children retrieval callback/block.
The callback is called for every node.

------
The class is NOT thread safe or re-entrant.

------
**** Subclassing
If you subclass HierNode, you MUST override CreateNode() or any created children & siblings
will not your subclass.

-----
Ownership

Any node returned by any method belongs to the node you called to retrieve it unless
specifically stated otherwise.
DO NOT delete the node otherwise a crash will eventually occur.

These methods transfer ownership to you
 	DetachChildren() - node returned is the first sibling. You do not own any
		other sibling or children.
 	GoItAlone() - node returned by method only
 	SplitSiblings() - all nodes returned in array, but not their children.

*/
class HierNode {
	PathClass path;
	void* reservedPtr;

#ifdef HierNodeDebug
	static uint64_t nodeID;
	uint64_t ID;
public:
	uint64_t NodeID() const { return nodeID; }
private:
	
#endif
	
	// b0-7 are reserved
	// b8-15 are for users
	uint16_t flags;
	
public:

	// flag bit number
	void UserSetFlag(uint8_t f);
	void UserClearFlag(uint8_t f);
	
	// Flag bit
	bool IsFlagSet(uint8_t f);
	
	// returns flags & 0xFF00
	uint16_t UserFlags() const;
private:
	
	// Root has no parent or siblings.
	HierNodeRef parent;
	HierNodeRef firstChild;
	HierNodeRef prev;
	HierNodeRef next;
	
	// Adds child to end of child list
	void AddChild(HierNodeRef newChild);
	
	// Delta position relative to this node.
	// If zero, the routine will do nothing and return false.
	// The newSibling will be detached (GoItAlone() ) from its parent and siblings.
	bool AddSibling(HierNodeRef newSibling, int64_t where);
	
	// Copies some information of the passed node.
	// Children & siblings are not copied.
	// parent will be set to nil.
	HierNode(const HierNode* node);
	
public:
#pragma mark Constructors
	// The class does not check the validity of the path.
	HierNode(const std::string& path);
	HierNode(const PathClass& path);
	
	// Copy constructor is currently disabled because a deep copy of a large hierarchy
	// can be high in time and memory consumption.
	HierNode(const HierNode& obj) = delete;
	HierNode operator=(const HierNode& obj) = delete;
	
	// Deletes this node and all children.
	// If the node is not detached, the parent and siblings will possess dangling
	// pointers.
	// Use DeleteNode() instead.
	~HierNode();
	
	// This will detach the node and then delete it.
	static void DeleteNode(HierNodeRef node);
	
	PathClass Path() const { return path; }
	
	//**********************************************
	// If you subclass HierNode, you MUST implement this method calling your subclass's
	// constructor otherwise when new nodes are created, the subclass will not be created but this class.
	virtual HierNodeRef CreateNode(const PathClass& path);
	
	// Duplicates the node an its chidlren. Siblings of the node
	// will not be copied.
//	static HierNodeRef DuplicateHier(HierNodeRef root);
	
	//----
	
#ifdef InclUserValues
#pragma mark User Values

private:
	// Define 'InclUserValues' if you want the nodes to use memory to hold user values.
	void* userData;
	uint64_t user64;
public:
	void SetUserPtr(void* ptr) { userData = ptr; }
	void* UserPtr() const { return userData; }
	void SetUser64(uint64_t v) { user64 = v; }
	uint64_t User64() const { return user64; }
#endif
	
	//----
#pragma mark Gathering
	// Have node recursively gather all its children.
	// If the node has any existing children, the routine will be return immediately
	// and do nothing.
	// To force an update, you must either erase all children or
	// detach all children from the node.
	
	// Return all children for this node.
	// The default returns an empty array.
	// This is only used by GatherChildren(), not the callback versions.
	// See subclassing notes.
	virtual const std::vector<PathClass> Children();
	
	// These methods currently can only return false. false implies no issues.
	// true usually means all stop has been flagged. This currently can never happen.
	bool GatherChildren();
private:
	bool GatherChildren_R();
	
public:
	
	// These methods currently can only return false. false implies no issues.
	// true usually means all stop has been flagged. This currently can never happen.
	// A HierNodeNilProcEx exception will be thrown proc or block are nil.
	bool GatherChildren(HierNodeChildrenProc proc, void* userCtx);
	bool GatherChildren(HierNodeChildrenBlock block);
private:
	bool GatherChildren_R(HierNodeChildrenProc proc, void* userCtx);
	bool GatherChildren_R(HierNodeChildrenBlock block);
	
public:
#pragma mark Deletion
	// Recursively delete all children.
	// Does not delete siblings.
	void DeleteAllChildren();
private:
	void Delete_R(HierNodeRef node);
public:
	
	// Delete this and all its siblings.
	void SiblingDelete();
	
	// Detach children from node.
	// Caller is responsible for deletion.
	// Returns (ex-)first child of this node.
	HierNodeRef DetachChildren();
	
	// Separate sidblings into individual trees.
	// If they have a parent, a HierNodeEx will be thrown.
	static std::vector<HierNodeRef> SplitSiblings(HierNodeRef node);
	
	// Detach this node from its parent and siblings
	HierNodeRef GoItAlone();
	
	//--------
#pragma mark Counts
	// Count of immediate children
	uint64_t ChildCount() const;
	
	// Count of siblings
	uint64_t SiblingCount() const;
	
	// Count of all children, including any siblings of the node.
	uint64_t DeepChildCount() const;
	
	// Counts of siblings before and after this node
	void SiblingCount(uint64_t& beforeMe, uint64_t& afterMe) const;

private:
	// Deep count recursive method. Used by DeepChildCount()
	void DeepChildCount_R(uint64_t& count) const;
public:
	
	// Used by child count methods. Return true if you want the node to be part of the count.
	// Default is true.
	virtual bool CountThis() const;
	
	// Item count for all rows. Siblings for this node are factored in.
	// This node is considered row 0 even if it is a sub-node of a hierarchy.
	// CountThis() is not called.
	std::vector<uint64_t> WidthForRow();
	
	// Widest row. Siblings for this node are factored in.
	// CountThis() is not called.
	uint64_t WidestRow(uint64_t& count);
	
	// How deep the hierarchy is from this node
	uint64_t GreatestDepth();
	
	//---------
#pragma mark Siblings
	// Can return nil
	HierNodeRef NextSibling() const;
	HierNodeRef PrevSibling() const;
	
	// Can return this
	HierNodeRef FirstSibling() const;
	HierNodeRef LastSibling() const;
private:
	// sort flag is currently NYI.
	std::vector<HierNodeRef> SiblingArray(bool setSortFlag);
public:
	// All siblings including this node.
	std::vector<HierNodeRef> SiblingArray();
	
	// Get SIBLING at index 'idx' RELATIVE to this node.
	// Will throw a HierNodeBoundsEx if idx exceeds sibling range.
	HierNodeRef operator[](int64_t idx);
	
	// Can return nil
	HierNodeRef FirstChild() const;
	
	// Return all nodes of depth N that are only direct descendants of this node.
	// Ignore siblings and other possible branches of ancestors.
	std::vector<HierNodeRef> AllAtDepthDown(int64_t depth);
	
	// Return all nodes of depth N, incl. siblings and ancestor branches.
	std::vector<HierNodeRef> AllAtDepth(int64_t depth);
	
	// Find ancestor with no parent.
	// If the top row has siblings, it will be the first sibling.
	// delta is how many rows above root is relative to this node.
	// delta will always be <= 0.
	HierNodeRef GetRoot(int64_t& delta);
	
	//-------------------------------
	// Iteration
#pragma mark Iteration
	enum class IterationMode {
		// Depth first, first child
		depthFirst			=0,
		// Depth first, but work backwards from last child
		depthFirstEnd		=1,
		// Breadth first, first child
		breadthFirst		=2,
		// Breadth first, but work backwards from last child
		breadthFirstEnd		=3,
		
	};
	
	// Iterate through all children of this node.
	// It will not iterate through this node's siblings.
	// Return values
	//	rtnContinue (0) - Success/All done
	//	rtnAllStop (-1) - all stop has been flagged by callbacks
	//	rtnBadParam (1) - proc/block is nil.
	// Currently the callbacks are only expected to return rtnContinue(0) or rtnAllStop(-1).
	// Any other result will result in an all stop.
	int Iterate(HierNodeItrProc proc, void* userCtx, IterationMode mode);
	int Iterate(HierNodeItrBlock block, IterationMode mode);
private:
	// Depth is relative to this node, not necessarily the root of the hierarchy.
	int Iterate_Depth_R(HierNodeItrProc proc, void* userCtx, int64_t depth);
	int Iterate_Depth_End_R(HierNodeItrProc proc, void* userCtx, int64_t depth);
	int Iterate_Breadth_R(HierNodeItrProc proc, void* userCtx, int64_t depth);
	int Iterate_Breadth_End_R(HierNodeItrProc proc, void* userCtx, int64_t depth);
	
	int Iterate_Depth_R(HierNodeItrBlock block, int64_t depth);
	int Iterate_Depth_End_R(HierNodeItrBlock block, int64_t depth);
	int Iterate_Breadth_R(HierNodeItrBlock block, int64_t depth);
	int Iterate_Breadth_End_R(HierNodeItrBlock block, int64_t depth);
	
public:
	// Subclass iteration.
	// There is no point in calling this if IterationAction() is not overridden.
	// Just chews up CPU time.
	int Iterate(IterationMode mode);
	
	// Called by Iterate(mode) only.
	// Default action is to do nothing and return rtnContinue(0).
	virtual int IterationAction(int64_t depth);
private:
	// Called by Iterate(mode).
	int Iterate_Depth_R(int64_t depth);
	int Iterate_Depth_End_R(int64_t depth);
	int Iterate_Breadth_R(int64_t depth);
	int Iterate_Breadth_End_R(int64_t depth);
	
	
	//---------------------------
	// Sorting
#pragma mark Sorting
private:
	// Change all prev/next pointers for all nodes in
	// the order passed in the array.
	// The first node will become the parent's first node if the
	// parent exists.
	friend void RelinkSorted(std::vector<HierNodeRef>& nodes);
public:
	
	// Sort comparison is with path property.
	// Sorting is based on path string comparison.
	static void SiblingSort(HierNodeRef startNode);
	
	// If either parameters are nil, the routine returns immediately and does nothing.
	static void SiblingSort(HierNodeRef startNode, HierNodeSortProc proc, void* userCtx);
	static void SiblingSort(HierNodeRef startNode, HierNodeSortBlock block);
};

} // namespace

#endif /* DirHier_hpp */
