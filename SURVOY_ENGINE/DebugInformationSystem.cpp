#include "DebugInformationSystem.h"

ECS::DebugInformationSystem::DebugInformationSystem(EntityManager& entityManager, Physics& physics):
	_entityManager(entityManager), _physics(physics)
{
}

void ECS::DebugInformationSystem::Load()
{
}

void ECS::DebugInformationSystem::UpdatePreFixedTimestep()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Engine Information");
	ImGui::PushItemWidth(312);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::Text("DeltaTime: %.6f", _deltaTime);
	ImGui::PushItemWidth(312);
	ImGui::SeparatorText("Character");
	ImGui::End();
}

void ECS::DebugInformationSystem::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ECS::DebugInformationSystem::UpdateOnFixedTimestep(float deltaTime)
{
	_deltaTime = deltaTime;
}

void ECS::DebugInformationSystem::Unload()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
