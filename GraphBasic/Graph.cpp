#include "stdafx.h"
#include "Graph.h"

#include "Error.h"
#include "json/json.h"


#include <iostream>
#include <string>
#include <fstream>


#define DISTANCE 200
//https://bl.ocks.org/cjrd/raw/6863459/
//{"nodes":[{"title":"new concept","id":0,"x":455,"y":100},{"title":"new concept","id":1,"x":455,"y":300}],"edges":[{"source":1,"target":0}]}


Graph::~Graph() {
	for (CFGNodeVector::iterator it = nodes().begin(); it != nodes().end(); it++) {
		delete *it;
	}
}


bool Graph::PrintTree(const char* file_path) {

	json json_graph;
	json_graph["nodes"] = json::array();
	json_graph["edges"] = json::array();

	int x = 0, y = 0, level = 0;
	CFGNodeVector visited;
	CFGNodeVector stack_current;
	CFGNodeVector stack_next;

	stack_current.push_back(start());

	while (!stack_current.empty()) {
		x = level * DISTANCE;
		level++;
		size_t size = stack_current.size();

		for (size_t stack_cnt = 0; stack_cnt < size; ) {
			CFGNode* node = stack_current.at(stack_cnt++);
			//std::cout << node->id() << std::endl;
			if (!visited.IsContain(node)) {
				visited.push_back(node);
				node->set_x(x);
				node->set_y(y);
				json json_node = node->GetJson();
				json_graph["nodes"].push_back(json_node);
				x += DISTANCE;
				//std::cout << "x:" << x << std::endl;
				CFGNodeVector childs = node->childs();
				size_t to_size = childs.size();
				for (int child_cnt = 0; child_cnt < to_size; child_cnt++) {
					json json_edge;
					json_edge["source"] = node->id();
					json_edge["target"] = childs.at(child_cnt)->id();
					json_graph["edges"].push_back(json_edge);
					stack_next.push_back(childs.at(child_cnt));
				}
			}
		}
		stack_current = stack_next;
		stack_next.clear();

		y += DISTANCE;
	}

	std::ofstream file(file_path);
	file << json_graph;
	file.close();

	return true;
}

bool Graph::PrintBlockTree(const char* file_path) {

	json json_graph;
	json_graph["nodes"] = json::array();
	json_graph["edges"] = json::array();

	int x = 0, y = 0, level = 0;
	BlockVector visited;
	BlockVector stack_current;
	BlockVector stack_next;

	stack_current.push_back(start()->block());

	while (!stack_current.empty()) {
		x = level * DISTANCE;
		level++;
		size_t size = stack_current.size();

		for (size_t stack_cnt = 0; stack_cnt < size; ) {
			Block* node = stack_current.at(stack_cnt++);
			//std::cout << node->id() << std::endl;
			if (!visited.IsContain(node)) {
				visited.push_back(node);
				node->set_x(x);
				node->set_y(y);
				json json_node = node->GetJson();
				json_graph["nodes"].push_back(json_node);
				x += DISTANCE;
				//std::cout << "x:" << x << std::endl;
				BlockVector childs = node->childs();
				size_t to_size = childs.size();
				for (int child_cnt = 0; child_cnt < to_size; child_cnt++) {
					json json_edge;
					json_edge["source"] = node->id();
					json_edge["target"] = childs.at(child_cnt)->id();
					json_graph["edges"].push_back(json_edge);
					stack_next.push_back(childs.at(child_cnt));
				}
			}
		}
		stack_current = stack_next;
		stack_next.clear();

		y += DISTANCE;
	}

	std::ofstream file(file_path);
	file << json_graph;
	file.close();

	return true;


}
Block* Graph::FindLCA(Block* block_1, Block* block_2) {
	if (!this->blocks().IsContain(block_1)
		|| !this->blocks().IsContain(block_2)
		) {
		_Errmsg( "Graph::FindLCA node is not contained in graph");
	}

	Block* root = this->start()->block();
	if (block_1 == root || block_2 == root)
		return root;

	int depth_gap = block_1->depth() - block_2->depth();
	if (depth_gap > 0) {
		while (depth_gap) {
			block_1 = block_1->parent();
			depth_gap--;
		}
	}
	else if (depth_gap < 0) {
		while (depth_gap) {
			block_2 = block_2->parent();
			depth_gap++;
		}
	}

	while (block_1 != block_2 && block_1 != root) {
		block_1 = block_1->parent();
		block_2 = block_2->parent();
	}

	return block_1;
}



