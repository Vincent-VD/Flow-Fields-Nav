#pragma once

namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			}

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			}
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

		std::vector<std::pair<T_NodeType*, float>> Dijsktra(T_NodeType* pStartNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		vector<T_NodeType*> path;
		vector<NodeRecord> openList;
		vector<NodeRecord> closedList;
		NodeRecord currRec;

		currRec = NodeRecord{ pStartNode, nullptr, 0.f, GetHeuristicCost(pStartNode, pGoalNode) };
		openList.push_back(currRec);

		while (!openList.empty())
		{
			currRec = *std::min_element(openList.begin(), openList.end());

			if (currRec.pNode == pGoalNode)
			{
				break; // found end node
			}
			for (auto con : m_pGraph->GetNodeConnections(currRec.pNode))
			{
				T_NodeType* pNextNode{ m_pGraph->GetNode(con->GetTo()) };
				//Create NodeRecord with: target connection, curr connection, costSoFar + connection cost, F-cost
				NodeRecord nextNodeRec{ pNextNode, con, currRec.costSoFar + con->GetCost(), 0.f };
				nextNodeRec.estimatedTotalCost = nextNodeRec.costSoFar + GetHeuristicCost(pNextNode, pGoalNode);

				// D
				//Check closed list if connection is already on there
				auto isSameNode = [nextNodeRec](const NodeRecord& current) {return current.pNode == nextNodeRec.pNode; };
				auto foundNode{ std::find_if(closedList.begin(), closedList.end(), isSameNode) };
				if (foundNode != closedList.end())
				{
					if (nextNodeRec.costSoFar < foundNode->costSoFar)
					{
						closedList.erase(std::remove(closedList.begin(), closedList.end(), *foundNode));
					}
					else
					{
						continue;
					}
				}
				// E
				//Check open list if connection is already on there
				foundNode = std::find_if(openList.begin(), openList.end(), isSameNode);
				if (foundNode != openList.end())
				{
					if (nextNodeRec.costSoFar < foundNode->costSoFar)
					{
						openList.erase(std::remove(openList.begin(), openList.end(), *foundNode));
					}
					else
					{
						continue;
					}
				}
				// F
				openList.push_back(nextNodeRec);
			}
			// G
			openList.erase(std::remove(openList.begin(), openList.end(), currRec)); // take node from open list
			closedList.push_back(currRec);
		}

		while (currRec.pNode != pStartNode)
		{
			path.push_back(currRec.pNode);
			for (size_t iter = 0; iter < closedList.size(); ++iter)
			{
				if (closedList[iter].pNode->GetIndex() == currRec.pConnection->GetFrom())
				{
					currRec = closedList[iter];
					break;
				}
			}
		}
		path.push_back(pStartNode);
		std::reverse(path.begin(), path.end());

		return path;
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<std::pair<T_NodeType*, float>> AStar<T_NodeType, T_ConnectionType>::Dijsktra(T_NodeType* pStartNode)
	{
		vector<T_NodeType*> path;
		vector<NodeRecord> openList;
		vector<NodeRecord> closedList;
		NodeRecord currRec;

		currRec = NodeRecord{ pStartNode, nullptr, 0.f, 0.f };
		openList.push_back(currRec);

		while (!openList.empty())
		{
			currRec = *std::min_element(openList.begin(), openList.end());

			//NodeRecord nodeToAdd{};

			for (auto con : m_pGraph->GetNodeConnections(currRec.pNode))
			{
				T_NodeType* pNextNode{ m_pGraph->GetNode(con->GetTo()) };
				NodeRecord nextNodeRec{ pNextNode, con, currRec.costSoFar + con->GetCost(), 0.f };
				nextNodeRec.estimatedTotalCost = nextNodeRec.costSoFar + GetHeuristicCost(pNextNode, pStartNode);

				auto isSameNode = [nextNodeRec](const NodeRecord& current) {return current.pNode == nextNodeRec.pNode; };
				auto foundNode{ std::find_if(closedList.begin(), closedList.end(), isSameNode) };
				if(foundNode != closedList.end())
				{
					float distanceToEnd{ FLT_MAX };

					if (nextNodeRec.estimatedTotalCost < distanceToEnd)
					{
						distanceToEnd = nextNodeRec.estimatedTotalCost;
						//nodeToAdd = nextNodeRec;
					}
				}

				
			}





				/*auto isSameNode = [nextNodeRec](const NodeRecord& current) {return current.pNode == nextNodeRec.pNode; };
				auto foundNode{ std::find_if(closedList.begin(), closedList.end(), isSameNode) };

				float distance{ Elite::Distance(m_pGraph->GetNodePos(pNextNode), m_pGraph->GetNodePos(pStartNode)) };

				if(distance < distanceToEnd)
				{
					distanceToEnd = distance;
					nodeToAdd = pNextNode;
				}
			}*/

		}
	}

}