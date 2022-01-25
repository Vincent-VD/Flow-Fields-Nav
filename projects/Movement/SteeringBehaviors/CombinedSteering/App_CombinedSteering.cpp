//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "App_CombinedSteering.h"
#include "../SteeringAgent.h"
#include "CombinedSteeringBehaviors.h"
#include "projects\Movement\SteeringBehaviors\Obstacle.h"

using namespace Elite;
App_CombinedSteering::~App_CombinedSteering()
{	
	//Blended Steering
	SAFE_DELETE(m_pDrunkAgent);
	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pDrunkWander);
	SAFE_DELETE(m_pSeek);

	//Priority Steering
	SAFE_DELETE(m_pSoberAgent);
	SAFE_DELETE(m_pPrioritySteering);
	SAFE_DELETE(m_pSoberWander);
	SAFE_DELETE(m_pFlee);
}

void App_CombinedSteering::Start()
{
	//Blended Steering
	m_pSeek = new Seek();
	m_pDrunkWander = new Wander();
	m_pDrunkWander->SetWanderOffset(0.f);
	vector<BlendedSteering::WeightedBehavior> weightedSteeringBehaviors;
	weightedSteeringBehaviors.push_back({ m_pSeek, 0.5f });
	weightedSteeringBehaviors.push_back({ m_pDrunkWander, 0.5f });
	m_pBlendedSteering = new BlendedSteering(weightedSteeringBehaviors);

	m_pDrunkAgent = new SteeringAgent();
	m_pDrunkAgent->SetSteeringBehavior(m_pBlendedSteering);
	m_pDrunkAgent->SetBodyColor(Color(1, 0, 0));
	m_pDrunkAgent->SetAutoOrient(true);

	//Priority Steering
	m_pFlee = new Flee();
	m_pSoberWander = new Wander();
	m_pPrioritySteering = new PrioritySteering({ m_pFlee, m_pSoberWander });

	m_pSoberAgent = new SteeringAgent();
	m_pSoberAgent->SetSteeringBehavior(m_pPrioritySteering);
	m_pSoberAgent->SetBodyColor(Color(0, 1, 0));
	m_pSoberAgent->SetAutoOrient(true);
}

void App_CombinedSteering::Update(float deltaTime)
{
	//INPUT
	if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft) && m_VisualizeMouseTarget)
	{
		auto const mouseData = INPUTMANAGER->GetMouseData(InputType::eMouseButton, InputMouseButton::eLeft);
		m_MouseTarget.Position = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld({ static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y) });
	}

#ifdef PLATFORM_WINDOWS

	

	#pragma region UI
	//UI
	{
		//Setup
		int const menuWidth = 235;
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
		ImGui::Spacing();

		ImGui::Text("Flocking");
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("Debug Rendering", &m_CanDebugRender);
		ImGui::Checkbox("Trim World", &m_TrimWorld);
		if (m_TrimWorld)
		{
			ImGui::SliderFloat("Trim Size", &m_TrimWorldSize, 0.f, 500.f, "%1.");
		}
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Behavior Weights");
		ImGui::Spacing();
		ImGui::SliderFloat("Seek", &m_pBlendedSteering->GetWeightedBehaviorsRef()[0].weight, 0.f, 1.f, "%.2");
		ImGui::SliderFloat("Wander", &m_pBlendedSteering->GetWeightedBehaviorsRef()[1].weight, 0.f, 1.f, "%.2");

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
	#pragma endregion
#endif

	m_pSeek->SetTarget(m_MouseTarget);
	m_pDrunkAgent->Update(deltaTime);

	m_pFlee->SetTarget(TargetData(m_pDrunkAgent->GetPosition()));
	m_pSoberAgent->Update(deltaTime);

	if (m_TrimWorld) {
		m_pDrunkAgent->TrimToWorld(m_TrimWorldSize);
		m_pSoberAgent->TrimToWorld(m_TrimWorldSize);
	}


}

void App_CombinedSteering::Render(float deltaTime) const {

	m_pDrunkAgent->Render(deltaTime);
	m_pDrunkAgent->SetRenderBehavior(m_CanDebugRender);

	m_pSoberAgent->Render(deltaTime);
	m_pSoberAgent->SetRenderBehavior(m_CanDebugRender);

	if (m_TrimWorld)
	{
		std::vector<Elite::Vector2> points =
		{
			{ -m_TrimWorldSize,m_TrimWorldSize },
			{ m_TrimWorldSize,m_TrimWorldSize },
			{ m_TrimWorldSize,-m_TrimWorldSize },
			{-m_TrimWorldSize,-m_TrimWorldSize }
		};
		DEBUGRENDERER2D->DrawPolygon(&points[0], 4, { 1,0,0,1 }, 0.4f);
	}

	//Render Target
	if(m_VisualizeMouseTarget)
		DEBUGRENDERER2D->DrawSolidCircle(m_MouseTarget.Position, 0.3f, { 0.f,0.f }, { 1.f,0.f,0.f },-0.8f);
}
