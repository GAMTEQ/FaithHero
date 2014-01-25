#ifndef _LAYER_TOUCH__H_
#define _LAYER_TOUCH__H_

#include "Base.h"

// 可以相应触摸层的层所需要满足的协议
class TouchLayerDelegate {
public:
	virtual bool singleTouch(const CCPoint& pos) = 0; // 单点触摸,不能处理则返回false
	virtual bool movingTouch(const CCPoint& from, const CCPoint& to) = 0; // 划动,不能处理则返回false
};

class TouchLayer : public CCLayer {
// 构造
public:
	static TouchLayer* createSelf();
	~TouchLayer();
protected:
	virtual bool initSelf();
// 接口
public:
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	void addDelegate(TouchLayerDelegate* delegate);
	void cleanDelegate();
	void onExit();
// 工具
private:
	void dispatchSingleTouch(const CCPoint& pos);
	void dispatchMovingTouch(const CCPoint& from, const CCPoint& to);
// 属性
private:
	list<TouchLayerDelegate*> _delegateChain; // 触摸层所代理的层,优先级从高到低
	CCTouch* _beganTouch;
	CCPoint _beganTouchPos;
	bool _isMoved;
};

#endif  // _LAYER_TOUCH__H_