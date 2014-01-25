#ifndef _SWORDSMAN__H_
#define _SWORDSMAN__H_

#include "../Base.h"
#include "../Map/SpriteBase.h"
#include "PathFinder.h"
#include "StateMachine.h"
#include "SwordsmanStates.h"

class HealthBar : public CCSprite {
// 构造
public:
	static HealthBar* createSelf(float maxHealth, float health) {
		HealthBar* self = new HealthBar;
		if(self && self->initSelf(maxHealth, health)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(float maxHealth, float health) {
		if(init()) {
			_barSize = CCSize(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);
			_health = health;
			_maxHealth = maxHealth;
			drawTexture("blank.png", CCRect(0, 0, _barSize.width, _barSize.height), ccp(0, 0));
			this->setColor(ccc3(0, 0, 0));
			//this->setOpacity(0.1);
			return true;
		}
		return false;
	}
// 接口
public:
    virtual void draw() {
		CCSprite::draw();
		glLineWidth(1);
		if(_health <= 0)
			return;
		ccDrawSolidRect(
			ccp(0, 0), 
			ccp(_barSize.width * _health / _maxHealth, _barSize.height), 
			ccc4f(1.0f, 0.0f, 0.0f, 1));
	}
	bool update(double dt) {
		return true;
	}
protected:
	void drawTexture(const char *pszFilename, const CCRect& textureRect, const CCPoint& anchorPoint) {
		CCImage* blankImage = new CCImage; // 加载图片
		blankImage->initWithImageFile(pszFilename);
		blankImage->autorelease();
		CCTexture2D* blankTexture2D = new CCTexture2D; // 用图片制作纹理
		blankTexture2D->initWithImage(blankImage);
		blankTexture2D->autorelease();
		this->setTexture(blankTexture2D);
		this->setTextureRect(textureRect);
		this->setAnchorPoint(anchorPoint);
	}
// 属性
protected:
	CC_SYNTHESIZE(float, _health, Health);
	CC_SYNTHESIZE(float, _maxHealth, MaxHealth);
	CCSize _barSize;
};

class Swordsman : public PathFinder {
// 构造
public:
	static Swordsman* createSelf(Map* map) {
		Swordsman* self = new Swordsman;
        CCLog("====1swordman create trace retain count %d", self->retainCount());
		if(self && self->initSelf(map)) {
            CCLog("====2swordman create trace retain count %d", self->retainCount());
			self->autorelease();
            CCLog("====3swordman create trace retain count %d", self->retainCount());
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
	~Swordsman() {
		CC_SAFE_RELEASE(_stateMachine);
		CC_SAFE_RELEASE(_damageNumAction);
	}
protected:
	bool initSelf(Map* map);
// 接口
public:
    virtual void handleServerMsg(MsgBase* msgBase);
	virtual bool update(double dt) {
		_stateMachine->update(dt);
		return true;
	}
	void detectEnemyOval(PathFinder* pathFinder) {
		_isRoleInAttack = _attackOval.isInOval(this->getPosition(), pathFinder->getPosition());
		_isRoleInDetect = _detectOval.isInOval(this->getPosition(), pathFinder->getPosition());
	}
	virtual void setHealth(float health) {
		PathFinder::setHealth(health);
		_healthBar->setHealth(health);
	}
	void showHealthBar() {
		if(!_healthBar) return;
		_healthBar->setVisible(true);
	}
	void closeHealthBar() {
		if(!_healthBar) return;
		_healthBar->setVisible(false);	
	}
	void dealDead();
	virtual void beingAttack(float decreaseHealth);
// 工具
protected:
	virtual void initSpecialProperty();
	void callbackRecycleDamageNum(CCNode* actionOwner);
// 属性
protected:
	CCAction* _damageNumAction;
	HealthBar* _healthBar;
	CC_SYNTHESIZE_PASS_BY_REF(DetectOval, _detectOval, DetectOval);
	CC_SYNTHESIZE(bool, _isRoleInDetect, IsRoleInDetect);
	CC_SYNTHESIZE(TEEnemyState, _state, State);
	CC_SYNTHESIZE_READONLY(StateMachine<Swordsman>*, _stateMachine, StateMachine); // 状态机
};

#endif // _SWORDSMAN__H_

