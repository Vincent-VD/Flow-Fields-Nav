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
	SteeringAgent* pAgentToEvade /*= nullptr*/, 
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld { trimWorld }
	, m_pAgentToEvade{pAgentToEvade}
	, m_NeighborhoodRadius{ 15 }
	, m_NrOfNeighbors{0}
	, m_CellSpace{ CellSpace{ worldSize, worldSize, 10, 10, 16 } }
	//, m_Neighbors{std::vector<SteeringAgent*>}
{
	m_Agents.resize(m_FlockSize);
	m_Neighbors.resize(16);

	// TODO: initialize the flock and the memory pool
	Start();
}

void Flock::Start() {

	//Blended Steering
	m_pSeekBehavior = new Seek();
	m_pWanderBehavior = new Wander();
	m_pCohesionBehavior = new Cohesion(this);
	m_pSeparationBehavior = new Separation(this);
	m_pVelMatchBehavior = new VelocityMatch(this);
	m_pWanderBehavior->SetWanderOffset(0.f);
	vector<BlendedSteering::WeightedBehavior> weightedSteeringBehaviors;
	weightedSteeringBehaviors.push_back({ m_pCohesionBehavior, 0.5f });
	weightedSteeringBehaviors.push_back({ m_pSeparationBehavior, 0.5f });
	weightedSteeringBehaviors.push_back({ m_pVelMatchBehavior, 0.5f });
	weightedSteeringBehaviors.push_back({ m_pSeekBehavior, 0.5f });
	weightedSteeringBehaviors.push_back({ m_pWanderBehavior, 0.5f });
	m_pBlendedSteering = new BlendedSteering(weightedSteeringBehaviors);

	/*m_pAgentToEvade = new SteeringAgent();
	m_pAgentToEvade->SetSteeringBehavior(m_pWanderBehavior);
	const float randX{ 1.f + (((float)rand()) / (float)RAND_MAX) * (m_WorldSize - (1.f + 1)) };
	const float randY{ 1.f + (((float)rand()) / (float)RAND_MAX) * (m_WorldSize - (1.f + 1)) };
	m_pAgentToEvade->SetPosition(Elite::Vector2(50, 50));
	m_pAgentToEvade->SetMaxLinearSpeed(100);
	m_pAgentToEvade->SetBodyColor(Elite::Color(1, 0, 0));
	m_pAgentToEvade->SetAutoOrient(true);*/

	//Priority Steering
	//m_pFleeBehavior = new Evade();
	//auto target = TargetData{};
	//target.Position = m_pAgentToEvade->GetPosition();
	//target.Orientation = m_pAgentToEvade->GetOrientation();
	//target.LinearVelocity = -m_pAgentToEvade->GetLinearVelocity();
	//target.AngularVelocity = m_pAgentToEvade->GetAngularVelocity();
	////m_pFleeBehavior->SetTarget(target);
	////m_pSeparationBehavior->SetTarget(target);
	//m_pPrioritySteering = new PrioritySteering({ m_pFleeBehavior, m_pBlendedSteering }); //Evade from agentToAvoid and Blended from Cohesion and Wander
	////m_pPrioritySteering->SetTarget(target);

	for (int iter = 0; iter < m_FlockSize; iter++) {
		const float randX{ (1.f + (((float)rand()) / (float)RAND_MAX) * (m_WorldSize - (1.f + 1))) - (m_WorldSize / 2.f) };
		const float randY{ (1.f + (((float)rand()) / (float)RAND_MAX) * (m_WorldSize - (1.f + 1))) - (m_WorldSize / 2.f) };
		m_Agents[iter] = new SteeringAgent();
		const float maxVel{m_Agents[iter]->GetMaxLinearSpeed()};
		const float randVel{ maxVel / 10.f + (((float)rand()) / (float)RAND_MAX) * (maxVel - (maxVel / 10.f)) };
		m_Agents[iter]->SetPosition(Elite::Vector2(randX, randY));
		m_Agents[iter]->SetMaxLinearSpeed(randVel * 8);
		m_Agents[iter]->SetSteeringBehavior(m_pSeekBehavior);
		m_Agents[iter]->SetBodyColor(Elite::Color(0, 1, 0));
		m_Agents[iter]->SetAutoOrient(true);
		m_CellSpace.AddAgent(m_Agents[iter]);
	}
	//SetFleeTarget(target);

}

