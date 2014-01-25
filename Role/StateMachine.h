#ifndef _STATE_MACHINE__H_
#define _STATE_MACHINE__H_

#include "Base.h"

#define DISPATCHER MsgDispatcher::instance()

///////////////////////////////////////////////////////////////////////////////
// 状态
///////////////////////////////////////////////////////////////////////////////

template <class T>
class State : public CCObject {
public:
	virtual void enter(T* stateOwner) = 0;
	virtual void execute(T* stateOwner, double dt = 1.0) = 0;
	virtual void exit(T* stateOwner) = 0;
};

///////////////////////////////////////////////////////////////////////////////
// 状态机
///////////////////////////////////////////////////////////////////////////////

template <class T>
class StateMachine : public CCObject {
// 构造
public:
	static StateMachine* createStateMachine(T* owner) {
		StateMachine* fsm = new StateMachine(owner);
		if(fsm) {
			fsm->autorelease();
			return fsm;
		}
		CC_SAFE_DELETE(fsm);
		return NULL;
	}
protected:
		StateMachine(T* owner)
		: _owner(owner)
		, _currentState(NULL)
		, _previousState(NULL)
		, _globalState(NULL)
	{
	}
// 接口
public:
	void changeState(State<T>* newState) {
		assert(newState != NULL);
		_previousState = _currentState;
		_currentState->exit(_owner);
		_currentState = newState;
		_currentState->enter(_owner);
	}
	void update(double dt) {
		if(_globalState)
			_globalState->execute(_owner, dt);
		if(_currentState)
			_currentState->execute(_owner, dt);
	}
	void revertToPreviousState() {
		changeState(_previousState);
	}
	bool isInState(const State<T>& state) const {
		return typeid(state) == typeid(*_currentState);
	}
	std::string getStateName() {
		return typeid(*_currentState).name();
	}
// 属性
private:
	T* _owner;
	CC_SYNTHESIZE(State<T>*, _currentState, CurrentState);
	CC_SYNTHESIZE(State<T>*, _previousState, PreviousState);
	CC_SYNTHESIZE(State<T>*, _globalState, GlobalState);
};

#endif // _STATE_MACHINE__H_
