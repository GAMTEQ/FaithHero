//
//  UserInfo.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-6.
//
//

#include "UserInfo.h"

UserInfo* UserInfo::sharedUserInfo() {
    static UserInfo userInfo;
    return &userInfo;
}

