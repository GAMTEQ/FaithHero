//
//  UserInfo.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-6.
//
//

#ifndef mmo2_UserInfo_h
#define mmo2_UserInfo_h

#include "pomelo.h"
#include "uv.h"
#include "jansson.h"

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

#define GATE_HOST "127.0.0.1"
#define GATE_PORT 3014

// typedef void (*pc_request_cb)(pc_request_t *req, int status, json_t *resp);
#define PC_REQUEST_CB_SELECTOR(SELECTOR) (pc_request_cb)(&SELECTOR)
// pc_request(client, request, route, msg, pc_request_cb_selector(ClassName::staticCallBackFuncName));

// typedef void (*pc_event_cb)(pc_client_t *client, const char *event, void *data);
#define PC_EVENT_CB_SELECTOR(SELECTOR) (pc_event_cb)(&SELECTOR)
// pc_add_listener(client, "disconnect", pc_event_cb_selector(ClassName::staticCallBackFuncName));

class UserInfo : public CCObject
{
public:
    UserInfo() {
        _messageQueue = NULL;
        _userQueue = NULL;
        _userList = NULL;
        _pomeloClient = NULL;
        setUserState(0);
        init();
    }
    ~UserInfo() {
        clean();
    }
    static UserInfo* sharedUserInfo();
public:
    void init() {
        clean();
        _userName = "";
        _userPassword = "";
        _connectorHost = "";
        _connectorPort = 0;
        _pomeloClient = NULL;
        _messageQueue = CCArray::create();
        _messageQueue->retain();
        _userQueue = CCArray::create();
        _userQueue->retain();
        _userList = NULL;
        _error = 0;
        setUserState(0);
    }
    void addRecvJsonQueue(json_t* msg) {
        _recvJsonQueue.push(msg);
    }
    queue<json_t*>* getRecvJsonQueue() {
        return &_recvJsonQueue;
    }
private:
    void clean() {
        CC_SAFE_RELEASE_NULL(_messageQueue);
        CC_SAFE_RELEASE_NULL(_userQueue);
        if(_pomeloClient) {
            pc_client_destroy(_pomeloClient);
            _pomeloClient = NULL;
        }
        if(_userList) {
            json_decref(_userList);
            _userList = NULL;
        }
        setUserState(0);
    }
private:
    CC_SYNTHESIZE(int, _userState, UserState); // 用户的状态，0代表未登录，1代表已登录
    CC_SYNTHESIZE(string, _userName, UserName); //用户名
    CC_SYNTHESIZE(string, _userPassword, UserPassword); //密码
    CC_SYNTHESIZE(string, _connectorHost, ConnectorHost); //连接ip
    CC_SYNTHESIZE(int, _connectorPort, ConnectorPort); //连接port
    CC_SYNTHESIZE(pc_client_t*, _pomeloClient, PomeloClient); // pomelo连接对象
    CC_SYNTHESIZE_READONLY(CCArray*, _messageQueue, MessageQueue);
    CC_SYNTHESIZE_READONLY(CCArray*, _userQueue, UserQueue);
    queue<json_t*> _recvJsonQueue; // 回调中收到的json都保存在这个队列里，然后游戏线程逐步消耗
    json_t* _userList;
    CC_SYNTHESIZE(int, _error, Error); // 错误码
};

#endif
