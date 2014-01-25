#ifndef _NAME_BOARD__H_
#define _NAME_BOARD__H_

#include "Base.h"
#include "SpriteBase.h"

class NameBoard : public CCSprite {
// 构造
public:
	static NameBoard* createSelf(const string& frameName, const string& name, unsigned level) {
		NameBoard* self = new NameBoard;
		if(self && self->initSelf(frameName, name, level)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf(const string& frameName, const string& name, unsigned level) {
		if(CCSprite::init()) {
			this->initWithSpriteFrameName(frameName.c_str());
			this->setAnchorPoint(ccp(0, 0));
			_boardSize = this->getContentSize();
			_nameOriRelPos = ccp(0, 0);
			_nameEndRelPos = ccp(2.0 / 3.0, 1.0);
			_levelOriRelPos = ccp(2.0 / 3.0, 0.0);
			_levelEndRelPos = ccp(1.0, 1.0);
			_nameOriPos = ccp(_boardSize.width * _nameOriRelPos.x, _boardSize.height * _nameOriRelPos.y);
			_nameEndPos = ccp(_boardSize.width * _nameEndRelPos.x, _boardSize.height * _nameEndRelPos.y);
			_levelOriPos = ccp(_boardSize.width * _levelOriRelPos.x, _boardSize.height * _levelOriRelPos.y);
			_levelEndPos = ccp(_boardSize.width * _levelEndRelPos.x, _boardSize.height * _levelEndRelPos.y);
			_name = CCLabelTTF::create(
				name.c_str(), 
				"Comic Sans MS", 
				28, 
				CCSizeMake(_nameEndPos.x - _nameOriPos.x, _nameEndPos.y - _nameOriPos.y),
				kCCTextAlignmentCenter,
				kCCVerticalTextAlignmentCenter);
			_name->setAnchorPoint(ccp(0, 0));
			_name->setPosition(_nameOriPos);
			this->addChild(_name);
			char levelStr[16] = "Lv.";
			//itoa(level, levelStr + 3, 10);
			level = 10;
            _level = CCLabelTTF::create(
				levelStr, 
				"Comic Sans MS", 
				28, 
				CCSizeMake(_levelEndPos.x - _levelOriPos.x, _levelEndPos.y - _levelOriPos.y),
				kCCTextAlignmentLeft,
				kCCVerticalTextAlignmentCenter);
			_level->setAnchorPoint(ccp(0, 0));
			_level->setPosition(_levelOriPos);
			//_level->setColor(ccc3(255, 255, 0));
			this->addChild(_level);
			return true;
		}
		return false;
	}
// 接口
public:
	bool update(double dt) {
		return true;
	}
// 属性
protected:
	CCLabelTTF* _name;
	CCLabelTTF* _level;
protected: // 比例位置
	CCSize _boardSize;
	CCPoint _nameOriRelPos;
	CCPoint _nameEndRelPos;
	CCPoint _levelOriRelPos;
	CCPoint _levelEndRelPos;
protected: // 实际位置
	CCPoint _nameOriPos;
	CCPoint _nameEndPos;
	CCPoint _levelOriPos;
	CCPoint _levelEndPos;
};

#endif // _NAME_BOARD__H_
