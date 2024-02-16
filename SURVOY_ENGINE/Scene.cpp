#include "Scene.h"

Scene::Scene()
{
	defaultShader_ = std::make_unique<Shader>("lighting_3d_vertex.glsl", "lighting_3d_fragment.glsl");
	skyboxShader_ = std::make_unique<Shader>("skybox_vertex.glsl", "skybox_fragment.glsl");
	physics_ = std::make_unique<ENGINE::Physics>();

	systemManager_ = std::make_unique<ECS::SystemManager>();
	entityManager_ = std::make_unique<ECS::EntityManager>();
	eventManager_ = std::make_unique<EventManager>();
}

/*
GAME LOOP
*/

void Scene::Load()
{
	systemManager_->AddSystem<ECS::FBOBeginSystem>(*eventManager_);
	systemManager_->AddSystem<ECS::PhysicsSystem>(*entityManager_, *physics_, *eventManager_);
	//systemManager_->AddSystem<ECS::CharacterControllerSystem>(*entityManager_, *physics_, *eventManager_);
	//systemManager_->AddSystem<ECS::CameraThirdPersonSystem>(*entityManager_, *physics_, *eventManager_);
	systemManager_->AddSystem<ECS::CameraFreeLookSystem>(*entityManager_, *physics_, *eventManager_);
	systemManager_->AddSystem<ECS::RendererStatic3DSystem>(*entityManager_, *physics_, *eventManager_);
	systemManager_->AddSystem<ECS::SkyBoxSystem>(*entityManager_, *eventManager_);
	systemManager_->AddSystem<ECS::FBORendererSystem>(*eventManager_);

	systemManager_->Load();
}

void Scene::UpdatePrePhysics()
{
	systemManager_->UpdatePrePhysics();
}

void Scene::UpdatePhysics(float deltaTime)
{
	systemManager_->Update(deltaTime);
}

void Scene::UpdatePostPhysics()
{
	systemManager_->UpdatePostPhysics();
}

void Scene::Render()
{
	systemManager_->Render();
}

void Scene::Unload()
{
	systemManager_->Unload();
}
