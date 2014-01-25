#ifndef _ACTION_ANIMATION__H_
#define _ACTION_ANIMATION__H_

#include "../Base.h"

// 动画
class ActionAnimation : public CCObject {
public:
	static ActionAnimation* createSelf(const string& frameFileHead, int frameNum, float interval);
	~ActionAnimation();
	TEActionState continueAction(CCSprite* sprite, float dt);
	void resetAction();
protected:
	bool initSelf(const string& frameFileHead, int frameNum, float interval);
	CCArray* createFrameArray(const string& frameFileHead, int frameNum);
protected:
	float _resetInterval; // 切换帧的时间间隔
	int _frameCount; // 帧的个数
	unsigned _curFrameId; // 当前帧ID
	float _curTimeAccumulate; // 当前累积的时间
	CCArray* _frameArray; // 帧容器
};

// 不同方向不同动画
class ActionAnimationDirect : public CCObject {
// 构造
public:
	static ActionAnimationDirect* createSelf(vector<string> frameNameVector, unsigned frameCount, float interval);
	~ActionAnimationDirect();
protected:
	bool initSelf(vector<string> frameNameVector, unsigned frameCount, float interval);
// 接口
public:
	TEActionState runDirect(CCSprite* sprite, TEDirect direct, float dt);
// 属性
private:
	TEDirect _lastDirect;
	CCArray* _actionAnimationArray;
};

#endif // _ACTION_ANIMATION__H_
