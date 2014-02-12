//
//  Header.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-6.
//
//

#ifndef mmo2_LoginScene_h
#define mmo2_LoginScene_h

#include "Base.h"

class LoginScene : public cocos2d::CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
// 构造
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(LoginScene);
// 继承
public:
	// cocos2d-x events callback
	virtual void onEnter();
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
// 接口
public:
    virtual void draw();
    void onButtonLogin(CCObject* pSender);
    void onButtonClose(CCObject* pSender);
	void update(float delta); // cocos2d-x schedule handle callback
// 实现
private:
    void login();
// 成员
private:
	CCEditBox* _editBoxUserName;
    CCEditBox* _editBoxUserPassword;
    CCMenu* _loginMenu;
    bool _loginSuccess;
};


#endif