bool Graph::PrintGraph(const char* file_path, bool _printf) {

	json json_graph;
	json_graph["nodes"] = json::array();
	json_graph["edges"] = json::array();

	int x = 0, y = 0, level = 0;
	CFGNodeVector visited;
	CFGNodeVector stack_current;
	CFGNodeVector stack_next;

	stack_current.push_back(start());

	while (!stack_current.empty()) {
		x = level * DISTANCE;
		level++;
		size_t size = stack_current.size();

		for (size_t stack_cnt = 0; stack_cnt < size; ) {
			CFGNode* node = stack_current.at(stack_cnt++);

			if (!visited.IsContain(node)) {
				visited.push_back(node);
				node->set_x(x);
				node->set_y(y);
				json json_node = node->GetJson();
				json_graph["nodes"].push_back(json_node);
				x += DISTANCE;

				EdgeVector edges = node->out_edges();
				size_t edge_cnt = edges.size();
				CFGNode* to_node;
				for (int edge_idx = 0; edge_idx < edge_cnt; edge_idx++) {
					
					json json_edge;
					Edge* edge = edges.at(edge_idx);
					to_node = edge->to();
					json_edge["source"] = node->id();
					json_edge["target"] = to_node->id();
					json_graph["edges"].push_back(json_edge);
					stack_next.push_back(to_node);
				}
			}
		}
		stack_current = stack_next;
		stack_next.clear();

		y += DISTANCE;
	}

	std::ofstream file(file_path);
	file << json_graph;
	file.close();

	return true;
}

bool Graph::PrintBlockGraph(const char* file_path, bool _printf) {

	json json_graph;
	json_graph["nodes"] = json::array();
	json_graph["edges"] = json::array();

	int x = 0, y = 0, level = 0;
	BlockVector visited;
	BlockVector stack_current;
	BlockVector stack_next;

	stack_current.push_back(start()->block());

	while (!stack_current.empty()) {
		x = level * DISTANCE;
		level++;
		size_t size = stack_current.size();

		for (size_t stack_cnt = 0; stack_cnt < size; ) {
			Block* node = stack_current.at(stack_cnt++);

			if (!visited.IsContain(node)) {
				visited.push_back(node);
				node->set_x(x);
				node->set_y(y);
				json json_node = node->GetJson();
				json_graph["nodes"].push_back(json_node);
				x += DISTANCE;

				BlockVector out_blocks = node->outs();
				size_t out_cnt = out_blocks.size();
				for (int block_idx = 0; block_idx < out_cnt; block_idx++) {

					json json_edge;
					Block* to_node = out_blocks.at(block_idx);
					
					json_edge["source"] = node->id();
					json_edge["target"] = to_node->id();
					json_graph["edges"].push_back(json_edge);
					stack_next.push_back(to_node);
				}
			}
		}
		stack_current = stack_next;
		stack_next.clear();

		y += DISTANCE;
	}

	std::ofstream file(file_path);
	file << json_graph;
	file.close();

	return true;
}

void Graph::AddNodeAsStart(CFGNode* node) {
	nodes().push_back(node);
	start_ = node;
	node->set_id(NODE_ID_START);
	node_map().insert(std::make_pair(node->id(), node));
	node->set_title("start");
	
}

void Graph::AddNodeAsEnd(CFGNode* node) {
	nodes().push_back(node);
	end_ = node;
	node->set_id(NODE_ID_END);
	node_map().insert(std::make_pair(node->id(), node));

	node->set_title("end");
}

void Graph::AddNodeAsEntry(CFGNode* node) {
	nodes().push_back(node);
	end_ = node;
	node->set_id(NODE_ID_ENTRY);
	node_map().insert(std::make_pair(node->id(), node));

	node->set_title("entry");
}


void Graph::AddNode(CFGNode* node) {
	nodes().push_back(node);
	node->set_id(IncreaseIdCount());
	node_map().insert(std::make_pair(node->id(), node));
}

void Graph::AddNode(CFGNode* node,int id) {
	nodes().push_back(node);
	node->set_id(id);
	node_map().insert(std::make_pair(node->id(), node));
}

void Graph::AddRegionNode(CFGNode* node) {
	nodes().push_back(node);
	node->set_id(IncreaseIdCount());
	node_map().insert(std::make_pair(node->id(), node));
	node->set_title("R");
}

void Graph::AddEdge(Edge* edge) {
	edge_vector().push_back(edge);
}