Flock::~Flock()
{
	// TODO: clean up any additional data

	delete(m_pBlendedSteering);
	delete(m_pPrioritySteering);
	delete(m_pSeekBehavior);
	delete(m_pWanderBehavior);
	delete(m_pCohesionBehavior);
	delete(m_pSeparationBehavior);
	delete(m_pVelMatchBehavior);
	delete(m_pFleeBehavior);

	delete m_pAgentToEvade;

	for(auto pAgent: m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();

	for (size_t iter = 0; iter < m_NrOfNeighbors; iter++) {
		delete(m_Neighbors[iter]);
	}
	m_Neighbors.clear();
}

void Flock::Update(float deltaT)
{
	// TODO: update the flock
	// loop over all the agents
		// register its neighbors	(-> memory pool is filled with neighbors of the currently evaluated agent)
		// update it				(-> the behaviors can use the neighbors stored in the pool, next iteration they will be the next agent's neighbors)
		// trim it to the world

	/*m_pAgentToEvade->Update(deltaT);
	if (m_TrimWorld)
	{
		m_pAgentToEvade->TrimToWorld(Elite::Vector2(0.f, 0.f), Elite::Vector2(m_WorldSize, m_WorldSize));
	}*/
	//auto target = TargetData{};
	//target.Position = m_pAgentToEvade->GetPosition();
	//target.Orientation = m_pAgentToEvade->GetOrientation();
	//target.LinearVelocity = -m_pAgentToEvade->GetLinearVelocity();
	//target.AngularVelocity = m_pAgentToEvade->GetAngularVelocity();
	//SetFleeTarget(target);

	for (SteeringAgent* currAgent : m_Agents)
	{
		if (useCellSpace)
		{
			m_CellSpace.RegisterNeighbors(currAgent, m_NeighborhoodRadius);
		}
		else
		{
			RegisterNeighbors(currAgent);

		}
		const Cell cell{ m_CellSpace.GetAgentCell(currAgent) };
		const Elite::Vector2 oldPos{ currAgent->GetPosition() };
		currAgent->GetSteeringBehavior()->SetTarget(cell.target);
		currAgent->Update(deltaT);
		if (m_TrimWorld)
		{
			currAgent->TrimToWorld(Elite::Vector2(-m_WorldSize / 2.f, -m_WorldSize / 2.f), Elite::Vector2(m_WorldSize / 2.f, m_WorldSize / 2.f));
		}
		m_CellSpace.UpdateAgentCell(currAgent, oldPos);
		if (!useCellSpace)
		{
			m_NrOfNeighbors = 0;
		}
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
	// TODO: render the flock
	m_pAgentToEvade->Render(deltaT);

	for (SteeringAgent* currAgent : m_Agents) {
		currAgent->Render(deltaT);
		//currAgent->SetRenderBehavior(m_CanDebugRender);
	}
	m_CellSpace.RenderCells();
	//DebugRenderAgent();



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

	// TODO: Implement checkboxes for debug rendering and weight sliders here
	ImGui::Text("Behavior Weights");
	ImGui::Spacing();
	ImGui::SliderFloat("Cohesion", &m_pBlendedSteering->GetWeightedBehaviorsRef()[0].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Seperation", &m_pBlendedSteering->GetWeightedBehaviorsRef()[1].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Velocity Match", &m_pBlendedSteering->GetWeightedBehaviorsRef()[2].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Seek", &m_pBlendedSteering->GetWeightedBehaviorsRef()[3].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Wander", &m_pBlendedSteering->GetWeightedBehaviorsRef()[4].weight, 0.f, 1.f, "%.2");

	ImGui::Checkbox("Use Cell Space", &useCellSpace);

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	
}

void Flock::RegisterNeighbors(SteeringAgent* pAgent) {

	for (SteeringAgent* currAgent : m_Agents)
	{
		Elite::Vector2 distanceVect{ currAgent->GetPosition() - pAgent->GetPosition() };
		float distanceSquared{ distanceVect.MagnitudeSquared() };
		if (distanceSquared < m_NeighborhoodRadius * m_NeighborhoodRadius && m_NrOfNeighbors < m_Neighbors.size()) {
			m_Neighbors[m_NrOfNeighbors] = currAgent;
			++m_NrOfNeighbors;
		}
	}
}

Elite::Vector2 Flock::GetAverageNeighborPos() const {

	Elite::Vector2 avgPos{};
	if (useCellSpace)
	{
		for (size_t iter = 0; iter < m_CellSpace.GetNrOfNeighbors(); iter++)
		{
			Elite::Vector2 currPos{m_CellSpace.GetNeighbors()[iter]->GetPosition()};
			avgPos += currPos;
		}
		avgPos /= m_CellSpace.GetNrOfNeighbors();
	}
	else
	{
		for (size_t iter = 0; iter < m_NrOfNeighbors; iter++)
		{
			Elite::Vector2 currPos{ m_Neighbors[iter]->GetPosition() };
			avgPos += currPos;
		}
		avgPos /= m_NrOfNeighbors;

	}
	return avgPos;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const 
{
	Elite::Vector2 avgVel;
	if (useCellSpace)
	{
		for (size_t iter = 0; iter < m_CellSpace.GetNrOfNeighbors(); iter++) {
			Elite::Vector2 currPos{ m_CellSpace.GetNeighbors()[iter]->GetLinearVelocity() };
			avgVel += currPos;
		}
		avgVel /= m_CellSpace.GetNrOfNeighbors();

	}
	else
	{
		for (size_t iter = 0; iter < m_NrOfNeighbors; iter++)
		{
			Elite::Vector2 currPos{ m_Neighbors[iter]->GetLinearVelocity() };
			avgVel += currPos;
		}
		avgVel /= m_NrOfNeighbors;

	}

	return avgVel;
}

void Flock::SetSeekTarget(TargetData target)
{
	// TODO: set target for Seek behavior
	for (SteeringAgent* currAgent : m_Agents)
	{
		m_pSeekBehavior->SetTarget(target);
	}
}

void Flock::SetFleeTarget(TargetData target)
{
	// TODO: set target for Seek behavior
	for (SteeringAgent* currAgent : m_Agents)
	{
		m_pFleeBehavior->SetTarget(target);
	}
}

float* Flock::GetWeight(ISteeringBehavior* pBehavior) 
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if(it!= weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}

void Flock::DebugRenderAgent() {

	SteeringAgent* agent{ m_Agents[15] };

	//if (agent->CanRenderBehavior()) {
		DEBUGRENDERER2D->DrawDirection(agent->GetPosition(), agent->GetLinearVelocity(), 5.f, Elite::Color{ 1, 1, 0, 1 });
		DEBUGRENDERER2D->DrawCircle(agent->GetPosition(), m_NeighborhoodRadius, Elite::Color{ 1, 1, 1, 1 }, 20.0f );
		/*m_CellSpace.RegisterNeighbors(agent, m_NeighborhoodRadius);
		for(size_t iter = 0; iter < m_CellSpace.GetNrOfNeighbors(); iter++){
			DEBUGRENDERER2D->DrawCircle(m_CellSpace.GetNeighbors()[iter]->GetPosition(), 3.f, Elite::Color{ 0, 1, 1, 1 }, 2.0f);
		}*/
	
	//}
}
