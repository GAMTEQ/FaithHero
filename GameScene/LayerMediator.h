#ifndef _MIDIATOR__H
#define _MIDIATOR__H

#include "Base.h"

class MapLayer;
class PanelLayer;
class MediatorEvent;

// 事件:中介事件
class MediatorEvent : public CCObject {
// 构造
public:
	static MediatorEvent* createSelf();
// 接口
public:
protected:
	bool initSelf() {
		_eventType = eEventNull;
		return true;
	}
// 接口
public:
// 属性
protected:
	CC_SYNTHESIZE_READONLY(TEEventType, _eventType, EventType);
};

// Panel层和Map层同时持有一个Midiator对象
// Panel把层消息LayerEvent发送给Midiator,Midiator转发给Map,Map同理
class LayerMediator : public CCObject {
// 构造
public:
	static LayerMediator* createSelf() {
		LayerMediator* self = new LayerMediator;
		if(self && self->initSelf()) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
protected:
	bool initSelf() {
		return true;
	}
// 接口
public:
	void sendMapEvent(MediatorEvent* mediatorEvent);
	void sendPanelEvent(MediatorEvent* mediatorEvent);
// 属性
protected:
	CC_SYNTHESIZE(MapLayer*, _mapLayer, MapLayer);
	CC_SYNTHESIZE(PanelLayer*, _panelLayer, PanelLayer);
};

#endif // _MIDIATOR__H
