#ifndef _MAINROLE_STATES__H_
#define _MAINROLE_STATES__H_

#include "Base.h"
#include "StateMachine.h"

class Mainrole;

///////////////////////////////////////////////////////////////////////////////
// 全局状态
///////////////////////////////////////////////////////////////////////////////

class MainroleGlobal : public State<Mainrole> {
// 构造
private:
	MainroleGlobal() {}
public:
	static MainroleGlobal* instance();
// 接口
public:
	virtual void enter(Mainrole* stateOwner);
	virtual void execute(Mainrole* stateOwner, double dt = 1.0);
	virtual void exit(Mainrole* stateOwner);
};

///////////////////////////////////////////////////////////////////////////////
// 站立
///////////////////////////////////////////////////////////////////////////////

class MainroleStand : public State<Mainrole> {
// 构造
private:
	MainroleStand() {}
public:
	static MainroleStand* instance();
// 接口
public:
	virtual void enter(Mainrole* stateOwner);
	virtual void execute(Mainrole* stateOwner, double dt = 1.0);
	virtual void exit(Mainrole* stateOwner);
};

///////////////////////////////////////////////////////////////////////////////
// 行走
///////////////////////////////////////////////////////////////////////////////

class MainroleWalk : public State<Mainrole> {
// 构造
private:
	MainroleWalk() {}
public:
	static MainroleWalk* instance();
// 接口
public:
	virtual void enter(Mainrole* stateOwner);
	virtual void execute(Mainrole* stateOwner, double dt = 1.0);
	virtual void exit(Mainrole* stateOwner);
};

///////////////////////////////////////////////////////////////////////////////
// 追击
///////////////////////////////////////////////////////////////////////////////

class MainrolePursuit : public State<Mainrole> {
// 构造
private:
	MainrolePursuit() {}
public:
	static MainrolePursuit* instance();
// 接口
public:
	virtual void enter(Mainrole* stateOwner);
	virtual void execute(Mainrole* stateOwner, double dt = 1.0);
	virtual void exit(Mainrole* stateOwner);
};

///////////////////////////////////////////////////////////////////////////////
// 攻击
///////////////////////////////////////////////////////////////////////////////

class MainroleAttack : public State<Mainrole> {
// 构造
private:
	MainroleAttack() {}
public:
	static MainroleAttack* instance();
// 接口
public:
	virtual void enter(Mainrole* stateOwner);
	virtual void execute(Mainrole* stateOwner, double dt = 1.0);
	virtual void exit(Mainrole* stateOwner);
};

#endif //_MAINROLE_STATES__H_

