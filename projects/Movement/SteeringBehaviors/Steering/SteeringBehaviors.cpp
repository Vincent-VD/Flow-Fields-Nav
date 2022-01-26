//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"

using namespace Elite;

float F_PI{ static_cast<float>(E_PI) };

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	//... logic
	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	/*if (pAgent->CanRenderBehavior()) {
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Color{ 0,1,0,1 });
	}*/

	return steering;
}

//FLEE
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	float distanceToTarget = Distance(pAgent->GetPosition(), m_Target.Position);
	if (distanceToTarget > m_FleeRadius) {
		return SteeringOutput(ZeroVector2, 0.f, false);
	}

	SteeringOutput steering = {};

	//... logic
	steering.LinearVelocity = -(m_Target.Position - pAgent->GetPosition());
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior()) {
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Color{ 0,1,0,1 });
	}

	return steering;
}


//Arrive
//****

SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	SteeringOutput steering = {};
	SetMaxSpeed(pAgent->GetMaxLinearSpeed());

	Vector2 toTarget = m_Target.Position - pAgent->GetPosition();
	const float distance = toTarget.Magnitude();
	if (distance < GetArrivalRadius()) {
		steering.LinearVelocity = ZeroVector2;
		return steering;
	}
	Vector2 velocity{ toTarget };
	velocity.Normalize();
	if (distance <= GetSlowRadius()) {
		velocity *= pAgent->GetMaxLinearSpeed() * distance / GetSlowRadius();
	}
	else {
		velocity *= pAgent->GetMaxLinearSpeed();
	}

	steering.LinearVelocity = velocity;

	return steering;
}

void Arrive::SetMaxSpeed(const float newSpeed) {

	m_MaxSpeed = newSpeed;
}

float Arrive::GetMaxSpeed() const {

	return m_MaxSpeed;
}

void Arrive::SetArrivalRadius(const float newRadius) {

	m_ArrivalRadius = newRadius;
}

float Arrive::GetArrivalRadius() const {
	
	return m_ArrivalRadius;
}

void Arrive::SetSlowRadius(const float newSlowRadius) {

	m_SlowRadius = newSlowRadius;
}

float Arrive::GetSlowRadius() const {

	return m_SlowRadius;
}


//Face
//****

SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	SteeringOutput steering{};

	pAgent->SetAutoOrient(false);

	Vector2 agentDirection = pAgent->GetDirection();
	Vector2 toTarget = m_Target.Position - pAgent->GetPosition();
	toTarget.Normalize();
	const float distAngle{ ToDegrees(acos(Dot(toTarget, agentDirection))) };
	pAgent->SetRotation(distAngle + 90);

	return steering;
}

//Wander
//****

SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	SteeringOutput steering{};

	Vector2 agentPos{ pAgent->GetPosition() };
	Vector2 agentDir{ pAgent->GetDirection() };
	Vector2 circleCenter{ agentPos + (agentDir * m_OffsetDistance) };

	const float randAngleDelt{ (-m_MaxAngleChange) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * m_MaxAngleChange))) };
	m_WanderAngle = ToDegrees(m_WanderAngle) + randAngleDelt;
	m_WanderAngle = ToRadians(m_WanderAngle);
	if (m_WanderAngle > 2 * F_PI) {
		m_WanderAngle -= 2 * F_PI;
	}
	else if(m_WanderAngle < 0) {
		m_WanderAngle = 2 * F_PI - m_WanderAngle;
	}
	Vector2 originAngle{ m_Radius * cos(m_WanderAngle), m_Radius * sin(m_WanderAngle) };
	Vector2 circleAngle{ originAngle + circleCenter };
	Vector2 target{ circleAngle - agentPos };

	steering.LinearVelocity = target;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior()) {
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Color{ 0,1,0,1 });
		DEBUGRENDERER2D->DrawCircle(circleCenter, m_Radius, Color{ 0,1,0,1 }, 0);
	}

	return steering;
}

void Wander::SetWanderOffset(float offset) {
	
	m_OffsetDistance = offset;
}

void Wander::SetWanderRadius(float radius) {

	m_Radius = radius;
}

void Wander::SetMaxAngleChange(float rad) {

	m_MaxAngleChange = rad;
}


//Pursuit
//****

SteeringOutput Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent) {
	
	SteeringOutput steering{};

	Vector2 toTarget{ m_Target.Position  + m_Target.LinearVelocity - pAgent->GetPosition() };
	//toTarget += Vector2{ 10.f, 10.f };

	steering.LinearVelocity = toTarget;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	return steering;

}


//Evade
//****

SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent) {

	SteeringOutput steering = {};
	float distanceToTarget = Distance(pAgent->GetPosition(), m_Target.Position);
	if (abs(distanceToTarget) > m_Radius) {
		steering.IsValid = false;
		//steering.LinearVelocity = pAgent->GetLinearVelocity();
		return steering;
	}


	steering.LinearVelocity = distanceToTarget  / -(m_Target.Position - pAgent->GetPosition());
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	/*if (pAgent->CanRenderBehavior()) {
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Color{ 0,1,0,1 });
	}*/
	return steering;

}

void Evade::SetEvadeRadius(float radius) {

	m_Radius = radius;
}
