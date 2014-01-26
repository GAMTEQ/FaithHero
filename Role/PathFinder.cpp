#include "PathFinder.h"
#include "Map.h"
#include "SearchAstar.h"
#include "SearchDijkstra.h"
#include "NaviGraph.h"
#include "ActionAnimation.h"

void PathFinder::callbackDead(CCNode* actionOwner) {
    int retainCount = actionOwner->retainCount();
    CCLog("PathFinder::callbackDead count %d", retainCount);
    //actionOwner->retain();
    //actionOwner->retain();
	actionOwner->removeFromParent();
    CCLog("PathFinder::callbackDead2 count %d", actionOwner->retainCount());
}

PathFinder::~PathFinder() {
	CC_SAFE_RELEASE(_walkAnimations);
	CC_SAFE_RELEASE(_fightAnimations);
	CC_SAFE_RELEASE(_standAnimations);
	CC_SAFE_RELEASE(_laydownAnimations);
	CC_SAFE_RELEASE(_deadAction);
    _outlineSprite->removeFromParentAndCleanup(TRUE);// 析构角色时，同时析构轮廓精灵
}

void PathFinder::setTarget(CCPoint target) {
	_target = target;
    CCLog("PathFinder::setTarget %f %f", target.x, target.y);
	if(_target.equals(INVALID_POINT))
		return;
	calculateShotestPath();
}

TEDirect PathFinder::getClosestDirect(const CCPoint& direct) {
	return _map->getClosestDirect(direct);
}

bool PathFinder::isObstacleOnDirect() {
	DetectTiles detectTiles = getDetectTilesOnDirect();
	const int tile1 = _map->getTileType(getTilePos() + detectTiles.tile1);
	const int tile2 = _map->getTileType(getTilePos() + detectTiles.tile2);
	const int tile3 = _map->getTileType(getTilePos() + detectTiles.tile3);
	return (tile1 || tile2 || tile3);
}

TETraceResult PathFinder::naviByPath(double dt) {
	// 如果目标点无效,则不走
	if(getTarget().equals(INVALID_POINT)) {
		//stopAllActions();
		setTarget(INVALID_POINT);
		return eTraceResultFailure;
	}
	// 如果下一个路径点在地图外或是墙,则不走(应该不会出现这种情况)
	CCPoint targetTilePos = _map->realPosToTilePos(getTarget());
	if(int(targetTilePos.x + targetTilePos.y) == -2 || _map->getTileType(targetTilePos) == emWall) {
		//stopAllActions();
		setTarget(INVALID_POINT);
		return eTraceResultFailure;
	}
	// 如果已经到达目标点,则寻找下一个目标点
	if(getPosition().getDistanceSq(getTarget()) < 1) {
		//CCLog("reach half way pos");
		//stopAllActions();
		if(_currPathNode == _shortestPath.size() - 1) { // 如果目标点是最后一个,则寻路结束
			//stopAllActions();
			setTarget(INVALID_POINT);
			return eTraceResultSuccess;
		}
		else { // 如果目标点后面还有,则继续
			//CCLog("go to next pos");
			++_currPathNode;
			_target = _map->getNaviGraph().getNode(_shortestPath[_currPathNode]).getCenterPos();
			CCPoint toTarget = getTarget() - getPosition();
			setDirect(_map->getClosestDirect(toTarget.normalize()));
		}
	}
	// 根据不同的方向,执行不同的动作
	TEActionState actionState = _walkAnimations->runDirect(this, getDirect(), dt);
	// 走向目标点
	CCPoint tilePos = _map->realPosToTilePos(getPosition());
	int tileZorder = tilePos.x + tilePos.y;
	setZOrder(tileZorder);
	CCPoint toTarget = getTarget() - getPosition();
	toTarget = toTarget.normalize();
	setPosition(getPosition() + toTarget * _speed * dt);
	return eTraceResultHalfway;
}

bool PathFinder::update(double dt) {
	if(_health <= 0) {
		CCLog("PathFinder die");
	}
	naviByPath(dt);
	return true;
}

bool PathFinder::initSelf(Map* map) {
	if(BaseSprite::initSelf(map)) {
        _outlineSprite = NULL;
        // 初始化轮廓精灵，注意，这个精灵必须先初始化，因为PathFinder的setPosition和setDisplayFrame方法都要用到这个精灵！！！（否则会CORE）
        createOutlineSprite();
	
        _dead = false;
		_target = INVALID_POINT;
		_direct = eDirectDown;
		_targetSprite = NULL;
		_currPathNode = 0;
		_isRoleInAttack = false;
		// 动作
		_deadAction = CCSequence::createWithTwoActions(CCFadeOut::create(4.0f),
                                                       CCRemoveSelf::create());
        _deadAction->retain();
        // CCCallFuncN::create(this, callfuncN_selector(PathFinder::callbackDead))
        //??? 不知道为什么，这两句移到基类就没效果了，在子类才有效果，坑！！！待解决
        //this->setAnchorPoint(ccp(0.5, 0.05));
        //this->setScale(3);
        
		initSpecialProperty();
		// 动作类 实验
		return true;
	}
	return false;
}

