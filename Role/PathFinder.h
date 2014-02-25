#ifndef _PATH_FINDER__H_
#define _PATH_FINDER__H_

#include "Base.h"
#include "MsgHandler.h" //???!!!
#include "SpriteBase.h"

class Map;
class Steering;
class ActionAnimationDirect;

// 抽象类
class PathFinder : public BaseSprite, public MsgHandler {
// 构造
public:
	~PathFinder();
protected:
    PathFinder()
    : _outlineSprite(NULL)
    {
    }
	bool initSelf(Map* map);
// 接口
public:
    virtual void handleServerMsg(MsgBase* msgBase);
    virtual int getObjId();
    virtual void setDisplayFrame(CCSpriteFrame *pNewFrame);
	virtual void setPosition(const CCPoint& pos);
    virtual void setAnchorPoint(const CCPoint& anchor);
    virtual void setScale(float fScale);
public:
	virtual bool update(double dt);
	virtual void setTarget(CCPoint target);
	bool isObstacleOnDirect();
	TEDirect getClosestDirect(const CCPoint& direct);
	virtual TETraceResult naviByPath(double dt);
	virtual void runActionFight(float dt);
	void runActionStand(float dt);
	void runActionLaydown(float dt);
	virtual void runActionDead();
	virtual CCPoint getAttackPosition() { // 比如射箭的起始位置和中箭的终止位置
		return getPosition() + ccp(0, 70);
	}
	virtual void beingAttack(float decreaseHealth) = 0;
// 工具
protected:
	// 初始化角色值,站立,行走,战斗动作纹理
	virtual void initSpecialProperty() = 0;
    void createOutlineSprite();
private:
	void calculateShotestPath();
	DetectTiles getDetectTilesOnDirect();
// 属性
protected:
	int _currPathNode; // 当前走到路径的哪个节点
	vector<int> _shortestPath; // 最短路径
	CC_SYNTHESIZE(PathFinder*, _targetSprite, TargetSprite); // 正在攻击,追逐,躲避的精灵(不引用计数)
	CC_SYNTHESIZE(bool, _isRoleInAttack, IsRoleInAttack);
	CC_SYNTHESIZE_PASS_BY_REF(DetectOval, _attackOval, AttackOval);
	CC_SYNTHESIZE(TEDirect, _direct, Direct); // 当前行进的方向
    CCSprite* _outlineSprite; // 轮廓精灵
protected: // 角色值
	CC_SYNTHESIZE(float, _speed, Speed);
	CC_SYNTHESIZE(float, _health, Health);
	CC_SYNTHESIZE(float, _attack, Attack);
	CC_SYNTHESIZE(bool, _dead, Dead);
public:
	CCAction* _deadAction; // 死亡时调用的动作
	void callbackDead(CCNode* actionOwner);
public: // 动作类
	ActionAnimationDirect* _fightAnimations;
	ActionAnimationDirect* _walkAnimations;
	ActionAnimationDirect* _standAnimations;
	ActionAnimationDirect* _laydownAnimations;
protected: // 网络
	CC_SYNTHESIZE(float, _objectId, ObjectId); // 活动对象ID
};

#endif // _PATH_FINDER__H_

