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
	createSkillButton();
	// 人物面板
	_rolePanel = RolePanel::createSelf(this);
	_rolePanel->setAnchorPoint(ccp(0, 0));
	_rolePanel->setPosition(ccp(0, _screenSize.height - _rolePanel->getPanelSize().height));
	this->addChild(_rolePanel);
    // 按钮
    createMenu();
    return;
}

bool PanelLayer::singleTouch(const CCPoint& pos) {
    for(int i = 0; i != _buttonVector.size(); ++i) {
        if(_buttonVector[i]->boundingBox().containsPoint(pos)) {
            CCLog("PanelLayer::singleTouch touched!");
            //_buttonVector[i]->singleTouch(pos);
            // 1 找到对应的动作
            // 2 判断动作是否能执行
            // 3 按钮开始冷却
            // 4 发送动作给精灵
            map<int, SSkillDetail>::const_iterator iSkill = _skillDetailMap.find(_buttonVector[i]->getSkillId());
            if(iSkill == _skillDetailMap.end()) {
                CCLog("did't find skillId");
            }
            else {
                if(!_buttonVector[i]->getIsCold()) {
                    _buttonVector[i]->cold();
                    CCLog("use fire skill");
                    EventSkillFire* newEvent = EventSkillFire::createSelf();
                    _mediatorToMap->sendPanelEvent(newEvent);
                }
            }
            return true;
        }
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

void PanelLayer::createSkillButton() {
    SkillButton* baseButton = NULL;
    baseButton = SkillButton::createSelf(this, "skill_normal_00.png", skill1);
    baseButton->setPosition(ccp(40, 50) + getButtonCenterPos(baseButton->boundingBox().size, 2));
    this->addChild(baseButton);
    _buttonVector.push_back(baseButton);
    baseButton = SkillButton::createSelf(this, "skill_rush_00.png", skill2);
    baseButton->setPosition(ccp(40, 50) + getButtonCenterPos(baseButton->boundingBox().size, 1));
    this->addChild(baseButton);
    _buttonVector.push_back(baseButton);
    baseButton = SkillButton::createSelf(this, "skill_fire_00.png", skill3);
    baseButton->setPosition(ccp(40, 50) + getButtonCenterPos(baseButton->boundingBox().size, 0));
    this->addChild(baseButton);
    _buttonVector.push_back(baseButton);
    baseButton = SkillButton::createSelf(this, "skill_heal_00.png", skill4);
    baseButton->setPosition(ccp(_screenSize.width - 240, 50) + getButtonCenterPos(baseButton->boundingBox().size, 2));
    this->addChild(baseButton);
    _buttonVector.push_back(baseButton);
    baseButton = SkillButton::createSelf(this, "skill_ice_00.png", skill5);
    baseButton->setPosition(ccp(_screenSize.width - 240, 50) + getButtonCenterPos(baseButton->boundingBox().size, 1));
    this->addChild(baseButton);
    _buttonVector.push_back(baseButton);
    baseButton = SkillButton::createSelf(this, "skill_ride_00.png", skill6);
    baseButton->setPosition(ccp(_screenSize.width - 240, 50) + getButtonCenterPos(baseButton->boundingBox().size, 0));
    this->addChild(baseButton);
    _buttonVector.push_back(baseButton);
    // init skill map
    //!!! should load from sever or json config file later
    SSkillDetail dtl;
    dtl._skillId = 1;
    dtl._coldTime = 6;
    dtl._mageCost = 10;
    _skillDetailMap[1] = dtl;
}


