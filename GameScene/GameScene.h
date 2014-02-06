#ifndef _SCENE_RUN__H_
#define _SCENE_RUN__H_

#include "Base.h"

class MapLayer;
class TouchLayer;
class PanelLayer;
class LayerMediator;

class GameScene : public CCScene {
// 构造
public:
	~GameScene();
	static GameScene* createSelf();
protected:
	virtual bool initSelf();
// 接口
public:
	virtual void update(float dt);
// 属性
private:
	LayerMediator* _mediatorPanelAndMap;
	CC_SYNTHESIZE_READONLY(TouchLayer*, _TouchLayer, TouchLayer);
	CC_SYNTHESIZE_READONLY(MapLayer*, _MapLayer, MapLayer);
	CC_SYNTHESIZE_READONLY(PanelLayer*, _PanelLayer, PanelLayer);
};

#endif  // _SCENE_RUN__H_