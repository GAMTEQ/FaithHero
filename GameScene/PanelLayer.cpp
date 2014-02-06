#include "PanelLayer.h"
#include "SkillBar.h"
#include "BaseButton.h"
#include "RolePanel.h"
#include "MiniMap.h"
#include "LayerEvent.h"
#include "LayerMediator.h"
#include "MenuLayer.h"

PanelLayer::~PanelLayer() {
	//CC_SAFE_RELEASE_NULL(_mediatorToMap);
	CC_SAFE_RELEASE_NULL(_menuLayer);
}

PanelLayer* PanelLayer::createSelf() {
	PanelLayer* layer = new PanelLayer;
	if(layer && layer->initSelf()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;	
}

bool PanelLayer::initSelf() {
	if(!CCLayer::init()) {
		return false;
	}
	_mediatorToMap = NULL;
	//_screenSize = CCDirector::sharedDirector()->getWinSize();
	_screenSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
    createSprites();
	return true;
}

void PanelLayer::createSprites() {
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("uis.plist");
	// 左侧动作条
	_skillBarLeft = SkillBar::createSelf(3, 20, CCSize(200, 200), true);
	this->addChild(_skillBarLeft);
	_skillBarLeft->setPosition(ccp(40, 50));
	_skillBarLeft->setButton(BaseButton::createSelf(this, "skill_normal_00.png"), 2);
	_skillBarLeft->setButton(BaseButton::createSelf(this, "skill_rush_00.png"), 1);
	_skillBarLeft->setButton(BaseButton::createSelf(this, "skill_fire_00.png"), 0);
	// 右侧动作条
	_skillBarRight = SkillBar::createSelf(3, 20, CCSize(200, 200), true);
	this->addChild(_skillBarRight);
	_skillBarRight->setPosition(ccp(_screenSize.width - 240, 50));
	_skillBarRight->setButton(BaseButton::createSelf(this, "skill_heal_00.png"), 2);
	_skillBarRight->setButton(BaseButton::createSelf(this, "skill_ice_00.png"), 1);
	_skillBarRight->setButton(BaseButton::createSelf(this, "skill_ride_00.png"), 0);
	// 人物面板
	_rolePanel = RolePanel::createSelf(this);
	_rolePanel->setAnchorPoint(ccp(0, 0));
	_rolePanel->setPosition(ccp(0, _screenSize.height - _rolePanel->getPanelSize().height));
	this->addChild(_rolePanel);
	// 迷你地图
	//_miniMap = MiniMap::createSelf("minimap.png");
	//_miniMap->setPosition(ccp(_screenSize.width - _miniMap->getContentSize().width, _screenSize.height - _miniMap->getContentSize().height));
	//this->addChild(_miniMap);
    // 按钮
    createMenu();
    return;
}

bool PanelLayer::singleTouch(const CCPoint& pos) {
	if(_skillBarLeft->boundingBox().containsPoint(pos)) {
		
	}
	else if(_skillBarRight->boundingBox().containsPoint(pos)) {
	
	}
	return false;
}

bool PanelLayer::movingTouch(const CCPoint& from, const CCPoint& to) {
	return false;
}

void PanelLayer::update(float dt) {
}

int PanelLayer::handleEvent(MediatorEvent* evt) {
	switch (evt->getEventType())
	{
	case eEventMapDecreaseLifeAndMage:
		_rolePanel->updateLifeAndMage(
			((EventDecreaseLifeAndMage*)evt)->getDeltaLife(),
			((EventDecreaseLifeAndMage*)evt)->getDeltaMage());
		break;
	case eEventMapSetMaxLifeAndMage:
		_rolePanel->setMaxLifeAndMage(
			((EventSetMaxLifeAndMage*)evt)->getMaxLife(),
			((EventSetMaxLifeAndMage*)evt)->getMaxMage());
		break;
	default:
		break;
	}
	return 0;
}

void PanelLayer::createMenu() {
    // 创建菜单层
    _menuLayer = CCLayer::create();
    _menuLayer->retain();
    // 创建背景
    CCSprite* bg = CCSprite::create("tableview_bg.png");
	bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	_menuLayer->addChild(bg, 0);
    // 创建tableview
    MenuLayer* tableview = MenuLayer::createSelf();
    tableview->retain();
    // CCEGLView::sharedOpenGLView()->getVisibleOrigin()
    // tableview->setAnchorPoint(ccp(1, 1)); //!!! 为什么设置锚点无用？！
    // tableview的尺寸要在tableview内部获得，通过boundingbox获得的是 tableviewSize 1620.000000 1080.000000
    _menuLayer->addChild(tableview, 1);
    // 创建返回按钮

	CCMenuItemFont *itemBack = CCMenuItemFont::create("返回", this, menu_selector(PanelLayer::onButtonBack));
	itemBack->setFontSizeObj(50);
    itemBack->setPosition(ccp(bg->boundingBox().size.width / 2 -50, bg->boundingBox().size.height / 2 - 25));
	CCMenu *menuBack = CCMenu::create(itemBack, NULL);
	menuBack->setPosition(bg->getPosition());
	_menuLayer->addChild(menuBack);
    // 关闭按钮
    CCMenuItemImage *buttonGameClose = CCMenuItemImage::create("GameCloseButton.png",
                                                               "GameCloseButton.png",
                                                               this,
                                                               menu_selector(PanelLayer::onButtonGameClose));
    buttonGameClose->setAnchorPoint(ccp(0, 0));
    buttonGameClose->setPosition(ccp(_screenSize.width - buttonGameClose->boundingBox().size.width,
                                     _screenSize.height - buttonGameClose->boundingBox().size.height));
    buttonGameClose->setAnchorPoint(ccp(0, 0));
    buttonGameClose->setPosition(ccp(_screenSize.width - buttonGameClose->boundingBox().size.width,
                                     _screenSize.height - buttonGameClose->boundingBox().size.height));
    // 把菜单项添加到菜单
    _buttons = CCMenu::create(buttonGameClose, NULL);
    _buttons->setPosition(CCPointZero);
    this->addChild(_buttons);
}

void PanelLayer::onButtonGameClose() {
    this->addChild(_menuLayer);
    _buttons->setTouchEnabled(false);
    _touchLayer->setTouchEnabled(false);
}

void PanelLayer::onButtonBack(cocos2d::CCObject *sender)
{
    _menuLayer->removeFromParent();
    _buttons->setTouchEnabled(true);
    _touchLayer->setTouchEnabled(true);
}


