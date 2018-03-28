#pragma once

#include <unordered_map>
#include <mutex>
#include <condition_variable>
template <typename I,class T>
class MtxUMap {

private:
	std::mutex mtx;
	std::unordered_map<I, T> map;
	std::condition_variable cond;
	bool active;
public:
	MtxUMap() {
		active = true;
	}

	void push(std::pair<I, T> p) {
		std::unique_lock<std::mutex> lock(mtx);
		if (map[p.first]) delete map[p.first];
		map[p.first] = p.second;
		lock.unlock();
		cond.notify_all();
	}

	void passTo(std::unordered_map<I, T>& m) {
		std::lock_guard<std::mutex> locker(mtx);
		for (auto &i : map) {
			m[i.first] = i.second;
		}
		map.clear();
	}

	void condPassTo(std::unordered_map<I, T>& m) {
		std::unique_lock<std::mutex> lock(mtx);
		while(map.empty()&& active) cond.wait(lock);
		for (auto &i : map) {
			m[i.first] = i.second;
		}
		map.clear();
		lock.unlock();
	}

	void getFrom(std::unordered_map<I, T>& m) {
		std::unique_lock<std::mutex> lock(mtx);
		for (auto &i : m) {
			if (map[i.first]) delete map[i.first];
			map[i.first] = i.second;
		}
		m.clear();
		lock.unlock();
		cond.notify_all();
	}

	void getFrom(MtxUMap<I, T>& m) {
		std::lock_guard<std::mutex> lock(m.mtx);
		getFrom(m.map);
	}

	void close() {
		active = false;
		cond.notify_all();
	}

	~MtxUMap() {
		std::lock_guard<std::mutex> lock(mtx);
		close();
		for (auto& t : map) {
			delete t.second;
		}
	}
	

};