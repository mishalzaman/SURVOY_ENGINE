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
	int e = _entityManager.getIdByTag("CharacterController");

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Engine Information");
	ImGui::PushItemWidth(312);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::Text("DeltaTime: %.6f", _deltaTime);
	ImGui::SeparatorText("Character Orientation");
	ImGui::Text("Position: %.2f %.2f %.2f", orientation->Position.x, orientation->Position.y, orientation->Position.z);
	ImGui::Text("Yaw: %.2f", orientation->Yaw);
	ImGui::Text("Pitch: %.2f", orientation->Pitch);
	ImGui::Text("Forward: %.2f %.2f %.2f", orientation->Forward.x, orientation->Forward.y, orientation->Forward.z);
	ImGui::Text("Right: %.2f %.2f %.2f", orientation->Right.x, orientation->Right.y, orientation->Right.z);
	ImGui::Text("Up: %.2f %.2f %.2f", orientation->Up.x, orientation->Up.y, orientation->Up.z);
	ImGui::SeparatorText("Character Movement");
	ImGui::Text("Velocity: %.4f %.4f %.4f", motion->Velocity.x, motion->Velocity.y, motion->Velocity.z);
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
