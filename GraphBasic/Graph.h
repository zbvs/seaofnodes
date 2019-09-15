#pragma once
#include "CFG.h"



typedef std::map<Value, BlockVector> DoneList;



class Graph {
public:
	enum GraphType{GRAPH,TREE};
	Graph(GraphType type) :start_(NULL), end_(NULL), id_count_(1), region_id_count_(1), type_(type){
	}

	~Graph();
	Graph* CreateGraphClone();
	Graph* CreateClone();
	void CreateDominatorTree();
	void CreateDFST();
	bool IsDominate(Block* parent, Block* child);
	void DFVisit(Block* node);
	void InsertPhiToBlock(Block* DF, Value variable);
	void InsertPhi(); 
	void CalculateDominanceFrontier();
	void SSAVisit(CFGNode* node, DoneList& done_list);
	void ToSSA();
	void AddNodeAsStart(CFGNode* node);
	void AddNodeAsEnd(CFGNode* node);
	void AddNodeAsEntry(CFGNode* node);
	void AddNode(CFGNode* node);
	void AddNode(CFGNode* node, int id);
	void AddRegionNode(CFGNode* node);
	void AddEdge(Edge* edge);
	void RemoveEdge(Edge* edge);

	CFGNode* GetIdNode(int id) { return node_map_[id]; }
	int IncreaseIdCount() {
		return id_count_++;
	}
	int IncreaseRegionIdCount() {
		return region_id_count_++;
	}
	bool PrintGraph(const char* file_path, bool _printf=false);
	bool PrintBlockGraph(const char* file_path, bool _printf = false);
	bool PrintTree(const char* file_path);
	bool PrintBlockTree(const char* file_path);
	
	Block* FindLCA(Block* node_1, Block* node_2);

	void set_type(GraphType type) { type_ = type; }
	GraphType type() { return type_; }
	void set_id(Value id) { id_ = id; }
	Value id() { return id_; }
	CFGNode* start() { return start_; }
	CFGNode* end() { return end_; }
	void set_start(CFGNode* start) { start_ = start; }
	void set_end(CFGNode*end) { end_ = end;; }
	CFGNodeVector& nodes() { return nodes_; }
	void set_region_node_vector(CFGNodeVector vector) { region_node_vector_ = vector; }
	CFGNodeVector& region_node_vector() { return region_node_vector_; }
	void set_edge_vector(EdgeVector vector) { edge_vector_ = vector; }
	EdgeVector& edge_vector() { return edge_vector_; }
	void set_node_map(NodeMap map) { node_map_ = map; }
	NodeMap& node_map() { return node_map_; }
	void set_region_node_map(NodeMap map) { region_node_map_ = map; }
	NodeMap& region_node_map() { return region_node_map_; }
	BlockVector& blocks() { return blocks_; }
	void set_id_count(int id_count) { id_count_ = id_count; }
	int id_count() { return id_count_; }
private:
	GraphType type_;
	Value id_;
	int id_count_;
	int region_id_count_;
	CFGNodeVector nodes_;
	CFGNodeVector region_node_vector_;
	EdgeVector edge_vector_;
	NodeMap node_map_;
	NodeMap region_node_map_;
	CFGNode* start_;
	CFGNode* end_;
	BlockVector blocks_;
	
};



