//
//  MsgHandler.h
//  mmo2
//
//  Created by 王 崇欢 on 14-1-15.
//
//

#ifndef mmo2_MsgHandler_h
#define mmo2_MsgHandler_h

#include "Base.h"

class MsgBase;
class MsgHandler {
public:
    virtual void handleServerMsg(MsgBase* msgBase) = 0;
    virtual int getObjId() = 0;
};

#endif
