/*
 * SimpleCyclicBuffer.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Lior Timor
 *
 *  Feel free to distribute this code and use it where ever you wish.
 *  I take no responsibility for the code, its effects or any outcome from using it.
 *
 */

#ifndef SIMPLECYCLICBUFFER_H_
#define SIMPLECYCLICBUFFER_H_

#include <array>
#include <exception>

template <class T, int bufferSize>
class SimpleCyclicBuffer
{
	private:
		std::array<T, bufferSize> _cycBuffer;
		int _head = 0;
		int _tail = 0;
		int _count = 0;

		/// Advances a head/tail pointer
		static inline void advancePointer(int &p)
		{
			p = (p + 1) % bufferSize;
		}

	public:
		SimpleCyclicBuffer() = default;

		/// Clears the buffer
		void clear()
		{
			_head = _tail = _count = 0;
		}


		/// Pushes an item into the cyclic buffer using the move semantics.
		bool push(T&& item)
		{
			if (bufferSize < (_count + 1))
				throw std::overflow_error("Buffer overflow") ;

			_cycBuffer[_head] = std::move(item);
			advancePointer(_head);
			_count++;
			return true;
		}

		/// Pushes an item into the cyclic buffer (invoking copy).
		bool push(const T& item)
		{
			T temp = item;

			return push(std::move(temp));
		}


		/// Pops an item from the buffer using the move semantics
		T& pop()
		{
			if (-1 == (_count - 1))
				throw std::underflow_error("Buffer is empty") ;

			const auto old_tail = _tail;
			advancePointer(_tail);
			_count--;

			return _cycBuffer[old_tail];
		}


		/// Peeks into the next item on the list
		const T& peek()
		{
			if (0 == _count)
				throw std::underflow_error("Buffer is empty") ;

			return _cycBuffer[_tail];
		}

		/// Returns the number of items in the list
		int len() { return _count; }

		/// Returns the total size of the buffer (in number of items)
		constexpr int size() { return bufferSize; }

};




#endif /* SIMPLECYCLICBUFFER_H_ */
