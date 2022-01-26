/*=============================================================================*/
// Copyright 2019-2020
// Authors: Yosha Vandaele
/*=============================================================================*/
// SpacePartitioning.h: Contains Cell and Cellspace which are used to partition a space in segments.
// Cells contain pointers to all the agents within.
// These are used to avoid unnecessary distance comparisons to agents that are far away.

// Heavily based on chapter 3 of "Programming Game AI by Example" - Mat Buckland
/*=============================================================================*/

#pragma once
#include <list>
#include <vector>
#include <iterator>
#include "framework\EliteMath\EVector2.h"
#include "framework\EliteAI\EliteGraphs\ENavGraph.h"
#include "framework\EliteGeometry\EGeometry2DTypes.h"
#include "framework\EliteAI\EliteNavigation\Algorithms\EPathSmoothing.h"


class SteeringAgent;

// --- Cell ---
// ------------
struct Cell
{
	Cell() = default;
	Cell(float left, float bottom, float width, float height);

	std::vector<Elite::Vector2> GetRectPoints() const;

	// all the agents currently in this cell
	std::list<SteeringAgent*> agents;
	Elite::Vector2 directionVect;
	Elite::Vector2 target;
	Elite::Vector2 center;
	Elite::Rect boundingBox;
};

// --- Partitioned Space ---
// -------------------------
class CellSpace
{
public:
	CellSpace(Elite::Vector2 bottomLeft, float width, float height, int rows, int cols, int maxEntities);
	~CellSpace();

	void AddAgent(SteeringAgent* agent);
	void UpdateAgentCell(SteeringAgent* agent, Elite::Vector2 oldPos);

	Cell GetAgentCell(SteeringAgent* agent) const;

	void ChangeCellDirectionVect(Elite::Vector2 mousePos, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2>& debugNodePositions, std::vector<Elite::Portal>& debugPortals);

	void RegisterNeighbors(SteeringAgent* agent, float queryRadius);
	const std::vector<SteeringAgent*>& GetNeighbors() const { return m_Neighbors; }
	int GetNrOfNeighbors() const { return m_NrOfNeighbors; }

	void RenderCells()const;

private:
	bool m_DisplayGrid{ false };

	// Cells and properties
	std::vector<Cell> m_Cells;
	Elite::Vector2 m_BottomLeft;

	float m_SpaceWidth;
	float m_SpaceHeight;

	int m_NrOfRows;
	int m_NrOfCols;

	float m_CellWidth;
	float m_CellHeight;

	// Members to avoid memory allocation on every frame
	vector<SteeringAgent*> m_Neighbors;
	int m_NrOfNeighbors;

	// Helper functions
	int PositionToIndex(const Elite::Vector2 pos) const;
	Elite::Rect GetAgentBoundingBox(SteeringAgent* agent, float queryRadius) const;
};
