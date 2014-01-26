#ifndef _ARROW_FIRE_BOMB__H_
#define _ARROW_FIRE_BOMB__H_

#include "Base.h"
#include "SpriteBase.h"
#include "ActionAnimation.h"
#include "Arrow.h"

class Map;
class PathFinder;

class ArrowFireBomb : public Arrow {
// 构造
public:
	static ArrowFireBomb* createSelf(Map* map, const CCPoint& startPos, PathFinder* targetSprite) {
		ArrowFireBomb* self = new ArrowFireBomb;
		if(self && self->initSelf(map, startPos, targetSprite)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
	~ArrowFireBomb();
protected:
	bool initSelf(Map* map, const CCPoint& startPos, PathFinder* targetSprite);
// 接口
public:
// 工具
protected:
	virtual void rotateToVector(CCNode* node, const CCPoint& vec);
	virtual void initDisplay();
	virtual void reachTarget();
// 属性
protected:
    CCParticleSystem* _jet;
};

#endif // _ARROW_FIRE_BOMB__H_
