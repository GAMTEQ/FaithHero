//
//  Network.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-9.
//
//

#include "Network.h"

void Network::login() {
    CCLog("Network::login");
    // 创建连接实例
    // 这个连接是用来和gate交互的，所以同步等待其处理完毕后，就销毁掉
    pc_client_t *client = pc_client_new();
    
    // 准备发起连接
    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(GATE_PORT);
    address.sin_addr.s_addr = inet_addr(GATE_HOST);
    
	// 为连接添加事件回调：连接关闭
    pc_add_listener(client, PC_EVENT_DISCONNECT, PC_EVENT_CB_SELECTOR(Network::onEventDisconnectCb));
    
    // 发起连接
    if(pc_client_connect(client, &address)) {
        CCLOG("fail to connect server.\n");
        pc_client_destroy(client);
        return;
    }
    
    // 准备请求gate服务
    const char *route = "gate.gateHandler.queryEntry";
    json_t *msg = json_object(); // 创建json对象，计数＋1
    json_t *str = json_string(USER_INFO->getUserName().c_str()); // 创建json对象，计数＋1
    json_object_set(msg, "uid", str);
    json_decref(str); // 释放json对象，计数－1
    pc_request_t *request = pc_request_new();//!! 登录成功的回调
    
    // 请求gate服务
    pc_request(client, request, route, msg, PC_REQUEST_CB_SELECTOR(Network::requstGateCb));
    
    // 主线程等待client线程结束，即等待Network::requstGateCb执行完毕
    pc_client_join(client);
    
    // 释放各种资源
    //json_decref(msg); //!! 千万不要释放这个对象！大bug
    pc_client_destroy(client); // client在Network::requstGateCb中已经stop，此处destroy
    CCLog("Network::login ~");
}

void Network::send(json_t* jsonMsg) {
    CCLog("Network::send");
    // 把要发送的消息转成json格式
    //json_t *msg = msgBase.getSerializedJson();
    
    // 确定路由和请求对象
    const char *route = "chat.chatHandler.send";
    pc_request_t *request = pc_request_new();
    
    // 请求
    pc_request(USER_INFO->getPomeloClient(), request, route, jsonMsg, PC_REQUEST_CB_SELECTOR(requestSendCb));
    CCLog("Network::send ~");
}

void Network::requestSendCb(pc_request_t *req, int status, json_t *resp) {
    CCLog("Network::requestSendCb");
    json_t *msg = req->msg;
    json_decref(msg);
    pc_request_destroy(req);
    CCLog("Network::requestSendCb ~");
}

