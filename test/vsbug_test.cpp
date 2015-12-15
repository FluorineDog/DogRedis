//#define DOG_VSBUG_TEST_CPP
#ifdef DOG_VSBUG_TEST_CPP

#include <stdio.h>

#define TEST_1 printf(__FUNCTION__ ##"hello"  ); // works fine

#define TEST_2 printf("hello" ## __FUNCTION__ ); // compile error
int main(){
	
	TEST_1; // works fine
	TEST_2;	// compile error
	return 0;
}

#endif // DOG_VSBUG_TEST_CPP