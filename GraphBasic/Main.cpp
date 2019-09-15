#include "stdafx.h"

#include <iostream>
#include "CFG.h"
#include "Graph.h"
#include "json/json.h"
#include "UCodeInterpreter.h"
#include "GraphOptimizer.h"


int main()
{

	UCodeInterpreter* interpreter = new UCodeInterpreter("C:\\Users\\tr\\Downloads\\perfect.uco");
	FuntionGraphMap fmap = interpreter->function_graphs();
	Graph* graph = fmap["main"];
	CFGNode* cur = graph->start();
	
	std::cout << fmap.size() << std::endl;
	std::cout << graph << std::endl;
	
	graph->PrintGraph("C:\\Users\\tr\\Downloads\\cfg.json");
	graph->PrintBlockGraph("C:\\Users\\tr\\Downloads\\block_cfg.json");
	graph->CreateDFST();
	graph->PrintBlockTree("C:\\Users\\tr\\Downloads\\DFST.json");
	graph->CreateDominatorTree();
	graph->PrintBlockTree("C:\\Users\\tr\\Downloads\\DOM_Tree.json");
	graph->CalculateDominanceFrontier();
	graph->InsertPhi();
	graph->PrintGraph("C:\\Users\\tr\\Downloads\\SSA_cfg.json");
	graph->PrintBlockGraph("C:\\Users\\tr\\Downloads\\block_SSA_cfg.json");
}



