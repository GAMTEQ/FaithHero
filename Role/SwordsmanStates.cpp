#include "SwordsmanStates.h"
#include "Swordsman.h"
#include "../Map/Map.h"

///////////////////////////////////////////////////////////////////////////////
// 全局
///////////////////////////////////////////////////////////////////////////////

SwordsmanGlobal* SwordsmanGlobal::instance() {
	static SwordsmanGlobal instance;
	return &instance;
}

void SwordsmanGlobal::enter(Swordsman* stateOwner) {
}

void SwordsmanGlobal::execute(Swordsman* stateOwner, double dt) {
	PathFinder* role = stateOwner->_map->getPathFinder();
	if(!role || role->getDead()) return; // 如果主角死了,就不移动了
	if(stateOwner->getHealth() <= 0.0) { // 如果自己已经死了,则不再处理
		stateOwner->dealDead();
		return;
	}
	stateOwner->detectEnemyOval(role);
}

void SwordsmanGlobal::exit(Swordsman* stateOwner) {
	stateOwner->setTarget(INVALID_POINT);
}

///////////////////////////////////////////////////////////////////////////////
// 等待
///////////////////////////////////////////////////////////////////////////////

SwordsmanWait* SwordsmanWait::instance() {
	static SwordsmanWait instance;
	return &instance;
}

void SwordsmanWait::enter(Swordsman* stateOwner) {
	//CCLog("state enter %s", "SwordsmanWait");
	stateOwner->setState(eEnemyStateWait);
	stateOwner->setTarget(INVALID_POINT);
	stateOwner->setTargetSprite(NULL);
}

void SwordsmanWait::execute(Swordsman* stateOwner, double dt) {
	if(stateOwner->getIsRoleInAttack()) {
		stateOwner->getStateMachine()->changeState(SwordsmanAttack::instance());
	}
	else if(stateOwner->getIsRoleInDetect()) {
		stateOwner->getStateMachine()->changeState(SwordsmanPursuit::instance());	
	}
	else {
		stateOwner->runActionStand(dt);
		return;
	}
}

void SwordsmanWait::exit(Swordsman* stateOwner) {
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 攻击
///////////////////////////////////////////////////////////////////////////////

SwordsmanAttack* SwordsmanAttack::instance() {
	static SwordsmanAttack instance;
	return &instance;
}

void SwordsmanAttack::enter(Swordsman* stateOwner) {
	//CCLog("state enter %s", "SwordsmanAttack");
	PathFinder* targetRole = stateOwner->_map->getPathFinder();
	if(!targetRole || targetRole->getDead()) return;
	stateOwner->setTargetSprite(targetRole);
	stateOwner->setTarget(INVALID_POINT);
	stateOwner->setState(eEnemyStateAttack);
	stateOwner->showHealthBar();
}

void SwordsmanAttack::execute(Swordsman* stateOwner, double dt) {
	if(!stateOwner->getTargetSprite() || stateOwner->getTargetSprite()->getDead()) {
		return stateOwner->getStateMachine()->changeState(SwordsmanWait::instance());	
	}
	if(stateOwner->getIsRoleInAttack()) {
		CCPoint toTarget = stateOwner->getTargetSprite()->getPosition() - stateOwner->getPosition();
		stateOwner->setDirect(stateOwner->getClosestDirect(toTarget.normalize()));
		stateOwner->runActionFight(dt);
	}
	else if(stateOwner->getIsRoleInDetect()) {
		stateOwner->getStateMachine()->changeState(SwordsmanPursuit::instance());	
	}
	else {
		stateOwner->getStateMachine()->changeState(SwordsmanWait::instance());	
	}
}

void SwordsmanAttack::exit(Swordsman* stateOwner) {
	// 在此处添加操作要谨慎,防止全局状态中该对象已死亡 !!!
	stateOwner->closeHealthBar();
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 追赶
///////////////////////////////////////////////////////////////////////////////

SwordsmanPursuit* SwordsmanPursuit::instance() {
	static SwordsmanPursuit instance;
	return &instance;
}

void SwordsmanPursuit::enter(Swordsman* stateOwner) {
	//CCLog("state enter %s", "SwordsmanPursuit");
	PathFinder* targetRole = stateOwner->_map->getPathFinder();
	if(!targetRole || targetRole->getDead()) return;
	stateOwner->setTargetSprite(targetRole);
	stateOwner->setState(eEnemyStatePursuit);
	stateOwner->setTarget(stateOwner->_map->getPathFinder()->getPosition());
	stateOwner->showHealthBar();
}

void SwordsmanPursuit::execute(Swordsman* stateOwner, double dt) {
	if(!stateOwner->getTargetSprite() || stateOwner->getTargetSprite()->getDead()) {
		return stateOwner->getStateMachine()->changeState(SwordsmanWait::instance());	
	}	
	if(stateOwner->getIsRoleInAttack()) {
		return stateOwner->getStateMachine()->changeState(SwordsmanAttack::instance());
	}
	else if(!stateOwner->getIsRoleInDetect()) {
        //!!! 暂时去掉追踪范围
		//return stateOwner->getStateMachine()->changeState(SwordsmanWait::instance());
	}
	// 执行追踪动作
	TETraceResult result = stateOwner->naviByPath(dt);
	switch(result) {
	case eTraceResultFailure:
		return stateOwner->getStateMachine()->changeState(SwordsmanWait::instance());
	case eTraceResultHalfway:
		return;
	case eTraceResultSuccess:
		return stateOwner->setTarget(stateOwner->_map->getPathFinder()->getPosition());
	}
}

void SwordsmanPursuit::exit(Swordsman* stateOwner) {
	stateOwner->closeHealthBar();
	return;
}