// 请求gate回调
void Network::requstGateCb(pc_request_t *req, int status, json_t *resp) {
    CCLog("Network::requstGateCb");
    if(status == -1) {
        CCLOG("Network::requstGateCallback Fail to send request to server.\n");
    }
    else if(status == 0) {
        // 获得gate返回的connector端口
        // 注意，gate的端口是3014，而登录返回回来的端口是3050，返回的是connector的端口
        USER_INFO->setConnectorHost(json_string_value(json_object_get(resp, "host")));
        USER_INFO->setConnectorPort(json_number_value(json_object_get(resp, "port")));
        CCLOG("connector host: %s port: %d", USER_INFO->getConnectorHost().c_str(), USER_INFO->getConnectorPort());
        
        // 创建连接实例
        // 这个连接是和connector交互的，要保存下来，不能销毁
        pc_client_t *client = pc_client_new();
        
        // 准备发起连接
        struct sockaddr_in address;
        memset(&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_port = htons(USER_INFO->getConnectorPort());
        address.sin_addr.s_addr = inet_addr(USER_INFO->getConnectorHost().c_str());
        
        // 为连接添加事件回调
        pc_add_listener(client, PC_EVENT_DISCONNECT, PC_EVENT_CB_SELECTOR(Network::onEventDisconnectCb));
        pc_add_listener(client, "onChat", PC_EVENT_CB_SELECTOR(Network::onEventChatCb));
        pc_add_listener(client, "onAdd", PC_EVENT_CB_SELECTOR(Network::onEventAddCb));
        pc_add_listener(client, "onLeave", PC_EVENT_CB_SELECTOR(Network::onEventLeaveCb));
        
        // 发起连接
        if(pc_client_connect(client, &address)) {
            CCLOG("fail to connect server.\n");
            pc_client_destroy(client);
            return ;
        }
        
        // 准备
        const char *route = "connector.entryHandler.enter";
        json_t *msg = json_object();
        json_t *jsonStrUserName = json_string(USER_INFO->getUserName().c_str());
        json_t *jsonStrUserPassword = json_string(USER_INFO->getUserPassword().c_str());
        json_object_set(msg, "username", jsonStrUserName);
        json_object_set(msg, "rid", jsonStrUserPassword);
        pc_request_t *request = pc_request_new();
        
        // 向connector发起请求
        pc_request(client, request, route, msg, PC_REQUEST_CB_SELECTOR(Network::requstConnectorCb));
        
        // 这个client不join，因为要让这个线程独立运行
        json_decref(jsonStrUserName);
        json_decref(jsonStrUserPassword);
    }
    
    // 清除pc_request_t的相关资源
    json_decref(req->msg);
    pc_client_stop(req->client); // 停止gate连接
    pc_request_destroy(req);
    
    CCLog("Network::requstGateCb ~");
}

void Network::requstConnectorCb(pc_request_t *req, int status, json_t *resp) {
    CCLog("Network::requstConnectorCb");
    USER_INFO->setError(0);
    if(status == -1) {
        CCLOG("Network::requstConnectorCb Fail to send request to server.\n");
    }
    else if(status == 0) {
        // 把服务端返回值json转换成字符串打印出来
        char* jsonStr = json_dumps(resp, 0);
        if(jsonStr != NULL) {
            CCLog("resp json str: %s", jsonStr);
            free(jsonStr);
        }
        
        // 如果出错则返回
        if(json_object_get(resp, "error")) {
            USER_INFO->setError(1);
            CCLOG("connect error");
            return;
        }
        // 保存连接对象
        USER_INFO->setPomeloClient(req->client);
        USER_INFO->setUserState(1);
        
        // 把其他用户列表保存下来
		json_t* users = json_object_get(resp, "users");
        for (unsigned i = 0; i < json_array_size(users); ++i) {
            json_t* val = json_array_get(users, i);
            USER_INFO->getUserQueue()->addObject(CCString::create(json_string_value(val)));
        }
    }
    
    // 清除pc_request_t的相关资源
    json_t *msg = req->msg;
    json_decref(msg);
    pc_request_destroy(req);
    CCLog("Network::requstConnectorCb ~");
}

// 断开连接事件回调
void Network::onEventDisconnectCb(pc_client_t *client, const char *event, void *data)
{
    CCLOG("client closed: %d.\n", client->state);
}

// 聊天信息事件回调
void Network::onEventChatCb(pc_client_t *client, const char *event, void *data)
{
    json_t* jsonMsg = (json_t*)data;
    json_incref(jsonMsg); // 01 incref !!!注意这个消息不是临时用的，而是要保存入队列的，所以必须json_incref/json_decref
    const char* strMsg = json_dumps(jsonMsg, 0);
    CCLOG("Network::onEventChatCb: %s %s", event, strMsg);
    USER_INFO->addRecvJsonQueue(jsonMsg); // 把从服务器收到的聊天信息保存到消息队列里！
    //USER_INFO->getMessageQueue()->addObject(CCString::create(msg));
    return;
}

// 新增用户事件回调
void Network::onEventAddCb(pc_client_t *client, const char *event, void *data)
{
    json_t* json = (json_t*)data;
    json_t* user = json_object_get(json, "user");
    const char* msg = json_string_value(user);
    CCLOG("Network::onEventAddCb: %s %s", event, msg);
    USER_INFO->getUserQueue()->addObject(CCString::create(msg));
    return;
}

// 用户离开事件回调
void Network::onEventLeaveCb(pc_client_t *client, const char *event, void *data)
{
    json_t* json = (json_t* )data;
    json_t* user = json_object_get(json, "user");
    const char* msg = json_string_value(user);
    CCLOG("Network::onEventLeaveCb: %s %s", event, msg);
    for(unsigned i=0; i < USER_INFO->getUserQueue()->count(); ++i) {
        CCString* userStr = (CCString*)(USER_INFO->getUserQueue()->objectAtIndex(i));
        if(strcmp(userStr->getCString(), msg) == 0) {
            USER_INFO->getUserQueue()->removeObjectAtIndex(i);
            break;
        }
    }
    return;
}
