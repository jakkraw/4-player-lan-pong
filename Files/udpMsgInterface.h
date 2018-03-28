#pragma once
#include "msgInterface.h"

struct UdpMsgInterface :public MsgInterface, public IdInterface {

	UdpMsgInterface* copy() {
		UdpMsgInterface* newMsg = (UdpMsgInterface*)malloc(this->size);
		std::memcpy(newMsg, this, this->size);
		return newMsg;
	}
protected:

	UdpMsgInterface(int id, int size) : MsgInterface(size) {
		setId(id);
	};

};
