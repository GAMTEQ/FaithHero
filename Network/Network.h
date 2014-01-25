//
//  Network.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-9.
//
//

#ifndef mmo2_NetworkCb_h
#define mmo2_NetworkCb_h

#include "Base.h"
#include "MsgBase.h"

class Network {
public:
    static void login(); // 登录
    static void send(json_t* jsonMsg); // 发送消息
    static void requstGateCb(pc_request_t *req, int status, json_t *resp); // 请求gate
    static void requstConnectorCb(pc_request_t *req, int status, json_t *resp); // 请求connector
    static void requestSendCb(pc_request_t *req, int status, json_t *resp); // 发送消息回调
    static void onEventDisconnectCb(pc_client_t *client, const char *event, void *data); // 断开回调
    static void onEventChatCb(pc_client_t *client, const char *event, void *data); // 聊天回调
    static void onEventAddCb(pc_client_t *client, const char *event, void *data); // 增加新用户回调
    static void onEventLeaveCb(pc_client_t *client, const char *event, void *data); // 用户离开回调
};

#endif
