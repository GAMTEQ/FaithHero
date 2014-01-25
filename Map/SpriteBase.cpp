#include "SpriteBase.h"
#include "Map.h"
#include "PathFinder.h"

void BaseSprite::setPosition(const CCPoint& pos) {
	CCSprite::setPosition(pos);
	_tilePos = _map->realPosToTilePos(getPosition());
    changeZOrder(_tilePos);
}

void BaseSprite::changeZOrder(const CCPoint& tilePos) {
	int tileZorder = tilePos.x + tilePos.y;
	setZOrder(tileZorder);
}

void BaseSprite::drawTexture(const char *pszFilename, CCRect& textureRect, const CCPoint& anchorPoint) {
	CCImage* blankImage = new CCImage; // 加载图片
	blankImage->initWithImageFile(pszFilename);
	blankImage->autorelease();
	CCTexture2D* blankTexture2D = new CCTexture2D; // 用图片制作纹理
	blankTexture2D->initWithImage(blankImage);
	blankTexture2D->autorelease();
	this->setTexture(blankTexture2D);
	this->setTextureRect(textureRect);
	this->setAnchorPoint(anchorPoint);
}
