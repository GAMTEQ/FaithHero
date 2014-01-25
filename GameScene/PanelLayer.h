#ifndef _LAYER_PANEL__H_
#define _LAYER_PANEL__H_

#include "Base.h"
#include "TouchLayer.h"
#include "LayerMediator.h"

class SkillBar;
class RolePanel;
class MiniMap;

// 先构造MapLayer,然后用MapLayer生成的Map来构造PanelLayer
// 然后把PanelLayer生成的Mediator注册进MapLayer
class PanelLayer : public CCLayer, public TouchLayerDelegate, public MediatorEventHandleDelegate{
// 构造
public:
	~PanelLayer();
	static PanelLayer* createSelf();
// 接口
public:
	virtual bool initSelf();
	virtual void update(float dt);
	virtual bool singleTouch(const CCPoint& pos);
	virtual bool movingTouch(const CCPoint& from, const CCPoint& to);
	virtual int handleEvent(MediatorEvent* mediatorEvent);
// 工具
private:
	void createSprites();
// 属性
private:
	CCSize _screenSize; // 屏幕尺寸
private: // 面板组件
	SkillBar* _skillBarLeft;
	SkillBar* _skillBarRight;
	RolePanel* _rolePanel;
	//MiniMap* _miniMap;
	//Mediator* _mediatorToMap;
	CC_SYNTHESIZE(LayerMediator*, _mediatorToMap, MediatorToMap); // 必须初始化成NULL
};

#endif  // _LAYER_PANEL__H_

