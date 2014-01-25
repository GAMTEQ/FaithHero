#ifndef _SKILL_BAR__H_
#define _SKILL_BAR__H_

#include "Base.h"
#include "SpriteBase.h"

class BaseButton;

class SkillBar : public CCSprite {
// 构造
public:
	static SkillBar* createSelf(unsigned buttonCount, float spacing, const CCSize& buttonSize, bool vertical);
	~SkillBar();
protected:
	bool initSelf(unsigned buttonCount, float spacing, const CCSize& buttonSize, bool vertical);
// 接口
public:
	bool update(double dt) {
		return true;
	}
	void dealTargetTouch(const CCPoint& pos);
	bool setButton(BaseButton* newButton, unsigned index);
	bool removeButton(unsigned index);
// 工具
protected:
	CCPoint getCenterPos(unsigned index) {
		float indexf = (float)index;
		float x = _buttonSize.width / 2;
		float y = indexf * (_spacing + _buttonSize.height) + _buttonSize.height / 2;
		return _vertical ? ccp(x, y) : ccp(y, x);
	}
// 属性
protected:
	unsigned _buttonCount; // 按钮数量
	float _spacing; // 按钮间距
	bool _vertical; // 竖直方向是true, 水平方向是false
	CCSize _buttonSize; // 按钮尺寸
	vector<BaseButton*> _buttonVector; // 保存所有按钮
};

#endif // _SKILL_BAR__H_
