#pragma once
#include "server.h"
#include "gameEntityCollection.h"
#include "player.h"
#include "gPointersInterface.h"
#include "netStageInterface.h"
#include "activeInterface.h"
#include "simPaddleController.h"
#include <thread>

class Simulation :public NetStageInterface,protected GPointersInterface,public ActiveInterface
{
	std::thread simulationThread;
	GameEntityCollection ec;
	void run();
	void processTcpMsg(MsgInterface*msg);
	void processUdpMsg(UdpMsgInterface *msg);
public:
	bool initialize();
	void shutdown();
	

private:
	void createBoardAction();
	void initializeAction();
	void shutdownAction();
	void sendEntityInfoAction();
	void recvAcquireEntitiesAction(AcquireEntitiesMsg* aemsg);
	void recvCreatePlayerAction(CreatePlayerMsg* cpmsg);
	void recvChatMsgAction(ChatMsg* cmsg);
	void recvDisconnectMsgAction(DisconnectMsg* dmsg);
	void recvControllerInfoMsgAction(ControllerInfoMsg* eimsg);
	void updateGameplay();

};