#include "stdafx.h"
#include "TheFlock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"

bool useCellSpace{ false };

//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/, 
	float worldSize /*= 100.f*/, 
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld { trimWorld }
	, m_NeighborhoodRadius{ 15 }
	, m_NrOfNeighbors{0}
	, m_CellSpace{ CellSpace{ Elite::Vector2{-worldSize / 2.f, -worldSize / 2.f}, worldSize, worldSize, 10, 10, 16 } }
{
	m_Agents.resize(m_FlockSize);
	m_Neighbors.resize(16);

	// TODO: initialize the flock and the memory pool
	Start();
}

void Flock::Start() {

	//Steering
	m_pArriveBehavior = new Arrive();

	for (int iter = 0; iter < m_FlockSize; iter++) {
		const float randX{ (1.f + (((float)rand()) / (float)RAND_MAX) * (m_WorldSize - (1.f + 1))) - (m_WorldSize / 2.f) };
		const float randY{ (1.f + (((float)rand()) / (float)RAND_MAX) * (m_WorldSize - (1.f + 1))) - (m_WorldSize / 2.f) };
		m_Agents[iter] = new SteeringAgent();
		const float maxVel{m_Agents[iter]->GetMaxLinearSpeed()};
		const float randVel{ maxVel / 10.f + (((float)rand()) / (float)RAND_MAX) * (maxVel - (maxVel / 10.f)) };
		m_Agents[iter]->SetPosition(Elite::Vector2(randX, randY));
		m_Agents[iter]->SetMaxLinearSpeed(randVel * 8);
		m_Agents[iter]->SetSteeringBehavior(m_pArriveBehavior);
		m_Agents[iter]->SetBodyColor(Elite::Color(0, 1, 0));
		m_Agents[iter]->SetAutoOrient(true);
		m_CellSpace.AddAgent(m_Agents[iter]);
	}

}

Flock::~Flock()
{
	delete(m_pArriveBehavior);

	for(auto pAgent: m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();

	for (size_t iter = 0; iter < (UINT)m_NrOfNeighbors; iter++) {
		delete(m_Neighbors[iter]);
	}
	m_Neighbors.clear();
}

void Flock::Update(float deltaT)
{
	for (SteeringAgent* currAgent : m_Agents)
	{
		const Cell cell{ m_CellSpace.GetAgentCell(currAgent) };
		const Elite::Vector2 oldPos{ currAgent->GetPosition() };
		currAgent->GetSteeringBehavior()->SetTarget(cell.target);
		currAgent->Update(deltaT);
		if (m_TrimWorld)
		{
			currAgent->TrimToWorld(Elite::Vector2(-m_WorldSize / 2.f, -m_WorldSize / 2.f), Elite::Vector2(m_WorldSize / 2.f, m_WorldSize / 2.f));
		}
		m_CellSpace.UpdateAgentCell(currAgent, oldPos);
	}
}


void Flock::ChangeCellDirectionVect(Elite::Vector2 mousePos, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2>& debugNodePositions, std::vector<Elite::Portal>& debugPortals)
{
	m_CellSpace.ChangeCellDirectionVect(mousePos, pNavGraph, debugNodePositions, debugPortals);
}

void Flock::SetMaxLinearSpeed(float agentSpeed)
{
	for (SteeringAgent* currAgent : m_Agents)
	{
		currAgent->SetMaxLinearSpeed(agentSpeed);
	}
}

void Flock::Render(float deltaT)
{
	for (SteeringAgent* currAgent : m_Agents)
	{
		currAgent->Render(deltaT);
	}
	m_CellSpace.RenderCells();
}

void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Flocking");
	ImGui::Spacing();

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	
}

void Flock::RegisterNeighbors(SteeringAgent* pAgent) {

	for (SteeringAgent* currAgent : m_Agents)
	{
		Elite::Vector2 distanceVect{ currAgent->GetPosition() - pAgent->GetPosition() };
		float distanceSquared{ distanceVect.MagnitudeSquared() };
		if (distanceSquared < m_NeighborhoodRadius * m_NeighborhoodRadius && (UINT)m_NrOfNeighbors < m_Neighbors.size()) {
			m_Neighbors[m_NrOfNeighbors] = currAgent;
			++m_NrOfNeighbors;
		}
	}
}

Elite::Vector2 Flock::GetAverageNeighborPos() const {

	Elite::Vector2 avgPos{};
	if (useCellSpace)
	{
		for (size_t iter = 0; iter < (UINT)m_CellSpace.GetNrOfNeighbors(); iter++)
		{
			Elite::Vector2 currPos{m_CellSpace.GetNeighbors()[iter]->GetPosition()};
			avgPos += currPos;
		}
		avgPos /= static_cast<float>(m_CellSpace.GetNrOfNeighbors());
	}
	else
	{
		for (size_t iter = 0; iter < (UINT)m_NrOfNeighbors; iter++)
		{
			Elite::Vector2 currPos{ m_Neighbors[iter]->GetPosition() };
			avgPos += currPos;
		}
		avgPos /= static_cast<float>(m_NrOfNeighbors);

	}
	return avgPos;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const 
{
	Elite::Vector2 avgVel;
	if (useCellSpace)
	{
		for (size_t iter = 0; iter < (UINT)m_CellSpace.GetNrOfNeighbors(); iter++) {
			Elite::Vector2 currPos{ m_CellSpace.GetNeighbors()[iter]->GetLinearVelocity() };
			avgVel += currPos;
		}
		avgVel /= static_cast<float>(m_CellSpace.GetNrOfNeighbors());

	}
	else
	{
		for (size_t iter = 0; iter < (UINT)m_NrOfNeighbors; iter++)
		{
			Elite::Vector2 currPos{ m_Neighbors[iter]->GetLinearVelocity() };
			avgVel += currPos;
		}
		avgVel /= static_cast<float>(m_NrOfNeighbors);

	}

	return avgVel;
}

void Flock::SetArriveTarget(TargetData target)
{
	// TODO: set target for Seek behavior
	for (SteeringAgent* currAgent : m_Agents)
	{
		m_pArriveBehavior->SetTarget(target);
	}
}

void Flock::DebugRenderAgent() {

	SteeringAgent* agent{ m_Agents[15] };

	DEBUGRENDERER2D->DrawDirection(agent->GetPosition(), agent->GetLinearVelocity(), 5.f, Elite::Color{ 1, 1, 0, 1 });
	DEBUGRENDERER2D->DrawCircle(agent->GetPosition(), m_NeighborhoodRadius, Elite::Color{ 1, 1, 1, 1 }, 20.0f );
	
}
