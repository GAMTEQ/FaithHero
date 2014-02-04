//
//  MenuLayer.cpp
//  mmo2
//
//  Created by 王 崇欢 on 14-2-3.
//
//

#include "MenuLayer.h"

//////////////////////////////////////////////////////////////////////////////// CustomTableViewCell

void MenuTableViewCell::draw()
{
	CCTableViewCell::draw();
	// draw bounding box
    // 	CCPoint pos = getPosition();
    // 	CCSize size = CCSizeMake(178, 200);
    // 	CCPoint vertices[4]={
    // 		ccp(pos.x+1, pos.y+1),
    // 		ccp(pos.x+size.width-1, pos.y+1),
    // 		ccp(pos.x+size.width-1, pos.y+size.height-1),
    // 		ccp(pos.x+1, pos.y+size.height-1),
    // 	};
    // 	ccDrawColor4B(0, 0, 255, 255);
    // 	ccDrawPoly(vertices, 4, true);
}

//////////////////////////////////////////////////////////////////////////////// MenuLayer

MenuLayer* MenuLayer::createSelf() {
	MenuLayer* layer = new MenuLayer;
	if(layer && layer->initSelf()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool MenuLayer::initSelf() {
	if(!CCLayer::init()) {
		return false;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCTableView* tableView = CCTableView::create(this, CCSizeMake(250, 60));
    tableView->setDirection(kCCScrollViewDirectionHorizontal);
    tableView->setPosition(ccp(20,winSize.height/2-30));
    tableView->setDelegate(this);
    this->addChild(tableView);
    tableView->reloadData();
    
	tableView = CCTableView::create(this, CCSizeMake(60, 250));
	tableView->setDirection(kCCScrollViewDirectionVertical);
	tableView->setPosition(ccp(winSize.width-150,winSize.height/2-120));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(tableView);
	tableView->reloadData();
    
	// Back Menu
	CCMenuItemFont *itemBack = CCMenuItemFont::create("Back", this, menu_selector(MenuLayer::toExtensionsMainLayer));
	itemBack->setPosition(ccp(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 25));
	CCMenu *menuBack = CCMenu::create(itemBack, NULL);
	menuBack->setPosition(CCPointZero);
	addChild(menuBack);
	return true;
}

void MenuLayer::toExtensionsMainLayer(cocos2d::CCObject *sender)
{
}

void MenuLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize MenuLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    if (idx == 2) {
        return CCSizeMake(100, 100);
    }
    return CCSizeMake(60, 60);
}

CCTableViewCell* MenuLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCString *string = CCString::createWithFormat("%d", idx);
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new MenuTableViewCell();
        cell->autorelease();
        CCSprite *sprite = CCSprite::create("Item.png");
        sprite->setAnchorPoint(CCPointZero);
        sprite->setPosition(ccp(0, 0));
        cell->addChild(sprite);
        
        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
        label->setPosition(CCPointZero);
		label->setAnchorPoint(CCPointZero);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(string->getCString());
    }
    
    
    return cell;
}

unsigned int MenuLayer::numberOfCellsInTableView(CCTableView *table)
{
    return 20;
}

