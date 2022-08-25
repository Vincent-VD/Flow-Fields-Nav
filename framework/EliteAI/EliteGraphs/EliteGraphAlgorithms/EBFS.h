#pragma once

namespace Elite 
{
	

	template <class T_NodeType, class T_ConnectionType>
	class BFS
	{
	public:

		BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph);

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

		//std::vector<std::pair<T_NodeType*, float>> Dijsktra(T_NodeType* pStartNode);
	private:
		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;

	};

	template <class T_NodeType, class T_ConnectionType>
	BFS<T_NodeType, T_ConnectionType>::BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph)
		: m_pGraph(pGraph)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> BFS<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode)
	{
		std::queue<T_NodeType*> openList{}; // nodes still to check
		std::map<T_NodeType*, T_NodeType*> closedList{}; // already checked nodes
		std::vector<float> dist(m_pGraph->GetNrOfNodes());
		
		openList.push(pStartNode);

		while (!openList.empty()) {

			T_NodeType* pCurrNode{ openList.front() }; // take node from open list
			openList.pop(); // remove the node
			
			if (pCurrNode == pDestinationNode) {
				break; // found end node
			}

			for (auto con : m_pGraph->GetNodeConnections(pCurrNode)) {
				T_NodeType* pNextNode{ m_pGraph->GetNode(con->GetTo()) };
				if (closedList.find(pNextNode) == closedList.end()) {
					openList.push(pNextNode);
					closedList[pNextNode] = pCurrNode;
				}
			}
		}
		//backtracking from end to start
		std::vector<T_NodeType*> path;
		T_NodeType* pCurrentNode = pDestinationNode;

		while (pCurrentNode != pStartNode) {
			path.push_back(pCurrentNode);
			pCurrentNode = closedList[pCurrentNode];
		}
		path.push_back(pStartNode);
		std::reverse(path.begin(), path.end());
		return path;
	}

	//template <class T_NodeType, class T_ConnectionType>
	//std::vector<std::pair<T_NodeType*, float>> BFS<T_NodeType, T_ConnectionType>::Dijsktra(T_NodeType* pStartNode)
	//{
	//	std::queue<T_NodeType*> openList{}; // nodes still to check
	//	std::map<T_NodeType*, T_NodeType*> closedList{}; // already checked nodes
	//	std::vector<float> dist(m_pGraph->GetNrOfNodes(), FLT_MAX);

	//	openList.push(pStartNode);

	//	while (!openList.empty()) {

	//		T_NodeType* pCurrNode{ openList.front() }; // take node from open list
	//		openList.pop(); // remove the node

	//		for (auto con : m_pGraph->GetNodeConnections(pCurrNode)) {
	//			T_NodeType* pNextNode{ m_pGraph->GetNode(con->GetTo()) };
	//			if (closedList.find(pNextNode) == closedList.end()) {
	//				openList.push(pNextNode);
	//				closedList[pNextNode] = pCurrNode;
	//			}
	//		}
	//	}
	//}

}

