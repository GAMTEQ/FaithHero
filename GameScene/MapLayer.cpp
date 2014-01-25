#include "MapLayer.h"
#include "Map.h"
#include "UserRecordDefault.h"
#include "UserRecordSqlite.h"

MapLayer::~MapLayer() {
	CC_SAFE_RELEASE_NULL(_map);
	//CC_SAFE_RELEASE_NULL(_mediatorToPanel);
}

MapLayer* MapLayer::createSelf() {
	MapLayer* layer = new MapLayer;
	if(layer && layer->initSelf()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;	
}

bool MapLayer::initSelf() {
	if(!CCLayer::init()) {
		return false;
	}
	_mediatorToPanel = NULL;
	//_screenSize = CCDirector::sharedDirector()->getWinSize();
    _screenSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
	createSprites();
	return true;
}

void MapLayer::createSprites() {
	// 背景
	CCSprite* bg = CCSprite::create("bg.png");
	//bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	//bg->setColor(ccc3(255, 255, 255));
	//this->addChild(bg, ezBackground);
	// 地图
    CCLog("screen size %f %f", _screenSize.width, _screenSize.height);
	_map = Map::createSelf();
	_map->retain();
	_map->resetMap(CCTMXTiledMap::create("map60.tmx"), _screenSize, this, _mediatorToPanel); // 必须先初始化
}

bool MapLayer::singleTouch(const CCPoint& pos) {
	CCPoint mapPos = _map->getTiledMap()->convertToNodeSpace(pos);
	_map->dealTargetTouch(mapPos);
	return true;
}

bool MapLayer::movingTouch(const CCPoint& from, const CCPoint& to) {
	_map->movingMapWhenDraging(from, to, 0.1f);
	return true;
}

void MapLayer::update(float dt) {
	_map->update(dt);
}

void MapLayer::setMediatorToPanel(LayerMediator* mediatorToPanel) {
	_mediatorToPanel = mediatorToPanel;
	_map->setMediator(mediatorToPanel);
	_map->sendInitEventToMediator();
}

