#ifndef _BASE__H_
#define _BASE__H_

#include "cocos2d.h"
#include "cocos-ext.h"

#include "pomelo.h"
#include "uv.h"
#include "jansson.h"

#include "UserInfo.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

enum zLoginScene {
    zLoginSceneBackground = 0,
    zLoginSceneLogo,
    zLoginSceneEditBox
};

#define USER_INFO UserInfo::sharedUserInfo()

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

#define HEALTH_BAR_WIDTH 30
#define HEALTH_BAR_HEIGHT 3
#define DAMAGE_NUM_COUNT 10
#define DAMAGE_NUM_STEP 4
#define DAMAGE_NUM_REL_SIZE 0.05f

enum ELayerZorder {
	eLayerZorderMap = 0,
	eLayerZorderPanel,
	eLayerZorderTouch
};

enum EZOrder {
	ezBackground = -1,
	ezMap = 0
};

enum EMapElem {
	emFree = 0,
	emWall
};

typedef enum EDirect {
	eDirectUp = 0,
	eDirectRightup,
	eDirectRight,
	eDirectRightdown,
	eDirectDown,
	eDirectLeftdown,
	eDirectLeft,
	eDirectLeftup
} TEDirect;

typedef enum EActionState {
	eActionStateNone = 0,
	eActionStateBegin,
	eActionStateMiddle,
	eActionStateEnd
} TEActionState;

typedef enum EEventType {
	eEventNull = 0,
	eEventMapDecreaseLifeAndMage,
	eEventMapSetMaxLifeAndMage,
	eEventSkillHeal,
	eEventSkillNull
} TEEventType;

enum ESpriteType {
	eTypeWall = 0,
	eTypePathFinder = 1,
	eTypeEnemySwordsman = 11,
    eType2x2building = 21,
    eType3x3building = 31,
    eType4x4building = 41,
    eType5x5building = 51
};

typedef enum EGameState {
	eGameStateStart, /*开始界面*/
	eGameStateSetup, /*设置界面*/
	eGameStateLoad, /*加载界面*/
	eGameStateRun, /*运行*/
	eGameStatePause, /*暂停*/
	eGameResetMap, /*更换地图界面*/
	eGameStateLose, /*失败*/
	eGameStateWin /*成功*/
} TEGameState;

typedef enum EEnemyState {
	eEnemyStateAttack = 0,
	eEnemyStatePursuit,
	eEnemyStateWait
} TEEnemyState;

typedef enum EMainroleState {
	eMainroleStateStand = 0,
	eMainroleStateWalk,
	eMainroleStatePursuit,
	eMainroleStateAttack
} TEMainroleState;

typedef enum ETraceResult {
	eTraceResultFailure = 0, /*寻路失败*/
	eTraceResultHalfway, /*寻路途中*/
	eTraceResultSuccess /*寻路成功*/
} TETraceResult;

#define NUM_DIRECT 8

#define LINEMATH_EPS 1e-8
#define LINEMATH_ZERO(x) (((x)>0?(x):-(x))<LINEMATH_EPS)

// 无效点
#define INVALID_VALUE -1
#define INVALID_POINT ccp(INVALID_VALUE, INVALID_VALUE)
#define IS_INVALID_POINT(point) (point.x < 0 || point.y < 0)

#define CREATE_SELF(__TYPE__) \
static __TYPE__* createSelf() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->initSelf()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#define SWAP(T,x,y) {T temp; temp = x; x = y; y = temp;}

