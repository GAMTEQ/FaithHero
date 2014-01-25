//
//  LoginScene.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-6.
//
//

#include "LoginScene.h"
#include "GameScene.h"
#include "Network.h"

bool LoginScene::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(! CCLayer::init());
        _loginSuccess = false;
        CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
        CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
        // 用户名
        CCSize editBoxSize = CCSizeMake(visibleSize.width - 100, 60);
        _editBoxUserName = CCEditBox::create(editBoxSize , CCScale9Sprite::create("green_edit.png"));
        _editBoxUserName->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                          visibleOrigin.y + visibleSize.height * 3 / 4));
        _editBoxUserName->setFont("Paint Boy", 25);
        _editBoxUserName->setFontColor(ccBLACK);
        _editBoxUserName->setPlaceHolder("Name:");
        _editBoxUserName->setPlaceholderFontColor(ccWHITE);
        _editBoxUserName->setMaxLength(8);
        _editBoxUserName->setReturnType(kKeyboardReturnTypeDone);
        _editBoxUserName->setDelegate(this);
        this->addChild(_editBoxUserName, zLoginSceneEditBox);
        // 密码
        _editBoxUserPassword = CCEditBox::create(editBoxSize, CCScale9Sprite::create("green_edit.png"));
        _editBoxUserPassword->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                              visibleOrigin.y + visibleSize.height / 2));
        _editBoxUserPassword->setFont("American Typewriter", 30);
        _editBoxUserPassword->setFontColor(ccBLACK);
        _editBoxUserPassword->setPlaceHolder("Password:");
        _editBoxUserPassword->setPlaceholderFontColor(ccWHITE);
        _editBoxUserPassword->setMaxLength(8);
        _editBoxUserPassword->setReturnType(kKeyboardReturnTypeDone);
        _editBoxUserPassword->setDelegate(this);
        this->addChild(_editBoxUserPassword, zLoginSceneEditBox);
        // 按钮
        CCLabelTTF *label = CCLabelTTF::create("Login", "Arial", 50);
        CCMenuItemLabel *pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(LoginScene::onButtonLogin));
        pMenuItem->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                   visibleOrigin.y + visibleSize.height / 4));
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create("close_normal.png",
                                                              "close_selected.png",
                                                              this,
                                                              menu_selector(LoginScene::onButtonClose));
        pCloseItem->setPosition(ccp(visibleOrigin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                    visibleOrigin.y + pCloseItem->getContentSize().height/2));
        _loginMenu = CCMenu::create(pMenuItem, pCloseItem, NULL);
        _loginMenu->setPosition(CCPointZero);
        this->addChild(_loginMenu, zLoginSceneEditBox);
        // LOGO
        CCLabelTTF *pLabel = CCLabelTTF::create("Massive-Multiplayer-Online-2", "Arial", 30);
        CC_BREAK_IF(!pLabel);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 30));
        this->addChild(pLabel, zLoginSceneLogo);
        // 背景图片
        CCSprite *pSprite = CCSprite::create("login_scene_background.png");
        CC_BREAK_IF(!pSprite);
        pSprite->setPosition(ccp(size.width / 2, size.height / 2));
        this->addChild(pSprite, zLoginSceneBackground);
        
        bRet = true;
    } while (0);
    return bRet;
}

CCScene* LoginScene::scene()
{
    CCScene *scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        LoginScene *layer = LoginScene::create();
        CC_BREAK_IF(! layer);
        scene->addChild(layer);
    } while(0);
    return scene;
}

void LoginScene::onEnter()
{
    CCLayer::onEnter();
    USER_INFO; // 清空并初始化用户信息
    CCLOG("LoginScene::onEnter");
    CCDirector::sharedDirector()->getScheduler()->
        scheduleSelector(schedule_selector(LoginScene::update), this, 0, false);
    //?? 区别
    //scheduleUpdate();
    //this->schedule(schedule_selector(LoginScene::update));
}

void LoginScene::onButtonClose(CCObject* pSender)
{
    CCLog("LoginScene::onClose");
    CCDirector::sharedDirector()->end();
    exit(0);
}

void LoginScene::onButtonLogin(CCObject* pSender)
{
    CCLog("LoginScene::onLogin");
    CCLog("UserName: %s UserPassword: %s", USER_INFO->getUserName().c_str(), USER_INFO->getUserPassword().c_str());
    //USER_INFO->setUserState(1);
    login();
    CCLog("login finish, wait for connector callback(client thread)");
}

void LoginScene::editBoxEditingDidBegin(CCEditBox *editBox)
{
    CCLog("LoginScene::editBoxEditingDidEnd %p", editBox);
}

void LoginScene::editBoxEditingDidEnd(CCEditBox *editBox)
{
    CCLog("LoginScene::editBoxEditingDidEnd %p", editBox);
}

void LoginScene::editBoxTextChanged(CCEditBox *editBox, const string& text)
{
    if (editBox == _editBoxUserName) {
        USER_INFO->setUserName(text);
        CCLog("LoginScene::editBoxTextChanged %p userName: %s", editBox, text.c_str());
    } else {
        USER_INFO->setUserPassword(text);
        CCLog("LoginScene::editBoxTextChanged %p userPassword: %s", editBox, text.c_str());
    }
}

void LoginScene::editBoxReturn(CCEditBox *editBox)
{
    CCLog("LoginScene::editBoxReturn %p", editBox);
}

void LoginScene::update(float delta) {
    if(USER_INFO->getUserState() == 1) {
        CCLog("LoginScene::update enter GameScene");
        CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
        CCScene* pScene = GameScene::createSelf();
        if(pScene && pScene->init()) {
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, pScene));
        }
        //CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, CCScene::create()));
        CCLog("LoginScene::update enter GameScene ~");
    }
}

void LoginScene::login() {
    Network::login();
}



