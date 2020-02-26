
#pragma once

#include "Common.h"
class VDG;
class Node;

typedef CommonVector<Node*> NodeVector;


class Node {
public:
#define NODE_ID_START 0
#define NODE_ID_END -1
#define NODE_ID_ENTRY -2

	enum Opcode {
		START, REGION, PROJECTION_TRUE, PROJECTION_FALSE, REGION_START, IF, END, notop, neg, incop, decop, OC_dup, swp, add, sub, mult, divop,
		modop, andop, orop, gt, lt, ge, le, eq, ne,
		lod, ldc, lda, ldi, ldp, str, sti, ujp, tjp, fjp,
		call, ret, retv, chkh, chkl, nop, proc, endop, bgn, sym,
		dump, phi, none
	};


	static const int NO_OPCODES = 45;
	static constexpr const char* opcode_names_[NO_OPCODES] = {
	 "START", "REGION", "PROJECTION_TRUE", "PROJECTION_FALSE", "REGION_START", 
	 "IF", "END", "notop",  "neg",   "inc",    "dec",    "dup",   "swp",  "add", "sub",
	 "mult",     "div",   "mod",    "and",  "or",    "gt",     "lt",    "ge",
	 "le",     "eq",    "ne",     "lod",    "ldc",   "lda",    "ldi",   "ldp",
	 "str",    "sti",   "ujp",    "tjp",    "fjp",   "call",   "ret",   "retv",
	 "chkh",   "chkl",  "nop",  "proc",   "end",   "bgn",    "sym", "dump",
	 "phi","none"
	};


	Node(Opcode opcode) : opcode_(opcode) {}
	Node(Opcode opcode, int id) : opcode_(opcode) {}
	~Node() {
	}


	void set_vdg(VDG* vdg) { vdg_ = vdg; }
	VDG* vdg() { return vdg_; }

	Opcode set_opcode(Opcode opcode) { return opcode_ = opcode; }
	Opcode opcode() { return opcode_; }



	void push_control_input(Node* control_node);
	void push_value_input(Node* data_node);
	void set_title(std::string title) { title_ = title; }
	NodeVector& inputs() { return inputs_; }
	NodeVector& uses() { return uses_; }
	std::string title() { return title_; }
	std::string GetInstruction();
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
	Opcode opcode_;
	std::string title_;
	NodeVector inputs_;
	NodeVector uses_;
	int control_count_;
	int value_count_;
	int id_;
	int x_;
	int y_;


};

class USE {

	
};