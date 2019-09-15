#pragma once

#include <vector>
#include <set>
#include <map>
#include <json/json.h>
#include "Error.h"

class Graph;



template<class T>
class CommonVector;
class Block;
class CFGNode;
class Edge;
enum EDGE_LABEL;

using json = nlohmann::json;

//typedef std::vector<Node*> NodeVector;
typedef size_t Value;
#define VARIABLE_ID_SHIFT 0x10
#define TO_ID(Loc) Loc << VARIABLE_ID_SHIFT
#define TO_LOC(ID) ID >> VARIABLE_ID_SHIFT
typedef std::vector<Value> ValueVector;
typedef CommonVector <Block*> BlockVector;
typedef CommonVector <CFGNode*> CFGNodeVector;
typedef CommonVector <Edge*> EdgeVector;
typedef std::map<int, CFGNode*> NodeMap;
typedef std::set<CFGNode*> NodeSet;

template<class T>
class CommonVector {
public:
	CommonVector() {}
	CommonVector(size_t size):vector_(size) {}
	using iterator = typename std::vector<T>::iterator;
	std::vector<T>& vector() { return vector_; }
	void push_back(T element) { vector_.push_back(element); }
	void clear() { vector_.clear(); }
	bool empty() { return vector_.empty(); }
	size_t size() { return vector_.size(); }
	void pop_back() { return vector_.pop_back(); }
	T back() { return vector_.back(); }
	void pop_front() {
		if (size < 0)_Errmsg( "pop_front() on invalid size vector");
		remove(vector_.at(0));
	}
	T front() {
		if( size < 0 )_Errmsg( "front() on invalid size vector");
		return vector_.at(0);
	}
	iterator  insert(iterator position, const T& val) { return vector_.insert(position, val); }
	T operator [](size_t index) { return vector_[index]; }
	T at(const size_t _Pos) { return vector_.at(_Pos); }
	iterator begin() { return vector_.begin(); }
	iterator end() { return vector_.end(); }
	iterator remove(T element) {
		if (!IsContain(element)) {
			_Errmsg("remove(): vector does not element");
		}
		return vector_.erase(std::remove(vector_.begin(), vector_.end(), element), vector_.end());
	}
	void resize(size_t size) { vector_.resize(size); }
	bool IsContain(T element) {
		if (std::find(vector_.begin(), vector_.end(), element) != vector_.end()) {
			return true;
		}
		else {
			return false;
		}
	}
	void concatenate(CommonVector<T> common_vector) {
		std::vector<T> target_vector = common_vector.vector();
		vector_.insert(vector_.end(), target_vector.begin(), target_vector.end());
	}
	void printVector(std::string msg) {
		std::cout << "printVector " << msg << " " << std::endl;
		for (iterator it = vector_.begin(); it != vector_.end(); it++) {
			T element = (*it);
			element->Print("");
		}
	}


	//T at(){}
private:
	std::vector<T> vector_;
};

#define DEPTH_ROOT 0

class CFGNode {
public:
#define NODE_ID_START 0
#define NODE_ID_END -1
#define NODE_ID_ENTRY -2
	

	enum UOpcode {
		notop, neg, incop, decop, OC_dup, swp, add, sub, mult, divop,
		modop, andop, orop, gt, lt, ge, le, eq, ne,
		lod, ldc, lda, ldi, ldp, str, sti, ujp, tjp, fjp,
		call, ret, retv, chkh, chkl, nop, proc, endop, bgn, sym,
		dump, phi,none
	};
	static const int NO_OPCODES = 42;
	static constexpr const char* opcode_names_[NO_OPCODES] = {
	 "notop",  "neg",   "inc",    "dec",    "dup",   "swp",  "add", "sub",
	 "mult",     "div",   "mod",    "and",  "or",    "gt",     "lt",    "ge",
	 "le",     "eq",    "ne",     "lod",    "ldc",   "lda",    "ldi",   "ldp",
	 "str",    "sti",   "ujp",    "tjp",    "fjp",   "call",   "ret",   "retv",
	 "chkh",   "chkl",  "nop",  "proc",   "end",   "bgn",    "sym", "dump",
	 "phi","none"
	};

	enum NodeType {
		INST, DATA, REGION
	};

	CFGNode(Graph* graph) : graph_(graph), title_(""), depth_(-1),opcode_(none) {

	}
	CFGNode(Graph* graph, UOpcode opcode) : graph_(graph), opcode_(opcode),title_(""), depth_(-1) {
		
	}
	CFGNode(Graph* graph, const char* title) : graph_(graph), title_(title), depth_(-1){

	}

