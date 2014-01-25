#include "Map.h"
#include "SpriteBase.h"
#include "PathFinder.h"
#include "Swordsman.h"
#include "Mainrole.h"
#include "Arrow.h"
#include "DamageNumPool.h"
#include "LayerEvent.h"
#include "ParticleSys.h"
#include "Building.h"


Map* Map::createSelf() {
	Map* self = new Map;
	if(self && self->initSelf()) {
		self->autorelease();
		return self;
	}
	CC_SAFE_DELETE(self);
	return NULL;
}

Map::~Map() {
	CC_SAFE_RELEASE_NULL(_walls);
	CC_SAFE_RELEASE_NULL(_mainrole);
	CC_SAFE_RELEASE_NULL(_enemySwordsman);
	CC_SAFE_RELEASE_NULL(_allEffects);
	CC_SAFE_RELEASE_NULL(_allPathFinders);
	CC_SAFE_RELEASE_NULL(_damageNumPool);
	CC_SAFE_RELEASE_NULL(_paritcleSys);
}

bool Map::initSelf() { 
	_tiledMap = NULL;
	// 方向,这个方向是图片坐标系(左下角坐标系)的方向,不是地图坐标系的方向
	// 对于左下角坐标系,(0,1)是正上方,(2,1)是右上方,注意(1,1)不是右上方,因为瓦块的比例是2:1
	CCPoint up = ccp(0, 1), rightup = ccp(2, 1), right = ccp(1, 0), rightdown = ccp(2, -1);
	CCPoint	down = ccp(0, -1), leftdown = ccp(-2, -1), left = ccp(-1, 0), leftup = ccp(-2, 1);
	_directs[eDirectUp] = up.normalize();
	_directs[eDirectRightup] = rightup.normalize();
	_directs[eDirectRight] = right.normalize();
	_directs[eDirectRightdown] = rightdown.normalize();
	_directs[eDirectDown] = down.normalize();
	_directs[eDirectLeftdown] = leftdown.normalize();
	_directs[eDirectLeft] = left.normalize();
	_directs[eDirectLeftup] = leftup.normalize();
	// 对象数组
	_walls = CCArray::createWithCapacity(0);
	_walls->retain();
	_allPathFinders = CCArray::createWithCapacity(0);
	_allPathFinders->retain();
	_enemySwordsman = CCArray::createWithCapacity(0);
	_enemySwordsman->retain();
	_allEffects = CCArray::createWithCapacity(0);
	_allEffects->retain();
	// 精灵表和精灵帧缓存
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("00.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("01.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("building1.plist");
	// 池
	//_screenSize = CCDirector::sharedDirector()->getWinSize();
	_screenSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
    _damageNumPool = DamageNumPool::createSelf(DAMAGE_NUM_COUNT);
	_damageNumPool->retain();
	// 粒子效果
	_paritcleSys = ParticleSys::createSelf(this);
	_paritcleSys->retain();
	return true; 
}

TEDirect Map::getClosestDirect(const CCPoint& direct) {
	float largestDot = (std::numeric_limits<float>::min)();
	int closestDirect = 0;
	for(int i = 0; i < NUM_DIRECT; ++i) {
		float dot = direct.dot(_directs[i]);
		if(dot > largestDot) {
			largestDot = dot;
			closestDirect = i;
		}
	}
	return (TEDirect)closestDirect;
}

void Map::movingMapWhenDraging(const CCPoint& startPos, const CCPoint& nowPos, float factor) {
    movingMapByVectorAndBase(startPos, nowPos, _tiledMap->getPosition(), factor);
}

void Map::movingMapWhenRoleMoving(const CCPoint& startPos, const CCPoint& nowPos, float factor) {
    movingMapByVectorAndBase(startPos, nowPos, _mapOriginCenter, factor);
}

void Map::movingMapByVectorAndBase(const CCPoint& startPos, const CCPoint& nowPos, const CCPoint& base, float factor) {
	if(_isMapSmallerThanScreen) return;
	CCPoint toNow = nowPos - startPos;
	CCPoint newPos = base + toNow * factor;
	if(newPos.x < _xRight) newPos.x = _xRight;
	if(newPos.x > _xLeft) newPos.x = _xLeft;
	if(newPos.y > _yUp) newPos.y = _yUp;
	if(newPos.y < _yDown) newPos.y = _yDown;
	_tiledMap->setPosition(newPos);
}

// 返回(-1, -1)代表该实际坐标超出了地图,即不在菱形内
CCPoint Map::realPosToTilePos(const CCPoint& realPos) const
{
	double w = _tileSize.width / 2;
	double h = _tileSize.height / 2;
	double W = _mapOriPos.x;
	double H = _mapOriPos.y;
	double x = realPos.x;
	double y = realPos.y;
	double m = (H*w - y*w + x*h - W*h) / (2*w*h);
	double n = (H*w - y*w + W*h - x*h) / (2*w*h); // 必须用double判断,因为-0.4会变成0,本来不合法的点又合法了
	if(m < _layerSize.width && n < _layerSize.height && m >=0 && n >=0)
		return ccp(int(m), int(n));
	else
		return INVALID_POINT; // out of map
}

int Map::getTileType(const CCPoint& tilePoint) {
	int x = tilePoint.x;
	int y = tilePoint.y;
	//CCLog("getTileType %d %d", x, y);
	if(x >= int(_layerSize.width) || x < 0 || y >= int(_layerSize.height) || y < 0) {
		return emWall;
	}
	else {
		return _naviGraph.getNodeFlag(tilePoint);
	}
}

void Map::resetMap(CCTMXTiledMap* newMap, CCSize& screenSize, CCNode* parent, LayerMediator* mediator) {
	assert(newMap);
	if(_tiledMap) {
		_tiledMap->removeAllChildrenWithCleanup(true);
	}
	_mediator = mediator;
	_tiledMap = newMap;
	parent->addChild(_tiledMap, ezMap);
	_mapContentSize = _tiledMap->getContentSize();
    _mapCenterPos = ccp(_mapContentSize.width / 2, _mapContentSize.height / 2);
    _mapOriginCenter = ccp(screenSize.width / 2, screenSize.height / 2);
    _mapOriPos = ccp(_mapContentSize.width * 0.5f, _mapContentSize.height);
	_tiledMap->setAnchorPoint(ccp(0.5, 0.5)); // 必须是(0.5, 0.5)，其他的不行
    _tiledMap->setPosition(_mapOriginCenter);
    CCLog("_tiledMap->setPosition %f %f", _tiledMap->getPosition().x, _tiledMap->getPosition().y);
    //_tiledMap->setPosition(ccp(0, 0));
	_xRight = screenSize.width - _mapContentSize.width / 2;
	_xLeft = _mapContentSize.width / 2;
	_yUp = _mapContentSize.height / 2;
	_yDown = screenSize.height - _mapContentSize.height / 2;
	_isMapSmallerThanScreen = _mapContentSize.width <= screenSize.width && _mapContentSize.height <= screenSize.height ? true : false;
	CCLog("map tile width %f height %f", _tiledMap->getTileSize().width, _tiledMap->getTileSize().height);
	CCLog("map size width %f height %f", _tiledMap->getMapSize().width, _tiledMap->getMapSize().height);
	CCLog("map content width %f height %f", _mapContentSize.width, _mapContentSize.height);
	// 层
	_tileSize = _tiledMap->getTileSize();
	_mapSize = _tiledMap->getMapSize();
	_layer0 = _tiledMap->layerNamed("layer0");
	_layer0->setupTiles();
	_layerSize = _layer0->getLayerSize();
	_layer0zorder = _layer0->getZOrder();
	_highestZorder = 3 + (int)sqrt((float)_mapSize.width * (float)_mapSize.width + (float)_mapSize.height * (float)_mapSize.height);
	//initMapInfo();
	// 初始化导航图
	_naviGraph.init(*_tiledMap, *_layer0);
	// 对象层
	CCTMXObjectGroup* obj0Group = _tiledMap->objectGroupNamed("object0");
	CCArray* objects = obj0Group->getObjects();
	for(int i = 0; i != objects->count(); ++i) {
		CCDictionary* dict = (CCDictionary*)objects->objectAtIndex(i);
		if(!dict) {
			CCLog("null object in objectGroup");
			break;
		}
		CCString* objType = ((CCString*)dict->objectForKey("type"));
		if(!objType) {
			CCLog("wrong type object in objectGroup");
			continue;
		}
		float x = ((CCString*)dict->objectForKey("x"))->floatValue();
		float y = ((CCString*)dict->objectForKey("y"))->floatValue(); // xy是转换成实际坐标的
		CCPoint tilePosOnMap = ccp(0, 0);
		CCPoint posOnMap = convertPosToMap(ccp(x, y), tilePosOnMap);
		float type = objType->intValue();
        // 此处暂时用i作为ObjectId，以后这些对象都应该从服务端创建
		buildSprite(type, posOnMap, tilePosOnMap, i);
	}
	CCLog("Map::resetMap success");
	CCLog("enemy count %d", _enemySwordsman->count());
	// 重启粒子系统
	_paritcleSys->addParticlesToMap();
	return;
}

// 把地图编辑器上对象的坐标转换成瓦块坐标，返回瓦块坐标和瓦块中心点坐标
CCPoint Map::convertPosToMap(const CCPoint& pos, CCPoint& tilePosOnMap) {
	int tileX = pos.x / _tiledMap->getTileSize().height;
	int tileY = pos.y / _tiledMap->getTileSize().height;
	int tileX1 = tileX;
	int tileY1 = _tiledMap->getMapSize().height - 1 - tileY;
	tilePosOnMap = ccp(tileX1, tileY1);
	//CCLog("pos %f %f tile1 %d %d tile2 %d %d", pos.x, pos.y, tileX, tileY, tileX1, tileY1);
	return getTileCenter(tilePosOnMap);
}

CCPoint Map::getTileCenter(const CCPoint& tilePos) {
	CCPoint tileOrigin = _layer0->positionAt(tilePos);
	return tileOrigin + ccp(_tiledMap->getTileSize().width / 2, _tiledMap->getTileSize().height / 2);
}

PathFinder* Map::getPathFinder() {
	assert(_mainrole);
	return _mainrole;
}

bool Map::buildSprite(const int& type, const CCPoint& pos, const CCPoint& tilePosOnMap, int objectId) {
	//CCLog("builderSprite x:%f y:%f type:%d", pos.x, pos.y, type);
	CCSprite* newSprite = NULL;
	switch(type) {
	case eTypeWall:
		newSprite = Wall::createSelf(this);
		_walls->addObject(newSprite);
		newSprite->setPosition(pos);
		_tiledMap->addChild(newSprite);
		_naviGraph.setNodeFlag(tilePosOnMap, emWall);
		break;
    case eType2x2building: {
        break;
        newSprite = Building::createSelf(this, "21_hill1.png", tilePosOnMap);
        _walls->addObject(newSprite);
        int zorder = newSprite->getZOrder();
        newSprite->setPosition(pos);
        newSprite->setZOrder(zorder);
        _tiledMap->addChild(newSprite);
        _naviGraph.setNodeFlag(tilePosOnMap, emWall);
        break;
    }
    case eType3x3building: {
        newSprite = Building::createSelf(this, "31_tree1.png", tilePosOnMap);
        _walls->addObject(newSprite);
        int zorder = newSprite->getZOrder();
        newSprite->setPosition(pos);
        newSprite->setZOrder(zorder);
        _tiledMap->addChild(newSprite);
        _naviGraph.setNodeFlag(tilePosOnMap, emWall);
        break;
    }
    case eType4x4building: {
        newSprite = Building::createSelf(this, "41_house1.png", tilePosOnMap);
        _walls->addObject(newSprite);
        int zorder = newSprite->getZOrder();
        newSprite->setPosition(pos);
        newSprite->setZOrder(zorder);
        _tiledMap->addChild(newSprite);
        _naviGraph.setNodeFlag(tilePosOnMap, emWall);
        break;
    }
    case eType5x5building: {
        newSprite = Building::createSelf(this, "51_house1.png", tilePosOnMap);
        _walls->addObject(newSprite);
        int zorder = newSprite->getZOrder();
        newSprite->setPosition(pos);
        newSprite->setZOrder(zorder);
        _tiledMap->addChild(newSprite);
        _naviGraph.setNodeFlag(tilePosOnMap, emWall);
        break;
    }
	case eTypePathFinder:
        CCLog("eTypePathFinder objectId %d", objectId);
		newSprite = Mainrole::createSelf(this);
		_mainrole = (Mainrole*)newSprite;
		_mainrole->retain();
		_mainrole->setPosition(pos);
        _mainrole->setObjectId(objectId);
		_allPathFinders->addObject(newSprite);
		_tiledMap->addChild(newSprite);
        _msgDispatcher.addObject((MsgHandler*)_mainrole);
        //!!! 移动地图，让主角在屏幕中心
        movingMapWhenRoleMoving(_mainrole->getPosition(), _mapCenterPos, 1.0f);
		break;
	case eTypeEnemySwordsman:
        CCLog("eTypeEnemySwordsman objectId %d", objectId);
		newSprite = Swordsman::createSelf(this);
		newSprite->setPosition(pos);
        ((Swordsman*)newSprite)->setObjectId(objectId);
		_allPathFinders->addObject(newSprite);
		_enemySwordsman->addObject(newSprite);
		_tiledMap->addChild(newSprite);
        _msgDispatcher.addObject((MsgHandler*)(Swordsman*)newSprite); //!!!注意，指针不能随便强转！只能转成父类
		break;
	default:
		return false;
	}
	return true;
}


void Map::update(float dt) {
    _msgDispatcher.consumeNewServerMsg();
	// 对每个角色执行循环
	//CCPoint jetV = ccp(-10, -20);
	//_paritcleSys->getJet()->setPosition(_paritcleSys->getJet()->getPosition() + jetV * dt);
	//_paritcleSys->getJet()->setAngle(atan2f(jetV.y, jetV.x) * 180 / 3.1416 + 180);
	getPathFinder()->update(dt);
	for(int i = 0; i != _allEffects->count(); ++i) {
		Arrow* sprite = (Arrow*)_allEffects->objectAtIndex(i);
		sprite->update(dt);		
	}
	for(int i = 0; i != _enemySwordsman->count(); ++i) {
		Swordsman* sprite = (Swordsman*)_enemySwordsman->objectAtIndex(i);
		sprite->update(dt);		
	}
	// 清除标记了死亡的精灵(不能再主循环中直接删除,因为会造成指针异常)
	for(vector<CCSprite*>::iterator itr = _deadPathFinders.begin();
		itr != _deadPathFinders.end(); ++itr)
	{
		PathFinder* deadRole = (PathFinder*)(*itr);
		if(deadRole == _mainrole) {
			CCLog("mainrole is dead, game over.");
			assert(0);
			// 游戏状态进入结束状态
		}
		else {
			_enemySwordsman->removeObject(deadRole);
		}
		_allPathFinders->removeObject(deadRole);
		deadRole->runActionDead();
		// 如果角色是主角,则游戏结束
	}
	for(vector<CCSprite*>::iterator itr = _deadEffects.begin();
		itr != _deadEffects.end(); ++itr)
	{
		Arrow* arrow = (Arrow*)(*itr);
		_allEffects->removeObject(arrow);
		arrow->removeFromParent();
	}
	// 清除本次循环标记的死亡精灵
	_deadPathFinders.clear();
	_deadEffects.clear();
}

vector<int> Map::lineCrossTiles(const CCPoint& pointA, const CCPoint& pointB) {
	// 起点和终点所在的格子
	vector<int> crossIdList;
	crossIdList.push_back(realPosToNodeId(pointA));
	crossIdList.push_back(realPosToNodeId(pointB));
	// 求包裹矩形的4个顶点(包含AB的最小矩形)
	CCPoint vertexs[4] = {realPosToTilePos(pointA), realPosToTilePos(pointB), ccp(0, 0), ccp(0, 0)};
	// 0,2的连线和X轴平行 2,3的连线和Y轴平行
	vertexs[2] = vertexs[0] - ccp(vertexs[0].x - vertexs[1].x, 0); // 不会减成INVALID_POINT
	vertexs[3] = vertexs[1] + ccp(vertexs[0].x - vertexs[1].x, 0);
	// 找到离原点[0, 0]最近和最远的端点(用分量和判断)
	float maxSum = 0, minSum = 0;
	int upIndex = 0, downIndex = 0; // 上顶点和下顶点
	for(int i = 0; i != 4; ++i) {
		float sum = vertexs[i].x + vertexs[i].y;
		if(maxSum > sum)
			maxSum = sum, downIndex = i;
		if(minSum < sum)
			minSum = sum, upIndex = i;
	}
	// 右顶点和左顶点()
	int rightIndex = abs(upIndex - 2), leftIndex = abs(downIndex - 2);
	// 确定扫描线
	int scanBegin, scanEnd, scanLineBegin, scanLineEnd; // 扫描的起点和终点,扫描线的起点和终点
	CCPoint scanLineBeginStep, scanLineEndStepChange; // 扫描线起点和终点的步长
	if(upIndex == 0 || upIndex == 1) { // 如果upIndex是线段的端点,则从上往下扫描
		scanBegin = upIndex; scanEnd = downIndex;
		scanLineBegin = leftIndex; scanLineEnd = rightIndex;
		scanLineBeginStep = ccp(0, 1); scanLineEndStepChange = ccp(0, 1);
	}
	else { // 如果upIndex不是线段的端点,则从左到右扫描
		scanBegin = leftIndex; scanEnd = rightIndex;
		scanLineBegin = upIndex; scanLineEnd = downIndex;
		scanLineBeginStep = ccp(0, -1); scanLineEndStepChange = ccp(0, -1);
	}
	// 开始扫描
	CCPoint nextBeginTilePos = vertexs[scanBegin] + scanLineBeginStep; // 下一次扫描线的左端点
	CCPoint nextEndTilePos = vertexs[scanBegin] + ccp(1, 0); // 下一次扫描线的右端点
	while(!nextBeginTilePos.equals(vertexs[scanEnd]) && !nextEndTilePos.equals(vertexs[scanEnd])) {
		// 生成扫描线的2个端点
		CCPoint realBeginPos = _naviGraph.nodeIdToVertex(tilePosToNodeId(nextBeginTilePos), 3);
		CCPoint realEndPos = _naviGraph.nodeIdToVertex(tilePosToNodeId(nextBeginTilePos), 1);
		// 如果不平行且相交,则求交点
		if(!MathFunc::parallel(realBeginPos, realEndPos, pointA, pointB) 
			&& MathFunc::intersect_in(realBeginPos, realEndPos, pointA, pointB))
		{
			CCPoint crossPoint = MathFunc::intersection(realBeginPos, realEndPos, pointA, pointB);
			crossIdList.push_back(realPosToNodeId(crossPoint));
		}
		// 计算下一次的端点
		nextBeginTilePos = nextBeginTilePos.equals(vertexs[scanLineBegin]) ? 
			nextBeginTilePos + ccp(1, 0) : nextBeginTilePos + scanLineBeginStep;
		nextEndTilePos = nextEndTilePos.equals(vertexs[scanLineEnd]) ? 
			nextEndTilePos + scanLineEndStepChange : nextEndTilePos + ccp(1, 0);
	}
	return crossIdList;		
}

// 处理触摸
// 如果触摸在敌人身上,则判断是否进入攻击范围,没进入则靠近,并随时判断,进入攻击距离就攻击
// 如果触摸在墙上,则不移动,并且失去移动目标,失去攻击目标
// 如果触摸在空地上,则移动到该空地
int Map::dealTargetTouch(const CCPoint& pos) { 
	// 首先判断是否触摸到敌人
	for(int i = 0; i != _enemySwordsman->count(); ++i) {
		Swordsman* enemy = (Swordsman*)_enemySwordsman->objectAtIndex(i);
		if(enemy->boundingBox().containsPoint(pos)) { // 如果触摸到敌人,则以该敌人为目标,进入追击状态
			_mainrole->setTargetSprite(enemy);
			enemy->showHealthBar();
			_mainrole->getStateMachine()->changeState(MainrolePursuit::instance());
			return 0;
		}
	}
	// 如果没有触碰敌人,则移动
	_mainrole->setTarget(pos);
	_mainrole->getStateMachine()->changeState(MainroleWalk::instance());
	return 0;
}

void Map::dealDead(PathFinder* deadRole) {
	deadRole->setDead(true);
	deadRole->setTarget(INVALID_POINT);
	deadRole->setTargetSprite(NULL);
	deadRole->runActionLaydown(1.0);
	// 如果有角色的目标是死亡角色,则把目标改成NULL
	for(int i = 0; i != _allPathFinders->count(); ++i) {
		PathFinder* role = (PathFinder*)_allPathFinders->objectAtIndex(i);
		if(role != deadRole && role->getTargetSprite() == deadRole) {
			role->setTargetSprite(NULL);
		}
	}
	// 如果有子弹的目标是死亡角色,则把目标改成NULL
	for(int i = 0; i != _allEffects->count(); ++i) {
		Arrow* arrow = (Arrow*)_allEffects->objectAtIndex(i);
		if(arrow->getTargetSprite() == deadRole) {
			arrow->setTargetSprite(NULL);
		}
	}
	_deadPathFinders.push_back(deadRole);
}

void Map::dealDeadEffect(Arrow* arrow) {
	_deadEffects.push_back(arrow);
}

void Map::registerEffect(Arrow* arrow) {
	_allEffects->addObject(arrow);
	_tiledMap->addChild(arrow, _highestZorder);
}

void Map::sendEventLifeAndMage(float deltaLife, float deltaMage) {
	EventDecreaseLifeAndMage* newEvent = EventDecreaseLifeAndMage::createSelf(deltaLife, deltaMage);
	_mediator->sendMapEvent(newEvent);
}

void Map::sendInitEventToMediator() {
	EventSetMaxLifeAndMage* evt = EventSetMaxLifeAndMage::createSelf(_mainrole->getHealth(), _mainrole->getMage());
	getMediator()->sendMapEvent(evt);
	EventDecreaseLifeAndMage* newEvent = EventDecreaseLifeAndMage::createSelf(_mainrole->getHealth(), _mainrole->getMage());
	_mediator->sendMapEvent(newEvent);
}




