#include "Arrow.h"
#include "Map.h"
#include "PathFinder.h"

bool Arrow::initSelf(Map* map, const CCPoint& startPos, PathFinder* targetSprite) {
	if(BaseSprite::initSelf(map)) {
		_startPos = startPos;
		CCLog("Arrow::initSelf startPos %f %f", startPos.x, startPos.y);
		_targetSprite = targetSprite;
		_speed = 200.0;
		_attack = 15.0;
		setScale(2);
		// 如果目标已经死亡,则不生成箭
		if(!_targetSprite || _targetSprite->getDead())
			return false;
		initDisplay();
		setAnchorPoint(ccp(0.5, 0.5));
		Arrow::setPosition(_startPos);
		Arrow::rotateToVector(this, targetSprite->getAttackPosition() - startPos);
		// 在map中注册箭
		_map->registerEffect(this);
		return true;
	}
	return false;
}

bool Arrow::update(double dt) {
	// 如果目标已经死亡,则箭消失
	if(!_targetSprite || _targetSprite->getDead()) {
		CCLog("Arrow::update target die");
		_map->dealDeadEffect(this);
		return true;
	}
	// 如果箭已射到目标,则箭消失,目标掉血
	if(_targetSprite->getAttackPosition().getDistanceSq(this->getPosition()) < 3) { // !!! 很重要,3才不会出BUG
		CCLog("Arrow::update end pos %f %f", this->getPosition().x, this->getPosition().y);
		reachTarget();
		return true;
	}
	//CCLog("Arrow::update rotateToVector");
	// 调整箭的方向并移动
	CCPoint toTarget = _targetSprite->getAttackPosition() - this->getPosition();
	rotateToVector(this,  toTarget);
	this->setPosition(getPosition() + toTarget.normalize() * _speed * dt);
	return true;
}

void Arrow::rotateToVector(CCNode* node, const CCPoint& vec) {
	// setRotation(angle) 其中angle为角度不是弧度。正数为顺时针旋转，负数为逆时针旋转。
	node->setRotation((-1.0) * vec.getAngle() * 180.0 / 3.1416);
	//CCLog("atan2f_angle %f, toDegree %f", vec.getAngle(), (180.0) * vec.getAngle());
}
void Arrow::initDisplay() {
	CCSpriteFrame* oriFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("01_fight1_arrow_00.png");
	this->setDisplayFrame(oriFrame);	
}
void Arrow::reachTarget() {
	_targetSprite->beingAttack(_attack);
	CCLog("Arrow::update hit targetHealth %f", _targetSprite->getHealth());
	_map->dealDeadEffect(this);	
}

