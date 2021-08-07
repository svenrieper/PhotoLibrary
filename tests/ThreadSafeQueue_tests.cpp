/*
 * ThreadSafeQueue_test.cpp
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2021 Sven Rieper
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

#include "../src/Support/ThreadSafeQueue.h"
#include <catch2/catch.hpp>
#include <vector>
#include <string>
#include <utility>
#include <thread>
#include <algorithm>

namespace PhotoLibrary {
namespace Support {

namespace ThreadSafeQueue_tests {

template<typename T>
void basic_funtionality_test(const std::vector<T>& vec) {
	ThreadSafeQueue<T> queue;

	for(auto& a : vec) {
		queue.push(a);
	}

	REQUIRE(vec.size() == queue.size());

	for(auto& a : vec) {
		T queue_entry {};
		REQUIRE(queue.pop(queue_entry));
		REQUIRE(a == queue_entry);
	}

	REQUIRE(queue.empty());
}

template<typename T, typename Arg0, typename... Args>
void test_emplace(const std::vector<Arg0> v0, const std::vector<Args>&... vecs) {
	ThreadSafeQueue<T> queue;

	const auto n { v0.size() };
	for(int i=0; i<n; ++i) {
		queue.emplace(v0[i], vecs[i]...);
	}

	REQUIRE(v0.size() == queue.size());

	for(int i=0; i<n; ++i) {
		T queue_entry {};
		REQUIRE(queue.pop(queue_entry));
		REQUIRE(queue_entry == T({v0[i], vecs[i]...}));
	}

	REQUIRE(queue.empty());
}

template<typename T>
void test_thread_safety(ThreadSafeQueue<T>* in, ThreadSafeQueue<T>* out) {
	T value {};
	while(in->pop(value)) {
		out->push(value);
	}
}

TEST_CASE( "Construction, push, and pop", "[support][ThreadSafeQueue]" ) {
	SECTION("ThreadSafeQueue<int>") {
		std::vector<int> vec {1, 4, 0, -15, 42, -155, 255, 77};
		basic_funtionality_test(vec);
	}

	SECTION("ThreadSafeQueue<std::string>") {
		std::vector<std::string> vec {"One", "2", "42", "Some string", "another string", "some more text", "", "ßäÖ"};
		basic_funtionality_test(vec);
	}
}

TEST_CASE( "Construction, emplace, and pop", "[support][ThreadSafeQueue]" ) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist_int(-100, 100);
	std::uniform_real_distribution<> dist_double(-1000, 1000);

	std::vector<int> vec_int;
	std::vector<double> vec_double;

	for(int i=0; i<50; ++i) {
		vec_int.push_back(dist_int(gen));
		vec_double.push_back(dist_double(gen));
	}

	test_emplace<std::pair<int,double>>(vec_int, vec_double);
}

TEST_CASE( "clear()", "[support][ThreadSafeQueue]" ) {
	std::vector<int> vec {1, 4, 0, -15, 42, -155, 255, 77};
	ThreadSafeQueue<int> queue;

	REQUIRE(queue.empty());

	for(auto i : vec) {
		queue.push(i);
	}

	REQUIRE(queue.size() == vec.size());

	queue.clear();

	REQUIRE(queue.empty());
}

TEST_CASE("Test ThreadSafty", "[support][ThreadSafeQueue]" ) {
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1000000, 1000000);

	std::vector<int> in_vec;
	std::vector<int> out_vec;

	for(int i=0; i<100000; ++i) {
		//in_vec.push_back(dist(gen));
		in_vec.push_back(i);
	}

	ThreadSafeQueue<int> in_queue;
	ThreadSafeQueue<int> out_queue;

	for(auto i : in_vec) {
		in_queue.push(i);
	}

	REQUIRE(in_vec.size() == in_queue.size());

	std::vector<std::thread> threads(128);
	for(auto& t : threads) {
		t = std::thread(test_thread_safety<int>, &in_queue, &out_queue);
	}

	for(auto& t : threads) {
		t.join();
	}

	REQUIRE(in_queue.empty());
	REQUIRE(out_queue.size() == in_vec.size());

	int value{};
	while(out_queue.pop(value)) {
		out_vec.push_back(value);
	}

	std::sort(out_vec.begin(), out_vec.end());

	REQUIRE(in_vec == out_vec);
}

} /* namespece ThreadSafeQueue_tests */

} /* namespace Support */
} /* namespace PhotoLibrary */
