/*
 * ProtectedSimpleCyclicBuffer.h
 *
 *  Created on: Jan 29, 2021
 *      Author: Lior Timor
 *
 *      The protected version of the Simple cyclic buffer is a decorator version
 *      of the simple cyclic buffer in which an atomic access is utilized to prevent unmanaged
 *      multi-threaded access to the data structure.
 */

#ifndef PROTECTEDSIMPLECYCLICBUFFER_H_
#define PROTECTEDSIMPLECYCLICBUFFER_H_

#include <thread>
#include <mutex>

#include "SimpleCyclicBuffer.h"


class ScopedLockMutex
{
	protected:
		std::mutex* _mtx;

	public:

		ScopedLockMutex() = delete;

		ScopedLockMutex(std::mutex* mtx)
		{
			//Note: By design not checking null'ity. Exception should be thrown if null.

			_mtx = mtx;
			_mtx->lock();
		}

		virtual ~ScopedLockMutex()
		{
			_mtx->unlock();
		}

};

template <class T, int bufferSize>
class ProtectedSimpleCyclicBuffer : public CyclicBufferInterface<T>
{
	protected:
		SimpleCyclicBuffer<T, bufferSize> _cyclicBffer;
		std::mutex _mutex;

	public:

		virtual ~ProtectedSimpleCyclicBuffer()
		{

		};

		/// Clears the buffer
		void clear()
		{
			ScopedLockMutex autoScopedMutex(&_mutex);
			_cyclicBffer.clear();
		}


		/// Pushes an item into the cyclic buffer using the move semantics.
		bool push(T&& item)
		{
			ScopedLockMutex autoScopedMutex(&_mutex);
			return _cyclicBffer.push(item);
		}

		/// Pushes an item into the cyclic buffer (invoking copy).
		bool push(const T& item)
		{
			ScopedLockMutex autoScopedMutex(&_mutex);
			return _cyclicBffer.push(item);
		}


		/// Pops an item from the buffer using the move semantics
		T& pop()
		{
			ScopedLockMutex autoScopedMutex(&_mutex);
			return _cyclicBffer.pop();
		}


		/// Peeks into the next item on the list
		const T& peek()
		{
			ScopedLockMutex autoScopedMutex(&_mutex);
			return _cyclicBffer.peek();
		}

		/// Returns the number of items in the list
		int len()
		{
			ScopedLockMutex autoScopedMutex(&_mutex);
			return _cyclicBffer.len();
		}

		/// Returns the total size of the buffer (in number of items)
		constexpr int size()
		{
			ScopedLockMutex autoScopedMutex(&_mutex);
			return _cyclicBffer.size();
		}


};




#endif /* PROTECTEDSIMPLECYCLICBUFFER_H_ */
