#include "SkillBar.h"
#include "BaseButton.h"

SkillBar* SkillBar::createSelf(unsigned buttonCount, float spacing, const CCSize& buttonSize, bool vertical) {
	SkillBar* self = new SkillBar;
	if(self && self->initSelf(buttonCount, spacing, buttonSize, vertical)) {
		self->autorelease();
		return self;
	}
	CC_SAFE_DELETE(self);
	return NULL;
}

SkillBar::~SkillBar() {
}

bool SkillBar::initSelf(unsigned buttonCount, float spacing, const CCSize& buttonSize, bool vertical) {
	if(CCSprite::init()) {
		_buttonCount = buttonCount;
		_spacing = spacing;
		_buttonSize = buttonSize;
		_vertical = vertical;
		_buttonVector.resize(3, NULL);
		for(int i = 0; i != _buttonCount; ++i) {
			CCSprite* emptyButton = CCSprite::createWithSpriteFrameName("skill_button_empty.png");
			emptyButton->setAnchorPoint(ccp(0.5, 0.5));
			emptyButton->setPosition(getCenterPos(i));
			if(buttonSize.width - emptyButton->getContentSize().width > 25)
				emptyButton->setScale(buttonSize.width / emptyButton->getContentSize().width);
			this->addChild(emptyButton);
		}
		return true;
	}
	return false;
}

bool SkillBar::setButton(BaseButton* newButton, unsigned index) {
	if(newButton == NULL)
		return false;
	if(index < 0 || index >= _buttonCount)
		return false;
	BaseButton* oldButton = (BaseButton*)_buttonVector[index];
	if(oldButton == newButton)
		return false;
	if(oldButton != NULL) { 
		oldButton->removeFromParent();
		CC_SAFE_RELEASE_NULL(oldButton);
	}
	newButton->setPosition(getCenterPos(index));
	_buttonVector[index] = newButton;
	this->addChild(newButton);
	return true;
}

bool SkillBar::removeButton(unsigned index) {
	if(index < 0 || index >= _buttonCount)
		return false;	
	BaseButton* oldButton = (BaseButton*)_buttonVector[index];
	if(oldButton == NULL)
		return false;
	oldButton->removeFromParent();
	CC_SAFE_RELEASE_NULL(oldButton);
	return true;
}

void SkillBar::dealTargetTouch(const CCPoint& pos) {
	for(vector<BaseButton*>::iterator i = _buttonVector.begin();
		i != _buttonVector.end(); ++i)
	{
		if((*i)->boundingBox().containsPoint(pos)) {
			(*i)->singleTouch(pos);
		}
	}
}


