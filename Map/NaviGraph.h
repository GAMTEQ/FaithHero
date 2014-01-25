#ifndef _NAVI_GRAPH__H_
#define _NAVI_GRAPH__H_

#include "Base.h"

class NaviNode {
public:
	NaviNode()
		: _id(INVALID_VALUE)
		, _tilePos(INVALID_POINT)
		, _centerPos(INVALID_POINT)
		, _flag(emFree)
	{
	}
	CC_SYNTHESIZE(int, _id, Id); // 节点索引
	CC_SYNTHESIZE(CCPoint, _tilePos, TilePos); // 瓦块坐标
	CC_SYNTHESIZE(CCPoint, _centerPos, CenterPos); // 实际坐标
	CC_SYNTHESIZE(int, _flag, Flag); // 节点信息(可选)
};

class NaviEdge {
public:
	NaviEdge(const int from)
		: _from(from)
		, _to(INVALID_VALUE)
		, _cost(1.0)
	{
	}
	CC_SYNTHESIZE(int, _from, From); // 起点
	CC_SYNTHESIZE(int, _to, To); // 终点
	CC_SYNTHESIZE(double, _cost, Cost); // 代价
};

template <class TNodeType, class TEdgeType>   
class NaviGraph {
public:
	typedef TNodeType NodeType;
	typedef TEdgeType EdgeType;
public:
	void init(const CCTMXTiledMap& tiledMap, CCTMXLayer& layer0);
	void setNodeFlag(const CCPoint& tilePos, const int flag) {
		const int nodeId = tilePosToNodeId(tilePos);
		_nodes[nodeId].setFlag(flag);
		if(flag == emWall) { // 如果某节点是墙,则通往该节点的所有路径都失效
			vector<EdgeType>& wallNodeEdges = _edges[nodeId];
			for(int i = 0; i != NUM_DIRECT; ++i) { // 遍历墙节点的所有出边,然后把墙节点的所有入边置失效
				const int toNodeId = wallNodeEdges[i].getTo();
				if(toNodeId != INVALID_VALUE) {
					vector<EdgeType>& toNodeEdges = _edges[toNodeId]; // 墙节点的出边指向的节点的所有出边
					const int oppEdgeIndex = i < 4 ? i + 4 : i - 4; // 反向边序号
					toNodeEdges[oppEdgeIndex].setTo(INVALID_VALUE); // 置通往墙节点的边失效
				}
			}
		}
	}
	int getNodeFlag(const CCPoint& tilePos) {
		return _nodes[tilePosToNodeId(tilePos)].getFlag();
	}
	// clockwise 0 上 1 右 2 下 3 左
	CCPoint nodeIdToVertex(int nodeId, int clockwise = 0) {
		const NodeType& node = _nodes[nodeId];
		switch (clockwise)
		{
		case 0:
			return node.getCenterPos() + ccp(0, -_tileSize.height / 2);
		case 1:
			return node.getCenterPos() + ccp(_tileSize.width / 2, 0);
		case 2:
			return node.getCenterPos() + ccp(0, _tileSize.height / 2);
		case 3:
			return node.getCenterPos() + ccp(-_tileSize.width / 2, 0);
		default:
			assert(0);
			return ccp(0, 0);
			break;
		}
	}
	const NodeType& getNode(int nodeId) const {
		//CCLog("getNode %d", nodeId);
		assert(nodeId >= 0 && nodeId < _tileNum);
		return _nodes[nodeId];
	}
	void clean() {
		_nodes.clear();
		_edges.clear();
	}
	int getNodeNum() const {
		return _nodes.size();
	}
	int tilePosToNodeId(const CCPoint& tilePos) const { // tile坐标转换为nodeId
		if(IS_INVALID_POINT(tilePos)) return INVALID_VALUE;
		return int(tilePos.x) + int(tilePos.y) * _tileWidthNum;
	}
	CCPoint nodeIdToTilePos(const int nodeId) const { // nodeId转换为tile坐标
		if(nodeId < 0 || nodeId >= _tileNum) return INVALID_POINT;
		return ccp(nodeId % _tileWidthNum, nodeId / _tileWidthNum); 
	}
private:
	int _tileWidthNum;
	int _tileHeightNum;
	int _tileNum;
	CCSize _tileSize;
	vector<NodeType> _nodes; // 点,NodeId是索引
	vector<vector<EdgeType> > _edges; // 边,NodeId是索引,每个节点有8个临边
public: // 迭代器
	class EdgeIterator {
	private:                                                                
        typename vector<EdgeType>::const_iterator _edgeItr;
        const NaviGraph<TNodeType, TEdgeType>& _graph;
        const int _nodeId;
	public:
        EdgeIterator(const NaviGraph<TNodeType, TEdgeType>& graph, int nodeId)
			: _graph(graph), _nodeId(nodeId)
        {
			_edgeItr = _graph._edges[_nodeId].begin();
        }
        const EdgeType* begin() {        
			_edgeItr = _graph._edges[_nodeId].begin();
			return &(*_edgeItr);
        }
		bool end() {
          return _edgeItr == _graph._edges[_nodeId].end();
        }
        const EdgeType* next() {
          ++_edgeItr;
		  if(end()) return NULL;
          return &(*_edgeItr);
        }
	};
	friend class EdgeIterator;
public: // 静态成员
	static CCPoint deltaTileDirects[NUM_DIRECT];
};

