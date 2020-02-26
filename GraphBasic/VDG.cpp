#include "stdafx.h"
#include "VDG.h"

Environment::Environment() {
	std::cout << "Environment()" << std::endl;
}

Environment::~Environment() {
	std::cout << "~Environment()" << std::endl;
}

Node* Environment::pop_node(Node* caller) {
	Node* node = stack_.back();
	stack_.pop_back();
	return node;
}
void Environment::push_node(Node* node) {
	stack_.push_back(node);
}


Node* Environment::GetDef(Variable variable) {
	VariableVector var_vec = variables();
	for (VariableVector::iterator it = var_vec.begin(); it != var_vec.end(); it++) {
		var_pair pair = *it;
		if (pair.first.isSameVariableDef(variable)) {
			return pair.second;
		}
	}
	Node* node = new Node(Node::none, 0);

	PutDef(node, variable);
	return node;
}

void Environment::PutDef(Node* def_node, Variable variable) {
	VariableVector var_vec = variables();
	for (VariableVector::iterator it = var_vec.begin(); it != var_vec.end(); it++) {
		var_pair pair = *it;
		if (pair.first.isSameVariableDef(variable))
			_Errmsg("def variable twice");
	}
	variables().push_back(std::make_pair(variable, def_node));
}


Node* VDGBuilder::VisitBlock(Block* block, Node* region) {
	//printf("Visit REGION: %d\n", region->id());

	CFGNodeVector cfg_nodes = block->nodes();
	Node* last_control_out = region;
	Environment& env = environment();
	Node* node;

	Node* region_start = new Node(Node::REGION_START, TO_REGION_START_ID(block->id()));
	region_start->push_control_input(region);

	for (CFGNodeVector::iterator cfg_it = cfg_nodes.begin(); cfg_it != cfg_nodes.end(); cfg_it++) {
		CFGNode* cfg_node = *cfg_it;
		//printf("!!%d: %s\n", cfg_node->id(), cfg_node->title().c_str());
		switch (cfg_node->opcode()) {
			//control node
		case CFGNode::fjp:
		case CFGNode::tjp: {
			node = new Node(Node::IF, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			break;
		}
		case CFGNode::sym: {
			Variable variable = Variable::toVariable(cfg_node->operands()[0]);
			Node* node = env.GetDef(variable);
			node->set_id(cfg_node->id());
			node->set_opcode(Node::sym);
			break;
		}
						   //push 1 stack
		case CFGNode::ldc: {
			node = new Node(Node::ldc, cfg_node->operands()[0], cfg_node->id());
			env.push_node(node);
			break;
		}
		case CFGNode::lod: {
			node = new Node(Node::lod, cfg_node->id());
			Variable variable = Variable::toVariable(cfg_node->operands()[0]);
			Node* def_node = env.GetDef(variable);
			node->push_value_input(def_node);
			env.push_node(node);
			break;
		}
		case CFGNode::lda: {
			Value addr_value = cfg_node->operands()[0];
			node = new Node(Node::ldc, addr_value, cfg_node->id());
			env.push_node(node);
			break;
		}
						   //consume 1 stack	
		case CFGNode::str: {
			Variable variable = Variable::toVariable(cfg_node->operands()[0]);
			node = env.GetDef(variable);
			node->set_id(cfg_node->id());
			node->set_opcode(Node::str);
			node->push_value_input(env.pop_node(node));

			break;
		}
		case CFGNode::call: {
			node = new Node(Node::call, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			break;
		}
							//consume 2 stack
		case CFGNode::sti: {
			node = new Node(Node::sti, cfg_node->id());
			node->set_opcode(Node::sti);
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			break;
		}

						   //consume arbitary stack
		case CFGNode::phi: {
			Variable variable = Variable::toVariable(cfg_node->operands()[0]);
			node = env.GetDef(variable);
			node->set_id(cfg_node->id());
			node->set_opcode(Node::phi);
			//index 0 is result_variable of phi.
			//start from index 1
			for (int i = 1; i < cfg_node->operands().size(); i++) {
				//printf("phi: input %llx\n", cfg_node->operands()[i]);
				Node* def_node = env.GetDef(Variable::toVariable(cfg_node->operands()[i]));
				node->push_value_input(def_node);
			}
			break;
		}
						   //consume 1 stack , and push 1 stack

		case CFGNode::incop: {
			node = new Node(Node::incop, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::ldi: {
			node = new Node(Node::ldi, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
						   //----------------------------------
						   //consume 2 stack, and push 1 stack
						   //-----------------------------------

						   //binary op
		case CFGNode::add: {
			node = new Node(Node::add, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::divop: {
			node = new Node(Node::divop, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::modop: {
			node = new Node(Node::modop, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::sub: {
			node = new Node(Node::sub, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
						   //condition
		case CFGNode::gt: {
			node = new Node(Node::gt, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::lt: {
			node = new Node(Node::lt, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::ge: {
			node = new Node(Node::ge, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::le: {
			node = new Node(Node::le, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::eq: {
			node = new Node(Node::eq, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
		case CFGNode::ne: {
			node = new Node(Node::ne, cfg_node->id());
			node->push_value_input(env.pop_node(node));
			node->push_value_input(env.pop_node(node));
			env.push_node(node);
			break;
		}
						  //does'n use stack
		default: {
			node = new Node(Node::none, cfg_node->id());
			break;
		}
		}
		if (!this->vdg()->nodes().IsContain(node))
			this->vdg()->add_node(node);

		//printf("!!%d: %s\n", cfg_node->id(), cfg_node->title().c_str());
		switch (node->opcode()) {
		case Node::phi: {
			if (node->control_count() == 0)
				node->push_control_input(region);
			break;
		}
		case Node::sym:
		case Node::call:
		case Node::IF: {
			if (node->control_count() != 0)
				_Errmsg("wrong control count");
			node->push_control_input(last_control_out);
			last_control_out = node;
			break;
		}
		default:
			if (node->control_count() != 0)
				_Errmsg("wrong control count");
			node->push_control_input(region_start);
			break;
		}

		//node = NULL;
	}
	//return region , or IF 

	return last_control_out;
}

void VDGBuilder::BuildVDG(CFG* cfg_graph) {
	typedef std::pair<Block*, Node*> BlockPair;

	
	
	VDG* vdg = new VDG();
	set_vdg(vdg);
	BlockVector  visited;
	std::vector<BlockPair>  to_visit;

	std::map<Block*, Node*> ControlOutMap;

	Node* end = new Node(Node::Opcode::END, -1);
	vdg->set_end(end);
	vdg->add_node(end);
	
	Block* current_block = cfg_graph->end()->block();
	Node* current_region = new Node(Node::Opcode::REGION, TO_REGION_ID(current_block->id()));
	Node* control_out = VisitBlock(current_block, current_region);
	end->push_control_input(control_out);
	ControlOutMap[current_block] = current_region;

	visited.push_back(current_block);
	to_visit.push_back(std::make_pair(current_block, current_region));

	while (!to_visit.empty()) {
		BlockPair pair = to_visit.back();
		current_block = pair.first;
		current_region = pair.second;
		to_visit.pop_back();

		BlockVector srcs = current_block->ins();
		if (srcs.size() == 0) {
			printf("start block %d %s\n", current_block->nodes()[0]->id(), current_block->title().c_str());
			Node* start_node = new Node(Node::Opcode::START, -2);
			current_region->push_control_input(start_node);
			vdg->set_start(start_node);
			vdg->add_node(start_node);
		}
		else
			for (BlockVector::iterator it = srcs.begin(); it != srcs.end(); it++) {
				Block* src_block = *it;
				if (!visited.IsContain(src_block)) {
					Node* src_region = new Node(Node::Opcode::REGION, TO_REGION_ID(src_block->id()));
					vdg->add_node(src_region);
					printf("visit block %d %s\n", src_block->id(), src_block->title().c_str());
					control_out = VisitBlock(src_block, src_region);
					ControlOutMap[src_block] = control_out;
					if (control_out->opcode() == Node::IF) {
						size_t index = src_block->outs().GetIndexOf(current_block);
						Node* projection;
						if (src_block->out_directions()[index] == true)
							projection = new Node(Node::Opcode::PROJECTION_TRUE, TO_TRUE_ID(src_block->id()));
						else
							projection = new Node(Node::Opcode::PROJECTION_FALSE, TO_FALSE_ID(src_block->id()));
						projection->push_control_input(control_out);
						control_out = projection;
					}
					current_region->push_control_input(control_out);
					to_visit.push_back(std::make_pair(src_block, src_region));
					visited.push_back(src_block);
				}
				else {
					Node* control_out = ControlOutMap[src_block];
					if (control_out->opcode() == Node::IF) {
						size_t index = src_block->outs().GetIndexOf(current_block);
						Node* projection;
						if (src_block->out_directions()[index] == true)
							projection = new Node(Node::Opcode::PROJECTION_TRUE, TO_TRUE_ID(src_block->id()));
						else
							projection = new Node(Node::Opcode::PROJECTION_FALSE, TO_FALSE_ID(src_block->id()));
						projection->push_control_input(control_out);
						control_out = projection;
					}

					current_region->push_control_input(control_out);
				}
			}

	}
}




#define DISTANCE -200
void VDG::PrintGraph(const char* file_path) {
	json json_graph;
	json_graph["nodes"] = json::array();
	json_graph["edges"] = json::array();

	int x = 0, y = 0, level = 0;
	NodeVector visited;
	NodeVector stack_current;
	NodeVector stack_next;

	stack_current.push_back(end());

	while (!stack_current.empty()) {
		x = level * DISTANCE;
		level++;
		size_t size = stack_current.size();
		for (size_t stack_cnt = 0; stack_cnt < size; ) {
			Node* node = stack_current.at(stack_cnt++);
			if (!visited.IsContain(node)) {
				visited.push_back(node);
				node->set_x(x);
				node->set_y(y);
				json json_node = node->GetJson();
				json_graph["nodes"].push_back(json_node);
				x += DISTANCE;
				NodeVector inputs = node->inputs();
				size_t to_size = inputs.size();
				for (int input_cnt = 0; input_cnt < to_size; input_cnt++) {
					json json_edge;
					json_edge["source"] = node->id();
					json_edge["target"] = inputs.at(input_cnt)->id();
					json_graph["edges"].push_back(json_edge);
					stack_next.push_back(inputs.at(input_cnt));
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

}

