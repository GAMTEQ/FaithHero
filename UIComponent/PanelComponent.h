#ifndef _PANEL_COMPONENT__H_
#define _PANEL_COMPONENT__H_

#include "Base.h"
#include "SpriteBase.h"

class PanelLayer;

class PanelComponent : public CCSprite {
// 构造
public:
	static PanelComponent* createSelf(PanelLayer* panelLayer) {
		PanelComponent* self = new PanelComponent;
		if(self && self->initSelf(panelLayer)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(PanelLayer* panelLayer) {
		if(CCSprite::init()) {
			return true;
		}
		return false;
	}
// 接口
public:
	virtual void singleTouch(const CCPoint& pos) {}
	virtual void movingTouch(const CCPoint& from, const CCPoint& to) {}
	bool update(double dt) {
		return true;
	}
// 属性
protected:
	PanelLayer* _panelLayer;
};

#endif // _PANEL_COMPONENT__H_
