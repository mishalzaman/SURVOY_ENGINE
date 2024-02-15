#include "Scene0.h"

Scene0::Scene0()
{

}

void Scene0::Load()
{
	auto LevelModel = std::make_unique<BAE::Model>("assets/Level_0/level.fbx");

	int entityId = 0;
	
	// Mesh
	for (int i = 0; i < LevelModel->Meshes().size(); i++) {
		if (LevelModel->Meshes()[i].Name() == "PLAYER_START") {			
			entityId = entityManager_->createEntity();
			entityManager_->addComponent<ECS::DynamicCapsulePhysicsBodyComponent>(entityId);
			entityManager_->addComponent<ECS::VelocityComponent>(entityId);
			entityManager_->addComponent<ECS::OrientationComponent>(entityId, LevelModel->Meshes()[i].Position());
			entityManager_->addByTag("PlayerController", entityId);
		}
		else {
			entityId = entityManager_->createEntity();
	
			entityManager_->addComponent<ECS::TransformComponent>(entityId, LevelModel->Meshes()[i].Transformation());
			entityManager_->addComponent<ECS::MeshComponent>(
				entityId,
				LevelModel->Meshes()[i].Name(),
				LevelModel->Meshes()[i].Vertices(),
				LevelModel->Meshes()[i].Indices()
			);
			entityManager_->addComponent<ECS::BuffersComponent>(entityId);
			entityManager_->addComponent<ECS::TexturesComponent>(entityId, LevelModel->Meshes()[i].Textures());
			entityManager_->addComponent<ECS::StaticPhysicsBodyComponent>(entityId);
			entityManager_->addComponent<ECS::CameraMatricesComponent>(entityId);
	
			entityManager_->addByTag("Mesh", entityId);
		}
	}

	// CameraThirdPerson
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ScreenDimensionsComponent>(
		entityId,
		1024.f,
		768.f
	);
	entityManager_->addComponent<ECS::CameraMatricesComponent>(entityId);
	entityManager_->addComponent<ECS::OrientationComponent>(entityId, glm::vec3(0, 1, 0));
	entityManager_->addComponent<ECS::CameraMouseComponent>(entityId);
	entityManager_->addComponent<ECS::TargetComponent>(entityId);
	entityManager_->addByTag("CameraThirdPerson", entityId);
	
	// Shader
	entityManager_->addComponent<ECS::ProgramComponent>(entityId, *defaultShader_);
	entityManager_->addByTag("DefaultShader", entityId);

	Scene::Load();
}
