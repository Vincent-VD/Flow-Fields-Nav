/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../SteeringHelpers.h"
class SteeringAgent;
class Obstacle;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) = 0;

	//Seek Functions
	void SetTarget(const TargetData& target) { m_Target = target; }
	 
	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	TargetData m_Target;
};
#pragma endregion


///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};


/////////////////////////
//FLEE
//****
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Seek Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	float m_FleeRadius{ 10.f };


};


/////////////////////////
//ARRIVE
//****

class Arrive : public Seek
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Arrive Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetMaxSpeed(const float newSpeed);
	float GetMaxSpeed() const;
	void SetArrivalRadius(const float newRadius);
	float GetArrivalRadius() const;
	void SetSlowRadius(const float newSlowRadius);
	float GetSlowRadius() const;

private:
	float m_MaxSpeed{ 50.f };
	float m_ArrivalRadius{ 1.f };
	float m_SlowRadius{ 15.f };
};


/////////////////////////
//FACE
//****

class Face : public Seek
{
public:
	Face() = default;
	virtual ~Face() = default;

	//Face Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};


/////////////////////////
//WANDER
//****


class Wander : public Seek {
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetWanderOffset(float offset);
	void SetWanderRadius(float radius);
	void SetMaxAngleChange(float rad);

protected:
	float m_OffsetDistance{ 6.f };
	float m_Radius{ 4.f };
	float m_MaxAngleChange{ 45 };
	float m_WanderAngle{ 0.f };

};


/////////////////////////
//PURSUIT
//****

class Pursuit : public Seek {
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	//Pursuit Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};


/////////////////////////
//EVADE
//****
class Evade : public Seek {
public:
	Evade() = default;
	~Evade() = default;

	//Evade Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetEvadeRadius(float radius);

protected:
	float m_Radius{ 10.f };

};


#endif


