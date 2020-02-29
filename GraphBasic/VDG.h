#pragma once

#include "CFGNode.h"
#include "CFG.h"
#include "Node.h"
#include "Error.h"
#include <fstream>

typedef std::pair<Variable, Node*> var_pair;
typedef CommonVector<var_pair> VariableVector;

class Environment {
public:
	Environment();
	~Environment();
	Node* pop_node(Node* caller);
	void push_node(Node* node);
	Node* GetDef(Variable variable);
	void PutDef(Node* def_node, Variable variable);
	VariableVector& variables() { return variables_; }


private:
	VariableVector variables_;
	NodeVector stack_;
};



class VDGBuilder {
public:
	VDGBuilder();
	~VDGBuilder();
	void BuildVDG(CFG* cfg);
	Node* VisitBlock(Block* block, Node* region);
	Environment& environment() { return environment_; }

	void set_vdg(VDG* vdg) { vdg_ = vdg; }
	VDG* vdg() { return vdg_; }
private:
	VDG* vdg_;
	int addtional_id_;
	Environment environment_;
}; 


class VDG {
public:
	void add_node(Node* node) { nodes_.push_back(node); }
	void set_start(Node* start) { start_ = start; }
	void set_end(Node* end) { end_ = end; }
	void PrintGraph(const char* file_path);
	NodeVector& nodes() { return nodes_; }
	Node* start() { return start_; }
	Node* end() { return end_; }
private:
	NodeVector nodes_;
	Node* end_;
	Node* start_;
};
