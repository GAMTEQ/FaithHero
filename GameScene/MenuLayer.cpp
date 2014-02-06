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
	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize tableviewSize = CCSizeMake(TABLEVIEW_ITEM_WIDTH, TABLEVIEW_ITEM_HEIGHT * TABLEVIEW_ITEM_ORI_NUM);
    CCTableView* tableView = CCTableView::create(this, tableviewSize);
    this->setPosition(ccp(tableviewSize.width / (-2), tableviewSize.height / (-2) - TABLEVIEW_ITEM_HEIGHT));
	tableView->setDirection(kCCScrollViewDirectionVertical); // kCCScrollViewDirectionHorizontal
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setAnchorPoint(ccp(0.5, 0.5));
    tableView->setPosition(VisibleRect::center());
	this->addChild(tableView);
	tableView->reloadData();
	return true;
}



void MenuLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize MenuLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(TABLEVIEW_ITEM_WIDTH, TABLEVIEW_ITEM_HEIGHT);
}

CCTableViewCell* MenuLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCString *string = CCString::createWithFormat("SETUP ITEM %d", idx);
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new MenuTableViewCell();
        cell->autorelease();
        CCSprite *sprite = CCSprite::create("Item.png");
        sprite->setAnchorPoint(ccp(0, 0));
        sprite->setPosition(ccp(0, 0));
        cell->addChild(sprite);
        
        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 50.0);
        label->setPosition(ccp(sprite->boundingBox().size.width / 2, sprite->boundingBox().size.height / 2));
		label->setAnchorPoint(ccp(0.5, 0.5));
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
    return 10;
}

