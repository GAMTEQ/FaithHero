//
//  Msg.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-13.
//
//

#ifndef mmo2_Msg_h
#define mmo2_Msg_h

#include "Base.h"

enum {
    eMsgTypePath = 1,
    eMsgTypeTarget = 2
};

//////////////////////////////////////////////////////////////////////////////// 消息基类

class MsgBase : public CCObject {
public:
    static MsgBase* factory(json_t* jsonMsg); // 根据json构造消息
    virtual void serializeToJson(); // 把消息序列化成json
    virtual json_t* getSerializedJson(); // 从消息中把json取出
protected:
    MsgBase();
    virtual void deserializeByJson(json_t* msgJson); // 把json反序列化成消息（收到服务端发来的json，初始化消息）
protected:
    CC_SYNTHESIZE(int, _objectId, ObjectId); // 接受消息的对象ID：比如NPC，玩家，主角，聊天模块，背包，物品
    CC_SYNTHESIZE(int, _msgType, MsgType); // 消息类型：比如移动，攻击，死亡，下线
    CC_SYNTHESIZE(int, _fromUserId, FromUserId); // 消息的发送用户
    CC_SYNTHESIZE(int, _toUserId, ToUserId); // 消息的接收用户（一般是0，表示广播）
    CC_SYNTHESIZE(string, _sendTime, SendTime); // 发送消息的时刻（服务端时间）
    CC_SYNTHESIZE(string, _recvTime, RevcTime); // 收到消息的时刻（服务端时间）
    json_t* _jsonMsg; // 只保存序列化的结果，不保存从网络收到的json
};

//////////////////////////////////////////////////////////////////////////////// 寻路消息

class MsgPath : public MsgBase {
public:
    static MsgPath* createSelf();
    virtual void serializeToJson();
protected:
    MsgPath();
    virtual void deserializeByJson(json_t* msgJson);
protected:
    CC_SYNTHESIZE(CCPoint, _dest, Dest); // 目标位置
};

//////////////////////////////////////////////////////////////////////////////// 动作消息

class MsgTarget : public MsgBase {
public:
    static MsgTarget* createSelf();
    virtual void serializeToJson();
protected:
    MsgTarget();
    virtual void deserializeByJson(json_t* msgJson);
protected:
    CC_SYNTHESIZE(int, _target, Target); // 目标
};


#endif
