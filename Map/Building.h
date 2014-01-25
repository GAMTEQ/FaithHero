//
//  Building.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-22.
//
//

#ifndef mmo2_Building_h
#define mmo2_Building_h

#include "Base.h"
#include "SpriteBase.h"

class Map;

class Building : public BaseSprite {
    // 构造
public:
	static Building* createSelf(Map* map, const char* textureName, const CCPoint& tilePos) {
		Building* self = new Building;
		if(self && self->initSelf(map, textureName, tilePos)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(Map* map, const char* textureName, const CCPoint& tilePos);
    // 接口
public:
	bool update(double dt) {
		return true;
	}
    // 属性
protected:
    int _tileNum;
};

class Wall : public BaseSprite {
    // 构造
public:
	static Wall* createSelf(Map* map) {
		Wall* self = new Wall;
		if(self && self->initSelf(map)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(Map* map) {
		if(BaseSprite::initSelf(map)) {
			this->initWithSpriteFrameName("11_wall1.png");
			this->setAnchorPoint(ccp(0.5, double(16.0f / 64.0f)));
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

#endif
