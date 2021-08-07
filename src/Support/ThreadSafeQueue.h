/*
 * ThreadSafeQueue.h
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020-2021 Sven Rieper
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
#include <utility>

namespace PhotoLibrary {
namespace Support {

/**
 * Thread safe interface for an std::queue.
 *
 * @tparam T type of the Elements; \c T needs to be MoveAssignable
 * 		and MoveConstructible
 *
 * @throws std::system_error Any method not marked as noexcept
 * 		may throw a std::system_error if the mutex cannot be locked.
 *
 * \todo add unsafe methods
 */
template<typename T>
class ThreadSafeQueue : private std::queue<T> {
public:
	using size_type = typename std::queue<T>::size_type;

	/**
	 * Creates an empty queue.
	 */
	ThreadSafeQueue() noexcept = default;

	/**
	 * @throws noexcept unless ~T() throws
	 */
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
	bool empty() const noexcept;

	/**
	 * Number of elements in the queue.
	 *
	 * @return number of elements in the queue
	 */
	size_type size() const noexcept;

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
	 *
	 * @throws Any exception thrown during allocation or copying T
	 */
	void push(const T& t);

	/**
	 * \copybrief push(const T&)
	 *
	 * Moves an element to the end of the queue.
	 *
	 * @param t item to move to the end of the queue
	 *
	 * @throws Any exception thrown during allocation or moving/copying T
	 */
	void push(T&& t);

	/**
	 * \copybrief push(const T&)
	 *
	 * Create new element at the end of the queue
	 *
	 * @tparam Args... Argument types of the constructor of T
	 * @param args... Arguments of the constructor of T for the 
	 * 		construction of the new element
	 *
	 * @returns Reference to the newly created element at the end of the queue
	 *
	 * @throws Any exception thrown by the constructor if T or the allocator
	 */
	template<typename... Args>
	T& emplace(Args&&... args);

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
bool ThreadSafeQueue<T>::empty() const noexcept {
	return std::queue<T>::empty();
}

template<typename T>
typename ThreadSafeQueue<T>::size_type ThreadSafeQueue<T>::size() const noexcept {
	return std::queue<T>::size();
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T& t) {
	std::unique_lock<std::mutex> lck {queue_mutex};
	if(std::queue<T>::empty())
		return false;
	std::swap(t, std::queue<T>::front());
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
template<typename... Args>
T& ThreadSafeQueue<T>::emplace(Args&&... args) {
	std::unique_lock<std::mutex> lck {queue_mutex};
	return std::queue<T>::emplace(std::forward<Args>(args)...);
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
