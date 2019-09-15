#pragma once

#include <iostream>
#include <fstream>
#include <memory>

#include "Graph.h"
#include "UCodeScanner.h"
#include "UCodeGraphBuilder.h"


typedef std::map<std::string, Graph*> FuntionGraphMap;
typedef std::map<std::string, Value> FuntionIdMap;

class UCodeInterpreter {

public:
	UCodeInterpreter(const char* path);
	void Interpret();

	static const int MAXINSTR = 2000;
	static const int MAXLABELS = 300;
	static const int STACKSIZE = 20000;
	static const int LABELSIZE = 10;
	
	FuntionGraphMap& function_graphs() { return function_graphs_; }
	FuntionIdMap& function_id_map() { return function_id_map_; }
	Value CreateFunctionId() { return function_number_++; }
private:
	std::shared_ptr<Scanner*> scanner_;
	FuntionGraphMap function_graphs_;
	FuntionIdMap function_id_map_;
	Value function_number_;
};


