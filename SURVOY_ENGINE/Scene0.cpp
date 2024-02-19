#include "Scene0.h"

Scene0::Scene0()
{

}

void Scene0::Load()
{
	auto LevelModel = std::make_unique<ENGINE::Model>("assets/Scene_0/scene.fbx");

	int entityId = 0;

	/*----------
	 MESH
	-----------*/
	for (int i = 0; i < LevelModel->Meshes().size(); i++) {
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

	/*----------
	 CAMERA
	-----------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ScreenDimensionsComponent>(
		entityId,
		1024.f,
		768.f
	);
	entityManager_->addComponent<ECS::CameraMatricesComponent>(entityId);
	entityManager_->addComponent<ECS::OrientationComponent>(entityId, glm::vec3(0, 2, 16));
	entityManager_->addComponent<ECS::CameraMouseComponent>(entityId);
	entityManager_->addComponent<ECS::TargetComponent>(entityId);
	entityManager_->addByTag("CameraFreeLook", entityId);
	
	/*----------
	 SHADERS
	-----------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ProgramComponent>(entityId, *defaultShader_);
	entityManager_->addByTag("DefaultShader", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ProgramComponent>(entityId, *skyboxShader_);
	entityManager_->addByTag("SkyBoxShader", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ProgramComponent>(entityId, *debugDepthQuadShader_);
	entityManager_->addByTag("DebugDepthQuadShader", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ProgramComponent>(entityId, *depthShader_);
	entityManager_->addByTag("DepthShader", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ProgramComponent>(entityId, *shadowMappedColourShader_);
	entityManager_->addByTag("ShadowMapColourShader", entityId);

	/*----------
	 SKY BOX
	-----------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::SkyBoxComponent>(
		entityId,
		"assets/skybox/intersteller/right.png",
		"assets/skybox/intersteller/left.png",
		"assets/skybox/intersteller/top.png",
		"assets/skybox/intersteller/bottom.png",
		"assets/skybox/intersteller/front.png",
		"assets/skybox/intersteller/back.png"
	);
	entityManager_->addByTag("SkyBox", entityId);

	/*----------
	 DEPTH MAP
	-----------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::BuffersComponent>(entityId);
	entityManager_->addByTag("DepthFBO", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::TextureComponent>(entityId);
	entityManager_->addByTag("DepthTexture", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::LightSpaceMatrixComponent>(entityId);
	entityManager_->addByTag("LightSpaceMatrix", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::BuffersComponent>(entityId);
	entityManager_->addByTag("DepthQuadBuffers", entityId);

	/*----------
	 LIGHTING
	-----------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::DirectionalLightComponent>(entityId, glm::vec3(22,20,22));
	entityManager_->addByTag("DirectionalLight", entityId);

	Scene::Load();
}
