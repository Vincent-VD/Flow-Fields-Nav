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

	void SetArriveTarget(TargetData target);
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

	TargetData m_Target{};
	
	//Steering Behaviors
	Arrive* m_pArriveBehavior = nullptr;

	void Start();
	void DebugRenderAgent();

private:
	Flock(const Flock& other) = delete;
	Flock& operator=(const Flock& other) = delete;
};