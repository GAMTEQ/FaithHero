#include "Mainrole.h"
#include "Map.h"
#include "ActionAnimation.h"
#include "Arrow.h"
#include "ArrowFireBomb.h"

bool Mainrole::initSelf(Map* map) {
	if(PathFinder::initSelf(map)) {
		_attackOval.init(100, 100, map->getTileSize().width, map->getTileSize().height);
		_state = eMainroleStateStand;
		_stateMachine = StateMachine<Mainrole>::createStateMachine(this);
		_stateMachine->retain();
		// 状态机
		_stateMachine->setGlobalState(MainroleGlobal::instance());
		_stateMachine->setPreviousState(MainroleStand::instance());
		_stateMachine->setCurrentState(MainroleStand::instance());
		_stateMachine->getCurrentState()->enter(this);
		return true;
	}
	return false;
}

void Mainrole::initSpecialProperty() {
	_health = 100;
	_mage = 100;
	_attack = 10;
	_speed = 70;
	// 起始纹理
	this->initWithSpriteFrameName("01_stand_down_00.png");
    this->setOpacity(255);//!!! 必须在initWith函数后面，否则起不到透明效果，怀疑是上面的函数set过了
	this->setAnchorPoint(ccp(0.5, 0.05));
	this->setScale(3);
	// 战斗动作
	const char* fightFrameArray[NUM_DIRECT] = {
		"01_fight1_up_", "01_fight1_rightup_", "01_fight1_right_", "01_fight1_rightdown_",
		"01_fight1_down_", "01_fight1_leftdown_", "01_fight1_left_", "01_fight1_leftup_"};
	_fightAnimations = ActionAnimationDirect::createSelf(
		vector<string>(fightFrameArray, fightFrameArray + NUM_DIRECT), 7, float(1.0 / 5.0));
	_fightAnimations->retain();
	// 走路动作
	const char* walkFrameArray[NUM_DIRECT] = {
		"01_walk_up_", "01_walk_rightup_", "01_walk_right_", "01_walk_rightdown_",
		"01_walk_down_", "01_walk_leftdown_", "01_walk_left_", "01_walk_leftup_"};
	_walkAnimations = ActionAnimationDirect::createSelf(
		vector<string>(walkFrameArray, walkFrameArray + NUM_DIRECT), 8, float(1.0 / 4.0));
	_walkAnimations->retain();
	// 站立动作
	const char* standFrameArray[NUM_DIRECT] = {
		"01_stand_up_", "01_stand_rightup_", "01_stand_right_", "01_stand_rightdown_",
		"01_stand_down_", "01_stand_leftdown_", "01_stand_left_", "01_stand_leftup_"};
	_standAnimations = ActionAnimationDirect::createSelf(
		vector<string>(standFrameArray, standFrameArray + NUM_DIRECT), 1, float(1.0 / 5.0));
	_standAnimations->retain();
	// 卧倒动作
	const char* laydownFrameArray[NUM_DIRECT] = {
		"01_laydown_up_", "01_laydown_rightup_", "01_laydown_right_", "01_laydown_rightdown_",
		"01_laydown_down_", "01_laydown_leftdown_", "01_laydown_left_", "01_laydown_leftup_"};
	_laydownAnimations = ActionAnimationDirect::createSelf(
		vector<string>(laydownFrameArray, laydownFrameArray + NUM_DIRECT), 1, float(1.0 / 5.0));
	_laydownAnimations->retain();
}

void Mainrole::runActionFight(float dt) {
	TEActionState actionState = _fightAnimations->runDirect(this, getDirect(), dt);
	if(actionState == eActionStateEnd) {
		// 动作的末尾自动生成一支箭,自动注册进_map里
		ArrowFireBomb::createSelf(_map, this->getAttackPosition(), this->getTargetSprite());
	}
}

void Mainrole::setHealth(float health) {
	_map->sendEventLifeAndMage(health - getHealth(), 0);
	PathFinder::setHealth(health);
}

void Mainrole::handleServerMsg(MsgBase* msgBase) {
    int msgType = msgBase->getMsgType();
    switch(msgType) {
        case eMsgTypePath: {
            MsgPath* msgPath = (MsgPath*)msgBase;
            setTarget(msgPath->getDest());
            getStateMachine()->changeState(MainroleWalk::instance());
            break;
        }
        case eMsgTypeTarget:
            break;
        default:
            break;
    }
}

void Mainrole::cameraFollowMainrole() {
    _map->movingMapWhenRoleMoving(getPosition(), _map->getMapCenterPos(), 1.0f);
}

bool Mainrole::update(double dt) {
    _stateMachine->update(dt);
    return true;
}

void Mainrole::createOutlineSprite() {
    return;
}




