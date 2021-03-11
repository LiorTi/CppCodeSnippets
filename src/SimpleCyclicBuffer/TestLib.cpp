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

class TestFixtureCyclicBuffer : public ::testing::Test
{
	public:
		static constexpr int BUFFER_SIZE = 100;
		std::vector< CyclicBufferInterface<int>* > bufferList;

		TestFixtureCyclicBuffer()
		{
			bufferList = {
						new SimpleCyclicBuffer<int, BUFFER_SIZE>,
						new ProtectedSimpleCyclicBuffer<int, BUFFER_SIZE>
			};
		}

		virtual ~TestFixtureCyclicBuffer()
		{
			for (auto item = bufferList.begin(); item != bufferList.end(); ++item)
			{
				delete(*item);
			}

			bufferList.clear();

		}
};

TEST_F(TestFixtureCyclicBuffer, testLengthFunctions)
{

	for (auto item = bufferList.begin(); item != bufferList.end(); ++item)
	{
		ASSERT_TRUE(*item != NULL);

		ASSERT_TRUE((*item)->size() == 100);
		ASSERT_TRUE((*item)->len() == 0);
	}
}

TEST_F(TestFixtureCyclicBuffer, testPartialInsertByReference)
{
	int a = 0;

	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//---- Test partial insert (add by reference) ----
		for (int i=0; i < 20; i++)
		{
			a = i;
			cycBuff->push(a);
		}
		ASSERT_TRUE(cycBuff->len() == 20);

		for (int i=0; i < 20; i++)
		{
			a = cycBuff->pop();
			ASSERT_TRUE(a == i);
		}
		ASSERT_TRUE(cycBuff->len() == 0);
	}
}


TEST_F(TestFixtureCyclicBuffer, testPartialInsertByMove)
{
	int a = 0;

	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//---- Test partial insert (add by move) ----
		for (int i=0; i < 20; i++)
		{
			a = i;
			cycBuff->push(std::move(a));
		}
		ASSERT_TRUE(cycBuff->len() == 20);
		for (int i=0; i < 20; i++)
		{
			a = cycBuff->pop();
			ASSERT_TRUE(a == i);
		}
		ASSERT_TRUE(cycBuff->len() == 0);

	}
}

TEST_F(TestFixtureCyclicBuffer, testFullInsertByMove)
{
	int a = 0;

	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//---- Test FULL insert (add by move) ----
		for (int i=0; i < BUFFER_SIZE; i++)
		{
			a = i;
			cycBuff->push(std::move(a));
		}
		ASSERT_TRUE(cycBuff->len() == BUFFER_SIZE);
		for (int i=0; i < BUFFER_SIZE; i++)
		{
			a = cycBuff->pop();
			ASSERT_TRUE(a == i);
		}
		ASSERT_TRUE(cycBuff->len() == 0);
	}
}


TEST_F(TestFixtureCyclicBuffer, testPeek)
{
	int a = 0;

	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//---- Test PEEK ----
		for (int i=0; i < BUFFER_SIZE; i++)
		{
			a = i;
			cycBuff->push(a);
		}
		ASSERT_TRUE(cycBuff->len() == BUFFER_SIZE);
		for (int i=0; i < BUFFER_SIZE; i++)
		{
			a = cycBuff->peek();
			ASSERT_TRUE(a == i);
			a = cycBuff->pop();
		}
		ASSERT_TRUE(cycBuff->len() == 0);
	}
}

TEST_F(TestFixtureCyclicBuffer, testClear)
{
	int a = 0;

	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//---- Test Clear ----
		for (int i=0; i < BUFFER_SIZE/2; i++)
		{
			a = i;
			cycBuff->push(a);
		}
		cycBuff->clear();
		ASSERT_TRUE(cycBuff);
	}
}

TEST_F(TestFixtureCyclicBuffer, testMixedPushPop)
{
	int a = 0;

	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//---- Test Mixed push/pop ----
		std::list<int> backlog;
		int count = 0;
		for (int iteration = 0; iteration < 5; iteration++)
		{
			for (int i=0; i < (BUFFER_SIZE / 3); i++)
			{
				a = i * (iteration+i);
				cycBuff->push(a);
				backlog.push_front(a);

				count++;
			}
			for (int i=0; i < (BUFFER_SIZE / 3); i++)
			{
				a = cycBuff->peek();
				int b = cycBuff->pop();
				int e = backlog.back();

				ASSERT_TRUE(a == b);
				ASSERT_TRUE(a == e);
				backlog.pop_back();		//Extract from the list
				count--;
			}
		}
		ASSERT_TRUE(count == cycBuff->len());
	}
}

TEST_F(TestFixtureCyclicBuffer, testUnderflow)
{
	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//Test Underflow
		bool underflowCaught = false;
		try
		{
			for (int i=0; i <5; i++)
			{
				cycBuff->push(1);
			}
			for (int i=0; i < 6; i++)
			{
				cycBuff->pop();
			}
		} catch (const std::underflow_error&)
		{
			underflowCaught = true;
		}

		ASSERT_TRUE(true == underflowCaught);
	}
}

TEST_F(TestFixtureCyclicBuffer, testOverflow)
{
	for (auto iter = bufferList.begin(); iter != bufferList.end(); ++iter)
	{
		auto cycBuff = *iter;

		//Test Overflow
		bool overflowCaught = false;
		try
		{
			for (int i=0; i <cycBuff->size() + 1; i++)
			{
				cycBuff->push(1);
			}
		} catch (const std::overflow_error&)
		{
			overflowCaught = true;
		}
		ASSERT_TRUE(true == overflowCaught);
	}
}

