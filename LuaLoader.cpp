//
//  ConfigLoader.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-27.
//
//

#include "LuaLoader.h"

LuaLoader::LuaLoader() {
    _luaState = NULL;
    _luaState = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    // 加载Lua标准库
    luaopen_base(_luaState);
    luaopen_math(_luaState);
    luaopen_string(_luaState);
    // 字典
    _resultDict = CCDictionary::create();
    _resultDict->retain();
}

LuaLoader::~LuaLoader() {
    if(_luaState) {
        lua_close(_luaState);
    }
    CC_SAFE_RELEASE_NULL(_resultDict);
}


void LuaLoader::clean() {
    _resultDict->removeAllObjects();
    lua_settop(_luaState, 0);
}

bool LuaLoader::reloadScript(const char* scriptFile, const char* resultTable) {
    
    // 加载脚本
    
    assert(_luaState);
    clean();
    CCLog("Lua file name: %s, Lua variable name: %s", scriptFile, resultTable);
    // 执行Lua脚本，返回0代表成功
    int error = luaL_dofile(_luaState, CCFileUtils::sharedFileUtils()->fullPathForFilename(scriptFile).c_str()); // "Script/BaseConfig.lua"
    CCLog("ConfigLoader::loadConfigScript return %d", error);
    lua_settop(_luaState, 0);
    
    // 测试变量读取
    
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
    
    // 从脚本中读取resultTable
    
    clean();
    lua_getglobal(_luaState, resultTable);
    //parseLuaTable(_luaState);
    loadTableToDict(_luaState, resultTable);
    return true;
}

void LuaLoader::parseLuaTable(lua_State* l) {
    CCLog("parsing a table");
    if(!lua_istable(l, -1)) {
        CCLog("it is not a table");
        return;
    }
    lua_pushnil(l); // 因为lua_next要首先pop，所以先压入一个nil
    // 调用lua_next首先pop栈一次，然后把栈上-2（即table变量）中的一对kv压栈
    // 如果栈上-2中的table里没有kv了，则返回0
    while(lua_next(l, -2)) {
        CCLog("keyType: %s, valType: %s", luaL_typename(l, -2), luaL_typename(l, -1));
        int keyType = lua_type(l, -2);
        int valType = lua_type(l, -1);
        switch (keyType) {
            case LUA_TNUMBER: {
                CCLog("key: %g", lua_tonumber(l, -2));
                break;
            }
            case LUA_TSTRING: {
                CCLog("key: %s", lua_tostring(l, -2));
                break;
            }
            default:
                break;
        }
        switch (valType) {
            case LUA_TNUMBER: {
                CCLog("val: %g", lua_tonumber(l, -1));
                break;
            }
            case LUA_TSTRING: {
                CCLog("val: %s", lua_tostring(l, -1));
                break;
            }
            case LUA_TTABLE: {
                parseLuaTable(l);
                break;
            }
            default:
                break;
        }
        lua_pop(l, 1); // 弹出value，留下key做下一次迭代
    }
    // 注意，不要对key做lua_tolstring，因为会修改这个栈帧
    CCLog("parsing this table finish");
}

void LuaLoader::loadTableToDict(lua_State* l, const char* resultTable) {
    CCLog("LuaLoader::loadTableToDict");
    if(!lua_istable(l, -1)) {
        CCLog("it is not a table");
        return;
    }
    const int stackTop = lua_gettop(l);
    lua_pushnil(l);
    // 遍历外层table
    while(lua_next(l, stackTop)) {
        string key = lua_tostring(l, -2); // val在栈顶，key在栈顶下面一位
        if(lua_isstring(l, -1)) { // 如果当前栈顶是str
            string value = lua_tostring(l, -1);
            _resultDict->setObject(CCString::createWithFormat("%s", value.c_str()), key);
        }
        else if(lua_istable(l, -1)) { // 如果当前栈顶是tab
            lua_getglobal(l, resultTable); // 把tab压栈
            lua_pushstring(l, key.c_str()); // 把key压栈
            lua_gettable(l, -2); // 弹出key，从tab中取出key，压栈
            lua_rawgeti(l, -1, 1); // 弹出key，从tab中取第一个元素，压栈
            if(lua_tostring(l, -1) == NULL) { // 如果不能转成字符串，说明是kv
                CCDictionary* dict = CCDictionary::create();
                lua_pop(l, 1);
                const int currTop = lua_gettop(l);
                lua_pushnil(l);
                // 遍历内层table
                while(lua_next(l, currTop)) {
                    string innerKey = lua_tostring(l, -2);
                    float innerVal = lua_tonumber(l, -2);
                    dict->setObject(CCInteger::create(innerVal), innerKey);
                    lua_pop(l, 1);
                }
                _resultDict->setObject(dict, key);
            }
            else {
                CCArray* array = CCArray::create();
                lua_pop(l, 1);
                int i = 1;
                do {
                    lua_rawgeti(l, -1, i);
                    if(lua_tostring(l, -1) != NULL) {
                        array->addObject(CCInteger::create(lua_tonumber(l, -1)));
                        lua_pop(l, 1);
                        ++i;
                    }
                    else {
                        lua_pop(l, 1);
                        _resultDict->setObject(array, key);
                        break;
                    }
                } while(1);
            }
            lua_pop(l, 2);
        }
        lua_pop(l, 1);
    }
    lua_settop(l, 0);
}












