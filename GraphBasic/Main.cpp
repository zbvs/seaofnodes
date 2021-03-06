#include "stdafx.h"

#include <iostream>
#include "CFGNode.h"
#include "CFG.h"
#include "json/json.h"
#include "UCodeInterpreter.h"
#include "GraphOptimizer.h"
#include "VDG.h"

int main()
{

	UCodeInterpreter interpreter(".\\perfect.uco");
	FuntionGraphMap fmap = interpreter.function_graphs();
	CFG* cfg = fmap["main"];
	
	std::cout << fmap.size() << std::endl;
	std::cout << cfg << std::endl;
	
	cfg->PrintGraph("C:\\Users\\tr\\Downloads\\cfg.json");
	cfg->PrintBlockGraph("C:\\Users\\tr\\Downloads\\block_cfg.json");
	cfg->CreateDFST();
	cfg->PrintBlockTree("C:\\Users\\tr\\Downloads\\DFST.json");
	cfg->CreateDominatorTree();
	cfg->PrintBlockTree("C:\\Users\\tr\\Downloads\\DOM_Tree.json");
	cfg->CalculateDominanceFrontier();
	cfg->InsertPhi();
	cfg->PrintGraph("C:\\Users\\tr\\Downloads\\SSA_cfg.json");
	cfg->ToSSA();
	cfg->PrintBlockGraph("C:\\Users\\tr\\Downloads\\block_SSA_cfg.json");
	VDGBuilder vdg_builder;
	vdg_builder.BuildVDG(cfg);
	vdg_builder.vdg()->PrintGraph("C:\\Users\\tr\\Downloads\\DFG_cfg.json");

}