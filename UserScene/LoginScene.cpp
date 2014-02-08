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
        CCScale9Sprite* editbox9Sprite = CCScale9Sprite::create("editbox.png");
        _editBoxUserName = CCEditBox::create(editbox9Sprite->boundingBox().size, editbox9Sprite, NULL, NULL);
        _editBoxUserName->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                          visibleOrigin.y + visibleSize.height * 0.6));
        _editBoxUserName->setFont("American Typewriter", 50);
        _editBoxUserName->setFontColor(ccWHITE);
        _editBoxUserName->setPlaceHolder("用户名：");
        _editBoxUserName->setPlaceholderFontColor(ccWHITE);
        _editBoxUserName->setMaxLength(8);
        _editBoxUserName->setReturnType(kKeyboardReturnTypeDone);
        _editBoxUserName->setDelegate(this);
        this->addChild(_editBoxUserName, zLoginSceneEditBox);
        // 密码
        CCScale9Sprite* editbox9Sprite1 = CCScale9Sprite::create("editbox.png");
        _editBoxUserPassword = CCEditBox::create(editbox9Sprite1->boundingBox().size, editbox9Sprite1, NULL, NULL);
        _editBoxUserPassword->setPosition(ccp(_editBoxUserName->getPositionX(),
                                              _editBoxUserName->getPositionY() - editbox9Sprite1->boundingBox().size.height * 1.7));
        _editBoxUserPassword->setFont("American Typewriter", 50);
        _editBoxUserPassword->setFontColor(ccWHITE);
        _editBoxUserPassword->setPlaceHolder("密码：");
        _editBoxUserPassword->setPlaceholderFontColor(ccWHITE);
        _editBoxUserPassword->setMaxLength(8);
        _editBoxUserPassword->setReturnType(kKeyboardReturnTypeDone);
        _editBoxUserPassword->setDelegate(this);
        this->addChild(_editBoxUserPassword, zLoginSceneEditBox);
        // 按钮
        //CCLabelTTF *label = CCLabelTTF::create("Login", "Arial", 50);
        //CCMenuItemLabel *pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(LoginScene::onButtonLogin));
        // 登录按钮
        CCMenuItemImage* menuItemLogin = CCMenuItemImage::create("button_login.png",
                                                                 "button_login_pressed.png",
                                                                 this,
                                                                 menu_selector(LoginScene::onButtonLogin));
        menuItemLogin->setPosition(ccp(_editBoxUserPassword->getPositionX() - menuItemLogin->boundingBox().size.width,
                                       _editBoxUserPassword->getPositionY() - _editBoxUserPassword->boundingBox().size.height * 1.7));
        // 注册按钮
        CCMenuItemImage *menuItemRegister = CCMenuItemImage::create("button_register.png",
                                                                    "button_register_pressed.png",
                                                                    this,
                                                                    menu_selector(LoginScene::onButtonLogin));
        menuItemRegister->setPosition(ccp(_editBoxUserPassword->getPositionX() + menuItemRegister->boundingBox().size.width,
                                          _editBoxUserPassword->getPositionY() - _editBoxUserPassword->boundingBox().size.height * 1.7));
        // 退出按钮
        CCMenuItemImage *menuItemExit = CCMenuItemImage::create("close_normal.png",
                                                                "close_selected.png",
                                                                this,
                                                                menu_selector(LoginScene::onButtonClose));

        menuItemExit->setScale(2.0);
        menuItemExit->setPosition(ccp(visibleOrigin.x + visibleSize.width - menuItemExit->getContentSize().width * menuItemExit->getScale() / 2,
                                    visibleOrigin.y + menuItemExit->getContentSize().height * menuItemExit->getScale() / 2));
        _loginMenu = CCMenu::create(menuItemLogin, menuItemRegister, menuItemExit, NULL);
        _loginMenu->setPosition(CCPointZero);
        this->addChild(_loginMenu, zLoginSceneEditBox);
        // LOGO
        //CCLabelTTF *pLabel = CCLabelTTF::create("Massive-Multiplayer-Online-2", "Arial", 30);
        //CC_BREAK_IF(!pLabel);
        //CCSize size = CCDirector::sharedDirector()->getWinSize();
        //pLabel->setPosition(ccp(size.width / 2, size.height - 30));
        //this->addChild(pLabel, zLoginSceneLogo);
        // 背景图片
        CCSprite *pSprite = CCSprite::create("login_scene_background.png");
        CC_BREAK_IF(!pSprite);
        pSprite->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                 visibleOrigin.y + visibleSize.height / 2));
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
    if(USER_INFO->getUserName() == "" || USER_INFO->getUserPassword() == "") {
        CCLog("no name of pwd input");
        return;
    }
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



