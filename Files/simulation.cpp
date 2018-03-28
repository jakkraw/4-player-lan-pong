#include "simulation.h"


void Simulation::run()
{
	createBoardAction();
	while (isActive())
	{
		time()->manageFPS();
		recv();
		ec.update();
		updateGameplay();
		ec.collisions();
		sendEntityInfoAction();
	}
	shutdownAction();
}

void Simulation::processTcpMsg(MsgInterface * msg)
{
	Log("[S][TCP] <- %s [%d]", ((Msg*)msg)->toString(), msg->sender);

	switch (((Msg*)msg)->type) {

		case Disconnect:
			recvDisconnectMsgAction((DisconnectMsg*)msg);		
			return;
		case AcquireEntities:
			recvAcquireEntitiesAction((AcquireEntitiesMsg*)msg);
			return;
		case CreatePlayer: 
			recvCreatePlayerAction((CreatePlayerMsg*)msg);
			return;
		case Chat:
			recvChatMsgAction((ChatMsg*)msg);
			return;		
		case CreateBall:
			Ball* ball = ec.addBall();
			send(new AddEntityMsg(ball));
			return;
	}

}

void Simulation::processUdpMsg(UdpMsgInterface * msg)
{
	//Log("[S][UDP] <- %s [%d]", name(msg), msg->sender);

//
			recvControllerInfoMsgAction((ControllerInfoMsg*)msg);		
}

bool Simulation::initialize(){
	if (!isActive()) {
		ActiveInterface::start();
		resetPointers();
		setNCI(network()->getServer());
		initializeAction();
		simulationThread = std::thread(&Simulation::run, this);
		return true;
	}
	return false;
}

void Simulation::shutdown(){
	if (isActive()) {
		ActiveInterface::stop();
		simulationThread.join();
		send(new DisconnectMsg());
		Sleep(500);
	}	
}


void Simulation::createBoardAction()
{

	ec.createBoard();

	Ball* ball1 = new Ball();
	ball1->setVelocity(Vector(-30, 71) * 4);
	ball1->setPosition({ 200, 200 });
	ec.add(ball1);

	Ball* ball2 = new Ball();
	ball2->setScale({ 2,2 });
	ball2->setVelocity(Vector(-30, 71) * 4);
	ball2->setPosition({ 400, 350 });
	ec.add(ball2);

}

void Simulation::initializeAction(){
	Pointers ptr;
	ptr.time = new Time();
	ptr.physics = new Physics();
	setLocalPointers(ptr);
	ec.setLocalPointers(ptr);
	
	ptr.time->initialize();
	ptr.time->setMaxFrameRate(120);
	ptr.time->setMinFrameRate(60);
	ptr.time->setLimitFPS(true);
	ptr.physics->initialize();
}

void Simulation::shutdownAction(){
	ec.clear();
	time()->shutdown();
	physics()->shutdown();
	delete time();
	delete physics();
	resetPointers();
}


void Simulation::sendEntityInfoAction()
{
	UdpMsgMap map;
	for (auto &e : ec) {
		map[e.first] = new EntityInfoMsg(e.second);
	}
	send(map);
}

void Simulation::recvAcquireEntitiesAction(AcquireEntitiesMsg* aemsg)
{
	for (auto &e : ec) {
			AddEntityMsg* msg = new AddEntityMsg(e.second);
			msg->to = aemsg->sender;
			send(msg);
	}
}

void Simulation::recvCreatePlayerAction(CreatePlayerMsg * cpmsg)
{
	std::string surfaceName = cpmsg->surfaceName.text();
	std::string name = cpmsg->playerName.text();
	if (ec.pm.size() >= 4) return;

	Player* player = new Player();
	player->setName(name);
	player->getPaddle()->getTexture()->setName(surfaceName);
	player->getPaddle()->setScale(cpmsg->scale);
	player->setController(new SimPaddleController());
	player->setId(cpmsg->sender);
	ec.add(player);
	
	send(new AddEntityMsg(player->getPaddle()));
	send(new PlayerPaddleMsg(player, cpmsg->sender));


	Wall* wall = ec.get((Wall::Type)player->getPosition());
	send(new RemoveEntityMsg(wall->getId()));
	ec.remove(wall->getId());
}

