
#pragma once

#include "Common.h"
#include "CFGNode.h"
class VDG;
class Node;

typedef CommonVector<Node*> NodeVector;


class Node {
public:
#define NODE_INVALID_ID -1
#define NODE_END_ID -2
#define TO_REGION_ID(id) (id + 1000) 
#define TO_REGION_START_ID(id) (id + 2000) 
#define TO_TRUE_ID(id) (id + 8000) 
#define TO_FALSE_ID(id) (id + 9000) 


	enum Opcode {
		notop, neg, incop, decop, OC_dup, swp, add, sub, mult, divop,
		modop, andop, orop, gt, lt, ge, le, eq, ne,
		lod, ldc, lda, ldi, ldp, str, sti, ujp, tjp, fjp,
		call, ret, retv, chkh, chkl, nop, proc, endop, bgn, sym,
		dump, phi, none, START, REGION, PROJECTION_TRUE, PROJECTION_FALSE, REGION_START, CONSTANT,
		IF, END
	};


	
	static constexpr const char* opcode_names_[] = {
	"notop",  "neg",   "inc",    "dec",    "dup",   "swp",  "add", "sub", "mult", "div",   
	"mod",    "and",  "or",    "gt",     "lt",    "ge", "le", "eq","ne", 
	"lod",    "ldc",   "lda",    "ldi",   "ldp", "str", "sti", "ujp", "tjp", "fjp",   
	"call",   "ret",   "retv", "chkh", "chkl", "nop", "proc", "end", "bgn", "sym", 
	"dump", "phi", "none", "START", "REGION", "PROJECTION_TRUE", "PROJECTION_FALSE", "REGION_START", "CONSTANT",
	 "IF", "END",
	};

	
	Node(Opcode opcode) : opcode_(opcode), id_(NODE_INVALID_ID), has_value_(false), title_(opcode_names_[opcode]) {}
	Node(Opcode opcode, int id) : opcode_(opcode), id_(id), has_value_(false), title_(opcode_names_[opcode]) {}
	Node(Opcode opcode, int id, Value value) : opcode_(opcode),id_(id), value_(value), has_value_(true), title_(opcode_names_[opcode]) {}
	~Node() {
	}

	


	void set_vdg(VDG* vdg) { vdg_ = vdg; }
	VDG* vdg() { return vdg_; }

	void set_opcode(Opcode opcode) { 
		opcode_ = opcode;
		title_ = opcode_names_[opcode]; 		
	}
	Opcode opcode() { return opcode_; }


	static Node::Opcode toNodeOpcode(CFGNode::UOpcode uop);
	void push_control_input(Node* control_node);
	void push_value_input(Node* data_node);
	void set_title(std::string title) { title_ = title; }
	NodeVector& inputs() { return inputs_; }
	NodeVector& uses() { return uses_; }
	std::string title() { return title_; }
	std::string GetTitle();
	int control_count() { return control_count_; }
	int value_count() { return value_count_; }
	void set_id(int id) { id_ = id; }
	int id() {
		return id_;
	}
	void set_x(int x) { x_ = x; }
	int x() { return x_; }
	void set_y(int y) { y_ = y; }
	int y() { return y_; }

	json GetJson();
	
private:
	VDG* vdg_;
	Value value_;
	Opcode opcode_;
	std::string title_;
	NodeVector inputs_;
	NodeVector uses_;
	bool has_value_;
	int control_count_ = 0;
	int value_count_ = 0;
	int id_;
	int x_;
	int y_;


};

class USE {

	
};