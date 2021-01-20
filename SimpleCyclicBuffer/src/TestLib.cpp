//============================================================================
// Name        : SimpleCyclicBuffer.cpp
// Author      : Lior Timor
// Version     :
// Copyright   : MIT
// Description : Test SimpleCyclicBuffer - TODO: Need to swap with CppUTest
//============================================================================

#include <iostream>
using namespace std;


#include "SimpleCyclicBuffer.h"

//TODO: Swap with a CppUTest
#define _assert(eval_statement) 	if ((eval_statement) == false) return false


bool testSimpleBuffer()
{
	SimpleCyclicBuffer<int, 100> intBuffer;


	_assert(intBuffer.size() == 100);
	_assert(intBuffer.len() == 0);

	return true;
}

int main() {

	cout << "running tests (reminder - replace with CppUTest)\n\n";


	if (testSimpleBuffer() == true)
		cout << "testSimpleBuffer PASSED";
	else
		cout << "testSimpleBuffer FAILED";


	return 0;
}
