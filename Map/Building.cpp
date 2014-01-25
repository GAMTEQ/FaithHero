//
//  Building.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-1-22.
//
//

#include "Building.h"
#include "Map.h"

bool Building::initSelf(Map* map, const char* textureName, const CCPoint& tilePos) {
    if(BaseSprite::initSelf(map)) {
        this->initWithSpriteFrameName(textureName);
        _tileNum = this->boundingBox().size.width / map->getTileSize().width;
        CCLog("_tileNum %d", _tileNum);
        //this->setAnchorPoint(ccp(0.5, double(16.0f / 64.0f)));
        CCPoint oriPos = tilePos;
        for(int i = 0; i != _tileNum; ++i) {
            for(int j = 0; j != _tileNum; ++j) {
                CCPoint curPos = ccp(tilePos.x + i, tilePos.y + j);
                map->_naviGraph.setNodeFlag(curPos, emWall);
                // 在这里改ZORDER，等下setPosition就覆盖掉了！
                // 所以setPosition时要注意！
                // 注意，临界条件不是_tileNum - 1而是_tileNum / 2
                //
                //!!! 问题：如何实现当角色在建筑物背面时显示轮廓而非完全遮盖？
                // 是否可以为角色做一个轮廓精灵，其外形完全和角色一样，zorder最高
                // 这样当角色实体被遮盖时，其轮廓还能显示出来
                // 可以在角色每次更换纹理时，同时为其轮廓精灵也更换纹理，但是后者的纹理不是真实纹理，而是真实纹理的透明化或其他效果
                // 轮廓精灵addChild到角色精灵上，不行。必须也addChild到地图上。但是角色精灵必须持有其指针
                if(i == _tileNum / 2 && j == _tileNum / 2) {
                    changeZOrder(curPos);
                }
            }
        }
        return true;
    }
    return false;
}