/*
 * ThreadSafeQueue.h
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020 Sven Rieper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SRC_SUPPORT_THREADSAFEQUEUE_H_
#define SRC_SUPPORT_THREADSAFEQUEUE_H_

#include <queue>
#include <mutex>

namespace PhotoLibrary {
namespace Support {

/**
 * Thread safe interface for an std::queue.
 *
 * @tparam T type of the Elements; \c T needs to be movable
 *
 * \todo add emplace()?
 */
template<typename T>
class ThreadSafeQueue : private std::queue<T> {
public:
	using size_type = typename std::queue<T>::size_type;

	/**
	 * Creates an empty queue.
	 */
	ThreadSafeQueue();
	~ThreadSafeQueue() = default;
	//no default moving or copying (not thread safe)
	ThreadSafeQueue(const ThreadSafeQueue&) = delete;
	ThreadSafeQueue(ThreadSafeQueue&&) = delete;
	ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
	ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

	/**
	 * Whether the queue is empty.
	 *
	 * @retval true if the queue is empty
	 * @retval false otherwise
	 */
	bool empty() const;

	/**
	 * Number of elements in the queue.
	 *
	 * @return number of elements in the queue
	 */
	size_type size() const;

	/**
	 * Retrieve the first element from the queue.
	 *
	 * Get the first element and delete it from the queue.
	 *
	 * @param[out] t first element of the queue
	 * @retval true if an element could be retrieved
	 * @retval false if the queue was empty
	 */
	bool pop(T& t);

	/**
	 * Add an element to the queue.
	 *
	 * Copies an element to the end of the queue.
	 *
	 * @param t item to copy to the queue
	 */
	void push(const T& t);

	/**
	 * \copybrief push(const T&)
	 *
	 * Moves an element to the end of the queue.
	 *
	 * @param t item to move to the end of the queue
	 */
	void push(T&& t);

	/**
	 * Empty the queue.
	 *
	 * Removes all elements from the queue.
	 * \todo add option to give a hint whether or not to keep reserve?
	 */
	void clear();

private:
	mutable std::mutex queue_mutex;
};


//implementation
template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue() :
		std::queue<T>() {}

template<typename T>
bool ThreadSafeQueue<T>::empty() const {
	std::unique_lock<std::mutex> lck {queue_mutex};
	return std::queue<T>::empty();
}

template<typename T>
typename ThreadSafeQueue<T>::size_type ThreadSafeQueue<T>::size() const {
	std::unique_lock<std::mutex> lck {queue_mutex};
	return std::queue<T>::size();
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T& t) {
	if(empty())
		return false;
	std::unique_lock<std::mutex> lck {queue_mutex};
	t = std::move(std::queue<T>::front());
	std::queue<T>::pop();
	return true;
}

template<typename T>
void ThreadSafeQueue<T>::push(const T& t) {
	std::unique_lock<std::mutex> lck {queue_mutex};
	std::queue<T>::push(t);
}

template<typename T>
void ThreadSafeQueue<T>::push(T&& t) {
	std::unique_lock<std::mutex> lck {queue_mutex};
	std::queue<T>::push(std::move(t));
}

template<typename T>
void ThreadSafeQueue<T>::clear() {
	std::unique_lock<std::mutex> lck {queue_mutex};
	std::queue<T> temp;
	std::queue<T>::swap(temp);
}

} /* namespace Support */
} /* namespace PhotoLibrary */

#endif /* SRC_SUPPORT_THREADSAFEQUEUE_H_ */
