#include "stdafx.h"
#include "Node.h"


std::string Node::GetTitle() {
	
	std::ostringstream  msg;
	msg << id_ << ":" << title_;
	switch (opcode()) {

	case Opcode::ldc:
		msg << "[" << std::hex << value_ << "]";
		break;
	}
	return msg.str();
	
}

json Node::GetJson() {
	json j;

	j["title"] = GetTitle();
	j["id"] = id_;
	j["x"] = x_;
	j["y"] = y_;
	return j;
}

Node::Opcode Node::toNodeOpcode(CFGNode::UOpcode uop) {
	return (Node::Opcode)uop;
}

void Node::push_control_input(Node* control_node) {
	std::cout << "push control: " << this->control_count() << "  :" << this->GetTitle() << ".control_push(" << \
		control_node->GetTitle() << ")" << std::endl;
	this->inputs().insert(this->inputs().begin() + this->control_count(), control_node);
	this->control_count_++;
}


void Node::push_value_input(Node* data_node) {
	this->inputs().insert(this->inputs().begin() + (this->control_count()+this->value_count()), data_node);
	this->value_count_++;
}
