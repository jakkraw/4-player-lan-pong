#pragma once

class ActiveInterface {
	volatile bool active;
protected:
	void setActive(bool active) {
		this->active = active;
	};
public:

	volatile bool isActive() const {
		return active;
	};
	
	void start() {
		setActive(true);
	}
	void stop() {
		setActive(false);
	}

};

