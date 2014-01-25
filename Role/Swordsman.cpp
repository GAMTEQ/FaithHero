#include "Swordsman.h"
#include "Map.h"
#include "ActionAnimation.h"
#include "DamageNumPool.h"

bool Swordsman::initSelf(Map* map) {
	if(PathFinder::initSelf(map)) {
		_attackOval.init(2, 2, map->getTileSize().width, map->getTileSize().height);
		_detectOval.init(10, 10, map->getTileSize().width, map->getTileSize().height);
		_state = eEnemyStateWait;
		_stateMachine = StateMachine<Swordsman>::createStateMachine(this);
		_stateMachine->retain();
		_isRoleInDetect = false;
		// 状态机
		_stateMachine->setGlobalState(SwordsmanGlobal::instance());
		_stateMachine->setPreviousState(SwordsmanWait::instance());
		_stateMachine->setCurrentState(SwordsmanWait::instance());
		_stateMachine->getCurrentState()->enter(this);
		// healthBar
		_healthBar = HealthBar::createSelf(_health, _health);
		_healthBar->setAnchorPoint(ccp(0, 0));
		_healthBar->setPosition(ccp(0, getContentSize().height));
		closeHealthBar();
		this->addChild(_healthBar);
		// action
		_damageNumAction = CCSequence::createWithTwoActions(
			CCMoveBy::create(0.5, ccp(0, 20)), 
			CCCallFuncN::create(this, callfuncN_selector(Swordsman::callbackRecycleDamageNum)));
		_damageNumAction->retain();
		return true;
	}
	return false;
}

void Swordsman::initSpecialProperty() {
	_health = 50;
	_attack = 10;
	_speed = 50;
	// 起始纹理
	// 起始纹理
	this->initWithSpriteFrameName("00_stand_down_00.png");
	this->setAnchorPoint(ccp(0.5, 0.05));
	this->setScale(3);
	// 战斗动作
	const char* fightFrameArray[NUM_DIRECT] = {
		"00_fight1_up_", "00_fight1_rightup_", "00_fight1_right_", "00_fight1_rightdown_",
		"00_fight1_down_", "00_fight1_leftdown_", "00_fight1_left_", "00_fight1_leftup_"};
	_fightAnimations = ActionAnimationDirect::createSelf(
		vector<string>(fightFrameArray, fightFrameArray + NUM_DIRECT), 7, float(1.0 / 5.0));
	_fightAnimations->retain();
	// 走路动作	
	const char* walkFrameArray[NUM_DIRECT] = {
		"00_walk_up_", "00_walk_rightup_", "00_walk_right_", "00_walk_rightdown_",
		"00_walk_down_", "00_walk_leftdown_", "00_walk_left_", "00_walk_leftup_"};
	_walkAnimations = ActionAnimationDirect::createSelf(
		vector<string>(walkFrameArray, walkFrameArray + NUM_DIRECT), 6, float(1.0 / 4.0));
	_walkAnimations->retain();
	// 站立动作
	const char* standFrameArray[NUM_DIRECT] = {
		"00_stand_up_", "00_stand_rightup_", "00_stand_right_", "00_stand_rightdown_",
		"00_stand_down_", "00_stand_leftdown_", "00_stand_left_", "00_stand_leftup_"};
	_standAnimations = ActionAnimationDirect::createSelf(
		vector<string>(standFrameArray, standFrameArray + NUM_DIRECT), 1, float(1.0));
	_standAnimations->retain();
	// 卧倒动作
	const char* laydownFrameArray[NUM_DIRECT] = {
		"00_laydown_up_", "00_laydown_rightup_", "00_laydown_right_", "00_laydown_rightdown_",
		"00_laydown_down_", "00_laydown_leftdown_", "00_laydown_left_", "00_laydown_leftup_"};
	_laydownAnimations = ActionAnimationDirect::createSelf(
		vector<string>(laydownFrameArray, laydownFrameArray + NUM_DIRECT), 1, float(1.0));
	_laydownAnimations->retain();
}

void Swordsman::dealDead() {
	_healthBar->removeFromParent();
	_healthBar = NULL;
	this->_map->dealDead(this);
}

void Swordsman::beingAttack(float decreaseHealth) {
	this->setHealth(this->getHealth() - decreaseHealth);
	CCLabelBMFont* damageNum = _map->getDamageNumPool()->allocate();
    damageNum->setScale(0.3);
	char numStr[16] = ":";
	//_itoa(decreaseHealth, numStr + 1, 10);
	decreaseHealth = 5;
    damageNum->setString(":20");
	damageNum->setPosition(ccp(getContentSize().width / 2, getContentSize().height));
	this->addChild(damageNum);
	damageNum->runAction((CCAction*)_damageNumAction->copy());
	//_map->getDamageNumPool()->recycle(damageNum);
}

void Swordsman::callbackRecycleDamageNum(CCNode* actionOwner) {
	actionOwner->removeFromParent();
	this->_map->getDamageNumPool()->recycle((CCLabelBMFont*)actionOwner);
}

void Swordsman::handleServerMsg(MsgBase* msgBase) {
    int msgType = msgBase->getMsgType();
    switch(msgType) {
        case eMsgTypePath:
            break;
        case eMsgTypeTarget: {
            MsgTarget* msgTarget = (MsgTarget*)msgBase;
            int targetObjectId = msgTarget->getTarget();
            getStateMachine()->changeState(SwordsmanPursuit::instance());
            break;
        }
        default:
            break;
    }
}



