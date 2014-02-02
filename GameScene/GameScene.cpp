#include "GameScene.h"
#include "MapLayer.h"
#include "TouchLayer.h"
#include "PanelLayer.h"
#include "LayerMediator.h"
#include "LayerEvent.h"
#include "LuaLoader.h"

GameScene* GameScene::createSelf() {
	GameScene* scene = new GameScene;
	if(scene && scene->initSelf()) {
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return NULL;
}

GameScene::~GameScene() {
	CC_SAFE_RELEASE_NULL(_mediatorPanelAndMap);
	CC_SAFE_RELEASE_NULL(_TouchLayer);
	CC_SAFE_RELEASE_NULL(_MapLayer);
	CC_SAFE_RELEASE_NULL(_PanelLayer);
}

bool GameScene::initSelf() {
	if(CCScene::init()) {
        LuaLoader luaLoader;
        luaLoader.reloadScript("BaseConfig.lua", "resultTable");
		// 构造层
		_TouchLayer = TouchLayer::createSelf();
		_TouchLayer->retain();
		_MapLayer = MapLayer::createSelf();
		_MapLayer->retain();
		_PanelLayer = PanelLayer::createSelf();
		_PanelLayer->retain();
		// 把层添加给scene
        CCPoint ori = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
		this->addChild(_MapLayer, eLayerZorderMap);
		this->addChild(_PanelLayer, eLayerZorderPanel);
		this->addChild(_TouchLayer, eLayerZorderTouch);
        _PanelLayer->setPosition(ori);
        _MapLayer->setPosition(ori);
        _TouchLayer->setPosition(ori);
		// 为触摸层添加代理
		_TouchLayer->addDelegate(_PanelLayer); // 优先响应界面层
		_TouchLayer->addDelegate(_MapLayer); // 次优先相应地图层
		// 为界面层和地图层准备消息中介
		_mediatorPanelAndMap = LayerMediator::createSelf();
		_mediatorPanelAndMap->retain();
		_mediatorPanelAndMap->setPanelLayer(_PanelLayer);
		_mediatorPanelAndMap->setMapLayer(_MapLayer);
		_PanelLayer->setMediatorToMap(_mediatorPanelAndMap);
		_MapLayer->setMediatorToPanel(_mediatorPanelAndMap);
		// 场景注册更新
		scheduleUpdate();
		this->schedule(schedule_selector(GameScene::update));
		return true;
	}
	return false;
}

void GameScene::update(float dt) {
	_MapLayer->update(dt);
}


