#ifndef _DAMAGE_NUM_POOL__H_
#define _DAMAGE_NUM_POOL__H_

#include "Base.h"
#include "SpriteBase.h"

class DamageNumPool : public CCObject {
// 构造
public:
	static DamageNumPool* createSelf(unsigned capacity) {
		DamageNumPool* self = new DamageNumPool;
		if(self && self->initSelf(capacity)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
	~DamageNumPool() {
		for(list<CCLabelBMFont*>::iterator itr = _holdList.begin();
			itr != _holdList.end(); ++itr)
		{
			(*itr)->removeFromParent();
			CC_SAFE_RELEASE_NULL(*itr);
		}
	}
protected:
	bool initSelf(unsigned capacity) {
		_capacity = 0;
		_fontScale = 0.3f;
		addNewElement(capacity);
		return true;
	}
// 接口
public:
	CCLabelBMFont* allocate() {
		if(_unusedSet.size() == 0) { // 如果超出pool的容量,就新生成一个
			CCLabelBMFont* damageNum = CCLabelBMFont::create("0", "font.fnt");
			damageNum->retain();
			damageNum->setAnchorPoint(ccp(0.5, 0.5));
			damageNum->setPosition(INVALID_POINT);
			return damageNum;
		}
		CCLabelBMFont* ret = *(_unusedSet.begin());
		_unusedSet.erase(_unusedSet.begin());
		_usedSet.insert(set<CCLabelBMFont*>::value_type(ret));
		return ret;
	}
	void recycle(CCLabelBMFont* font) {
		set<CCLabelBMFont*>::iterator iFind = _usedSet.find(font);
		if(iFind == _usedSet.end()) { // 如果不在集合中,说明是新生成的,所以释放掉
			CC_SAFE_RELEASE_NULL(font);
			return;
		}
		_unusedSet.insert(set<CCLabelBMFont*>::value_type(font));
		_usedSet.erase(iFind);
	}
protected: // 工具
	void addNewElement(unsigned num) {
		for(int i = 0; i != num; ++i) {
			CCLabelBMFont* damageNum = CCLabelBMFont::create("0", "font.fnt");
			damageNum->retain();
			damageNum->setScale(_fontScale);
			damageNum->setAnchorPoint(ccp(0.5, 0.5));
			damageNum->setPosition(INVALID_POINT);
			_unusedSet.insert(set<CCLabelBMFont*>::value_type(damageNum));
			_holdList.push_back(damageNum);
		}
		_capacity += num;
	}
// 属性
protected:
	int _capacity;
	int _step;
	float _fontScale;
	set<CCLabelBMFont*> _usedSet;// 已使用资源列表
	set<CCLabelBMFont*> _unusedSet;// 未使用资源列表
	list<CCLabelBMFont*> _holdList; // 持有new出来的指针
};

#endif // _DAMAGE_NUM_POOL__H_
