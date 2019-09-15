#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include "CFG.h"
#include "Graph.h"
#include "UCodeScanner.h"


class UCodeInterpreter;

typedef std::map<std::string, CFGNode*> LabelMap;
typedef std::map<CFGNode*,std::string> JumpMap;


class UCodeGraphBuilder {
public:
	
	UCodeGraphBuilder(std::shared_ptr<Scanner*> scanner, UCodeInterpreter* interpreter) : scanner_(scanner), interpreter_(interpreter){
		
	}
	~UCodeGraphBuilder();
	UCodeInterpreter* interpreter() { return interpreter_; }
	Graph* BuildGraph();

	
private:
	UCodeInterpreter* interpreter_;
	std::shared_ptr<Scanner*> scanner_;
};