void Simulation::recvChatMsgAction(ChatMsg* cmsg)
{
	std::ostringstream str;
	str << "[" << cmsg->sender << "] " << cmsg->text();
	send(new ChatMsg(str.str().c_str()));
}

void Simulation::recvDisconnectMsgAction(DisconnectMsg * dmsg)
{
	::id sender = dmsg->sender;

	if (Player* player = ec.pm.find(sender)) {
		send(new RemoveEntityMsg(player->getPaddle()->getId()));
		ec.remove(player);
		SAFE_DELETE(player);
	}

	((Server*)nci)->disconnect(dmsg->sender);
}

void Simulation::recvControllerInfoMsgAction(ControllerInfoMsg * eimsg) {

	if (Player* player = ec.pm.find(eimsg->sender)) {
		((SimPaddleController*)player->getController())->set(eimsg);
	}

}

void Simulation::updateGameplay()
{
	if (ec.bm.balls.empty()) {
		std::stringstream ss;
		for (int i = 0; i < 4; i++) {
			Player* player = ec.pm.get((Player::Position)i);
			if(player)
				ss <<" ["<< player->getName() <<"](" << player->points<<")";
		}
		send(new ChatMsg(ss.str().c_str()));
		Ball* ball= ec.addBall();
		send(new AddEntityMsg(ball));
	}

	std::list<std::pair<::id, Ball*>> list = ec.bm.balls;
	for (auto &i : list) {
		Ball* ball = i.second;
		Point pos = ball->getPosition();
		Vector ss = { 600,600 };
		int offset = 50;
		if (pos.x > ss.x+ offset) {
			Player* player = ec.pm.get(Player::RIGHT);
			if (player && ball->lastTouched1 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched1);
				
					if (shooter) {
						shooter->points += 1;
						send(new ScoreMsg(shooter->points, shooter->getId()));
					}
			}
			else if (player && ball->lastTouched2 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched2);
				if (shooter) {
					shooter->points += 1;
					send(new ScoreMsg(shooter->points, shooter->getId()));
				}
			}
			send(new RemoveEntityMsg(ball->getId()));
			ec.remove(ball->getId());
		}

		if (pos.x + offset < 0) {
			Player* player = ec.pm.get(Player::LEFT);
			if (player && ball->lastTouched1 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched1);
				if (shooter) {
					shooter->points += 1;
					send(new ScoreMsg(shooter->points, shooter->getId()));
				}
			}
			else if (player && ball->lastTouched2 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched2);
				if (shooter) {
					shooter->points += 1;
					send(new ScoreMsg(shooter->points, shooter->getId()));
				}
			}
			send(new RemoveEntityMsg(ball->getId()));
			ec.remove(ball->getId());
		}

		if (pos.y + offset < 0) {
			Player* player = ec.pm.get(Player::UP);
			if (player && ball->lastTouched1 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched1);
				if (shooter) {
					shooter->points += 1;
					send(new ScoreMsg(shooter->points, shooter->getId()));
				}
			}
			else if (player && ball->lastTouched2 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched2);
				if (shooter) {
					shooter->points += 1;
					send(new ScoreMsg(shooter->points, shooter->getId()));
				}
			}
			send(new RemoveEntityMsg(ball->getId()));
			ec.remove(ball->getId());
		}


		if (pos.y >ss.y + offset) {
			Player* player = ec.pm.get(Player::DOWN);
			if (player && ball->lastTouched1 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched1);
				if (shooter) {
					shooter->points += 1;
					send(new ScoreMsg(shooter->points, shooter->getId()));
				}
			}
			else if (player && ball->lastTouched2 != player->getPaddle()->getId()) {
				Player* shooter = ec.pm.get(ball->lastTouched2);
				if (shooter) {
					shooter->points += 1;
					send(new ScoreMsg(shooter->points, shooter->getId()));
				}
			}

			send(new RemoveEntityMsg(ball->getId()));
			ec.remove(ball->getId());
			
		}

	}


}

