#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include "CFGNode.h"
#include "CFG.h"
#include "UCodeScanner.h"


class UCodeInterpreter;

typedef std::map<std::string, CFGNode*> LabelMap;
typedef std::map<CFGNode*,std::string> JumpMap;


class UCodeGraphBuilder {
public:
	
	UCodeGraphBuilder(std::shared_ptr<Scanner> scanner, UCodeInterpreter* interpreter);
	~UCodeGraphBuilder();
	UCodeInterpreter* interpreter() { return interpreter_; }
	CFG* BuildGraph();

	
private:
	UCodeInterpreter* interpreter_;
	std::shared_ptr<Scanner> scanner_;
};


