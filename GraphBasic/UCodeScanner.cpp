#include "stdafx.h"
#include "UCodeScanner.h"

Scanner::Scanner(const char* path) {
	in_file_.open(path);
	if (!in_file_.is_open()) {
		_Errmsg( "file open failed");
	}
	line_index_ = 0;
	start_with_label_ = false;
}

CFGNode::UOpcode Scanner::PreLookOpcode()
{
	std::string mnemonic;
	
	// always start at 12-th column.
	int buf_index_origin = line_index_;
	
	
	for (line_index_ = OPCODE_START_INDEX; line_index_ < line_buffer_.size() && !isspace(line_buffer_[line_index_]); \
		line_index_++)
		;

	mnemonic = line_buffer_.substr(OPCODE_START_INDEX, (line_index_ - OPCODE_START_INDEX));
	

	int opcode_number;
	for (opcode_number = CFGNode::notop; opcode_number < CFGNode::none; opcode_number++)
		if (!mnemonic.compare(CFGNode::opcode_names_[opcode_number]))
			break;

	if (opcode_number == CFGNode::none) {
		std::stringstream msg;
		int temp = line_buffer_[line_index_];
		msg << "invalid mnemonic :: " << mnemonic;
		_Errmsg( msg.str());
	}
	line_index_ = buf_index_origin;
	return (CFGNode::UOpcode)opcode_number;
}

CFGNode::UOpcode Scanner::GetOpcode()
{
	std::string mnemonic;

	
	if (line_buffer_.size() <= OPCODE_START_INDEX) {
		_Errmsg( "invalid assembly format");
	}


	for (line_index_ = OPCODE_START_INDEX; line_index_ < line_buffer_.size() && !isspace(line_buffer_[line_index_]); \
		line_index_++)
		;

	mnemonic = line_buffer_.substr(OPCODE_START_INDEX, (line_index_ - OPCODE_START_INDEX));


	int opcode_number;
	for (opcode_number = CFGNode::notop; opcode_number < CFGNode::none; opcode_number++)
		if (!mnemonic.compare(CFGNode::opcode_names_[opcode_number]))
			break;

	if (opcode_number == CFGNode::none) {
		std::stringstream msg;
		msg << "invalid mnemonic : " << mnemonic;
		_Errmsg( msg.str());
	}


	NextToken();

	return (CFGNode::UOpcode)opcode_number;
}

std::string Scanner::GetLabel()
{
	if (isspace(line_buffer_[line_index_])) {
		_Errmsg( "not start with label");
	}


	int start_index = line_index_;
	for (; line_index_ < line_buffer_.size() && !isspace(line_buffer_[line_index_]); line_index_++)
		;

	label_ = line_buffer_.substr(start_index, line_index_ - start_index);
	NextToken();
	return label_;
}

std::string Scanner::GetString()
{
	if (isspace(line_buffer_[line_index_])) {
		_Errmsg("not start with string");
	}


	int start_index = line_index_;
	for (; line_index_ < line_buffer_.size() && !isspace(line_buffer_[line_index_]); line_index_++)
		;

	label_ = line_buffer_.substr(start_index, line_index_ - start_index);
	NextToken();
	return label_;
}

Value Scanner::GetValue()
{
	int start_index = line_index_;
	for (; line_index_ < line_buffer_.size() && !isspace(line_buffer_[line_index_]); line_index_++)
		;

	label_ = line_buffer_.substr(start_index, line_index_ - start_index);
	NextToken();
	Value val;
	std::istringstream iss(label_);
	iss >> val;
	return val;
}

bool Scanner::NextToken() {
	
	bool token_found = false;
	for (; line_index_ < line_buffer_.size() && isspace(line_buffer_[line_index_]); line_index_++) {
		if (!isspace(line_buffer_[line_index_])) {
			token_found = true;
			break;
		}
	}
	
	return token_found;
}


bool Scanner::NextLine()
{
	line_index_ = 0;	
	while (!in_file_.eof() && !in_file_.fail()) {
		std::getline(in_file_, line_buffer_);
		size_t line_size = line_buffer_.size();
		if (line_size > 0 && !isspace(line_buffer_[0])) {
			start_with_label_ = true;//start with label
			return true;
		}
		else
		{
			// check if contain any non-space character
			for (int i = 0; i < line_size; i++)
				if (!isspace(line_buffer_[i])) {
					start_with_label_ = false;
					return true;
				}
		}
	}
	return false;
}

void Scanner::ResetStream() {
	in_file_.seekg(0, in_file_.beg);
}




