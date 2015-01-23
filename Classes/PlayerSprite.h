#pragma once

#include "cocos2d.h"


#include <spine/spine-cocos2dx.h>

class PlayerSprite : public cocos2d::Sprite {

private:
	spine::SkeletonAnimation* skeletonNode;


public:
	enum class RunState {
		Idle,
		RunLeft,
		RunRight
	};

	//map<string, int> item;



	int face; // 1 left,  right

	bool isJumping;

	virtual bool init();
	CREATE_FUNC(PlayerSprite);

	int timeCount;

	RunState runState;
	float speedX;
	

	int gridX;
	int gridY;

	void addPhysicsBody();

	void turnLeft();
	void turnRight();

	void jump();

	void setVelocity(cocos2d::Vec2 v);
	void moveGridPosition(int dir);

	void update(float dt);
};