// **caution : RemoveEdge will delete edge.
void Graph::RemoveEdge(Edge* edge) {
	if (!this->edge_vector().IsContain(edge)) {
		_Errmsg( "RemoveEdge() Graph does not contain edge");
	}
	std::string from_title = edge->from()->GetJson()["title"];
	std::string to_title = edge->to()->GetJson()["title"];

	
	CFGNode* from_node = edge->from();
	CFGNode* to_node = edge->to();
	
	
	from_node->out_edges().remove(edge);
	to_node->in_edges().remove(edge);
	
	Graph* graph = edge->graph();
	graph->edge_vector().remove(edge);
	delete edge;
}


Graph* Graph::CreateClone() {
	Graph* new_graph = new Graph(this->type());
	

	for (CFGNodeVector::iterator it = nodes().begin(); it != nodes().end(); it++) {
		CFGNode* node = *it;
		CFGNode* clone_node = node->CreateClone(new_graph);
		new_graph->AddNode(clone_node, clone_node->id());
	}
	new_graph->set_id_count(this->id_count());
	return new_graph;
}


Graph* Graph::CreateGraphClone() {
	Graph* new_graph = new Graph(this->type());
	

	CFGNodeVector visited;
	CFGNodeVector to_visit;
		
	CFGNode * start = this->start();
	new_graph->AddNodeAsStart((start->CreateClone(new_graph)));
	to_visit.push_back(start);
	visited.push_back(start);

	CFGNode * end = this->end();
	new_graph->AddNodeAsEnd((end->CreateClone(new_graph)));
	//no need to visit end node
	visited.push_back(end);

	while (to_visit.size() > 0) {

		CFGNode* cur = to_visit.back();
		to_visit.pop_back();
		CFGNode* clone_node = new_graph->GetIdNode(cur->id());

		EdgeVector edges = cur->out_edges();
		size_t edge_cnt = edges.size();

		for (EdgeVector::iterator it = edges.begin(); it != edges.end(); it++) {
			Edge* edge = *it;
			CFGNode* to_node = edge->to();
			CFGNode* to_clone_node = NULL;
			if (!visited.IsContain(to_node)) {
				to_clone_node = to_node->CreateClone(new_graph);
				new_graph->AddNode(to_clone_node, to_node->id());
				visited.push_back(to_node);
				to_visit.push_back(to_node);
			}
			else {
				to_clone_node = new_graph->GetIdNode(to_node->id());
			}
			clone_node->EdgeFromTo(to_clone_node,edge->label());
		}
	}	
	
	return new_graph;
}

void Graph::CreateDFST() {

	Block* root = this->start()->block();
	root->set_depth(DEPTH_ROOT);
	BlockVector visited;

	typedef std::pair<Block*, Block*> BlockPair;
	std::vector<BlockPair> stack;
	//NodeVector stack;


	stack.push_back(std::make_pair(root, reinterpret_cast<Block*>(NULL)));

	while (!stack.empty()) {
		BlockPair pair = stack.back();
		Block* block = pair.first;
		Block* parent = pair.second;
		stack.pop_back();
		if (!visited.IsContain(block)) {
			visited.push_back(block);
			if (parent != NULL) {
				parent->childs().push_back(block);
				block->set_parent(parent);
				block->set_depth(parent->depth() + 1);
			}

			BlockVector successors = block->outs();
			for (BlockVector::iterator it = successors.begin(); it != successors.end(); it++) {
				Block* to_block = *it;
				stack.push_back(std::make_pair(to_block, block));
			}
		}
	}
	this->set_type(Graph::GraphType::TREE);
}

void Graph::CreateDominatorTree() {
	Graph* DFST = this;

	BlockVector to_visit = DFST->blocks();
	BlockVector next_to_visit;


	while (!to_visit.empty()) {
		next_to_visit.clear();
		for (BlockVector::iterator visit_it = to_visit.begin(); visit_it != to_visit.end(); visit_it++) {
			Block* visit_block = *visit_it;
			BlockVector predecessors = visit_block->ins();

			for (BlockVector::iterator pred_it = predecessors.begin(); pred_it != predecessors.end(); pred_it++) {
				Block* pred_block = *pred_it;
				Block* parent = visit_block->parent();
				Block* LCA = DFST->FindLCA(pred_block, parent);
				if (pred_block != parent && \
					parent != LCA) {
					parent->childs().remove(visit_block);
					visit_block->set_parent(LCA);
					Block* new_parent = visit_block->parent();
					visit_block->set_depth(new_parent->depth() + 1);
					new_parent->childs().push_back(visit_block);

					BlockVector successors = visit_block->outs();
					for (BlockVector::iterator succ_it = successors.begin(); succ_it != successors.end(); succ_it++) {
						Block* succ_block = *succ_it;
						next_to_visit.push_back(succ_block);
					}
				}
			}
		}
		to_visit = next_to_visit;
	}
}

