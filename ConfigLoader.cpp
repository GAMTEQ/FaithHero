//
//  ConfigLoader.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-27.
//
//

#include "ConfigLoader.h"

ConfigLoader::ConfigLoader() {
    _luaState = NULL;
    _luaState = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();
}

ConfigLoader::~ConfigLoader() {
    if(_luaState) {
        lua_close(_luaState);
    }
}

bool ConfigLoader::loadConfigScript() {
    assert(_luaState);
    // 加载Lua标准库
    luaopen_base(_luaState);
    luaopen_math(_luaState);
    luaopen_string(_luaState);
    // 执行Lua脚本，返回0代表成功
    const char* baseConfigFileName = "Script/BaseConfig.lua";
    int error = luaL_dofile(_luaState, CCFileUtils::sharedFileUtils()->fullPathForFilename(baseConfigFileName).c_str());
    CCLog("ConfigLoader::loadConfigScript Lua do file %s return %d", baseConfigFileName, error);
    lua_settop(_luaState, 0);
    // 将lua脚本中的全局变量压入栈
    lua_getglobal(_luaState, "varString");
    lua_getglobal(_luaState, "varInteger");
    // 判断栈顶元素是否为string，返回非0代表成功
    int isStr = lua_isstring(_luaState, 1); // 1表示栈上第一个元素，即最先压入的元素
    CCLog("is stack 1 string %d", isStr);
    // 根据索引从栈上取值
    const char* varString = lua_tostring(_luaState, 1);
    CCLog("stack 1 string %s", varString);
    int varInteger = lua_tointeger(_luaState, 2);
    CCLog("stack 2 int %d", varInteger);
    // 弹出栈顶
    lua_pop(_luaState, 2);
    varInteger = lua_tointeger(_luaState, 2);
    CCLog("stack 2 int %d", varInteger); // 弹出栈顶后发现该元素被清除了
    return true;
}