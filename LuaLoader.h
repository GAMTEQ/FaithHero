//
//  ConfigLoader.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-27.
//
//

#ifndef mmo2_ConfigLoader_h
#define mmo2_ConfigLoader_h

#include "Base.h"

#include "CCLuaEngine.h"
extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

class LuaLoader {
public:
    LuaLoader();
    ~LuaLoader();
    bool reloadScript(const char* scriptFile, const char* resultTable);
private:
    void clean(); //!!! 清空栈和字典，如果加载其他lua脚本，用不用重生成lua_State?
    void parseLuaTable(lua_State* l);
    void loadTableToDict(lua_State* l, const char* resultTable);
private:
    lua_State* _luaState;
    CCDictionary* _resultDict;
};

#endif