struct DetectOval {
	// 给定椭圆f(x,y)= x^2/a^2 + y^2/b^2=1, 
	// f(x1,y1) <1说明x1,y1在椭圆内部
	// 其中a是x轴长度的一半,b是y轴长度的一半
	void init(int widthTile, int heightTile, float tileWidth, float tileHeight) {
		_widthTile = widthTile;
		_heightTile = heightTile;
		_sqrtA = ((float)widthTile / 2 * tileWidth) * ((float)widthTile / 2 * tileWidth); 
		_sqrtB = ((float)heightTile / 2 * tileHeight) * ((float)heightTile / 2 * tileHeight); 
	}
	int _widthTile;
	int _heightTile;
	float _sqrtA;
	float _sqrtB;
	// oriPos 椭圆圆心 pos 点
	// 判断pos是在椭圆内还是在椭圆外
	bool isInOval(const CCPoint& oriPos, const CCPoint& pos) const {
		CCPoint delta = pos - oriPos; 
		return (delta.x * delta.x) / _sqrtA + (delta.y * delta.y) / _sqrtB <= 1;
	}
};

class MathFunc {
public:
	static double xmult(CCPoint p1,CCPoint p2,CCPoint p0){
		return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
	}
 
	//判点是否在线段上,包括端点
	static int dot_online_in(CCPoint p,CCPoint l1,CCPoint l2){
		return LINEMATH_ZERO(xmult(p,l1,l2))&&(l1.x-p.x)*(l2.x-p.x)<LINEMATH_EPS&&(l1.y-p.y)*(l2.y-p.y)<LINEMATH_EPS;
	}
 
	//判两点在线段同侧,点在线段上返回0
	static int same_side(CCPoint p1,CCPoint p2,CCPoint l1,CCPoint l2){
		return xmult(l1,p1,l2)*xmult(l1,p2,l2)>LINEMATH_EPS;
	}
 
	//判两直线平行
	static int parallel(CCPoint u1,CCPoint u2,CCPoint v1,CCPoint v2){
		return LINEMATH_ZERO((u1.x-u2.x)*(v1.y-v2.y)-(v1.x-v2.x)*(u1.y-u2.y));
	}
 
	//判三点共线
	static int dots_inline(CCPoint p1,CCPoint p2,CCPoint p3){
		return LINEMATH_ZERO(xmult(p1,p2,p3));
	}
 
	//判两线段相交,包括端点和部分重合
	static int intersect_in(CCPoint u1,CCPoint u2,CCPoint v1,CCPoint v2){
		if (!dots_inline(u1,u2,v1)||!dots_inline(u1,u2,v2))
			return !same_side(u1,u2,v1,v2)&&!same_side(v1,v2,u1,u2);
		return dot_online_in(u1,v1,v2)||dot_online_in(u2,v1,v2)||dot_online_in(v1,u1,u2)||dot_online_in(v2,u1,u2);
	}
 
