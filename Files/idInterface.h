#pragma once
#include <mutex>
typedef int id;

class IdInterface
{
	::id id;
protected:
	::id generateId() {
		static std::mutex mtx;
		static ::id genId = 0;
		std::lock_guard<std::mutex> lock(mtx);
		return ++genId;
	}

public:
	::id getId() const {
		return id;
	}
	void setId(::id id) {
		this->id = id;
	}

};