void PathFinder::calculateShotestPath() {
	// 如果目标无效,则不寻路
	if(_target.equals(INVALID_POINT)) {
		//stopAllActions();
		return;
	}
	CCPoint targetTilePos = _map->realPosToTilePos(getTarget());
	 // 如果目标在地图外或是墙,则不寻路
	if(int(targetTilePos.x + targetTilePos.y) == -2 || _map->getTileType(targetTilePos) == emWall) {
		//stopAllActions();
		setTarget(INVALID_POINT);
		return;
	}
    
	SearchDijkstra<NaviGraph<NaviNode, NaviEdge> > search(
		_map->getNaviGraph(), _map->tilePosToNodeId(getTilePos()), _map->realPosToNodeId(getTarget()));
	_shortestPath = search.getPathToTarget();
	
    _currPathNode = 0;
	_target = _map->getNaviGraph().getNode(_shortestPath[_currPathNode]).getCenterPos();
    /*
    CCParticleSystem* _jet = CCParticleSystemQuad::create("particle/jet.plist");
    _jet->retain();
    //this->addChild(_jet);
    _jet->setSourcePosition(ccp(0, 0));
    _jet->setPosition(getPosition());
    _jet->setAngle(0);
    _jet->setVisible(true);
    _jet->resetSystem();
    */
}

// 在移动方向上,需要检测哪些瓦块上是否有障碍
DetectTiles PathFinder::getDetectTilesOnDirect() {
	DetectTiles tiles;
	switch(_direct)
	{
	case eDirectUp:
		tiles.tile1 = ccp(-1, 0); tiles.tile2 = ccp(-1, -1); tiles.tile3 = ccp(0, -1);
		break;
	case eDirectRightup:
		tiles.tile1 = ccp(0, -1); tiles.tile2 = ccp(0, 0); tiles.tile3 = ccp(0, -1);
		break;
	case eDirectRight:
		tiles.tile1 = ccp(0, -1); tiles.tile2 = ccp(1, -1); tiles.tile3 = ccp(1, 0);
		break;
	case eDirectRightdown:
		tiles.tile1 = ccp(1, 0); tiles.tile2 = ccp(0, 0); tiles.tile3 = ccp(0, 0);
		break;
	case eDirectDown:
		tiles.tile1 = ccp(1, 0); tiles.tile2 = ccp(1, 1); tiles.tile3 = ccp(0, 1);
		break;
	case eDirectLeftdown:
		tiles.tile1 = ccp(0, 1); tiles.tile2 = ccp(0, 0); tiles.tile3 = ccp(0, 0);
		break;
	case eDirectLeft:
		tiles.tile1 = ccp(0, 1); tiles.tile2 = ccp(-1, 1); tiles.tile3 = ccp(-1, 0);
		break;
	case eDirectLeftup:
		tiles.tile1 = ccp(-1, 0); tiles.tile2 = ccp(0, 0); tiles.tile3 = ccp(0, 0);
		break;
	}
	return tiles;
}

void PathFinder::runActionFight(float dt) {
	TEActionState actionState = _fightAnimations->runDirect(this, getDirect(), dt);
	if(actionState == eActionStateEnd) {
		PathFinder* enemy = this->getTargetSprite();
		enemy->setHealth(enemy->getHealth() - this->getAttack());
	}
}

void PathFinder::runActionStand(float dt) {
	_standAnimations->runDirect(this, getDirect(), dt);
}

void PathFinder::runActionLaydown(float dt) {
	_laydownAnimations->runDirect(this, getDirect(), dt);
}

void PathFinder::runActionDead() {
    CCLog("PathFinder::runActionDead retain count %d", this->retainCount()); //??? 是1，是正常的
    this->runAction(_deadAction);
    CCLog("PathFinder::runActionDead2 retain count %d", this->retainCount()); //??? 是2，奇怪。说明runAction时也会对this引用计数
}

void PathFinder::handleServerMsg(MsgBase* msgBase) {
}

int PathFinder::getObjId() {
    return getObjectId();
}

void PathFinder::createOutlineSprite() {
    _outlineSprite = CCSprite::create();
    _outlineSprite->setOpacity(100);
    _map->getTiledMap()->addChild(_outlineSprite, _map->getHighestZorder());
}

 
 // 重写setDisplayFrame函数，每次调用时，都会绘制轮廓精灵
void PathFinder::setDisplayFrame(CCSpriteFrame *pNewFrame) {
    BaseSprite::setDisplayFrame(pNewFrame);
    if(_outlineSprite)
        _outlineSprite->setDisplayFrame(pNewFrame);
}
 
void PathFinder::setPosition(const CCPoint& pos) {
    BaseSprite::setPosition(pos);
    if(_outlineSprite)
        _outlineSprite->setPosition(pos);
}


void PathFinder::setAnchorPoint(const CCPoint& anchor)
{
    CCSprite::setAnchorPoint(anchor);
    if(_outlineSprite)
        _outlineSprite->setAnchorPoint(anchor);
}

void PathFinder::setScale(float fScale)
{
    CCSprite::setScale(fScale);
    if(_outlineSprite)
        _outlineSprite->setScale(fScale);
}







