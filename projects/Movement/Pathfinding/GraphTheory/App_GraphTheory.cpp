//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "App_GraphTheory.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EEularianPath.h"

using namespace Elite;

//Destructor
App_GraphTheory::~App_GraphTheory() {
	delete m_pGraph;
}

//Functions
void App_GraphTheory::Start() {

	//Initialization of your application. If you want access to the physics world you will need to store it yourself.
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(80.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(0, 0));
	DEBUGRENDERER2D->GetActiveCamera()->SetMoveLocked(false);
	DEBUGRENDERER2D->GetActiveCamera()->SetZoomLocked(false);

	m_pGraph = new Graph2D<GraphNode2D, GraphConnection2D>(false);
	m_pGraph->AddNode(new GraphNode2D(0, Vector2(20.f, 30.f)));
	m_pGraph->AddNode(new GraphNode2D(1, Vector2(-10.f, -10.f)));
	m_pGraph->AddConnection(new GraphConnection2D(0, 1));

}

void App_GraphTheory::Update(float deltaTime) {

	m_GraphEditor.UpdateGraph(m_pGraph);
	m_pGraph->SetConnectionCostsToDistance();

	auto eulerFinder = EulerianPath<GraphNode2D, GraphConnection2D>(m_pGraph);
	auto euleranity = eulerFinder.IsEulerian();

	switch (euleranity) {
	case Eulerianity::eulerian:
		cout << "eulerian\n";
		break;
	case Eulerianity::semiEulerian:
		cout << "seni-eulerian\n";
		break;
	case Eulerianity::notEulerian:
		cout << "not eulerian\n";
		break;
	default:
		break;
	}

	//------- UI --------
#ifdef PLATFORM_WINDOWS
#pragma region UI
	{
		//Setup
		int menuWidth = 150;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 90));
		ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false);
		ImGui::SetWindowFocus();
		ImGui::PushItemWidth(70);
		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
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

		ImGui::Text("Graph Theory");
		ImGui::Spacing();
		ImGui::Spacing();

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion
#endif
}

void App_GraphTheory::Render(float deltaTime) const {

	m_GraphRenderer.RenderGraph(m_pGraph, true, true, true, true);

}
