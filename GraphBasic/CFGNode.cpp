#include "stdafx.h"

#include "CFGNode.h"
#include "CFG.h"
#include <iostream>


CFGNodeVector node_bucket_;


CFGNode* CFGNode::CreateClone(CFG* graph) {
	CFGNode* node = new CFGNode(graph);
	node->set_title(title());
	node->set_depth(depth());
	node->set_id(id());

	return node;
}

void CFGNode::EdgeFromTo(CFGNode* to, EDGE_LABEL label) {
	CFG* graph = this->graph();
	Edge* edge = new Edge(graph, this, to, label);
	graph->AddEdge(edge);
	out_edges().push_back(edge);
	to->in_edges().push_back(edge);
}



CFGNodeVector CFGNode::GetIntersection(CFGNodeVector set1, CFGNodeVector set2) {
	CFGNodeVector CD_set_chld;

	std::sort(set1.begin(), set1.end());
	std::sort(set2.begin(), set2.end());

	//1 if CD of node 
	CFGNodeVector intersecection(set1.size() + set2.size());
	CFGNodeVector::iterator insertection_end = std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), intersecection.begin());
	intersecection.resize((insertection_end - intersecection.begin()));
	return intersecection;
}


void CFGNode::Print(std::string msg) {
	std::cout  << msg << " Node::Print() " << this->title().c_str() << this->id()<< std::endl;
	
	EdgeVector in_edges = this->in_edges();
	int cnt = 1;
	for (EdgeVector::iterator it = in_edges.begin(); it != in_edges.end(); it++) {
		CFGNode* from_node = (*it)->from();
		printf("%d. from title:%s id:%d\n", cnt,  from_node->title().c_str(), from_node->id());
		cnt++;
	}
	cnt = 1;
	EdgeVector out_edges = this->out_edges();
	for (EdgeVector::iterator it = out_edges.begin(); it != out_edges.end(); it++) {
		CFGNode* to_node = (*it)->from();
		printf("%d. to title:%s id:%d\n", cnt, to_node->title().c_str(), to_node->id());
		cnt++;
	}
}

void CFGNode::TreePrint(std::string msg) {
	std::cout << msg << " Node::Print() " << this->title().c_str() << this->id() << std::endl;
	CFGNode* from_node = this->parent();
	if(from_node)
		printf("%d. parent title:%s id:%d\n", 1, from_node->title().c_str(), from_node->id());
	
	
	int cnt = 1;
	CFGNodeVector childs = this->childs();
	for (CFGNodeVector::iterator it = childs.begin(); it != childs.end(); it++) {
		CFGNode* to_node = *it;
		printf("%d. child title:%s id:%d\n", cnt, to_node->title().c_str(), to_node->id());
		cnt++;
	}
}



EdgeVector Edge::GetIntersection(EdgeVector set1, EdgeVector set2) {

	std::sort(set1.begin(), set1.end());
	std::sort(set2.begin(), set2.end());

	EdgeVector temp_set_1_t;
	EdgeVector temp_set_2_t;
	EdgeVector temp_set_1_f;
	EdgeVector temp_set_2_f;
	std::stringstream stream;
	
	for (EdgeVector::iterator it = set1.begin(); it != set1.end(); it++) {
		Edge* edge = *it;
		if (edge->label() == EDGE_LABEL::EDGE_TRUE) {
			temp_set_1_t.push_back(edge);
		}
		else if (edge->label() == EDGE_LABEL::EDGE_FALSE) {
			temp_set_1_f.push_back(edge);
		}
		else {
			
			stream << "intersection on invalied label," << edge->label() << "," << edge->from()->title().c_str() << edge->to()->title().c_str();
			_Errmsg(stream.str());
		}
	}
	for (EdgeVector::iterator it = set2.begin(); it != set2.end(); it++) {
		Edge* edge = *it;
		if (edge->label() == EDGE_LABEL::EDGE_TRUE) {
			temp_set_2_t.push_back(edge);
		}
		else if (edge->label() == EDGE_LABEL::EDGE_FALSE) {
			temp_set_2_f.push_back(edge);
		}
		else {
			stream << "intersection on invalied label," << edge->label() << "," << edge->from()->title().c_str() << edge->to()->title().c_str();
			_Errmsg(stream.str());
		}
	}

	

	//1 if CD of node 
	EdgeVector intersecection_t;
	for (EdgeVector::iterator it = temp_set_1_t.begin(); it != temp_set_1_t.end(); it++) {
		Edge* edge_t = (*it);
		for (EdgeVector::iterator it = temp_set_2_t.begin(); it != temp_set_2_t.end(); it++) {
			if (edge_t == (*it)) {
				intersecection_t.push_back(edge_t);
				break;
			}
		}
	}
	

	EdgeVector intersecection_f;
	for (EdgeVector::iterator it = temp_set_1_f.begin(); it != temp_set_1_f.end(); it++) {
		Edge* edge_f = (*it);
		for (EdgeVector::iterator it = temp_set_2_f.begin(); it != temp_set_2_f.end(); it++) {
			if (edge_f == (*it)) {
				intersecection_t.push_back(edge_f);
				break;
			}
		}
	}
	
	intersecection_t.concatenate(intersecection_f);
	return intersecection_t;
}


void Edge::Print(std::string msg) {
	CFGNode* from_node = this->from();
	CFGNode* to_node = this->to();

	printf("%s[Edge label %d]: from (title:%s id:%d), to (title:%s id:%d)\n", msg.c_str(),this->label(), from_node->title().c_str(), from_node->id(), to_node->title().c_str(), to_node->id());

}

std::vector<EDGE_LABEL> Block::out_directions() {
	std::vector<EDGE_LABEL> direction_vector;
	size_t size = this->nodes().size();
	if (size == 0)
		return direction_vector;
	CFGNode* node = this->nodes()[size - 1];
	for (EdgeVector::iterator it = node->out_edges().begin(); it != node->out_edges().end(); it++) {
		direction_vector.push_back((*it)->label());
	}
	return direction_vector;
}
json Block::GetJson() {
	json j;
	std::stringstream msg;
	for (CFGNodeVector::iterator it = nodes().begin(); it != nodes().end(); it++) {
		CFGNode* node = *it;
		msg << node->GetInstruction() << "\n";
	}

	j["title"] = msg.str();
	j["id"] = id_;
	j["x"] = x_;
	j["y"] = y_;
	return j;
}

std::string CFGNode::GetInstruction() {
	std::ostringstream  msg;
	msg << id_ << ":" << title_;
	switch (opcode()) {
	case UOpcode::phi: {
		msg << "[" << std::hex << (operands()[0]) << " :";
		for (ValueVector::iterator it = this->operands().begin() + 1; it != this->operands().end(); it++) {
			Value value = *it;
			msg << value << ",";
		}
		msg << "]";
		break;
	}
	case UOpcode::sym:
	case UOpcode::str:
	case UOpcode::lda:
	case UOpcode::lod:
		msg << "[" << std::hex << (operands()[0]) << "]";
		break;
	}
	return msg.str();
}

json CFGNode::GetJson() {
	json j;

	j["title"] = GetInstruction();
	j["id"] = id_;
	j["x"] = x_;
	j["y"] = y_;
	return j;
}



