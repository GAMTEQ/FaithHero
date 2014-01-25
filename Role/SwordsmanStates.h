#ifndef _SWORDSMAN_STATES__H_
#define _SWORDSMAN_STATES__H_

#include "../Base.h"
#include "StateMachine.h"

class Swordsman;

///////////////////////////////////////////////////////////////////////////////
// 全局状态
///////////////////////////////////////////////////////////////////////////////

class SwordsmanGlobal : public State<Swordsman> {
// 构造
private:
	SwordsmanGlobal() {}
public:
	static SwordsmanGlobal* instance();
// 接口
public:
	virtual void enter(Swordsman* stateOwner);
	virtual void execute(Swordsman* stateOwner, double dt = 1.0);
	virtual void exit(Swordsman* stateOwner);
};

///////////////////////////////////////////////////////////////////////////////
// 等待
///////////////////////////////////////////////////////////////////////////////

class Swordsman;

class SwordsmanWait : public State<Swordsman> {
// 构造
private:
	SwordsmanWait() {}
public:
	static SwordsmanWait* instance();
// 接口
public:
	virtual void enter(Swordsman* stateOwner);
	virtual void execute(Swordsman* stateOwner, double dt = 1.0);
	virtual void exit(Swordsman* stateOwner);
};

///////////////////////////////////////////////////////////////////////////////
// 进攻
///////////////////////////////////////////////////////////////////////////////

class SwordsmanAttack : public State<Swordsman> {
// 构造
private:
	SwordsmanAttack() {}
public:
	static SwordsmanAttack* instance();
// 接口
public:
	virtual void enter(Swordsman* stateOwner);
	virtual void execute(Swordsman* stateOwner, double dt = 1.0);
	virtual void exit(Swordsman* stateOwner);
};

///////////////////////////////////////////////////////////////////////////////
// 追赶
///////////////////////////////////////////////////////////////////////////////

class SwordsmanPursuit : public State<Swordsman> {
// 构造
private:
	SwordsmanPursuit() {}
public:
	static SwordsmanPursuit* instance();
// 接口
public:
	virtual void enter(Swordsman* stateOwner);
	virtual void execute(Swordsman* stateOwner, double dt = 1.0);
	virtual void exit(Swordsman* stateOwner);
};

#endif //_SWORDSMAN_STATES__H_

