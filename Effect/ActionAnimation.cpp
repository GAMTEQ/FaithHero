#include "ActionAnimation.h"

ActionAnimation* ActionAnimation::createSelf(const string& frameFileHead, int frameNum, float interval) {
	ActionAnimation* self = new ActionAnimation;
	if(self && self->initSelf(frameFileHead, frameNum, interval)) {
		self->autorelease();
		return self;
	}
	CC_SAFE_DELETE(self);
	return NULL;		
}
ActionAnimation::~ActionAnimation() {
	CC_SAFE_RELEASE_NULL(_frameArray);
}

TEActionState ActionAnimation::continueAction(CCSprite* sprite, float dt) {
	_curTimeAccumulate += dt; // 累积时间
	if(_curTimeAccumulate >= _resetInterval) { // 如果超过切换间隔
		_curTimeAccumulate = _curTimeAccumulate - _resetInterval; // 清空时间累计器并且以超过的时间为起始值
		sprite->setDisplayFrame((CCSpriteFrame*)_frameArray->objectAtIndex(_curFrameId));
		_curFrameId = _curFrameId == _frameCount - 1 ? 0 : _curFrameId + 1; // 改变帧ID
		if(_curFrameId == 0) // 进入第一帧前,是动作开始状态
			return eActionStateBegin;
		if(_curFrameId == _frameCount - 1) // 进入最后一帧前,是动作结束状态
			return eActionStateEnd;
		if(_curFrameId == _frameCount / 2) // 进入中间帧前,是动作中间状态
			return eActionStateMiddle;
	}
	return eActionStateNone; // 在帧和帧之间时,是无动作状态
}

bool ActionAnimation::initSelf(const string& frameFileHead, int frameNum, float interval) {
	_resetInterval = interval;
	_frameCount = frameNum;
	_curFrameId = 0;
	_curTimeAccumulate = 0.0f;
	_frameArray = createFrameArray(frameFileHead, frameNum);
	_frameArray->retain();
	return true;
}

void ActionAnimation::resetAction() {
	_curFrameId = 0;
	_curTimeAccumulate = 0.0f;
}


CCArray* ActionAnimation::createFrameArray(const string& frameFileHead, int frameNum) {
	CCArray* frameArray = CCArray::createWithCapacity(frameNum);
	for(int i = 0; i != frameNum; ++i) {
		stringstream fileHead;
		fileHead << frameFileHead << 0 << i << ".png";
		//CCLog("frameName: %s", fileHead.str().c_str());
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileHead.str().c_str());
		frameArray->addObject(frame);
	}
	return frameArray;
}

ActionAnimationDirect* ActionAnimationDirect::createSelf(vector<string> frameNameVector, unsigned frameCount, float interval) {
	ActionAnimationDirect* self = new ActionAnimationDirect;
	if(self && self->initSelf(frameNameVector, frameCount, interval)) {
		self->autorelease();
		return self;
	}
	CC_SAFE_DELETE(self);
	return NULL;
}

ActionAnimationDirect::~ActionAnimationDirect() {
	CC_SAFE_RELEASE_NULL(_actionAnimationArray);
}

bool ActionAnimationDirect::initSelf(vector<string> frameNameVector, unsigned frameCount, float interval) {
	_lastDirect = eDirectUp;
	_actionAnimationArray = CCArray::createWithCapacity(8);
	_actionAnimationArray->retain();
	for(vector<string>::iterator iter = frameNameVector.begin();
		iter != frameNameVector.end(); ++iter)
	{
		_actionAnimationArray->addObject(ActionAnimation::createSelf(*iter, frameCount, interval));
	}
	return true;
}

TEActionState ActionAnimationDirect::runDirect(CCSprite* sprite, TEDirect direct, float dt) {
	ActionAnimation* action = (ActionAnimation*)_actionAnimationArray->objectAtIndex(direct);
	if(direct != _lastDirect) {
		_lastDirect = direct;
		action->resetAction();
	}
	return action->continueAction(sprite, dt);
}


