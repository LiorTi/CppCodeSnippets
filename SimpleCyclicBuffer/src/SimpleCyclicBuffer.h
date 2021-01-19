/*
 * SimpleCyclicBuffer.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Lior Timor
 */

#ifndef SIMPLECYCLICBUFFER_H_
#define SIMPLECYCLICBUFFER_H_

#include <array>

template <class T, int bufferSize>
class SimpleCyclicBuffer
{
	public:
		SimpleCyclicBuffer();

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
		int len();
		/// Returns the size of the buffer (in number of items)
		int size();


	private:
		std::array<T, size> _cycBuffer;
};




#endif /* SIMPLECYCLICBUFFER_H_ */
