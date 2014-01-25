#ifndef _SEARCH_DIJKSTRA__H_
#define _SEARCH_DIJKSTRA__H_

#include "Base.h"
#include "PriorityQueue.h"
#include "NaviGraph.h"

template<typename GraphType>
class SearchDijkstra {
private:
	typedef typename GraphType::EdgeType Edge;
private:
	const GraphType& _graph; // 要搜索的图的引用
	vector<const Edge*> _shortestPathTree; // 最短路径树
	vector<const Edge*> _searchFrontier; // 当前的搜索边界
	vector<double> _costToThisNode; // 到每个节点的cost
	int _source; // 起点
	int _target; // 终点
private:
	void search();
public:
	SearchDijkstra(const GraphType& graph, int source, int target = INVALID_VALUE)
		: _graph(graph)
		, _shortestPathTree(graph.getNodeNum(), static_cast<const Edge*>(NULL))
		, _searchFrontier(graph.getNodeNum(), static_cast<const Edge*>(NULL))
		, _costToThisNode(graph.getNodeNum(), 0.0)
		, _source(source)
		, _target(target)
	{
		search();
	}
	vector<const Edge*> getShortestPathTree() const {
		return _shortestPathTree;
	}
	double getCostToNode(int node) const {
		return _costToThisNode[node];
	}
	vector<int> getPathToTarget() const;
};

/////////////////////////////////////////////////////////////////////////////// 实现

template<typename GraphType>
vector<int> SearchDijkstra<GraphType>::getPathToTarget() const {
	vector<int> path;
	if(_target < 0) return path;
	int node = _target;
	path.insert(path.begin(), node);
	while((node != _source) && (_shortestPathTree[node] != 0)) {
		node = _shortestPathTree[node]->getFrom();
		path.insert(path.begin(), node);
	}
	return path;
}

/**
首先原点放入SPT然后由起始点发出的边都放入边界
检查边界的边指向的节点,将距离原点最近的节点加入SPT,并把从该点出发的节点加入边界
如果边界的边指向的节点A已经在边界上了,则检查到节点A更短的开销,加入SPT
**/

template<typename GraphType>
void SearchDijkstra<GraphType>::search() {
    
	//CCLog("dij s%d [%d,%d] t%d [%d,%d]", _source, (int)_graph.nodeIdToTilePos(_source).x, (int)_graph.nodeIdToTilePos(_source).y, _target, (int)_graph.nodeIdToTilePos(_target).x, (int)_graph.nodeIdToTilePos(_target).y);
	IndexedPriorityQLow<double> queue(_costToThisNode, _graph.getNodeNum());
	queue.insert(_source);
	while(!queue.empty()) {
		// 从队列中取出的节点,是当前到原点代价最小的节点
		int nextClosetNode = queue.pop();
		//CCLog("insert SPT%d [%d,%d]", nextClosetNode, (int)_graph.nodeIdToTilePos(nextClosetNode).x, (int)_graph.nodeIdToTilePos(nextClosetNode).y);
		// 最短路径树的key是节点ID,value是该节点的一条出边,根据该出边,找到下一个节点ID
		// 最短路径树表示从原点到各个点的最短路径
		_shortestPathTree[nextClosetNode] = _searchFrontier[nextClosetNode];
		if(nextClosetNode == _target)
			return;
		typename GraphType::EdgeIterator edgeItr(_graph, nextClosetNode); // 获得点N发出的所有边(即连通的所有点)
		// 遍历N发出的所有边(点)
		for(const Edge* edge = edgeItr.begin(); !edgeItr.end(); edge = edgeItr.next()) {
			// 计算从点O到新的点I的代价 cost(OI) = cost(ON) + cost(NI)
			if(edge->getTo() == INVALID_VALUE) // 如果该出边不存在,则忽略
				continue;
			double newCost = _costToThisNode[nextClosetNode] + edge->getCost();
			//CCLog("from %d to %d [%d, %d] newCost %f", edge->getFrom(), edge->getTo(), (int)_graph.nodeIdToTilePos(edge->getTo()).x, (int)_graph.nodeIdToTilePos(edge->getTo()).y, newCost);
			// 如果I不在边界上,则加入到边界上(记录代价,并排序)
			if(_searchFrontier[edge->getTo()] == NULL) {
				_costToThisNode[edge->getTo()] = newCost;
				queue.insert(edge->getTo());
				_searchFrontier[edge->getTo()] = edge;
			}
			// 如果I已经在边界上,检查新的cost(OI)和原有的cost(OI)
			// 如果新的代价小并且还没记入SPT
			// 则修改边界(重写代价,重新排序)
			else if((newCost < _costToThisNode[edge->getTo()])
				&& (_shortestPathTree[edge->getTo()] == NULL)) 
			{
				_costToThisNode[edge->getTo()] = newCost;
				queue.ChangePriority(edge->getTo());
				_searchFrontier[edge->getTo()] = edge;			
			}
		}
	}
    
}

#endif // _SEARCH_DIJKSTRA__H_

