#pragma once
#include <vector>
#include <iostream>
#include "framework/EliteMath/EMath.h"
#include "framework\EliteAI\EliteGraphs\ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

namespace Elite
{
	class NavMeshPathfinding
	{
	public:
		static std::vector<Elite::Vector2> FindPath(Elite::Vector2 startPos, Elite::Vector2 endPos, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2>& debugNodePositions, std::vector<Elite::Portal>& debugPortals)
		{
			//Create the path to return
			std::vector<Elite::Vector2> finalPath{};

			//Get the start and endTriangle
			const Triangle* startTriangle{pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(startPos)};
			const Triangle* endTriangle{pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(endPos)};
			if (startTriangle == nullptr || endTriangle == nullptr)
			{
				return finalPath;
			}
			if (startTriangle == endTriangle)
			{
				finalPath.push_back(endPos);
				return finalPath;
			}

			//We have valid start/end triangles and they are not the same
			//=> Start looking for a path
			//Copy the graph
			auto pSmartGraph = pNavGraph->Clone();
			auto pGraph = pSmartGraph.get();
			
			//Create extra node for the Start Node (Agent's position)
			int startNodeIdx{ pGraph->AddNode(new NavGraphNode{ pGraph->GetNextFreeNodeIndex(), -1, startPos }) };
			std::vector<NavGraphNode*> pLocalNavNodes;
			for (auto idx : startTriangle->metaData.IndexLines)
			{
				for (auto node : pGraph->GetAllNodes())
				{
					if (node->GetLineIndex() == idx)
					{
						pLocalNavNodes.push_back(node);
					}
				}
			}
			for (auto node : pLocalNavNodes)
			{
				pGraph->AddConnection(new GraphConnection2D{ startNodeIdx, node->GetIndex(), Distance(startPos, node->GetPosition()) });
			}
			// Clear vector to make way for endNode connections
			pLocalNavNodes.clear();

			//Create extra node for the endNode
			int endNodeIdx{ pGraph->AddNode(new NavGraphNode{ pGraph->GetNextFreeNodeIndex(), -1, endPos }) };
			for (auto idx : endTriangle->metaData.IndexLines)
			{
				for (auto node : pGraph->GetAllNodes())
				{
					if (node->GetLineIndex() == idx)
					{
						pLocalNavNodes.push_back(node);
					}
				}
			}
			for (auto node : pLocalNavNodes)
			{
				pGraph->AddConnection(new GraphConnection2D{ endNodeIdx, node->GetIndex(), Distance(endPos, node->GetPosition()) });
			}

			//Run A star on new graph
			Heuristic heurFunc{ HeuristicFunctions::Chebyshev };
			auto pathfinder = AStar<NavGraphNode, GraphConnection2D>(pGraph, heurFunc);
			auto startNode = pGraph->GetNode(startNodeIdx);
			auto endNode = pGraph->GetNode(endNodeIdx);

			std::vector<NavGraphNode*> AStarPath{ pathfinder.FindPath(startNode, endNode) };
			
			//OPTIONAL BUT ADVICED: Debug Visualisation
			for (auto node : AStarPath)
			{
				debugNodePositions.push_back(node->GetPosition());
				finalPath.push_back(node->GetPosition());
			}

			//Run optimiser on new graph, MAKE SURE the A star path is working properly before starting this section and uncommenting this!!!
			std::vector<Elite::Portal> portals = SSFA::FindPortals(AStarPath, pNavGraph->GetNavMeshPolygon());
			finalPath = SSFA::OptimizePortals(portals);
			
			debugPortals = portals;

			return finalPath;
		}

	};
}
