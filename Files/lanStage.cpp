#include "lanStage.h"



bool LanStage::initialize(){
	start();
	setNCI(network()->getClient());
	graphics()->setBackgroundColor(color::LIGHT_BLUE - 200);
	graphics()->setScreenSize(600, 600);
	send(new AcquireEntitiesMsg());
	return true;

}

void LanStage::render(){
	ec.render();
}


void LanStage::update(){
	recv();
	if (!isActive()) { game()->setStage(new SinglePlayerStage()); return; }
	ec.texUpdate();
	
	if (player.isActive()&&isActive()){
		player.update();
		send(new ControllerInfoMsg((LanPaddleController*)player.getController()));
	}
	if (input()->keyPressed(SDLK_b)) send(new BallMsg());

	if (!isActive()) {
		game()->setStage(new SinglePlayerStage());
		return;
	}

	if (input()->keyPressed(SDLK_RETURN)&&!player.isActive()) {
		send(new CreatePlayerMsg(&player));
	}
	

}

void LanStage::collisions(){
	//ec.collisions();
}

void LanStage::shutdown(){
	send(new DisconnectMsg());
	Sleep(500);
	stop();
	ec.clear();
	((Client*)nci)->shutdown();
}


void LanStage::processTcpMsg(MsgInterface*msg){
	Log("[C][TCP] <- %s [%d]",((Msg*)msg)->toString(), msg->sender);
	switch (((Msg*)msg)->type) {
		case Disconnect:
			shutdown();
			return;		
		case AddEntity:
			transfer(&((AddEntityMsg*)msg)->eimsg);
			ec.add(((AddEntityMsg*)msg)->createEntity());
			return;		
		case RemoveEntity:
			ec.remove(((RemoveEntityMsg*)msg)->paddle_id);
			return;
		case PlayerPaddle: 
			recvPlayerPaddleMsgAction((PlayerPaddleMsg*)msg);
			return;
		case Chat: 
			console()->print(((ChatMsg*)msg)->text());
			return;
		case Score:
			player.points = ((ScoreMsg*)msg)->score;
	}
}

void LanStage::processUdpMsg(UdpMsgInterface * msg){
	switch (((UdpMsg*)msg)->type) {
		case EntityInfo:
			transfer((EntityInfoMsg*)msg);
			((EntityInfoMsg*)msg)->setEntity(ec.get(((EntityInfoMsg*)msg)->getId()));
			break;
	}
}

void LanStage::transfer(EntityInfoMsg* msg){
	Vector ss = graphics()->getScreenSize();
	deg  deg = 0;
	if (player.isActive()) {

		Player::Position pos = player.getPosition();

		if (pos == Player::DOWN) deg =0;

		if (pos == Player::UP) { deg = 180;	}

		if (pos == Player::LEFT) { deg = 270; }

		if (pos == Player::RIGHT) { deg = 90; }

		Texture*tex = &msg->texture;
		msg->velocity = math::rotatePointByAngle(msg->velocity, { 0,0 }, deg);
		Point center = tex->getPosition() + tex->getRotCenter();
		center = math::rotatePointByAngle(center, ss/2.0, deg);
		tex->setPosition(center- tex->getRotCenter());
		tex->setAngle(tex->getAngle() - deg);
	}

}

void LanStage::recvPlayerPaddleMsgAction(PlayerPaddleMsg * ppmsg) {
	player.setPosition(ppmsg->position);
	player.setController(new LanPaddleController());
	player.setPaddle((Paddle*)ec.get(ppmsg->paddleId));
	ec.add(&player);
	player.setPosition(ppmsg->position);
}

