//============================================================================
// Name        : SimpleCyclicBuffer.cpp
// Author      : Lior Timor
// Version     :
// Copyright   : MIT
// Description : Test SimpleCyclicBuffer - TODO: Need to swap with CppUTest/Gtest
//============================================================================

#include <iostream>
using namespace std;

#include <list>
#include "SimpleCyclicBuffer.h"

//TODO: Swap with a CppUTest
#define _assert(eval_statement) 	if ((eval_statement) == false) return false


bool testSimpleBuffer()
{
	constexpr int BUFFER_SIZE = 100;
	SimpleCyclicBuffer<int, BUFFER_SIZE> intCyclicBuffer;

	int a = 20;

	_assert(intCyclicBuffer.size() == 100);
	_assert(intCyclicBuffer.len() == 0);

	//---- Test partial insert (add by reference) ----
	for (int i=0; i < 20; i++)
	{
		a = i;
		intCyclicBuffer.push(a);
	}
	_assert(intCyclicBuffer.len() == 20);
	for (int i=0; i < 20; i++)
	{
		a = intCyclicBuffer.pop();
		_assert(a == i);
	}
	_assert(intCyclicBuffer.len() == 0);

	//---- Test partial insert (add by move) ----
	for (int i=0; i < 20; i++)
	{
		a = i;
		intCyclicBuffer.push(std::move(a));
	}
	_assert(intCyclicBuffer.len() == 20);
	for (int i=0; i < 20; i++)
	{
		a = intCyclicBuffer.pop();
		_assert(a == i);
	}
	_assert(intCyclicBuffer.len() == 0);


	//---- Test FULL insert (add by move) ----
	for (int i=0; i < BUFFER_SIZE; i++)
	{
		a = i;
		intCyclicBuffer.push(std::move(a));
	}
	_assert(intCyclicBuffer.len() == BUFFER_SIZE);
	for (int i=0; i < BUFFER_SIZE; i++)
	{
		a = intCyclicBuffer.pop();
		_assert(a == i);
	}
	_assert(intCyclicBuffer.len() == 0);


	//---- Test PEEK ----
	for (int i=0; i < BUFFER_SIZE; i++)
	{
		a = i;
		intCyclicBuffer.push(a);
	}
	_assert(intCyclicBuffer.len() == BUFFER_SIZE);
	for (int i=0; i < BUFFER_SIZE; i++)
	{
		a = intCyclicBuffer.peek();
		_assert(a == i);
		a = intCyclicBuffer.pop();
	}
	_assert(intCyclicBuffer.len() == 0);


	//---- Test Clear ----
	for (int i=0; i < BUFFER_SIZE/2; i++)
	{
		a = i;
		intCyclicBuffer.push(a);
	}
	intCyclicBuffer.clear();
	_assert(intCyclicBuffer.len() == 0);


	//---- Test Mixed push/pop ----
	std::list<int> backlog;
	int count = 0;
	for (int iteration = 0; iteration < 5; iteration++)
	{
		for (int i=0; i < (BUFFER_SIZE / 3); i++)
		{
			a = i * (iteration+i);
			intCyclicBuffer.push(a);
			backlog.push_front(a);

			count++;
		}
		for (int i=0; i < (BUFFER_SIZE / 3); i++)
		{
			a = intCyclicBuffer.peek();
			int b = intCyclicBuffer.pop();
			int e = backlog.back();

			_assert(a == b);
			_assert(a == e);
			backlog.pop_back();		//Extract from the list
			count--;
		}
	}
	_assert(count == intCyclicBuffer.len());

	//Test Underflow
	bool underflowCaught = false;
	try
	{
		for (int i=0; i <5; i++)
		{
			intCyclicBuffer.push(1);
		}
		for (int i=0; i < 6; i++)
		{
			intCyclicBuffer.pop();
		}
	} catch (const std::underflow_error&)
	{
		underflowCaught = true;
	}
	_assert(true == underflowCaught);



	//Test Overflow
	bool overflowCaught = false;
	try
	{
		for (int i=0; i <intCyclicBuffer.size() + 1; i++)
		{
			intCyclicBuffer.push(1);
		}
	} catch (const std::overflow_error&)
	{
		overflowCaught = true;
	}
	_assert(true == overflowCaught);


	return true;
}

int main() {

	cout << "running tests (reminder - replace with CppUTest/GTest)\n\n";


	if (testSimpleBuffer() == true)
		cout << "testSimpleBuffer PASSED";
	else
		cout << "testSimpleBuffer FAILED";


	return 0;
}
