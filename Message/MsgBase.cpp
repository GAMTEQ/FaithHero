//
//  MsgBase.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-13.
//
//

#include "MsgBase.h"

//////////////////////////////////////////////////////////////////////////////// MsgBase

MsgBase::MsgBase() {
    _objectId = 0;
    _msgType = 0;
    _fromUserId = 0;
    _toUserId = 0;
    _sendTime = "";
    _recvTime = "";
    _jsonMsg = NULL;
}

MsgBase* MsgBase::factory(json_t* jsonMsg) {
    json_t* msgTypeJson = json_object_get(jsonMsg, "MsgType");
    int msgType = json_integer_value(msgTypeJson);
    MsgBase* pMsgBase = NULL;
    switch(msgType) {
        case eMsgTypePath:
            pMsgBase = MsgPath::createSelf();
            break;
        case eMsgTypeTarget:
            pMsgBase = MsgTarget::createSelf();
            break;
        default:
            break;
    }
    if(!pMsgBase)
        return NULL;
    pMsgBase->deserializeByJson(jsonMsg);
    return pMsgBase;
}

void MsgBase::deserializeByJson(json_t* msgJson) {
    json_t* value = NULL;
    value = json_object_get(msgJson, "ObjectId");
    if(value) _objectId = json_integer_value(value);
    value = json_object_get(msgJson, "MsgType");
    if(value) _msgType = json_integer_value(value);
    value = json_object_get(msgJson, "FromUserId");
    if(value) _fromUserId = json_integer_value(value);
    value = json_object_get(msgJson, "ToUserId");
    if(value) _toUserId = json_integer_value(value);
    value = json_object_get(msgJson, "SendTime");
    if(value) _sendTime = json_string_value(value);
}

void MsgBase::serializeToJson() {
}

json_t* MsgBase::getSerializedJson() {
    return _jsonMsg;
}

//////////////////////////////////////////////////////////////////////////////// MsgPath

MsgPath::MsgPath()
: MsgBase() {
    _dest = CCPointZero;
}

MsgPath* MsgPath::createSelf() {
    MsgPath* pRet = new MsgPath;
    if(pRet) {
        pRet->autorelease();
        return pRet;
    }
    return NULL;
}

void MsgPath::deserializeByJson(json_t* msgJson) {
    MsgBase::deserializeByJson(msgJson);
    json_t* value = NULL;
    value = json_object_get(msgJson, "DestX");
    if(value) _dest.x = json_number_value(value);
    value = json_object_get(msgJson, "DestY");
    if(value) _dest.y = json_number_value(value);
}

void MsgPath::serializeToJson() {
    MsgBase::serializeToJson();
}

//////////////////////////////////////////////////////////////////////////////// MsgTarget

MsgTarget::MsgTarget()
: MsgBase() {
    _target = 0;
}

MsgTarget* MsgTarget::createSelf() {
    MsgTarget* pRet = new MsgTarget;
    if(pRet) {
        pRet->autorelease();
        return pRet;
    }
    return NULL;
}

void MsgTarget::deserializeByJson(json_t* msgJson) {
    MsgBase::deserializeByJson(msgJson);
    json_t* value = NULL;
    value = json_object_get(msgJson, "Target");
    if(value) _target = json_integer_value(value);
}

void MsgTarget::serializeToJson() {
    MsgBase::serializeToJson();
}

/*
 void Serialize::json2Map(json_t* jsonMsg, map<string, float>& contentMap) {
 const char* key = NULL;
 json_t* value = NULL;
 json_object_foreach(jsonMsg, key, value) {
 const char* strValue = json_string_value(value);
 contentMap.insert(map<string, float>::value_type(key, (float)atof(strValue)));
 }
 json_decref(jsonMsg);
 }
 
 void Serialize::map2Json(const map<string, float>& contentMap, json_t* jsonMsg) {
 char valueStr[32];
 for(map<string, float>::const_iterator itr = contentMap.begin();
 itr != contentMap.end(); ++itr)
 {
 snprintf(valueStr, 32, "%.4f", itr->second);
 json_t *valueJson = json_string(valueStr);
 json_object_set(jsonMsg, itr->first.c_str(), valueJson);
 }
 json_object_set(jsonMsg, "rid", json_string(USER_INFO->getUserPassword().c_str()));
 json_object_set(jsonMsg, "from", json_string(USER_INFO->getUserName().c_str()));
 json_object_set(jsonMsg, "target", json_string("*"));
 }
 
 
 void addKv(const char* key, float value) {
 char valueStr[32];
 snprintf(valueStr, 32, "%.4f", value);
 json_t *valueJson = json_string(valueStr);
 json_object_set(_jsonMsg, key, valueJson);
 }
 
*/