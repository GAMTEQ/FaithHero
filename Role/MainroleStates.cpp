#include "MainroleStates.h"
#include "Mainrole.h"
#include "Map.h"

///////////////////////////////////////////////////////////////////////////////
// 全局状态
///////////////////////////////////////////////////////////////////////////////

MainroleGlobal* MainroleGlobal::instance() {
	static MainroleGlobal instance;
	return &instance;
}

void MainroleGlobal::enter(Mainrole* stateOwner) {
	return;
}

void MainroleGlobal::execute(Mainrole* stateOwner, double dt) {
	if(stateOwner->getHealth() <= 0.0) { // 如果已经死了,则不再处理
		stateOwner->_map->dealDead(stateOwner);
		return;
	}
}

void MainroleGlobal::exit(Mainrole* stateOwner) {
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 站立
///////////////////////////////////////////////////////////////////////////////

MainroleStand* MainroleStand::instance() {
	static MainroleStand instance;
	return &instance;
}

void MainroleStand::enter(Mainrole* stateOwner) {
	stateOwner->setTargetSprite(NULL);
	stateOwner->setTarget(INVALID_POINT);
	stateOwner->setState(eMainroleStateStand);
}

void MainroleStand::execute(Mainrole* stateOwner, double dt) {
	stateOwner->runActionStand(dt);
}

void MainroleStand::exit(Mainrole* stateOwner) {
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 行走
///////////////////////////////////////////////////////////////////////////////

MainroleWalk* MainroleWalk::instance() {
	static MainroleWalk instance;
	return &instance;
}

void MainroleWalk::enter(Mainrole* stateOwner) {
	stateOwner->setTargetSprite(NULL);
	stateOwner->setState(eMainroleStateWalk);
}

void MainroleWalk::execute(Mainrole* stateOwner, double dt) {
	TETraceResult result = stateOwner->naviByPath(dt);
    stateOwner->cameraFollowMainrole(); //!!! 让镜头跟随主角移动
	
    switch(result) {
	case eTraceResultFailure:
		return stateOwner->getStateMachine()->changeState(MainroleStand::instance());
	case eTraceResultHalfway:
		return;
	case eTraceResultSuccess:
		return stateOwner->getStateMachine()->changeState(MainroleStand::instance());
	}
}

void MainroleWalk::exit(Mainrole* stateOwner) {
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 追击
///////////////////////////////////////////////////////////////////////////////

MainrolePursuit* MainrolePursuit::instance() {
	static MainrolePursuit instance;
	return &instance;
}

void MainrolePursuit::enter(Mainrole* stateOwner) {
	PathFinder* targetRole = stateOwner->getTargetSprite();
	if(!targetRole || targetRole->getDead()) return;
	stateOwner->setState(eMainroleStatePursuit);
	stateOwner->setTarget(targetRole->getPosition());
}

void MainrolePursuit::execute(Mainrole* stateOwner, double dt) {
	if(!stateOwner->getTargetSprite() || stateOwner->getTargetSprite()->getDead()) { // 如果敌人已经死亡
		return stateOwner->getStateMachine()->changeState(MainroleStand::instance());	
	}
	stateOwner->detectEnemyOval(stateOwner->getTargetSprite()); // 检测敌人是否在攻击范围
	if(stateOwner->getIsRoleInAttack()) { // 如果在攻击范围则进入攻击状态
		return stateOwner->getStateMachine()->changeState(MainroleAttack::instance());
	}
	TETraceResult result = stateOwner->naviByPath(dt); // 如果没在攻击范围,则继续寻路
	switch(result) {
	case eTraceResultFailure:
		return stateOwner->getStateMachine()->changeState(MainroleStand::instance());
	case eTraceResultHalfway:
		return;
	case eTraceResultSuccess:
		return stateOwner->setTarget(stateOwner->_map->getPathFinder()->getPosition());
	}
}

void MainrolePursuit::exit(Mainrole* stateOwner) {
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 攻击
///////////////////////////////////////////////////////////////////////////////

MainroleAttack* MainroleAttack::instance() {
	static MainroleAttack instance;
	return &instance;
}

void MainroleAttack::enter(Mainrole* stateOwner) {
	PathFinder* targetRole = stateOwner->getTargetSprite();
	if(!targetRole || targetRole->getDead()) return;
	stateOwner->setState(eMainroleStateAttack);
	stateOwner->setTarget(INVALID_POINT);
}

void MainroleAttack::execute(Mainrole* stateOwner, double dt) {
	if(!stateOwner->getTargetSprite() || stateOwner->getTargetSprite()->getDead()) { // 如果敌人已经死亡
		return stateOwner->getStateMachine()->changeState(MainroleStand::instance());	
	}
	stateOwner->detectEnemyOval(stateOwner->getTargetSprite()); // 检测敌人是否在攻击范围
	if(stateOwner->getIsRoleInAttack()) { // 如果敌人还在攻击范围
        CCPoint toTarget = stateOwner->getTargetSprite()->getPosition() - stateOwner->getPosition();
		stateOwner->setDirect(stateOwner->getClosestDirect(toTarget.normalize()));
		stateOwner->runActionFight(dt);
	}
	else { // 如果敌人不在攻击范围
		return stateOwner->getStateMachine()->changeState(MainrolePursuit::instance());	
	}
}

void MainroleAttack::exit(Mainrole* stateOwner) {
	return;
}
