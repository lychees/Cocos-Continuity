#include "GridLayer.h"
#include "ConstValue.h"

USING_NS_CC;

bool GridLayer::init() {
	if (!Layer::init()) {
		return false;
	}
	
	return true;
}

void GridLayer::setGridPosition(int x, int y) {
	this->_gridX = x;
	this->_gridY = y;
	y = GRID_SIZE - y - 1;
	this->setPosition(x * GRID_WIDTH, y * GRID_HEIGHT);
}

void GridLayer::initContent(int index) {
	this->setContentSize(Size(GRID_WIDTH, GRID_HEIGHT));
	this->setAnchorPoint(ccp(0, 0));
	this->_index = index;
	
	float minX = int(index / GRID_SIZE) * GRID_WIDTH;
	float maxX = minX + GRID_WIDTH;
	float minY = int(index % GRID_SIZE) * GRID_HEIGHT;
	float maxY = minY + GRID_HEIGHT;
	this->background = Sprite::create("map/map.png", Rect(minX, minY, GRID_WIDTH, GRID_HEIGHT));
	this->background->setPosition(GRID_WIDTH / 2, GRID_HEIGHT / 2);
	this->addChild(this->background);

	minY = GRID_HEIGHT * GRID_SIZE - maxY;
	maxY = minY + GRID_HEIGHT;

	this->_map = TMXTiledMap::create("map/map.tmx");

	auto objectGroup = this->_map->objectGroupNamed("block")->getObjects();
	for (auto& obj : objectGroup) {
		ValueMap& dict = obj.asValueMap();
		float x1 = dict["x"].asFloat();
		float y1 = dict["y"].asFloat();
		float x2 = x1 + dict["width"].asFloat();
		float y2 = y1 + dict["height"].asFloat();
		x1 = std::max(x1, minX);
		y1 = std::max(y1, minY);
		x2 = std::min(x2, maxX);
		y2 = std::min(y2, maxY);

		float x = x1 - minX, y = y1 - minY;
		float width = x2 - x1, height = y2 - y1;
		CCLOG("%d %f %f\n", index, x, y);
		if (width > 0 && height > 0) {
			auto body = PhysicsBody::createBox(Size(width, height), PHYSICSBODY_MATERIAL_BLOCK);
			body->setCategoryBitmask(ColliderTypeBlock);
			body->setCollisionBitmask(ColliderTypePlayer);
			body->setContactTestBitmask(ColliderTypePlayer);
			body->setDynamic(false);
			body->setLinearDamping(0.0f);

			auto edgeNode = Sprite::create();
			edgeNode->setPosition(Point(x + width / 2, y + height / 2));
			edgeNode->setPhysicsBody(body);
			this->addChild(edgeNode);
		}
	}
}

bool GridLayer::findObjPosition(const char *name, cocos2d::Vec2 &res) {
	auto objectGroup = this->_map->objectGroupNamed(name)->getObjects();
	if (objectGroup.size() > 0) {
		auto dict = objectGroup[0].asValueMap();
		float x = dict["x"].asFloat();
		float y = dict["y"].asFloat();
		res = Vec2(x, y);
		return true;
	}
	else
		return false;
}

void GridLayer::moveGridPosition(int dir) {
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, 1, -1 };
	float posX = this->getPositionX() + dx[dir] * GRID_WIDTH;
	float posY = this->getPositionY() - dy[dir] * GRID_HEIGHT;
	auto moveTo = MoveTo::create(0.3, Vec2(posX, posY));
	this->runAction(moveTo);
	this->_gridX += dx[dir];
	this->_gridY += dy[dir];
}