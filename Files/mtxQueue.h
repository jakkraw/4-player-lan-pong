#pragma once

#include <queue>
#include <mutex>
template <class T>
class MtxQueue {
	std::condition_variable cond;
	std::mutex condMtx;
	std::queue<T> q;
	std::mutex mtx;
	bool active;

public:
	MtxQueue() {
		active = true;
	}

	size_t size() {
		std::lock_guard<std::mutex> lock(mtx);
		return q.size();
	}

	void push(T item) {
		std::unique_lock<std::mutex> lock(mtx);
		q.push(item);
		lock.unlock();
		cond.notify_all();
	}

	void getFrom(MtxQueue<T>& queue) {
		std::lock_guard<std::mutex> lock(queue.mtx);
		getFrom(queue.q);	
	}

	void getFrom(std::queue<T>& queue) {
		std::unique_lock<std::mutex> lock(mtx);
		while (!queue.empty()) {
			q.push(queue.front());
			queue.pop();
		}
		lock.unlock();
		cond.notify_all();
	}

	void passTo(std::queue<T>& queue) {
		std::lock_guard<std::mutex> lock(mtx);
		while (!q.empty()) {
			queue.push(q.front());
			q.pop();
		}
	}

	void condPassTo(std::queue<T>& queue) {
		std::unique_lock<std::mutex> lock(mtx);
		while(q.empty()&&active)cond.wait(lock);
		while (!q.empty()) {
			queue.push(q.front());
			q.pop();
		}
		lock.unlock();
	}

	void pop() {
		std::lock_guard<std::mutex> lock(mtx);
		q.pop();
	}

	T front() {
		std::lock_guard<std::mutex> lock(mtx);
		return q.front();
	}
	void close() {
		active = false;
		cond.notify_all();
	}

	~MtxQueue() {
		std::lock_guard<std::mutex> lock(mtx);
		close();
		while (!q.empty()) {
			delete q.front();
			q.pop();
		}

	}

};