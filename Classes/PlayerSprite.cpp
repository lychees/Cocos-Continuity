#include "PlayerSprite.h"
#include "MainLayer.h"
#include "ConstValue.h"
#include "cmath"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

#include <spine/spine-cocos2dx.h>

using namespace spine;




bool PlayerSprite::init() {
	if (!Sprite::init()) {
		return false;
	}


	face = 0;
	timeCount = 0; speedX = RUN_SPEED_X;
	runState = RunState::Idle;

	/*ArmatureDataManager::getInstance()->addArmatureFileInfo("SmallEnemy/SmallEnemy.ExportJson");
	auto t = Armature::create("SmallEnemy");
	t->getAnimation()->play("SmallEnemyIdle");	
	addChild(t);*/
	
	skeletonNode = SkeletonAnimation::createWithFile("spine/spineboy.json", "spine/spineboy.atlas", 0.6f);
	skeletonNode->setScale(0.05);
	skeletonNode->setPosition(Vec2(0, -10));

	//skeletonNode->setMix("walk", "jump", 0.2f);
	//skeletonNode->setMix("jump", "run", 0.2f);
	//skeletonNode->setAnimation(0, "run", true);	
	//skeletonNode->addAnimation(0, "walk", true);

	skeletonNode->setAnimation(0, "idle", true);


	//skeletonNode->setAnimation(0, "stand", true);

	//spTrackEntry* jumpEntry = skeletonNode->addAnimation(0, "jump", false, 3);

	/*skeletonNode->setTrackStartListener(jumpEntry, [](int trackIndex) {
		log("jumped!");
	}); */



	addChild(skeletonNode);


	//return true;

	/*
	ArmatureDataManager::getInstance()->addArmatureFileInfo("NewAnimation/NewAnimation.ExportJson");
	auto t = Armature::create("Layer58");
	t->getAnimation()->play("stand");	
	addChild(t);
	*/





	//sprite = Sprite::create("MainScene.csb");

	//sprite->setContentSize(Size(2, 2));
	//sprite = Sprite::create("Texture/Hero1.png");
	//sprite->setScale(0.2f);

	//sprite->setContentSize(Size(2, 2));
		//Texture
	
	addPhysicsBody();

	return true;
}


void PlayerSprite::turnLeft(){

	//CCLOG("%d turn left", face);
	face = 1;

	skeletonNode->runAction(

		/*Action::create(RotateTo(0)));
		)*/

		RotateTo::create(0.3, Vec3(0, -180, 0))

		);

}

void PlayerSprite::turnRight(){

	//CCLOG("%d turn right", face);
	face = 0;

	skeletonNode->runAction(

		/*Action::create(RotateTo(0)));
		)*/

		RotateTo::create(0.3, Vec3(0, 0, 0))
		);

}

void PlayerSprite::jump(){
	isJumping = true;
	setVelocity(Vec2(0, JUMP_SPEED_Y));
	skeletonNode->setAnimation(0, "jump", true);
}

void PlayerSprite::update(float dt) {

	float vy = this->getPhysicsBody()->getVelocity().y;



	if (std::fabs(vy) < 1e-3){
		this->isJumping = false;
	}
	else{
		if (this->isJumping == false){
			isJumping = true;
			skeletonNode->setAnimation(0, "jump", true);
		}
	}

	this->gridX = (int)this->getPositionX() / GRID_WIDTH;
	this->gridY = GRID_SIZE - 1 - int(this->getPositionY() / GRID_HEIGHT);

	timeCount++;
	if (timeCount % 10 != 0)
		return;


	if (this->runState == RunState::RunRight) {
		this->getPhysicsBody()->setVelocity(Vec2(speedX, vy));
		if (!isJumping) skeletonNode->setAnimation(0, "run", true);		
	}
	else if (this->runState == RunState::RunLeft) {
		this->getPhysicsBody()->setVelocity(Vec2(-speedX, vy));
		if (!isJumping) skeletonNode->setAnimation(0, "run", true);
	}
	else if (this->runState == RunState::Idle) {		
		this->getPhysicsBody()->setVelocity(Vec2(0, vy));
		if (!isJumping) skeletonNode->setAnimation(0, "idle", true);
	}
}

void PlayerSprite::setVelocity(Vec2 v) {
	this->getPhysicsBody()->setVelocity(v);
}

void PlayerSprite::addPhysicsBody() {
	PhysicsBody *body = PhysicsBody::createBox(Size(20, 20), PHYSICSBODY_MATERIAL_PLAYER);
	body->setDynamic(true);
	body->setCategoryBitmask(ColliderTypePlayer);
	body->setContactTestBitmask(ColliderTypeBlock);
	body->setRotationEnable(false);
	body->setLinearDamping(0.0f);
	body->setGravityEnable(true);
	this->setPhysicsBody(body);
}

void PlayerSprite::moveGridPosition(int dir) {
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, 1, -1 };
	float posX = this->getPositionX() + dx[dir] * GRID_WIDTH;
	float posY = this->getPositionY() - dy[dir] * GRID_HEIGHT;
	auto moveTo = MoveTo::create(0.3, Vec2(posX, posY));
	this->runAction(moveTo);
	this->gridX += dx[dir];
	this->gridY += dy[dir];
	//this->setPosition(Vec2(posX, posY));
}
