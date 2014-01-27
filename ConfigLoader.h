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

class ConfigLoader {
public:
    ConfigLoader();
    ~ConfigLoader();
    bool loadConfigScript();
private:
    lua_State* _luaState;
};

#endif
