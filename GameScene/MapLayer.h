#ifndef _LAYER_MAP__H_
#define _LAYER_MAP__H_

#include "Base.h"
#include "TouchLayer.h"
#include "LayerMediator.h"
class Map;
class MediatorEvent;

class MapLayer : public CCLayer, public TouchLayerDelegate, public MediatorEventHandleDelegate {
// 构造
public:
	~MapLayer();
	static MapLayer* createSelf();
// 接口
public:
	virtual bool initSelf();
	virtual void update(float dt);
	virtual bool singleTouch(const CCPoint& pos);
	virtual bool movingTouch(const CCPoint& from, const CCPoint& to);
	virtual int handleEvent(MediatorEvent* mediatorEvent) { return 0;}
	void setMediatorToPanel(LayerMediator* mediatorToPanel);
// 工具
private:
	void createSprites();
// 属性
private: // 地图
	CCSize _screenSize;
	Map* _map;
	//Mediator* _mediatorToPanel;
	LayerMediator* _mediatorToPanel;
};

#endif  // _LAYER_MAP__H_