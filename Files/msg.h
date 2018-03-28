#pragma once
#include "msgInterface.h"
#include "udpMsgInterface.h"

#include "ball.h"
#include "wall.h"
#include "paddle.h"
#include "player.h"
#include "lanPaddleController.h"

#define MSG_ENUM \
MSG(EntityInfo)       \
MSG(ControllerInfo) \
MSG(Disconnect)   \
MSG(AcquireEntities) \
MSG(AddEntity) \
MSG(RemoveEntity) \
MSG(CreatePlayer) \
MSG(PlayerPaddle) \
MSG(Chat) \
MSG(Score) \
MSG(CreateBall)

#define MSG(a) a,
enum MsgType {
	MSG_ENUM
};
#undef MSG


struct UdpMsg : public UdpMsgInterface{
	MsgType type;
protected:
	UdpMsg(MsgType type,int id, int size) : UdpMsgInterface(id,size) {
		this->type = type;
	}
};

struct Msg : public MsgInterface {
	MsgType type;

	const char* toString();
protected:
	Msg(MsgType type, int size) : MsgInterface(size) {
		this->type = type;
	}


};

struct BallMsg : public Msg {

	BallMsg(): Msg(CreateBall,sizeof(BallMsg)){}
};

struct EntityInfoMsg : public UdpMsg
{
	Vector velocity;
	double angularVelocity;
	Vector scale;
	Hitbox hitbox;
	Texture texture;

	EntityInfoMsg(Entity*entity);
	void setEntity(Entity* entity);

};

struct DisconnectMsg : public Msg {
	DisconnectMsg() :Msg(Disconnect,sizeof(DisconnectMsg)) {}
};



class ChatMsg :public Msg {
	TextMsg<256> txmsg;
public:
	ChatMsg(std::string string) :Msg(Chat, sizeof(ChatMsg)), txmsg(string) {
	}
	const char* text() {
		return txmsg.text();
	}
};

class ScoreMsg :public Msg {
public:
	int score;

	ScoreMsg(int score,::id to):Msg(Score,sizeof(ScoreMsg)){
		this->score = score;
		this->to = to;
	}
};

//CLIENT

struct ControllerInfoMsg :public UdpMsg {
	bool up,down,left,right;
	ControllerInfoMsg(LanPaddleController* lpc) : UdpMsg(ControllerInfo, lpc->getEntity()->getId(), sizeof(ControllerInfoMsg)) {
		this->up = lpc->up;
		this->down = lpc->down;
		this->left = lpc->left;
		this->right = lpc->right;
	}
};


struct CreatePlayerMsg : public Msg {
	Vector scale;
	TextMsg<30> surfaceName;
	TextMsg<30> playerName;
	CreatePlayerMsg(Player*);
};

struct AcquireEntitiesMsg : public Msg {
	AcquireEntitiesMsg() : Msg(AcquireEntities, sizeof(AcquireEntitiesMsg)) {}
};


//SERVER
struct AddEntityMsg : public Msg {
private:
	enum Type {
		None,
		Paddle,
		Ball,
		Wall,
	};
public:
	Type type;
	EntityInfoMsg eimsg;
	AddEntityMsg(Entity*entity);
	Entity* createEntity();
};

struct PlayerPaddleMsg :public Msg {
	::id paddleId;
	Player::Position position;
	PlayerPaddleMsg(Player* player, ::id to);
};

struct RemoveEntityMsg :public Msg {
	bool test;
	::id paddle_id;
	RemoveEntityMsg(::id id);
};


