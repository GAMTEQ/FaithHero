#ifndef _EVENT_LAYER__H
#define _EVENT_LAYER__H

#include "Base.h"
#include "LayerMediator.h"

// 地图发送给界面:修改生命值和法力值
class EventDecreaseLifeAndMage : public MediatorEvent {
// 构造
public:
	static EventDecreaseLifeAndMage* createSelf(float deltaLife, float deltaMage) {
		EventDecreaseLifeAndMage* self = new EventDecreaseLifeAndMage;
		if(self && self->initSelf(deltaLife, deltaMage)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
// 接口
public:
protected:
	bool initSelf(float deltaLife, float deltaMage) {
		_eventType = eEventMapDecreaseLifeAndMage;
		_deltaLife = deltaLife;
		_deltaMage = deltaMage;
		return true;
	}
// 接口
public:
// 属性
protected:
	CC_SYNTHESIZE_READONLY(float, _deltaLife, DeltaLife); // 变化的生命值
	CC_SYNTHESIZE_READONLY(float, _deltaMage, DeltaMage); // 变化的法力值
};

// 地图发送给界面,修改最大生命值和法力值
class EventSetMaxLifeAndMage : public MediatorEvent {
// 构造
public:
	static EventSetMaxLifeAndMage* createSelf(float maxLife, float maxMage) {
		EventSetMaxLifeAndMage* self = new EventSetMaxLifeAndMage;
		if(self && self->initSelf(maxLife, maxMage)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
// 接口
public:
protected:
	bool initSelf(float maxLife, float maxMage) {
		_eventType = eEventMapSetMaxLifeAndMage;
		_maxLife = maxLife;
		_maxMage = maxMage;
		return true;
	}
// 接口
public:
// 属性
protected:
	CC_SYNTHESIZE_READONLY(float, _maxLife, MaxLife); // 最大生命值
	CC_SYNTHESIZE_READONLY(float, _maxMage, MaxMage); // 最大法力值
};

// 界面发送给地图: 选择治疗技能
class EventSkillHeal : public MediatorEvent {
// 构造
public:
	static EventSkillHeal* createSelf() {
		EventSkillHeal* self = new EventSkillHeal;
		if(self && self->initSelf()) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
// 接口
public:
protected:
	bool initSelf() {
		_eventType = eEventSkillHeal;
		return true;
	}
// 接口
public:
// 属性
protected:
};

// 界面发送给地图: 取消技能
class EventSkillNull : public MediatorEvent {
// 构造
public:
	static EventSkillNull* createSelf() {
		EventSkillNull* self = new EventSkillNull;
		if(self && self->initSelf()) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
// 接口
public:
protected:
	bool initSelf() {
		_eventType = eEventSkillNull;
		return true;
	}
// 接口
public:
// 属性
protected:
};

#endif // _EVENT_LAYER__H