	//计算两线段交点,请判线段是否相交(同时还是要判断是否平行!)
	static CCPoint intersection(CCPoint u1,CCPoint u2,CCPoint v1,CCPoint v2){
		CCPoint ret=u1;
		double t=((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
				/((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
		ret.x+=(u2.x-u1.x)*t;
		ret.y+=(u2.y-u1.y)*t;
		return ret;
	}

	// 获得当前时间的毫秒
	static long long getCurrentTimeMillisecond()
	{
		// tv.tv_usec 是微秒
		// millisec是毫秒
		// microsec是微秒
		struct cc_timeval tv;
		CCTime::gettimeofdayCocos2d(&tv, NULL);
		long long time = ((long long)tv.tv_sec) * 1000+ tv.tv_usec / 1000;
		return time;
	}

	static CCPoint truncate(CCPoint vector, double length) {
		if(vector.getLength() > length) {
			return vector.normalize() * length;
		}
		return vector;
	}

	//returns a random double between zero and 1
	static double randFloat() {
		// rand返回的是0含到RAND_MAX含,的整数
		return ((rand()) / (RAND_MAX + 1.0));
	}

	//returns a random double in the range -1 < n < 1
	static double randomClamped() {
		return randFloat() - randFloat();
	}

	static CCPoint pointToWorldSpace(
		const CCPoint& localPoint, 
		const CCPoint& agentAxisX, 
		const CCPoint& agentAxisY, 
		const CCPoint& agentPosition)
	{
		return ccp(0, 0);
	}

	static bool getTangentPoints(CCPoint P, CCPoint C, double R, CCPoint& T1, CCPoint& T2)
	{
	  CCPoint PmC = P - C;
	  double SqrLen = PmC.getLengthSq();
	  double RSqr = R*R;
	  if ( SqrLen <= RSqr )
	  {
		  // P is inside or on the circle
		  return false;
	  }

	  double InvSqrLen = 1/SqrLen;
	  double Root = sqrt(fabs(SqrLen - RSqr));

	  T1.x = C.x + R*(R*PmC.x - PmC.y*Root)*InvSqrLen;
	  T1.y = C.y + R*(R*PmC.y + PmC.x*Root)*InvSqrLen;
	  T2.x = C.x + R*(R*PmC.x + PmC.y*Root)*InvSqrLen;
	  T2.y = C.y + R*(R*PmC.y - PmC.x*Root)*InvSqrLen;

	  return true;
	}
};

class MediatorEvent;
class MediatorEventHandleDelegate {
public:
	virtual int handleEvent(MediatorEvent* mediatorEvent) = 0;
};



#endif  // _BASE__H_

/* 类模板

#ifndef _XX__H_
#define _XX__H_

#include "Base.h"
#include "SpriteBase.h"

class XX : public XXX {
// 构造
public:
	static XX* createSelf() {
		XX* self = new XX;
		if(self && self->initSelf()) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf() {
		if(init()) {
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
};

#endif // _XX__H_

*/



	// 以下是测试新功能
	// 测试默认持久化
	//UserRecordDefault userRecord;
	//userRecord.loadFromCCUserDefault();
	//CCLog("userRecord coin %d exp %d", userRecord.getCoin(), userRecord.getExp());
	//userRecord.saveToCCUserDefault();
	// 测试sqlite3
	//UserRecordSqlite userRecordSqlite;
	//userRecordSqlite.saveToSqlite();
//userRecordSqlite.loadFromSqlite();



// 关于回调函数
/*
 typedef void (CCObject::*SEL_SCHEDULE)(float);
 typedef void (CCObject::*SEL_CallFunc)();
 typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
 typedef void (CCObject::*SEL_CallFuncND)(CCNode*, void*);
 typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
 typedef void (CCObject::*SEL_MenuHandler)(CCObject*);
 typedef void (CCObject::*SEL_EventHandler)(CCEvent*);
 typedef int (CCObject::*SEL_Compare)(CCObject*);
 
 #define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
 #define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
 #define callfuncN_selector(_SELECTOR) (SEL_CallFuncN)(&_SELECTOR)
 #define callfuncND_selector(_SELECTOR) (SEL_CallFuncND)(&_SELECTOR)
 #define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
 #define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
 #define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
 #define compare_selector(_SELECTOR) (SEL_Compare)(&_SELECTOR)
 
 // typedef void (*pc_request_cb)(pc_request_t *req, int status, json_t *resp);
 #define pc_request_cb_selector(SELECTOR) (pc_request_cb)(&SELECTOR)
 
 _deadAction = CCSequence::createWithTwoActions(
 CCFadeOut::create(4.0f), CCCallFuncN::create(this, callfuncN_selector(PathFinder::callbackDead)));
 static CCCallFuncN * create(CCObject* pSelectorTarget, SEL_CallFuncN selector);
 
 
 void (*on_request_gate_cb)(pc_request_t *req, int status, json_t *resp) = &LoginScene::requstGateCallback;
 pc_request(client, request, route, msg, on_request_gate_cb);
 typedef void (*pc_request_cb)(pc_request_t *req, int status, json_t *resp);
 int pc_request(pc_client_t *client, pc_request_t *req, const char *route,
 json_t *msg, pc_request_cb cb);
 */
