#ifndef STEERINGBEHAVIORS_APPLICATION_H
#define STEERINGBEHAVIORS_APPLICATION_H
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

class SteeringAgent;
class BlendedSteering;
class PrioritySteering;

//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_CombinedSteering final : public IApp
{
public:
	//Constructor & Destructor
	App_CombinedSteering() = default;
	virtual ~App_CombinedSteering() final;

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:
	//Datamembers
	TargetData m_MouseTarget = {};
	bool m_UseMouseTarget = false;
	bool m_VisualizeMouseTarget = true;
	
	bool m_CanDebugRender = false;
	bool m_TrimWorld = true;
	float m_TrimWorldSize = 25.f;

	//Blended Steering
	SteeringAgent* m_pDrunkAgent = nullptr;
	BlendedSteering* m_pBlendedSteering = nullptr;
	Wander* m_pDrunkWander = nullptr;
	Seek* m_pSeek = nullptr;

	//Priority Steering
	SteeringAgent* m_pSoberAgent = nullptr;
	PrioritySteering* m_pPrioritySteering = nullptr;
	Wander* m_pSoberWander = nullptr;
	Flee* m_pFlee = nullptr;

};
#endif