#ifndef _ROLE_PANEL__H_
#define _ROLE_PANEL__H_

#include "Base.h"
#include "SpriteBase.h"
#include "PanelComponent.h"

class NameBoard;
class StatusBar;
class BaseButton;

class RolePanel : public PanelComponent {
// 构造
public:
	static RolePanel* createSelf(PanelLayer* panelLayer) {
		RolePanel* self = new RolePanel;
		if(self && self->initSelf(panelLayer)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(PanelLayer* panelLayer);
// 接口
public:
	bool update(double dt) {
		return true;
	}
	void updateLifeAndMage(float life, float mage);
	void setMaxLifeAndMage(float maxLife, float maxMage);
// 属性
protected: // 常量
	CCPoint _portraitRatioPos; // 头像的比例坐标(比例,基于左下角)
	CCPoint _lifebarRatioPos; // 生命条的比例坐标
	CCPoint _magebarRatioPos; // 魔法条的比例坐标
	CCPoint _nameboardRatioPos; // 名字的比例坐标
protected: // 初始化时计算	
	CC_SYNTHESIZE_READONLY(CCSize, _panelSize, PanelSize); // 角色面板的尺寸(角色面板的contentSize)
	CCPoint _portraitPos; // 头像的相对位置(相对坐标,基于左下角)
	CCPoint _lifebarPos; // 生命条的相对位置
	CCPoint _magebarPos; // 魔法条的相对位置
	CCPoint _nameboardPos; // 名字的相对位置
protected: // 组件
	BaseButton* _portrait; // 头像按钮(不引用计数)
	NameBoard* _nameboard; // 名字牌
	StatusBar* _lifebar; // 生命值
	StatusBar* _magebar; // 法力值
};

#endif // _ROLE_PANEL__H_
