#ifndef _ARROW__H_
#define _ARROW__H_

#include "Base.h"
#include "SpriteBase.h"
#include "ActionAnimation.h"

class Map;
class PathFinder;

class Arrow : public BaseSprite {
// 构造
public:
	static Arrow* createSelf(Map* map, const CCPoint& startPos, PathFinder* targetSprite) {
		Arrow* self = new Arrow;
		if(self && self->initSelf(map, startPos, targetSprite)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
    ~Arrow();
protected:
	bool initSelf(Map* map, const CCPoint& startPos, PathFinder* targetSprite);
// 接口
public:
	virtual bool update(double dt);
	void setPosition(const CCPoint& pos) { // !!! 可以根据需要选择子弹要不要被遮蔽
		CCSprite::setPosition(pos);
	}
// 工具
protected:
	virtual void rotateToVector(CCNode* node, const CCPoint& vec);
	virtual void initDisplay();
	virtual void reachTarget();
// 属性
protected:
	CC_SYNTHESIZE(PathFinder*, _targetSprite, TargetSprite); // 发射目标 不引用计数
	CCPoint _startPos;
	float _speed;
	float _attack;
	CC_SYNTHESIZE(bool, _dead, Dead);
	//ActionAnimationDirect* _moveAnimations;
};

#endif // _ARROW__H_

