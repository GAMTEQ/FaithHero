//
//  MenuLayer.h
//  mmo2
//
//  Created by 王 崇欢 on 14-2-3.
//
//

#ifndef mmo2_MenuLayer_h
#define mmo2_MenuLayer_h

#include "Base.h"
#include "VisibleRect.h"

//////////////////////////////////////////////////////////////////////////////// CustomTableViewCell

class MenuTableViewCell : public cocos2d::extension::CCTableViewCell
{
public:
	virtual void draw();
};

//////////////////////////////////////////////////////////////////////////////// TableViewTestLayer

class MenuLayer : public cocos2d::CCLayer
                , public cocos2d::extension::CCTableViewDataSource
                , public cocos2d::extension::CCTableViewDelegate
{
// 构造
public:
    static MenuLayer* createSelf();
private:
    bool initSelf();
// 接口
public:
	void toExtensionsMainLayer(cocos2d::CCObject *sender);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
};


#endif
