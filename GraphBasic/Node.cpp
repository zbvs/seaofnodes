#include "stdafx.h"
#include "Node.h"

void Node::push_control_input(Node* control_node) {
	this->inputs().insert(this->inputs().begin() + this->control_count(), control_node);
	this->control_count_++;
}


void Node::push_value_input(Node* data_node) {
	this->inputs().insert(this->inputs().begin() + (this->control_count()+this->value_count()), data_node);
	this->value_count_++;
}
