#ifndef _STATUS_BAR__H_
#define _STATUS_BAR__H_

#include "Base.h"
#include "SpriteBase.h"

class StatusBar : public CCSprite {
// 构造
public:
	static StatusBar* createSelf(const string& frameName, const ccColor4F& barColor) {
		StatusBar* self = new StatusBar;
		if(self && self->initSelf(frameName, barColor)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(const string& frameName, const ccColor4F& barColor) {
		if(CCSprite::init()) {
			_barColor = barColor;
			setValue(0.0f);
			setMaxValue(0.0f);
			this->initWithSpriteFrameName(frameName.c_str());
			this->setAnchorPoint(ccp(0, 0));
			_barSize = this->getContentSize();
			_oriRelPos = ccp(30.0f / 300.0f, 12.0f / 42.0f);
			_endRelPos = ccp(278.0f / 300.0f, 30.0f / 42.0f);
			_oriPos = ccp(_oriRelPos.x * _barSize.width, _oriRelPos.y * _barSize.height);
			_endPos = ccp(_endRelPos.x * _barSize.width, _endRelPos.y * _barSize.height);
			_deltaPos = _endPos - _oriPos;
			return true;
		}
		return false;
	}
// 接口
public:
    virtual void draw() {
		//CCLog("StatusBar draw %f %f", _value, _maxValue);
		CCSprite::draw();
		glLineWidth(1);
		ccDrawSolidRect(
			_oriPos, 
			_oriPos + ccp(_deltaPos.x * _value / _maxValue, _deltaPos.y), 
			_barColor);
	}
	bool update(double dt) {
		return true;
	}
// 属性
protected:
	ccColor4F _barColor; // 颜色
	CCSize _barSize; // 组件尺寸 不变
	CCPoint _oriRelPos; // 起点比例坐标 不变
	CCPoint _endRelPos; // 终点比例坐标 不变
	CCPoint _oriPos; // 起点位置
	CCPoint _endPos; // 终点位置
	CCPoint _deltaPos; // 终点减起点的长度
	CC_SYNTHESIZE(float, _value, Value);
	CC_SYNTHESIZE(float, _maxValue, MaxValue);
};

#endif // _STATUS_BAR__H_
