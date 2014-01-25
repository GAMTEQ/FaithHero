#ifndef _MINI_MAP__H_
#define _MINI_MAP__H_

#include "Base.h"
#include "SpriteBase.h"

class MiniMap : public CCSprite {
// 构造
public:
	static MiniMap* createSelf(const string& frameName) {
		MiniMap* self = new MiniMap;
		if(self && self->initSelf(frameName)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(const string& frameName) {
		if(CCSprite::init()) {
			this->initWithSpriteFrameName(frameName.c_str());
			this->setAnchorPoint(ccp(0, 0));
			return true;
		}
		return false;
	}
// 接口
public:
	bool update(double dt) {
		return true;
	}
// 属性
protected:
};

#endif // _MINI_MAP__H_
