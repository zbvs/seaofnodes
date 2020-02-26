#pragma once
#include "stdafx.h"

#include "UCodeInterpreter.h"
#include "Error.h"

UCodeInterpreter::UCodeInterpreter(const char* path) : function_number_(0){
	std::cout << "UCodeInterpreter()" << std::endl;
	scanner_ = std::make_shared<Scanner>(path);
	Interpret();
}

UCodeInterpreter::~UCodeInterpreter() {
	std::cout << "~UCodeInterpreter()" << std::endl;
}


void UCodeInterpreter::Interpret() {
	enum { FALSE, TRUE };
	std::shared_ptr<Scanner> scanner = scanner_;

	CFGNode::UOpcode opcode;
	std::cout << " == Interpreting ... ==" << '\n';
	function_id_map().insert(std::make_pair("read", CreateFunctionId()));
	function_id_map().insert(std::make_pair("write", CreateFunctionId()));

	while (scanner->NextLine()) {
		opcode = scanner->PreLookOpcode();
		if(opcode ==CFGNode::proc) {
			std::string proc_name = scanner->GetLabel();
			function_id_map().insert(std::make_pair(proc_name, CreateFunctionId()));
			break;
		}
	}
	scanner->ResetStream();

	while (scanner->NextLine()) {
		opcode = scanner->PreLookOpcode();
		std::cout << scanner->line_buffer() << '\n';
		switch (opcode) {
		case CFGNode::proc: {
			std::string proc_name = scanner->GetLabel();
			std::cout << "proc_name:::" << proc_name << std::endl;
			UCodeGraphBuilder builder(scanner, this);
			CFG* graph = builder.BuildGraph();
			graph->set_id(function_id_map()[proc_name]);
			function_graphs().insert(std::make_pair(proc_name, graph));
			break;
		}
		default: {
			std::stringstream msg;
			msg << "opcode must be proc or bgn::" << CFGNode::opcode_names_[opcode];
			_Errmsg(msg.str());
			
		}
		}

	}
}
