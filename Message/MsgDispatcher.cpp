//
//  MsgDispatcher.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-13.
//
//

#include "MsgDispatcher.h"
#include "Network.h"
#include "MsgHandler.h"

void MsgDispatcher::consumeNewServerMsg() {
    queue<json_t*>& jsonQueue = *USER_INFO->getRecvJsonQueue();
    while(!jsonQueue.empty()) {
        msgFromServer(jsonQueue.front());
        jsonQueue.pop();
    }
}

void MsgDispatcher::msgFromClient(MsgBase* msgBase) {
    msgBase->serializeToJson();
    json_t* jsonMsg = msgBase->getSerializedJson();
    Network::send(jsonMsg);
}

void MsgDispatcher::msgFromServer(json_t* msgJson) {
    MsgBase* msgBase = MsgBase::factory(msgJson);
    if(!msgBase) {
        CCLog("MsgDispatcher::msgFromServer unknown msgType");
        return;
    }
    MsgPath* msgPath = static_cast<MsgPath*>(msgBase);
    //CCLog("MsgType: %d, ObjectId: %d, RoleState: %d, DestX: %f, DestY: %f",
    //      msgPath->getMsgType(), msgPath->getObjectId(), msgPath->getRoleState(),
    //      msgPath->getDest().x, msgPath->getDest().y);
    json_decref(msgJson);
    // 此处是否要先排队再分发？还是分发了到角色里再排队？还是排2次队？
    map<int, MsgHandler*>::iterator iFind = _objectIdMap.find(msgBase->getObjectId());
    if(iFind != _objectIdMap.end()) {
        iFind->second->handleServerMsg(msgBase);
    }
}

void MsgDispatcher::removeObject(int objectId) {
    map<int, MsgHandler*>::iterator iFind = _objectIdMap.find(objectId);
    if(iFind != _objectIdMap.end()) {
        _objectIdMap.erase(iFind);
    }
}

void MsgDispatcher::addObject(MsgHandler* object) {
    _objectIdMap.insert(map<int, MsgHandler*>::value_type(object->getObjId(), object));
}

bool MsgDispatcher::isObjectRegistered(int objectId) {
    return _objectIdMap.find(objectId) != _objectIdMap.end();
}

void MsgDispatcher::cleanAllRegisters() {
    _objectIdMap.clear();
}



