#ifndef _SEARCH_ASTAR__H_
#define _SEARCH_ASTAR__H_

#include "Base.h"
#include "PriorityQueue.h"

template<typename GraphType, typename Heuristic>
class SearchAstar {
private:
	typedef typename GraphType::EdgeType Edge;
private:
	const GraphType& _graph; // 要搜索的图的引用
	vector<const Edge*> _shortestPathTree; // 最短路径树
	vector<const Edge*> _searchFrontier; // 当前的搜索边界
	vector<double> _gCosts; // 真实的从原点到目标点的代价,相当于Dijkstra里的_costToThisNode
	vector<double> _fCosts; // 真实的代价+启发的代价
	int _source; // 起点
	int _target; // 终点
private:
	void search();
public:
	SearchAstar(const GraphType& graph, int source, int target = INVALID_VALUE)
		: _graph(graph)
		, _shortestPathTree(graph.getNodeNum(), NULL)
		, _searchFrontier(graph.getNodeNum(), NULL)
		, _gCosts(graph.getNodeNum(), 0.0)
		, _fCosts(graph.getNodeNum(), 0.0)
		, _source(source)
		, _target(target)
	{
		search();
	}
	vector<const Edge*> getShortestPathTree() const {
		return _shortestPathTree;
	}
	double getCostToNode(int node) const {
		return _gCosts[node];
	}
	vector<int> getPathToTarget() const;
};

// 启发因子:目标点到当前点的直线距离
class HeuristicEuclid  {
public:
	template <typename GraphType>
	static double calculate(const GraphType& graph, int nodeA, int nodeB)
	{
		//CCLog("heuristic %d %d", nodeA, nodeB);
		return graph.getNode(nodeB).getCenterPos().getDistance(graph.getNode(nodeA).getCenterPos());
	}
};

// 启发因子:启发因子是0则回退到Dijkstra算法
class HeuristicDijkstra {
public:
	template <typename GraphType>
	static double calculate(const GraphType& graph, int nodeA, int nodeB)
	{
		return 0;
	}
};

class HeuristicManhattan {
public:
	template <typename GraphType>
	static double calculate(const GraphType& graph, int nodeA, int nodeB)
	{
		CCPoint tileNodeAToNodeB = graph.getNode(nodeB).getTilePos() - graph.getNode(nodeA).getTilePos();
		double manhattan = abs((int)tileNodeAToNodeB.x) + abs((int)tileNodeAToNodeB.y);
		return manhattan;
	}
};

/////////////////////////////////////////////////////////////////////////////// 实现

template<typename GraphType, typename Heuristic>
vector<int> SearchAstar<GraphType, Heuristic>::getPathToTarget() const {
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

template<typename GraphType, typename Heuristic>
void SearchAstar<GraphType, Heuristic>::search() {
	
	IndexedPriorityQLow<double> queue(_fCosts, _graph.getNodeNum());
	queue.insert(_source);
	while(!queue.empty()) {
		// 从队列中取出的节点,是当前到原点代价最小的节点
		int nextClosetNode = queue.pop();
		// 最短路径树的key是节点ID,value是该节点的一条出边,根据该出边,找到下一个节点ID
		// 最短路径树表示从原点到各个点的最短路径
		_shortestPathTree[nextClosetNode] = _searchFrontier[nextClosetNode];
		if(nextClosetNode == _target)
			return;
		typename GraphType::EdgeIterator edgeItr(_graph, nextClosetNode); // 获得点N发出的所有边(即连通的所有点)
		// 遍历N发出的所有边(点)
		for(const Edge* edge = edgeItr.begin(); !edgeItr.end(); edge = edgeItr.next()) {                   
			if(edge->getTo() == INVALID_VALUE) // 如果该出边不存在,则忽略
				continue;
			double hCost = Heuristic::calculate(_graph, _target, edge->getTo());  // 从点I到目标的启发因子,比如直线距离
			double gCost = _gCosts[nextClosetNode] + edge->getCost(); // 计算真实的从点O到新的点I的代价 cost(OI) = cost(ON) + cost(NI)
			// 如果I不在边界上,则加入到边界上(记录代价,并排序)
			if(_searchFrontier[edge->getTo()] == NULL) {
				_gCosts[edge->getTo()] = gCost;
				_fCosts[edge->getTo()] = gCost + hCost;
				queue.insert(edge->getTo());
				_searchFrontier[edge->getTo()] = edge;
			}
			// 如果I已经在边界上,检查新的cost(OI)和原有的cost(OI)
			// 如果新的代价小并且还没记入SPT
			// 则修改边界(重写代价,重新排序)
			else if((gCost < _gCosts[edge->getTo()])
				&& (_shortestPathTree[edge->getTo()] == NULL)) 
			{
				_gCosts[edge->getTo()] = gCost;
				_fCosts[edge->getTo()] = gCost + hCost;
				queue.ChangePriority(edge->getTo());
				_searchFrontier[edge->getTo()] = edge;			
			}
		}
	}
    
}

#endif //_SEARCH_ASTAR__H_
