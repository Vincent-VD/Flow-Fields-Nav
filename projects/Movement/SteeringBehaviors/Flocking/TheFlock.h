#pragma once
#include "../SteeringHelpers.h"
#include "FlockingSteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/SpacePartitioning/SpacePartitioning.h"

class ISteeringBehavior;
class SteeringAgent;
class BlendedSteering;
class PrioritySteering;


//#define USE_SPACE_PARTITIONING

class Flock
{
public:
	Flock(
		int flockSize = 100, 
		float worldSize = 100.f, 
		SteeringAgent* pAgentToEvade = nullptr, 
		bool trimWorld = false);

	~Flock();

	void Update(float deltaT);
	void UpdateAndRenderUI() ;
	void Render(float deltaT);

	void RegisterNeighbors(SteeringAgent* pAgent);
	//int GetNrOfNeighbors() const { return m_NrOfNeighbors; }
	int GetNrOfNeighbors() const { return m_CellSpace.GetNrOfNeighbors(); }
	//const vector<SteeringAgent*>& GetNeighbors() const { return m_Neighbors; }
	const vector<SteeringAgent*>& GetNeighbors() const { return m_CellSpace.GetNeighbors(); }

	void ChangeCellDirectionVect(Elite::Vector2 mousePos, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2>& debugNodePositions, std::vector<Elite::Portal>& debugPortals);

	void SetMaxLinearSpeed(float agentSpeed);

	Elite::Vector2 GetAverageNeighborPos() const;
	Elite::Vector2 GetAverageNeighborVelocity() const;

	void SetSeekTarget(TargetData target);
	void SetWorldTrimSize(float size) { m_WorldSize = size; }

private:
	//Datamembers
	int m_FlockSize = 100;
	vector<SteeringAgent*> m_Agents;
	vector<SteeringAgent*> m_Neighbors;
	CellSpace m_CellSpace;

	bool m_TrimWorld = false;
	float m_WorldSize = 0.f;

	float m_NeighborhoodRadius = 50.f;
	int m_NrOfNeighbors = 0;
	//size_t m_MaxNeighbors = 16;

	void SetFleeTarget(TargetData target);

	SteeringAgent* m_pAgentToEvade = nullptr;
	TargetData m_Target{};
	
	//Steering Behaviors
	Separation* m_pSeparationBehavior = nullptr;
	Cohesion* m_pCohesionBehavior = nullptr;
	VelocityMatch* m_pVelMatchBehavior = nullptr;
	Seek* m_pSeekBehavior = nullptr;
	Wander* m_pWanderBehavior = nullptr;
	Evade* m_pFleeBehavior = nullptr;

	BlendedSteering* m_pBlendedSteering = nullptr;
	PrioritySteering* m_pPrioritySteering = nullptr;

	void Start();
	float* GetWeight(ISteeringBehavior* pBehaviour);
	void DebugRenderAgent();

private:
	Flock(const Flock& other);
	Flock& operator=(const Flock& other);
};