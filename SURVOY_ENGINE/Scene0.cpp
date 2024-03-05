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

	/*-------------------
	 CHARACTER CONTROLLER
	--------------------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::OrientationComponent>(entityId, glm::vec3(0, 10, 0));
	entityManager_->addComponent<ECS::VelocityComponent>(entityId);
	entityManager_->addComponent<ECS::KinematicCapsulePhysicsBodyComponent>(entityId, 0.25f, 1.25f);
	entityManager_->addByTag("CharacterController", entityId);


	/*----------
	 CAMERAS
	-----------*/

	// First person
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::RenderTargetDimensionsComponent>(
		entityId,
		1024.f,
		768.f
	);
	entityManager_->addComponent<ECS::CameraMatricesComponent>(entityId);
	entityManager_->addComponent<ECS::OrientationComponent>(entityId, glm::vec3(0, 2, 16));
	entityManager_->addComponent<ECS::CameraMouseComponent>(entityId);
	entityManager_->addComponent<ECS::TargetComponent>(entityId);
	entityManager_->addComponent<ECS::CameraFOVComponent>(entityId, 60.f);
	entityManager_->addByTag("CameraFirstPerson", entityId);

	// Third person
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::RenderTargetDimensionsComponent>(
		entityId,
		1024.f,
		768.f
	);
	entityManager_->addComponent<ECS::CameraMatricesComponent>(entityId);
	entityManager_->addComponent<ECS::OrientationComponent>(entityId, glm::vec3(0, 2, 16));
	entityManager_->addComponent<ECS::CameraMouseComponent>(entityId);
	entityManager_->addComponent<ECS::TargetComponent>(entityId);
	entityManager_->addComponent<ECS::CameraFOVComponent>(entityId, 60.f);
	entityManager_->addByTag("CameraThirdPerson", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ActiveCameraComponent>(entityId, "CameraFirstPerson");
	entityManager_->addByTag("ActiveCamera", entityId);
	
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

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::ProgramComponent>(entityId, *fontShader_);
	entityManager_->addByTag("FontShader", entityId);

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
	entityManager_->addComponent<ECS::NearFarPlanesComponent>(entityId, 0.1f, 100.f);
	entityManager_->addComponent<ECS::ShadowResolutionComponent>(entityId, 1024.f, 1024.f);
	entityManager_->addByTag("DepthTexture", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::LightSpaceMatrixComponent>(entityId);
	entityManager_->addByTag("LightSpaceMatrix", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::BuffersComponent>(entityId);
	entityManager_->addByTag("DepthQuadBuffers", entityId);

	/*----------
	COLOUR MAP
	-----------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::BuffersComponent>(entityId);
	entityManager_->addByTag("ColourFBO", entityId);

	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::TextureComponent>(entityId);
	entityManager_->addByTag("ColourTexture", entityId);

	/*----------
	 LIGHTING
	-----------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::DirectionalLightComponent>(entityId, glm::vec3(22,20,22));
	entityManager_->addByTag("DirectionalLight", entityId);

	/*----------------
	 GRAPHICS PIPELINE
	-----------------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::RenderPassComponent>(entityId, ECS::RenderPassComponent::NULL_MAP);
	entityManager_->addComponent<ECS::RenderTargetDimensionsComponent>(
		entityId,
		1024.f,
		768.f
	);
	entityManager_->addByTag("RenderPipeline", entityId);

	/*----------------
	 FONT
	-----------------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::FontCharactersComponent>(entityId);
	entityManager_->addComponent<ECS::BuffersComponent>(entityId);
	entityManager_->addByTag("Font", entityId);

	/*----------------
	 DEBUG PRINT
	-----------------*/
	entityId = entityManager_->createEntity();
	entityManager_->addComponent<ECS::DebugPrintComponent>(entityId);
	entityManager_->addByTag("DebugPrint", entityId);

	Scene::Load();
}
