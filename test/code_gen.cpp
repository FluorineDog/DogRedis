//#define DOG_CODE_GEN

#ifdef DOG_CODE_GEN
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
#include <string>
#define PAUSE std::system("pause")


int main(){
	std::string s[]{ "hash","keys","set", "string",/*"list",*/ };
	for (auto str : s) {
		std::ifstream fin("code/cmd/" + str + ".txt");
		std::ofstream fout("code/src/"+str + ".h");
		std::string name;
		std::transform(str.begin(), str.end(), str.begin(), toupper);
		str = "DOG_" + str + "_H_";
		fout << "#ifndef " << str << endl;
		fout << "#define " << str << endl;
		//fout << R"(#include "hash_implement.h" )" << endl;
		fout << R"(#include "common.h" )" << endl;
		while (fin >> name) {
			fout << "DOG_CMD_FUNC_DEF(" << name << ");" << endl;
		}
		fout << "#endif // " << str << endl;
	}
	
	std::ofstream fout("code/cmd/functions.cpp");
	///fout << "hello";
	fout << R"(#include "exec_func.h")" << endl;
	for (auto str : s) {
		fout << R"(#include "../src/)" << str << R"(.h")" << endl;
	}
	fout << R"(function_map_item func_map[] = {)"<<endl;
	for (auto str : s) {
		std::ifstream fin("code/cmd/" + str + ".txt");
		fout<<R"(	// )"<<str<<R"(.h)" << endl;
		std::string m;
		while (fin >> m) {
			fout<<R"(	{ ")"<< m
				<<R"(", )"<<m<<R"( },)"<<endl;
		}
	}
	fout << R"(};
func_t function(char* name) {
	for (int i = 0; \
		i < sizeof(func_map) / sizeof(func_map[0]); ++i) 
	{
		if (!strcmp(name, func_map[i].name)) {
			return func_map[i].func;
		}
	}
	return nullptr;
})" << endl;
	return 0;
}
#endif // DOG_CODE_GEN