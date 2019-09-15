
#include "stdafx.h"

/*

#include "Error.h"
#include "GraphOptimizer.h"
#include <algorithm>


void GraphOptimizer::PDTPostOrderRegionInsertion(CFGNode* pdt_node) {
	NodeVector pdt_childs = pdt_node->childs();
	Graph* CDG = this->CDG();
	//do first post order.
	for (NodeVector::iterator it = pdt_childs.begin(); it != pdt_childs.end(); it++) {
		CFGNode* pdt_chld = *it;
		PDTPostOrderRegionInsertion(pdt_chld);
	}
	if (pdt_node == PDT()->start())//if root, return. nothing to do
		return;

	CFGNode* new_region_node = new CFGNode(CDG, CFGNode::NodeType::REGION);
	CDG->AddRegionNode(new_region_node);
	CFGNode* visit_cdg_node = this->CDG()->GetIdNode(pdt_node->id());
	EdgeVector in_edge_vector = visit_cdg_node->in_edges();
	for (EdgeVector::iterator it = in_edge_vector.begin(); it != in_edge_vector.end(); it++) {
		Edge* edge = *it;
		CFGNode* from_node = edge->from();
		EDGE_LABEL label_ = edge->label();
		CDG->RemoveEdge(edge);//free edge
		from_node->EdgeFromTo(new_region_node, label_);
	}
	
	new_region_node->EdgeFromTo(visit_cdg_node,EDGE_LABEL::EDGE_NONE);
	if (new_region_node->in_edges().size() != 0) {
		std::string to_title = new_region_node->in_edges().at(0)->from()->GetJson()["title"];
	}


	for (EdgeVector::iterator it = new_region_node->out_edges().begin(); it != new_region_node->out_edges().end(); it++) {
		Edge* edge = *it;
		CFGNode* to_node = edge->to();
		std::string to_title = to_node->GetJson()["title"];
	}
	
	
	//phase 2: replace dependeces
	
	
	for (NodeVector::iterator it = pdt_childs.begin(); it != pdt_childs.end(); it++) {
		CFGNode* child_cdg_node = CDG->GetIdNode((*it)->id());//child node in pdt
		EdgeVector chld_edge_vector = child_cdg_node->in_edges();
		// their must be only one child's dependence on a previously inserted region node 
		if (chld_edge_vector.size() != 1) {
			std::stringstream stream;
			stream << "invalid edge count" << chld_edge_vector.size();
			_Errmsg(stream.str());
		}

		CFGNode* chld_region_node = chld_edge_vector.at(0)->from();
		//Grape->RemoveEdge cause edge_vector to shrink. need copy
		EdgeVector chld_region_in_edge_vector = chld_region_node->in_edges();
		EdgeVector new_region_in_edge_vector = new_region_node->in_edges();
		EdgeVector intersection = Edge::GetIntersection(chld_region_in_edge_vector, new_region_in_edge_vector);

		//if parent CD set is equal to INT, 
		//delete child region node's dependences and replace it with only one dependence on new R
		if (new_region_in_edge_vector.size() == intersection.size()) {
			for (EdgeVector::iterator it = chld_region_in_edge_vector.begin(); it != chld_region_in_edge_vector.end(); it++) {
				CDG->RemoveEdge(*it);
			}
			//insert 
			chld_region_node->EdgeFromTo(new_region_node, EDGE_LABEL::EDGE_NONE);
		}
		//else if chld CD set is contained in INT 
		//delete parent's dependence and replace it with dependence on chld region node
		//( because it is intersection, it is actually chld CD set == INT set )
		else if (chld_region_in_edge_vector.size() == intersection.size()) {
			for (EdgeVector::iterator it = new_region_in_edge_vector.begin(); it != new_region_in_edge_vector.end(); it++) {
				CDG->RemoveEdge(*it);
			}
			//insert 
			new_region_node->EdgeFromTo(chld_region_node, EDGE_LABEL::EDGE_NONE);
		}

	}

}



void GraphOptimizer::CreatePDG() {
	Graph* PDT = this->PDT();
	PDTPostOrderRegionInsertion(PDT->start());
	Graph* CDG = this->CDG();
	NodeVector visited;
	NodeVector to_visit;
	CFGNode* cur = CDG->start();
	
	to_visit.push_back(cur);
	visited.push_back(cur);
	while (!to_visit.empty()) {
		cur = to_visit.back();
		cur->Print("cur");
		to_visit.pop_back();
		EdgeVector true_edge_set;
		EdgeVector false_edge_set;
		EdgeVector out_edges = cur->out_edges();

		if (cur->out_edges().size() > 0) {
			//if (cur->type() == Node::NodeType::REGION)_Errmsg("region node has more than one out edge");
			EdgeVector edges = cur->out_edges();
			
			for (EdgeVector::iterator it = edges.begin(); it != edges.end(); it++) {
				Edge* edge = *it;
				if (edge->label() == EDGE_LABEL::EDGE_TRUE) {
					true_edge_set.push_back(edge);
				}
				else if (edge->label() == EDGE_LABEL::EDGE_FALSE) {
					false_edge_set.push_back(edge);
				}
				CFGNode* node = edge->to();
				if (!visited.isContain(node)) {
					visited.push_back(node);
					to_visit.push_back(node);
				}
			}
			if (true_edge_set.size() > 1) {
				CFGNode* new_region_node = new CFGNode(CDG, CFGNode::NodeType::REGION);
				CDG->AddRegionNode(new_region_node);
				cur->EdgeFromTo(new_region_node, EDGE_LABEL::EDGE_TRUE);
				//no need to visit region node
				visited.push_back(new_region_node);
				for (EdgeVector::iterator it = true_edge_set.begin(); it != true_edge_set.end(); it++) {
					Edge* edge = *it;
					CFGNode* to_node = edge->to();
					new_region_node->EdgeFromTo(to_node, EDGE_LABEL::EDGE_NONE);
					CDG->RemoveEdge(edge);
				}
			}
			if (false_edge_set.size() > 1) {
				CFGNode* new_region_node = new CFGNode(CDG, CFGNode::NodeType::REGION);
				CDG->AddRegionNode(new_region_node);
				cur->EdgeFromTo(new_region_node, EDGE_LABEL::EDGE_FALSE);
				visited.push_back(new_region_node);
				for (EdgeVector::iterator it = false_edge_set.begin(); it != false_edge_set.end(); it++) {
					Edge* edge = *it;
					CFGNode* to_node = edge->to();
					new_region_node->EdgeFromTo(to_node, EDGE_LABEL::EDGE_NONE);
					CDG->RemoveEdge(edge);
				}
			}
		}
		true_edge_set.clear();
		false_edge_set.clear();
		//new region node may have been added above.
	}	
}

void GraphOptimizer::CreateCDG() {
	//https://www.cs.utexas.edu/~pingali/CS395T/2009fa/papers/ferrante87.pdf
	
	Graph* PDT = target_graph();
	set_PDT(PDT);
	Graph* CFG = origin_graph();
	CFGNode* start = CFG->start();
	CFGNode* end = CFG->end();
	
	// 1. Create basic CDG
	Graph* CDG = CFG->CreateClone();
	set_CDG(CDG);
	CDG->AddNodeAsStart(CDG->GetIdNode(NODE_ID_START));
	CDG->AddNodeAsEnd(CDG->GetIdNode(NODE_ID_END));

	EdgeVector visited;
	EdgeVector cfg_edges = CFG->edge_vector();
	
	for (EdgeVector::iterator edge_it = cfg_edges.begin(); edge_it != cfg_edges.end(); edge_it++) {
		Edge* cfg_edge = *edge_it;
		CFGNode* cfg_from = cfg_edge->from();
		CFGNode* cfg_to = cfg_edge->to();
		
		CFGNode* pdt_parent = PDT->GetIdNode(cfg_from->id())->parent();
		if ( pdt_parent->id() != cfg_to->id()) { //control dependence
			CFGNode* pdt_visit = PDT->GetIdNode(cfg_to->id());

			CFGNode* cdg_from = CDG->GetIdNode(cfg_from->id());
			
			while (pdt_visit->id() != pdt_parent->id() && pdt_visit->id() != cfg_from->id()) {
				CFGNode* cdg_to = CDG->GetIdNode(pdt_visit->id());
				cdg_from->EdgeFromTo(cdg_to, cfg_edge->label());
				pdt_visit = pdt_visit->parent();
			}
			if (pdt_visit->id() == cfg_from->id()) {//self control dependence
				cdg_from->EdgeFromTo(cdg_from, cfg_edge->label());
			}
		}
	}
}

void GraphOptimizer::CalculatePostDominator() {
	CreateReverseGraph();
	CreateDFST();
	CreateDominatorTree();
}

void GraphOptimizer::CreateDominatorTree() {
	Graph* DFST = this->target_graph();
	
	NodeVector to_visit = DFST->node_vector();
	NodeVector next_to_visit;


	while (!to_visit.empty()) {
		next_to_visit.clear();
		for (NodeVector::iterator visit_it = to_visit.begin(); visit_it != to_visit.end(); visit_it++) {
			CFGNode* visit_node = *visit_it;
			EdgeVector in_edges = visit_node->in_edges();

			for (EdgeVector::iterator edge_it = in_edges.begin(); edge_it != in_edges.end(); edge_it++) {
				Edge* in_edge = *edge_it;
				CFGNode* pred_node = in_edge->from();
				CFGNode* parent = visit_node->parent();
				CFGNode* LCA = DFST->FindLCA(pred_node, parent);
				if (pred_node != parent && \
					parent != LCA) {
					parent->childs().remove(visit_node);
					visit_node->set_parent(LCA);
					CFGNode* new_parent = visit_node->parent();
					visit_node->set_depth(new_parent->depth()+1 );
					new_parent->childs().push_back(visit_node);
					
					EdgeVector out_edges = visit_node->out_edges();
					for (EdgeVector::iterator out_edge_it = out_edges.begin(); out_edge_it != out_edges.end(); out_edge_it++) {
						Edge* out_edge = *out_edge_it;
						next_to_visit.push_back(out_edge->to());
					}
				}
			}
		}
		to_visit = next_to_visit;
		
	}
}



void GraphOptimizer::CreateDFST() {
	Graph* graph = this->target_graph();
	

	CFGNode* root = graph->start();
	root->set_depth(DEPTH_ROOT);
	NodeVector visited;

	typedef std::pair<CFGNode*, CFGNode*> NodePair;
	std::vector<NodePair> stack;
	//NodeVector stack;

	
	stack.push_back(std::make_pair(root, reinterpret_cast<CFGNode*>(NULL)) );

	while (!stack.empty()) {
		NodePair pair = stack.back();
		CFGNode* node = pair.first;
		CFGNode* parent = pair.second;
		stack.pop_back();
		if (!node->IsContainedIn(visited)) {
			visited.push_back(node);
			if (parent != NULL) {
				parent->childs().push_back(node);
				node->set_parent(parent);
				node->set_depth(parent->depth() + 1);
			}

			EdgeVector edges = node->out_edges();
			for (EdgeVector::iterator it = edges.begin(); it != edges.end(); it++) {
				Edge* edge = *it;
				CFGNode* to_node = edge->to();
				stack.push_back(std::make_pair(to_node,node));
			}
		}
	}
	graph->set_type(Graph::GraphType::TREE);
}

void GraphOptimizer::CreateReverseGraph() {
	
	Graph* origin_graph = this->origin_graph();
	Graph* reverse_graph = this->target_graph();

	NodeVector visited;
	NodeVector to_visit;
	// prepare augmented cfg
	CFGNode* start = origin_graph->start();
	CFGNode* end = origin_graph->end();
	start->out_edges().at(0)->set_label(EDGE_LABEL::EDGE_TRUE);
	start->EdgeFromTo(end, EDGE_LABEL::EDGE_FALSE);
	
	CFGNode* clone_start = start->CreateClone(reverse_graph);

	reverse_graph->AddNode(clone_start,clone_start->id());
	reverse_graph->set_end(clone_start);

	to_visit.push_back(start);
	visited.push_back(start);

	
	CFGNode* clone_end = end->CreateClone(reverse_graph);

	reverse_graph->AddNode(clone_end, clone_end->id());
	reverse_graph->set_start(clone_end);

	//no need to visit end node
	visited.push_back(end);
	
	while (!to_visit.empty()) {
		CFGNode* cur = to_visit.back();
		to_visit.pop_back();
		CFGNode* clone_node = reverse_graph->GetIdNode(cur->id());
		EdgeVector edges = cur->out_edges();
		size_t edge_cnt = edges.size();

		for (EdgeVector::iterator it = edges.begin(); it != edges.end(); it++) {
			Edge* edge = *it;
			CFGNode* to_node = edge->to();
			CFGNode* to_clone_node = NULL;
			if (!to_node->IsContainedIn(visited)) {
				to_clone_node = to_node->CreateClone(reverse_graph);
				reverse_graph->AddNode(to_clone_node, to_node->id());
				visited.push_back(to_node);
				to_visit.push_back(to_node);
			}
			else {
				to_clone_node = reverse_graph->GetIdNode(to_node->id());
			}
			to_clone_node->EdgeFromTo(clone_node,EDGE_LABEL::EDGE_NONE);
		}
	}
}

*/

