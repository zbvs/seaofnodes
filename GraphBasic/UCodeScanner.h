#pragma once
#include <iostream>
#include <fstream>
#include "CFG.h"
#include <ctype.h>
#include "Error.h"


class Scanner {
public:
	Scanner(const char* path);

	
	CFGNode::UOpcode GetOpcode();
	CFGNode::UOpcode PreLookOpcode();
	bool NextToken();
	bool NextLine();
	void ResetStream();
	std::string GetLabel();
	std::string GetString();
	Value GetValue();

	static const int OPCODE_START_INDEX = 11;
	static const int LABELSIZE = 10;

	int line_index() { return line_index_; }
	bool IsEnd() { return (in_file_.eof() || in_file_.fail()); }
	bool start_with_label() { return start_with_label_; }
	std::string line_buffer() { return line_buffer_; }
	std::string label() { return label_; }
	
private:
	int line_index_;
	bool start_with_label_;
	std::string line_buffer_;
	std::string label_;
	std::ifstream in_file_;
	
};