/////////////////////////////////////////////////////////////////////////////// 实现

template <class TNodeType, class TEdgeType>  
void NaviGraph<TNodeType, TEdgeType>::init(const CCTMXTiledMap& tiledMap, CCTMXLayer& layer0) {
	clean();
	_tileSize = tiledMap.getTileSize();
	_tileWidthNum = tiledMap.getMapSize().width;
	_tileHeightNum = tiledMap.getMapSize().height;
	_tileNum = _tileWidthNum * _tileHeightNum;
	//CCLog("_tileWidthNum %d %d", _tileWidthNum, _tileHeightNum);
	const CCPoint deltaCenter = ccp(tiledMap.getTileSize().width / 2, tiledMap.getTileSize().height / 2);
	_nodes.resize(_tileNum);
	_edges.resize(_tileNum);
	// 构造节点列表
	for(int i = 0; i != _nodes.size(); ++i) {
		_nodes[i].setId(i);
		_nodes[i].setTilePos(nodeIdToTilePos(i));
		_nodes[i].setCenterPos(layer0.positionAt(_nodes[i].getTilePos()) + deltaCenter);
	}
	// 构造边列表
	for(int i = 0; i != _edges.size(); ++i) {
		NaviEdge edge(i);
		//CCLog("build node egdes");
		for(int e = 0; e != NUM_DIRECT; ++e) {
			CCPoint neighbourNode = _nodes[i].getTilePos() + deltaTileDirects[e];
			if(IS_INVALID_POINT(neighbourNode) || neighbourNode.x >= _tileWidthNum || neighbourNode.y >= _tileHeightNum)
				edge.setTo(INVALID_VALUE);
			else {
				edge.setTo(tilePosToNodeId(neighbourNode));
				edge.setCost(e % 2 ? 1.0 : 1.5);
				//CCLog("e %d cost %f", e, edge.getCost());
			}
			_edges[i].push_back(edge);
		}
	}
}

// 从12点开始顺时针:上,右上,右,右下,下,左下,左,左上
template <class TNodeType, class TEdgeType>  
CCPoint NaviGraph<TNodeType, TEdgeType>::deltaTileDirects[NUM_DIRECT] = {
		ccp(-1, -1), ccp(0, -1), ccp(1, -1), ccp(1, 0),
		ccp(1, 1), ccp(0, 1), ccp(-1, 1), ccp(-1, 0)
};


#endif // _NAVI_GRAPH__H_