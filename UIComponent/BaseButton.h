#ifndef _BASE_BUTTON__H_
#define _BASE_BUTTON__H_

#include "Base.h"
#include "SpriteBase.h"
#include "PanelComponent.h"

class BaseButton : public PanelComponent {
// 构造
public:
	static BaseButton* createSelf(PanelLayer* panelLayer, const string& frameName) {
		BaseButton* self = new BaseButton;
		if(self && self->initSelf(panelLayer, frameName)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(PanelLayer* panelLayer, const string& frameName) {
		if(PanelComponent::initSelf(panelLayer)) {
            //this->initWithFile(frameName.c_str()); //!!! 不能用这个，FRAME_NAME和FILE_NAME只能选一个！！！
			this->initWithSpriteFrameName(frameName.c_str());
			this->setAnchorPoint(ccp(0.5, 0.5));
			return true;
		}
		return false;
	}
// 接口
public:
// 属性
protected:
};

class SkillButton : public BaseButton {
// 构造
public:
	static SkillButton* createSelf(PanelLayer* panelLayer, const string& frameName) {
		SkillButton* self = new SkillButton;
		if(self && self->initSelf(panelLayer, frameName)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(PanelLayer* panelLayer, const string& frameName) {
		if(BaseButton::initSelf(panelLayer, frameName)) {
			return true;
		}
		return false;
	}
// 接口
public:
	virtual void singleTouch(const CCPoint& pos);
// 属性
protected:
};

#endif // _BASE_BUTTON__H_
