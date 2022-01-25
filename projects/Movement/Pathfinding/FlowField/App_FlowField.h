#pragma once
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/Movement/SteeringBehaviors/SteeringHelpers.h"
#include "projects/Movement/SteeringBehaviors/Flocking/TheFlock.h"
#include "projects/Movement/SteeringBehaviors/SpacePartitioning/SpacePartitioning.h"

#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphRenderer.h"
#include "framework\EliteAI\EliteNavigation\Algorithms\EPathSmoothing.h"

class NavigationColliderElement;
class SteeringAgent;

namespace Elite
{
	class NavGraph;
}

class App_FlowField : public IApp
{
public:
	App_FlowField() = default;
	virtual ~App_FlowField();

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:
	Flock* m_pFlock = nullptr;
	TargetData m_Target = {};
	float m_TrimWorldSize = 100.f;
	float m_AgentRadius = 1.0f;
	float m_AgentSpeed = 16.0f;

	// --Level--
	std::vector<NavigationColliderElement*> m_vNavigationColliders = {};

	// --Pathfinder--
	std::vector<Elite::Vector2> m_vPath;

	// --Graph--
	Elite::NavGraph* m_pNavGraph = nullptr;
	Elite::GraphRenderer m_GraphRenderer{};

	// --Debug drawing information--
	std::vector<Elite::Portal> m_Portals;
	std::vector<Elite::Vector2> m_DebugNodePositions;
	static bool sShowPolygon;
	static bool sShowGraph;
	static bool sDrawPortals;
	static bool sDrawFinalPath;
	static bool sDrawNonOptimisedPath;

	void UpdateImGui();
private:
	//C++ make the class non-copyable
	App_FlowField(const App_FlowField&) = delete;
	App_FlowField& operator=(const App_FlowField&) = delete;
};


