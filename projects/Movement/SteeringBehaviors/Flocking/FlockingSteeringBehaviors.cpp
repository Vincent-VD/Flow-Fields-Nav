#include "stdafx.h"
#include "FlockingSteeringBehaviors.h"
#include "TheFlock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"

//*******************
//COHESION (FLOCKING)
SteeringOutput Cohesion::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	SteeringOutput steering{};

	const Elite::Vector2 avgPos{ m_pFlock->GetAverageNeighborPos() };
	steering.LinearVelocity = avgPos - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	return steering;
}


//*********************
//SEPARATION (FLOCKING)

SteeringOutput Separation::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	SteeringOutput steering{};
	Elite::Vector2 totalVect{};
	for(int iter = 0; iter < m_pFlock->GetNrOfNeighbors(); iter++){
		SteeringAgent* currAgent{ m_pFlock->GetNeighbors()[iter] };
		const Elite::Vector2 distanceToTarget{  pAgent->GetPosition() - currAgent->GetPosition() };
		totalVect += 1.f / distanceToTarget;
	}
	steering.LinearVelocity = -totalVect / float(m_pFlock->GetNrOfNeighbors());
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	return steering;
}


//*************************
//VELOCITY MATCH (FLOCKING)
SteeringOutput VelocityMatch::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	SteeringOutput steering{};

	const Elite::Vector2 avgVel{ m_pFlock->GetAverageNeighborVelocity() };
	steering.LinearVelocity = avgVel;

	return steering;
}