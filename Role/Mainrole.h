#ifndef _MAINROLE__H_
#define _MAINROLE__H_

#include "Base.h"
#include "PathFinder.h"
#include "StateMachine.h"
#include "MainroleStates.h"

class Mainrole : public PathFinder {
// 构造
public:
	static Mainrole* createSelf(Map* map) {
		Mainrole* self = new Mainrole;
		if(self && self->initSelf(map)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
	~Mainrole() {
		CC_SAFE_RELEASE(_stateMachine);
	}
protected:
	bool initSelf(Map* map);
// 接口
public:
	virtual bool update(double dt);
	void detectEnemyOval(PathFinder* pathFinder) {
		_isRoleInAttack = _attackOval.isInOval(this->getPosition(), pathFinder->getPosition());
	}
	virtual void runActionFight(float dt);
	virtual void setHealth(float health);
	virtual void beingAttack(float decreaseHealth) {}
    
    virtual void handleServerMsg(MsgBase* msgBase);
    void cameraFollowMainrole();
// 工具
protected:
	virtual void initSpecialProperty();
    void createOutlineSprite();
// 属性
protected:
	CC_SYNTHESIZE(float, _mage, Mage);
	CC_SYNTHESIZE(TEMainroleState, _state, State);
	CC_SYNTHESIZE_READONLY(StateMachine<Mainrole>*, _stateMachine, StateMachine); // 状态机
};

#endif // _MAINROLE__H_

