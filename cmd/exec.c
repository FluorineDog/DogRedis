#include "exec_func.h"
#include <stdio.h>
#include "../src/common.h"
#include "../src/common.h"
#define MAX_ARGV 0x100
int exec(char* output, char* input) {
	char* argv_buf[MAX_ARGV];
	int argc = analyse(input, argv_buf);
	if (argc == 0)
		return 0;
	func_t cmd = function(argv_buf[0]);
	if (cmd == NULL)
	{
		return snprintf(output, MAXLINE, "(error) commond not recognise");
	}
	output[0] = '\0';
	int status = cmd(argc, argv_buf, output);
	if (status == ERROR)
		return snprintf(output, MAXLINE, "%s", output);
	else if (!output[0]) {
		if (status == OK)
			return snprintf(output, MAXLINE, "OK");
		if (status == NIL) 
			return snprintf(output, MAXLINE, "(nil)");
		else
			return snprintf(output, MAXLINE, "(integer) %d", status);
	}
	else
		return snprintf(output, MAXLINE, "%s", output);
}
int analyse(char* input, char** argv_buf) {
	int argc = 0;
	char status[256] = { '\0' };
	status['\0'] = 8;
	status['\n'] = 8;
	status['\t'] = 2;
	status[' '] = 2;
	char last = 2;
	for (; status[*input] != 8; ++input) {
		if (status[*input] == 2 && last == 0) {
			*input = 0;
		}
		else if (status[*input] == 0 && last !=0 ) {
			argv_buf[argc] = input;
			++argc;
		}
		last = status[*input];
	}
	//if (last == 0) {
	//	++argc;
	//	status[*input] = 0;
	//}
	return argc;
}