//============================================================================
// Name        : SimpleCyclicBuffer.cpp
// Author      : Lior Timor
// Version     :
// Copyright   : MIT
// Description : Test SimpleCyclicBuffer - TODO: Need to swap with CppUTest/Gtest
//============================================================================


using namespace std;

#include <iostream>
#include <list>
#include <vector>
#include "SimpleCyclicBuffer.h"
#include "ProtectedSimpleCyclicBuffer.h"

#include <gtest/gtest.h>

//TODO: Swap with a CppUTest
#define _assert(eval_statement) 	if ((eval_statement) == false) return false


TEST(GENERAL, compile_test)
{
	ASSERT_TRUE(true);
}

bool testSimpleBuffer(CyclicBufferInterface<int> *buffer, int bufferSize)
{
	int a = 20;

	_assert(buffer->size() == 100);
	_assert(buffer->len() == 0);

	//---- Test partial insert (add by reference) ----
	for (int i=0; i < 20; i++)
	{
		a = i;
		buffer->push(a);
	}
	_assert(buffer->len() == 20);
	for (int i=0; i < 20; i++)
	{
		a = buffer->pop();
		_assert(a == i);
	}
	_assert(buffer->len() == 0);

	//---- Test partial insert (add by move) ----
	for (int i=0; i < 20; i++)
	{
		a = i;
		buffer->push(std::move(a));
	}
	_assert(buffer->len() == 20);
	for (int i=0; i < 20; i++)
	{
		a = buffer->pop();
		_assert(a == i);
	}
	_assert(buffer->len() == 0);


	//---- Test FULL insert (add by move) ----
	for (int i=0; i < bufferSize; i++)
	{
		a = i;
		buffer->push(std::move(a));
	}
	_assert(buffer->len() == bufferSize);
	for (int i=0; i < bufferSize; i++)
	{
		a = buffer->pop();
		_assert(a == i);
	}
	_assert(buffer->len() == 0);


	//---- Test PEEK ----
	for (int i=0; i < bufferSize; i++)
	{
		a = i;
		buffer->push(a);
	}
	_assert(buffer->len() == bufferSize);
	for (int i=0; i < bufferSize; i++)
	{
		a = buffer->peek();
		_assert(a == i);
		a = buffer->pop();
	}
	_assert(buffer->len() == 0);


	//---- Test Clear ----
	for (int i=0; i < bufferSize/2; i++)
	{
		a = i;
		buffer->push(a);
	}
	buffer->clear();
	_assert(buffer->len() == 0);


	//---- Test Mixed push/pop ----
	std::list<int> backlog;
	int count = 0;
	for (int iteration = 0; iteration < 5; iteration++)
	{
		for (int i=0; i < (bufferSize / 3); i++)
		{
			a = i * (iteration+i);
			buffer->push(a);
			backlog.push_front(a);

			count++;
		}
		for (int i=0; i < (bufferSize / 3); i++)
		{
			a = buffer->peek();
			int b = buffer->pop();
			int e = backlog.back();

			_assert(a == b);
			_assert(a == e);
			backlog.pop_back();		//Extract from the list
			count--;
		}
	}
	_assert(count == buffer->len());

	//Test Underflow
	bool underflowCaught = false;
	try
	{
		for (int i=0; i <5; i++)
		{
			buffer->push(1);
		}
		for (int i=0; i < 6; i++)
		{
			buffer->pop();
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
		for (int i=0; i <buffer->size() + 1; i++)
		{
			buffer->push(1);
		}
	} catch (const std::overflow_error&)
	{
		overflowCaught = true;
	}
	_assert(true == overflowCaught);


	return true;
}


int ___main() {

	cout << "running tests (reminder - replace with CppUTest/GTest)\n\n";

	constexpr int BUFFER_SIZE = 100;

	std::vector< CyclicBufferInterface<int>* > bufferList = {
			new SimpleCyclicBuffer<int, BUFFER_SIZE>,
			new ProtectedSimpleCyclicBuffer<int, BUFFER_SIZE>};


	bool success = true;

	for (CyclicBufferInterface<int>* b : bufferList)
	{
		if (testSimpleBuffer(b, BUFFER_SIZE) != true)
		{
			success = false;
		}
	}

	if (true == success)
		cout << "testSimpleBuffer PASSED";
	else
		cout << "testSimpleBuffer FAILED";


	return 0;
}
