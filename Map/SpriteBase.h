#ifndef _SPRITE_BASE__H_
#define _SPRITE_BASE__H_

#include "Base.h"

class Map;

struct DetectTiles {
	DetectTiles()
		: tile1(ccp(0, 0))
		, tile2(ccp(0, 0))
		, tile3(ccp(0, 0))
	{
	}
	DetectTiles(const CCPoint& pos)
		: tile1(pos)
		, tile2(pos)
		, tile3(pos)
	{
	}
	CCPoint tile1;
	CCPoint tile2;
	CCPoint tile3;
};

struct Detectors {
	
};

class BaseSprite : public CCSprite {
// 构造
public:
	static BaseSprite* createSelf(Map* map) {
		BaseSprite* self = new BaseSprite;
		if(self && self->initSelf(map)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(Map* map) {
		if(init()) {
			_map = map;
			return true;
		}
		return false;
	}
// 接口
public:
	virtual void setPosition(const CCPoint& pos);
	void drawTexture(const char *pszFilename, CCRect& textureRect, const CCPoint& anchorPoint = ccp(0.5, 0.5));
	virtual bool update(double dt) {
		return true;
	}
    void changeZOrder(const CCPoint& tilePos);
// 属性
public:
	Map* _map; // 所有map都不引用计数,只在GameLayer里引用一次
	CC_SYNTHESIZE_READONLY(CCPoint, _tilePos, TilePos); // 当前所处的瓦块坐标
	CC_SYNTHESIZE(CCPoint, _target, Target);
};

#endif  // _SPRITE_BASE__H_