#pragma once
#include "idInterface.h"


struct MsgInterface {
	const int size;
	id sender;
	id to;
	MsgInterface* copy() {
		MsgInterface* newMsg = (MsgInterface*)malloc(this->size);
		std::memcpy((void*)newMsg, (void*)this, this->size);
		return newMsg;
	}
protected:
	MsgInterface(int size) :size(size),sender(0),to(0) {};
};


template <int T>
class TextMsg :public MsgInterface {
private:
	char buffer[T];
public:
	TextMsg() : MsgInterface(sizeof(TextMsg<T>)) {}

	TextMsg(std::string string) : TextMsg() {
		strcpy(this->buffer, string.c_str());
	}
	const char* text() {
		char *str = (char*)malloc(strlen(buffer) + 1);
		strcpy(str, buffer);
		return str;
	}

	void set(std::string string) {
		strcpy(this->buffer, string.c_str());
	}

};


inline char * serialize(MsgInterface* msg) {
	return (char*)msg;
}

inline MsgInterface* deserialize(char* buffer) {
	
	MsgInterface* msg = (MsgInterface*)buffer;
	MsgInterface* newMsg = (MsgInterface*)malloc(msg->size);
	std::memcpy(newMsg, msg, msg->size);
	return newMsg;
}
