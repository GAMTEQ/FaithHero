//
//  MsgDispatcher.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-13.
//
//

#ifndef mmo2_MsgDispatcher_h
#define mmo2_MsgDispatcher_h

#include "Base.h"
#include "MsgBase.h"

class MsgHandler;

class MsgDispatcher {
public:
    void msgFromClient(MsgBase* msgBase);// 客户端消息入口
    void consumeNewServerMsg(); // 从服务端消息队列中取消息
public:
    void addObject(MsgHandler* object); // 注册一个对象
    void removeObject(int objectId); // 删除一个对象
    bool isObjectRegistered(int objectId); // 判断一个对象是否注册
    void cleanAllRegisters(); // 清楚所有监听者
private:
    void msgFromServer(json_t* msgJson); // 服务端消息入口
private:
    map<int, MsgHandler*> _objectIdMap; // 根据ObjectId映射到实际对象的指针，暂不引用计数
};

#endif