	~CFGNode() {

	}


	CFGNode* CreateClone(Graph* graph);
	static CFGNodeVector GetIntersection(CFGNodeVector set1, CFGNodeVector set2);
	void TreePrint(std::string msg);
	void Print(std::string msg);
	

	void set_graph(Graph* graph) { graph_ = graph; }
	Graph* graph() { return graph_; }
	void set_block(Block* block) { block_ = block; }
	Block* block() { return block_; }
	UOpcode set_opcode(UOpcode opcode) { return opcode_ = opcode; }
	UOpcode opcode() { return opcode_; }

	CFGNodeVector& childs() {
		return childs_;
	}

	void set_parent(CFGNode* parent) {
		parent_ = parent;
	}
	CFGNode* parent() {
		return parent_;
	}
	

	void set_title(std::string title) { title_ = title; }
	std::string title() { return title_; }
	std::string GetInstruction();
	void set_id(int id) { id_ = id;  }
	int id() {
		return id_;
	}
	void set_x(int x) { x_ = x; }
	int x() { return x_; }
	void set_y(int y) { y_ = y; }
	int y() { return y_; }
	void set_depth(int depth) { depth_ = depth; }
	int depth() { return depth_; }
	void set_out_edges(EdgeVector vector) { out_edges_ = vector; }
	EdgeVector& out_edges() { return out_edges_; }
	void set_in_edges(EdgeVector vector) { in_edges_ = vector; }
	EdgeVector& in_edges() { return in_edges_; }
	ValueVector& inputs() { return inputs_; }
	json GetJson();
	void EdgeFromTo(CFGNode* to, EDGE_LABEL label);
private:
	Graph* graph_;
	Block* block_;
	UOpcode opcode_;
	std::string title_;
	int id_;
	int x_;
	int y_;
	int depth_;
	ValueVector inputs_;
	EdgeVector in_edges_;
	EdgeVector out_edges_;
	CFGNode* parent_ = NULL;
	CFGNodeVector childs_;
};

class Block {
public:
	Block(CFGNode* node) {
		title_ = node->title();
		id_ = node->id();
		x_ = node->x();
		y_ = node->y();
	}
	void set_title(std::string title) { title_ = title; }
	std::string title() { return title_; }
	void set_id(int id) { id_ = id; }
	int id() {
		return id_;
	}
	void set_x(int x) { x_ = x; }
	void set_y(int y) { y_ = y; }
	void set_depth(int depth) { depth_ = depth; }
	int depth() { return depth_; }

	CFGNodeVector& nodes() { return nodes_; }
	BlockVector& dominance_frontiers() { return dominance_frontiers_; }
	void set_parent(Block* parent) { parent_ = parent; }
	Block* parent() { return parent_; }
	BlockVector& childs() { return childs_; }
	BlockVector& ins() { return ins_; }
	BlockVector& outs() { return outs_; }
	json GetJson();
private:
	std::string title_;
	int id_;
	int x_;
	int y_;
	int depth_;
	CFGNodeVector nodes_;
	BlockVector dominance_frontiers_;
	Block* parent_;
	BlockVector childs_;
	BlockVector ins_;
	BlockVector outs_;

};

enum EDGE_LABEL {
	EDGE_NONE, EDGE_TRUE, EDGE_FALSE
};

class Edge {
public:

	Edge(Graph* graph, CFGNode* from, CFGNode* to, EDGE_LABEL label) : graph_(graph), from_id_(from->id()) ,to_id_(to->id()),from_(from),to_(to), label_(label)
	{}
	~Edge() {}

	static EdgeVector GetIntersection(EdgeVector set1, EdgeVector set2);
	void Print(std::string msg);
	void set_graph(Graph* graph) { graph_ = graph; }
	Graph* graph() { return graph_; }
	void set_from_id(int id) { from_id_ = id; }
	int from_id() { return from_id_; }
	void set_to_id(int id) { to_id_ = id; }
	int to_id() { return to_id_; }
	void set_from(CFGNode* node) { from_ = node; }
	CFGNode* from() { return from_; }
	void set_to(CFGNode* node) { to_ = node; }
	CFGNode* to() { return to_; }
	void set_label(EDGE_LABEL label) { label_ = label; }
	EDGE_LABEL label() { return label_; }
private:
	Graph* graph_;
	int from_id_;
	int to_id_;
	CFGNode* from_;
	CFGNode* to_;
	EDGE_LABEL label_;
};




