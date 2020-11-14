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

template<typename T>
class ThreadSafeQueue : protected std::queue<T> {
public:
	using size_type = typename std::queue<T>::size_type;

	ThreadSafeQueue();
	~ThreadSafeQueue() = default;

	bool empty() const;
	size_type size() const;
	bool pop(T& t);
	void push(const T& t);
	void push(T&& t);
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
