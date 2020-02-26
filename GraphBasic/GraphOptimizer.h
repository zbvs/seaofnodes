/*
#pragma once
#include "CFG.h"

class GraphOptimizer {
public:
	GraphOptimizer(Graph* graph) : origin_graph_(graph){
		target_graph_ = new Graph(Graph::GraphType::GRAPH );
	}
	~GraphOptimizer(){}
	

	void CalculatePostDominator();
	void CreateDominatorTree();
	
	void CreateDFST();
	void CreateReverseGraph();
	void CreateCDG();
	void CreatePDG();
	void PDTPostOrderRegionInsertion(CFGNode* pdt_node);
	Graph* origin_graph() { return origin_graph_; }
	Graph* target_graph() { return target_graph_; }
	void set_CDG(Graph* CDG) { CDG_ = CDG; }
	Graph* CDG() { return CDG_; }
	void set_PDG(Graph* PDG) { PDG_ = PDG; }
	Graph* PDG() { return PDG_; }
	void set_PDT(Graph* PDT) { PDT_ = PDT; }
	Graph* PDT() { return PDT_; }
	void set_DFST(Graph* DFST) { DFST_ = DFST; }
	Graph* DFST() { return DFST_; }
private:
	Graph* origin_graph_;
	Graph* target_graph_;
	Graph* DFST_;
	Graph* CDG_;
	Graph* PDG_;
	Graph* PDT_;

};











*/