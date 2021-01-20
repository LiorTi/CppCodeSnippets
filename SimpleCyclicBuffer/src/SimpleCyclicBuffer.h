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

template <class T, int bufferSize>
class SimpleCyclicBuffer
{
	private:
		std::array<T, bufferSize> _cycBuffer;
		int _head = 0;
		int _tail = 0;

	public:
		SimpleCyclicBuffer() = default;

		/// Pushes an item into the cyclic buffer using the move semantics.
		bool push(T&& item);
		/// Pushes an item into the cyclic buffer (invoking copy).
		bool push(T& item);
		/// Pops an item from the buffer using the move semantics
		T&& pop();
		/// Peeks into the next item on the list
		const bool peek(T&& item);
		/// Peeks into the next item on the list (invokes a copy)
		const bool peek(T& item);

		/// Returns the number of items in the list
		int len() { return ((bufferSize) + (_head - _tail)) % bufferSize; }

		/// Returns the total size of the buffer (in number of items)
		constexpr int size() { return bufferSize; }

};




#endif /* SIMPLECYCLICBUFFER_H_ */
