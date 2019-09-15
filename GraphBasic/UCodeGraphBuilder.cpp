#include "stdafx.h"

#include <iostream>
#include <fstream>
#include "UCodeInterpreter.h"

UCodeGraphBuilder::~UCodeGraphBuilder() {}

Graph* UCodeGraphBuilder::BuildGraph() {
	Scanner* scanner = *scanner_.get();
	CFGNode::UOpcode opcode;
	LabelMap label_map;
	JumpMap jump_map;
	std::string label;
	Graph* graph = new Graph(Graph::GraphType::GRAPH);
	opcode = scanner->GetOpcode();
	CFGNode* start_node = new CFGNode(graph, opcode);
	graph->AddNodeAsStart(start_node);
	Block * current_block = new Block(start_node);
	graph->blocks().push_back(current_block);
	current_block->nodes().push_back(start_node);
	start_node->set_block(current_block);
	CFGNodeVector labeled_nodes;
	CFGNode::UOpcode prev_opcode = CFGNode::UOpcode::none;
	enum { FALSE, TRUE };
	int done = FALSE;

	std::cout << " == Building Graph ... ==" << '\n';
	while (done == FALSE && scanner->NextLine()) {
		opcode = scanner->PreLookOpcode();
		printf(":%s\n", CFGNode::opcode_names_[opcode]);
		
		//std::cout << "opcode:" << Node::opcode_names_[opcode] << '\n';
		CFGNode* cfg_node = new CFGNode(graph,opcode);
		cfg_node->set_title(CFGNode::opcode_names_[opcode]);
		graph->AddNode(cfg_node);
		if (scanner->start_with_label()) {
			label_map[scanner->GetLabel()]=cfg_node;
			current_block = new Block(cfg_node);
			graph->blocks().push_back(current_block);
		}
		else if (prev_opcode == CFGNode::tjp || prev_opcode == CFGNode::fjp || prev_opcode == CFGNode::ujp) {
			current_block = new Block(cfg_node);
			graph->blocks().push_back(current_block);
		}

		scanner->GetOpcode();
		current_block->nodes().push_back(cfg_node);
		cfg_node->set_block(current_block);

		switch (opcode) {
		case CFGNode::sym: {
			Value talble_num = scanner->GetValue();
			cfg_node->inputs().push_back(talble_num);
			Value location = scanner->GetValue();
			cfg_node->inputs().push_back(TO_ID(location));
			Value size = scanner->GetValue();
			cfg_node->inputs().push_back(size);
			break;
		}
		case CFGNode::lda:
		case CFGNode::lod:
		case CFGNode::str: {
			Value talble_num = scanner->GetValue();
			Value location = scanner->GetValue();
			cfg_node->inputs().push_back(TO_ID(location));
			cfg_node->inputs().push_back(talble_num);
			break;
		}
		case CFGNode::ldc: {
			Value value = scanner->GetValue();
			cfg_node->inputs().push_back(value);
			break;
		}
		case CFGNode::call: {
			std::string fname = scanner->GetString();
			Value f_id = this->interpreter()->function_id_map()[fname];
			cfg_node->inputs().push_back(f_id);
			break;
		}
		case CFGNode::ujp: 
		case CFGNode::tjp:
		case CFGNode::fjp: {
			jump_map[cfg_node] = std::string(scanner->GetLabel());
			break;
		}
		case CFGNode::endop: {
			done = TRUE;
			break;
		}
		default: {
			break;
		}
		}
		prev_opcode = opcode;
	}

	CFGNode* prev_node = start_node;
	current_block = start_node->block();
	Block* dst_block = NULL;
	
	prev_opcode = start_node->opcode();
	for (CFGNodeVector::iterator outer_it = graph->nodes().begin() + 1; outer_it != graph->nodes().end(); outer_it++) {
		CFGNode* cfg_node = static_cast<CFGNode*>(*outer_it);
		CFGNode::UOpcode opcode = cfg_node->opcode();
		if (prev_opcode != CFGNode::UOpcode::ujp) {
			switch (prev_opcode) {
			case CFGNode::tjp:
				prev_node->EdgeFromTo(cfg_node, EDGE_LABEL::EDGE_FALSE);
				dst_block = cfg_node->block();
				current_block->outs().push_back(dst_block);
				dst_block->ins().push_back(current_block);
				//new block start
				current_block = dst_block;
				break;
			case CFGNode::fjp:
				prev_node->EdgeFromTo(cfg_node, EDGE_LABEL::EDGE_TRUE);
				dst_block = cfg_node->block();
				current_block->outs().push_back(dst_block);
				dst_block->ins().push_back(current_block);
				//new block start
				current_block = dst_block;
				break;

			default:
				prev_node->EdgeFromTo(cfg_node, EDGE_LABEL::EDGE_NONE);
				break;
			}
			
		}
		else {//prev_opcode is ujp. new block start
			current_block = cfg_node->block();
		}
		
		switch (opcode) {
		case CFGNode::ujp: {
			label = jump_map[cfg_node];
			CFGNode* target_node = label_map[label];
			cfg_node->EdgeFromTo(target_node, EDGE_LABEL::EDGE_NONE);
			dst_block = target_node->block();
			current_block->outs().push_back(dst_block);
			dst_block->ins().push_back(current_block);
			break;
		}
		case CFGNode::tjp: {
			label = jump_map[cfg_node];
			CFGNode* target_node = label_map[label];
			cfg_node->EdgeFromTo(target_node,EDGE_LABEL::EDGE_TRUE);
			dst_block = target_node->block();
			current_block->outs().push_back(dst_block);
			dst_block->ins().push_back(current_block);
			break;
		}
		case CFGNode::fjp: {
			label = jump_map[cfg_node];
			CFGNode* target_node = label_map[label];
			cfg_node->EdgeFromTo(target_node, EDGE_LABEL::EDGE_TRUE);
			dst_block = target_node->block();
			current_block->outs().push_back(dst_block);
			dst_block->ins().push_back(current_block);
			break;
		}
		case CFGNode::nop: {//new block start
			if (prev_opcode!=CFGNode::UOpcode::ujp) {
				prev_node->EdgeFromTo(cfg_node, EDGE_LABEL::EDGE_NONE);
				dst_block = cfg_node->block();
				current_block->outs().push_back(dst_block);
				dst_block->ins().push_back(current_block);
				current_block = dst_block;
			}
			break;
		}
		default:
			break;
		}
		prev_opcode = opcode;		
		prev_node = cfg_node;
	}
	return graph;
}





