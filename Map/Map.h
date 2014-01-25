#ifndef _MAP__H_
#define _MAP__H_

#include "Base.h"
#include "NaviGraph.h"
#include "MsgDispatcher.h"

class PathFinder;
class Mainrole;
class Swordsman;
class Arrow;
class LayerMediator;
class DamageNumPool;
class ParticleSys;

// 封装CCTMXTiledMap
class Map : public CCNode {
// 构造
public:
	~Map();
	static Map* createSelf();
	virtual bool initSelf();
// 接口
public:
	virtual void update(float dt);
	void movingMapWhenDraging(const CCPoint& startPos, const CCPoint& currPos, float factor);
    void movingMapWhenRoleMoving(const CCPoint& startPos, const CCPoint& nowPos, float factor);
    CCPoint realPosToTilePos(const CCPoint& realPos) const; //把触摸点转换成网格坐标
	void resetMap(CCTMXTiledMap* newMap, CCSize& screenSize, CCNode* parent, LayerMediator* mediator); // 必须先初始化
	int getTileType(const CCPoint& tilePoint);
	CCPoint getDirectVector(const int& direct) {
		return _directs[direct];
	}
	TEDirect getClosestDirect(const CCPoint& direct);
	PathFinder* getPathFinder();
	CCPoint getTileCenter(const CCPoint& tilePos);
	const NaviGraph<NaviNode, NaviEdge>& getNaviGraph() {
		return _naviGraph;
	}
	int tilePosToNodeId(const CCPoint& tilePos) const { // tile坐标转换为nodeId
		return _naviGraph.tilePosToNodeId(tilePos);
	}
	int realPosToNodeId(const CCPoint& tilePos) const { // 地图坐标转换成nodeId
		return _naviGraph.tilePosToNodeId(realPosToTilePos(tilePos));
	}
	CCPoint nodeIdToTilePos(const int nodeId) const { // nodeId转换为tile坐标
		return _naviGraph.nodeIdToTilePos(nodeId);
	}
	CCArray* getAllPathFinders() const {
		return _allPathFinders;
	}
	int dealTargetTouch(const CCPoint& pos);
	void dealDead(PathFinder* deadRole);
	void dealDeadEffect(Arrow* arrow);
	void registerEffect(Arrow* arrow);
	vector<int> lineCrossTiles(const CCPoint& pointA, const CCPoint& pointB);
	CCPoint convertPosToMap(const CCPoint& pos, CCPoint& tilePosOnMap);
public: // 发送消息的接口
	void sendEventLifeAndMage(float deltaLife, float deltaMage);
	void sendInitEventToMediator(); // 连接地图和面板后,地图向面板发的第一次消息
// 工具
private:
    void movingMapByVectorAndBase(const CCPoint& startPos, const CCPoint& nowPos, const CCPoint& base, float factor);
	bool buildSprite(const int& type, const CCPoint& pos, const CCPoint& tilePosOnMap, int objectId);
// 属性
private: // 地图上的精灵
	CCArray* _walls;
	Mainrole* _mainrole; // 主角
	CCArray* _enemySwordsman; // 所有敌人
	CCArray* _allPathFinders; // 所有精灵,包括敌人和自己,用于分散
	CCArray* _allEffects; // 所有特效(即子弹)
	vector<CCSprite*> _deadPathFinders; // 已经标记死亡的角色,在主循环最后统一清除
	vector<CCSprite*> _deadEffects; // 已经标记死亡的子弹,在主循环最后统一清除
public: // 导航图
	NaviGraph<NaviNode, NaviEdge> _naviGraph;
private: // 地图
	CC_SYNTHESIZE_READONLY(CCTMXTiledMap*, _tiledMap, TiledMap); // 瓦块地图
	CC_SYNTHESIZE_READONLY(CCTMXLayer*, _layer0, Layer0); // 层0,基础层
	CC_SYNTHESIZE_READONLY(CCSize, _mapContentSize, MapContentSize); // 地图的尺寸,像素
	CC_SYNTHESIZE_READONLY(CCSize, _tileSize, TileSize); // 瓦块的尺寸,像素
	CC_SYNTHESIZE_READONLY(CCSize, _mapSize, MapSize); // 地图的尺寸,瓦块数
	CC_SYNTHESIZE_READONLY(CCSize, _layerSize, LayerSize); // 层0的尺寸,瓦块数
	int _layer0zorder; // 层0的zorder,其他zorder都不能小于层0的zorder
	CC_SYNTHESIZE(int, _highestZorder, HighestZorder); // 最高层的zorder,比任何zorder都大,用于一些暂时显示的精灵,比如子弹和伤害值
	CCPoint _mapOriPos; // 地图原点的实际坐标,即[0,0]瓦块的实际坐标
	CCPoint _directs[NUM_DIRECT]; // 8个方向向量
	bool _isMapSmallerThanScreen; // 地图是否小于屏幕(小于屏幕则不用拖动)
	double _xRight, _xLeft, _yUp, _yDown; // 用于拖动地图时限定范围
	CC_SYNTHESIZE(LayerMediator*, _mediator, Mediator);// 不计数
	CC_SYNTHESIZE(CCSize, _screenSize, ScreenSize);
	CC_SYNTHESIZE_READONLY(DamageNumPool*, _damageNumPool, DamageNumPool); // 伤害数值池
	CC_SYNTHESIZE(ParticleSys*, _paritcleSys, ParticlaSys);
    // 以下2个成员和主角跟随有关
    // 第一个是地图的中心，即地图图片的中心点；第二个是屏幕的中心，即显示屏幕的中心点
    // 初始化地图后，地图的锚点是中心点，把地图set到屏幕的中心，这样地图的中心就显示在屏幕的中心了
    // 加载好主角后，得到地图中心位置到主角位置的向量，然后把地图的坐标(相对于屏幕)偏移该向量，则主角就在屏幕中央了
    // 主角移动时，再得到地图中心位置到主角位置的向量，然后偏移地图位置，则主角随时都在屏幕中央。
 	CC_SYNTHESIZE(CCPoint, _mapCenterPos, MapCenterPos);// 地图中心（相对于地图）
 	CC_SYNTHESIZE(CCPoint, _mapOriginCenter, MapOriginCenter);// tiledMap首次setPosition的位置（相对于屏幕，即屏幕的中点）
private: // 网络消息处理
    MsgDispatcher _msgDispatcher;
};

#endif  // _MAP__H_