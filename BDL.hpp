#ifndef BDL

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

using std::string;
using std::ifstream;
using std::ofstream;
using std::cerr;

namespace BDL {
//Variables
	bool initialized = false;
	string globalString;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="str"></param>
	void globalStringFunc(std::string &str) {
		if (!initialized) {
			globalString = "Beginning of log\n";
			initialized = true;
			globalString::append(str);
			globalString::append("\n");
		}
		globalString::append(str);
	}
} // namespace BDL
#endif //BDL