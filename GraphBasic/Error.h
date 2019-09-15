#pragma once

#include <iostream>
#include <sstream>

#define _Errmsg(msg) ErrMsg::Errmsg(__FILE__, __LINE__,msg);

class ErrMsg {
public:
	static void Errmsg(std::string file, int line_number, std::string msg = "") {
		std::cerr << file << "," << line_number << ":  " << msg << "\n";
		abort();
	}
};


