#define DOG_EXEC_CPP_
#ifdef DOG_EXEC_CPP_
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
#include "../src/common.h"
#define PAUSE std::system("pause")
#include "../cmd/exec_func.h"
#include <stdio.h>
#include <stdlib.h>
#include "test_exec.h"
int main(){
	hash_table_init(&keySpace);
	char input_buf[MAXLINE];
	char output_buf[MAXLINE];
	while (1) {
		gets_s(input_buf);
		int n=exec(output_buf, input_buf);
		output_buf[n] = 0;
		cout << output_buf<<endl;
	}

	return 0;
}
#endif DOG_EXEC_CPP