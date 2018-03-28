#pragma once

#include "client.h"
#include "introStage.h"
#include "singlePlayerStage.h"
#include "netStageInterface.h"
#include "gameEntityCollection.h"
#include "msg.h"
#include "player.h"
#include "game.h"
#include "gPointersInterface.h"
#include "math.h"
class LanStage :public NetStageInterface,public GPointersInterface,public ActiveInterface
{
	GameEntityCollection ec;
	Player player;

protected:
	
	bool initialize();
	void render();
	void update();
	void collisions();
	void shutdown();

	void processTcpMsg(MsgInterface*msg);
	void processUdpMsg(UdpMsgInterface*msg);

private:
	void recvPlayerPaddleMsgAction(PlayerPaddleMsg* ppmsg);
	void transfer(EntityInfoMsg* msg);
};