bool Graph::IsDominate(Block* parent, Block* child) {
	Block* target = child;
	while (target != NULL) {
		if (parent == target)
			return true;
		target = target->parent();
	}
	return false;
}

void Graph::DFVisit(Block* node) {
	// DF local
	for (BlockVector::iterator succ_it = node->outs().begin(); succ_it != node->outs().end(); succ_it++) {
		Block* succ = *succ_it;
		if (succ->parent() != node) {
			node->dominance_frontiers().push_back(succ);
		}
	}
	// DF up 
	for (BlockVector::iterator child_it = node->childs().begin(); child_it != node->childs().end(); child_it++) {
		Block* child = *child_it;
		DFVisit(child);
		for (BlockVector::iterator DF_it = child->dominance_frontiers().begin(); DF_it != child->dominance_frontiers().end(); DF_it++) {
			Block* DF_node = *DF_it;
			// if child's DF is not strictly dominated by immediate dominator of child ( which is current node )
			if (node == DF_node || !IsDominate(node, DF_node))
				node->dominance_frontiers().push_back(DF_node);
		}
	}
}

void Graph::InsertPhiToBlock(Block* DF, Value variable) {
	CFGNode* phi_node = new CFGNode(this, CFGNode::UOpcode::phi);

	phi_node->set_title(CFGNode::opcode_names_[phi_node->opcode()]);
	this->AddNode(phi_node);
	phi_node->set_block(DF);
	CFGNode* old_first_node = DF->nodes()[0];

	//RemoveEdge will delete edge and shrink old_first_node's in_edge vector. 
	//so we need copy of vector at this time.
	EdgeVector edge_vector_copy = old_first_node->in_edges();
	for (EdgeVector::iterator edge_it = edge_vector_copy.begin(); edge_it != edge_vector_copy.end(); edge_it++) {
		Edge* edge = *edge_it;
		CFGNode* from_node = edge->from();
		EDGE_LABEL label = edge->label();
		this->RemoveEdge(edge);
		from_node->EdgeFromTo(phi_node, label);
		phi_node->inputs().push_back(variable);
	}
	phi_node->EdgeFromTo(old_first_node, EDGE_NONE);
	CFGNodeVector::iterator insert_position = DF->nodes().begin();
	DF->nodes().insert(insert_position, phi_node);
}


void Graph::InsertPhi() {
	DoneList done_list;
	
	for (CFGNodeVector::iterator node_it = this->nodes().begin(); node_it != this->nodes().end(); node_it++) {
		CFGNode* node = *node_it;
		if (node->opcode() == CFGNode::UOpcode::sym) {
			Value variable = node->inputs()[0];
			BlockVector temp_vector;
			done_list[variable] = temp_vector;
		}		
	}

	BlockVector to_visit = this->blocks();
	
	//for (BlockVector::iterator block_it = this->blocks().begin(); block_it != this->blocks().end(); block_it++) {
	//Block* block = *block_it;
	while (!to_visit.empty()){
		Block* block = to_visit.back();
		to_visit.pop_back();
		if (block->dominance_frontiers().size() != 0) {
			for (CFGNodeVector::iterator node_it = block->nodes().begin(); node_it != block->nodes().end(); node_it++) {
				CFGNode* node = *node_it;
				switch (node->opcode()) {
				case CFGNode::str:
				case CFGNode::phi: {
					Value variable = node->inputs()[0];
					for (BlockVector::iterator DF_it = block->dominance_frontiers().begin(); DF_it != block->dominance_frontiers().end(); DF_it++) {
						Block* DF = *DF_it;
						if(!done_list[variable].IsContain(DF) ) {
							InsertPhiToBlock(DF, variable);
							done_list[variable].push_back(DF);
							to_visit.push_back(DF);
						}
					}
				}
				}
			}
		}
	}
}

void Graph::CalculateDominanceFrontier() {
	Block* root = this->start()->block();
	DFVisit(root);
}

void Graph::SSAVisit(CFGNode* node, DoneList& done_list) {
	for (CFGNodeVector::iterator child_it = node->childs().begin(); child_it != node->childs().end(); child_it++) {
		CFGNode* child = *child_it;
		SSAVisit(child, done_list);
	}
}

void Graph::ToSSA() {
	CFGNodeVector stack;
	DoneList done_list;
	CFGNode* root = this->start();
	SSAVisit(root, done_list);
}

