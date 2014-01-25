#include "TouchLayer.h"

TouchLayer* TouchLayer::createSelf() {
	TouchLayer* layer = new TouchLayer;
	if(layer && layer->initSelf()) {
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

TouchLayer::~TouchLayer() {
}

bool TouchLayer::initSelf() {
	if(CCLayer::init()) {
		this->setTouchEnabled(true);
		//this->registerWithTouchDispatcher(); 已经setTouchEnabled了不能再设置了!
		_isMoved = false;
		_beganTouch = NULL;
		_beganTouchPos = INVALID_POINT;
		return true;
	}
	return false;
}

void TouchLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	if(touch) {
		_isMoved = false;
		_beganTouch = touch;
		_beganTouchPos = touch->getLocation();
	}
}

void TouchLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent) {
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	if(touch == _beganTouch) {
		_isMoved = true;
		dispatchMovingTouch(_beganTouchPos, touch->getLocation());
	}
}

void TouchLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	if(touch == _beganTouch) {
		if(!_isMoved) {
			dispatchSingleTouch(touch->getLocation());
		}
		_isMoved = false;
		_beganTouch = NULL;
		_beganTouchPos = INVALID_POINT;
	}
}

void TouchLayer::addDelegate(TouchLayerDelegate* delegate) {
	_delegateChain.push_back(delegate);
}

void TouchLayer::cleanDelegate() {
	_delegateChain.clear();
}

void TouchLayer::onExit() {
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void TouchLayer::dispatchSingleTouch(const CCPoint& pos) {
	for(list<TouchLayerDelegate*>::iterator itr = _delegateChain.begin();
		itr != _delegateChain.end(); ++itr)
	{
		if((*itr)->singleTouch(pos))
			break;
	}
}

void TouchLayer::dispatchMovingTouch(const CCPoint& from, const CCPoint& to) {
	for(list<TouchLayerDelegate*>::iterator itr = _delegateChain.begin();
		itr != _delegateChain.end(); ++itr)
	{
		if((*itr)->movingTouch(from, to))
			break;
	}		
}


