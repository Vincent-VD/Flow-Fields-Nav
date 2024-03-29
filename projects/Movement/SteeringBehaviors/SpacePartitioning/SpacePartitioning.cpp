#include "stdafx.h"

#include "string.h"
#include "SpacePartitioning.h"
#include "projects\Movement\SteeringBehaviors\SteeringAgent.h"
#include "projects/Movement/Pathfinding/FlowField/App_FlowField.h"
#include "framework\EliteAI\EliteNavigation\Algorithms\ENavGraphPathfinding.h"

// --- Cell ---
// ------------
Cell::Cell(float left, float bottom, float width, float height)
{
	boundingBox.bottomLeft = { left, bottom };
	boundingBox.width = width;
	boundingBox.height = height;

	directionVect = Elite::ZeroVector2;
	target = Elite::Vector2{ left + (width / 2.f), bottom + (height / 2.f) };
	center = Elite::Vector2{ left + (width / 2.f), bottom + (height / 2.f) };
}

std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	auto left = boundingBox.bottomLeft.x;
	auto bottom = boundingBox.bottomLeft.y;
	auto width = boundingBox.width;
	auto height = boundingBox.height;

	std::vector<Elite::Vector2> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(Elite::Vector2 bottomLeft, float width, float height, int rows, int cols, int maxEntities)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_Neighbors(maxEntities)
	, m_NrOfNeighbors(0)
	, m_BottomLeft{ bottomLeft }
{
	m_CellWidth = width / rows;
	m_CellHeight = height / cols;
	m_Cells.resize(rows * cols);
	for (int col = 0; col < cols; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			Cell newCell{ Cell(bottomLeft.x + col * m_CellWidth, bottomLeft.y + row * m_CellHeight, m_CellWidth, m_CellHeight) };
			m_Cells[row * rows + col] = newCell;
		}
	}
}

CellSpace::~CellSpace()
{
	for (size_t iter = 0; iter < (UINT)m_NrOfNeighbors; iter++)
	{
		delete(m_Neighbors[iter]);
	}
	m_Neighbors.clear();
}

Cell CellSpace::GetAgentCell(SteeringAgent* agent) const
{
	const Elite::Vector2 agentPos{ agent->GetPosition() };
	int cellIdx{ PositionToIndex(agentPos) };
	return m_Cells[cellIdx];
}

void CellSpace::AddAgent(SteeringAgent* agent)
{
	const Elite::Vector2 agentPos{ agent->GetPosition() };
	int cellIdx{ PositionToIndex(agentPos) };
	m_Cells[cellIdx].agents.push_back(agent);
}

void CellSpace::UpdateAgentCell(SteeringAgent* agent, Elite::Vector2 oldPos)
{
	const Elite::Vector2 agentPos{ agent->GetPosition() };
	int cellIdx{ PositionToIndex(agentPos) };
	int oldCellIdx{ PositionToIndex(oldPos) };
	if (cellIdx != oldCellIdx)
	{
		m_Cells[oldCellIdx].agents.remove(agent);
		AddAgent(agent);
	}
	m_NrOfNeighbors = 0;
}

void CellSpace::ChangeCellDirectionVect(Elite::Vector2 mousePos, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2>& debugNodePositions, std::vector<Elite::Portal>& debugPortals)
{
	int cellIdx{ PositionToIndex(mousePos) };
	Cell& mouseCell{ m_Cells[cellIdx] };
	mouseCell.directionVect = Elite::ZeroVector2;
	mouseCell.target = Elite::ZeroVector2;
	for (Cell& cell : m_Cells)
	{
		const std::vector<Elite::Vector2> pathToMouse{ Elite::NavMeshPathfinding::FindPath(cell.center, mousePos, pNavGraph, debugNodePositions, debugPortals) };
		if (!pathToMouse.empty())
		{
			cell.target = pathToMouse[0];
			cell.directionVect = Elite::GetNormalized(Elite::Vector2{ cell.target - cell.center });
		}
	}
}

void CellSpace::CalculateField(Cell* startCell)
{
	std::queue<Cell*> openList{}; // nodes still to check
	std::map<Cell*, Cell*> closedList{}; // already checked nodes

	openList.push(startCell);

	while (!openList.empty()) {

		Cell* pCurrNode{ openList.front() }; // take node from open list
		openList.pop(); // remove the node

		for(int x = -1; x < 1; ++x)
		{
			for(int y = -1; y < 1; ++y)
			{
				
			}
		}
	}
}



void CellSpace::RegisterNeighbors(SteeringAgent* agent, float queryRadius)
{
	const Elite::Vector2 agentPos{ agent->GetPosition() };
	const int cellIdx{ PositionToIndex(agentPos) };
	Elite::Rect agentBoundingBox{ GetAgentBoundingBox(agent, queryRadius) };
	for (Cell currCell : m_Cells)
	{
		if (Elite::IsOverlapping(agentBoundingBox, currCell.boundingBox))
		{
			for (SteeringAgent* currAgent : currCell.agents)
			{
				if (Elite::DistanceSquared(agent->GetPosition(), currAgent->GetPosition()) <= queryRadius * queryRadius &&
					(UINT)m_NrOfNeighbors < m_Neighbors.size())
				{
					m_Neighbors[m_NrOfNeighbors] = currAgent;
					++m_NrOfNeighbors;
				}
			}
		}
	}
}

void CellSpace::RenderCells() const
{
	for (Cell cell : m_Cells)
	{
		if (m_DisplayGrid)
		{
			Elite::Polygon points{ cell.GetRectPoints() };
			const std::string string{ to_string(cell.agents.size()) };
			Elite::Vector2 pos{ cell.boundingBox.bottomLeft };
			const std::string string2{ to_string(PositionToIndex(pos)) };
			pos.y += 5.f;
			DEBUGRENDERER2D->DrawPolygon(&points, Elite::Color{ 1,0,0,1 });

		}
		DEBUGRENDERER2D->DrawDirection(cell.center, cell.directionVect, 3.f, Elite::Color{ 1, 1, 0, 1 });
	}
}

int CellSpace::PositionToIndex(const Elite::Vector2 pos) const
{
	float posX{ pos.x + -m_BottomLeft.x };
	float posY{ pos.y + -m_BottomLeft.y };
	int xCoord{ static_cast<int>(posX / m_CellWidth) };
	int yCoord{ static_cast<int>(posY / m_CellHeight) };
	if (xCoord == 10)
	{
		--xCoord;
	}
	if (yCoord == 10)
	{
		--yCoord;
	}
	int res{ xCoord + (yCoord * m_NrOfCols) };
	return res;
}

Elite::Rect CellSpace::GetAgentBoundingBox(SteeringAgent* agent, float queryRadius) const
{
	Elite::Rect rect{};
	const Elite::Vector2 agentPos{ agent->GetPosition() };
	auto left = agentPos.x - (m_CellWidth / 2);
	auto bottom = agentPos.x + (m_CellWidth / 2);
	auto width = queryRadius;
	auto height = queryRadius;

	rect.bottomLeft = Elite::Vector2{ left, bottom };
	rect.width = queryRadius;
	rect.height = queryRadius;

	return rect;
}