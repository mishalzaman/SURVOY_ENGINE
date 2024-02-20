#include "RenderOutputSystem.h"

ECS::RenderOutputSystem::RenderOutputSystem(EntityManager& entityManager):
	_entityManager(entityManager), _nearPlane(0.1f), _farPlane(64.f)
{
}

void ECS::RenderOutputSystem::Render()
{
	ECS::RenderPassComponent* renderPipeline = _entityManager.getComponent<ECS::RenderPassComponent>(
		_entityManager.getByTags("RenderPipeline")[0]
	);

	switch (renderPipeline->State) {
	case ECS::RenderPassComponent::SHADOW_MAP:
		_renderForDepthMap();
		break;
	case ECS::RenderPassComponent::COLOUR_MAP:
		_renderForColourMap();
		break;
	}
}

void ECS::RenderOutputSystem::_renderForDepthMap()
{
	glm::mat4 lightProjection, lightView;

	lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, _nearPlane, _farPlane);

	ECS::DirectionalLightComponent* directionalLight = _entityManager.getComponent<ECS::DirectionalLightComponent>(
		_entityManager.getByTags("DirectionalLight")[0]
	);
	ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
		_entityManager.getByTags("DepthFBO")[0]
	);
	ECS::TextureComponent* depthTexture = _entityManager.getComponent<ECS::TextureComponent>(
		_entityManager.getByTags("DepthTexture")[0]
	);
	ECS::ProgramComponent* depthShader = _entityManager.getComponent<ECS::ProgramComponent>(
		_entityManager.getByTags("DepthShader")[0]
	);
	ECS::LightSpaceMatrixComponent* lsm = _entityManager.getComponent<ECS::LightSpaceMatrixComponent>(
		_entityManager.getByTags("LightSpaceMatrix")[0]
	);
	ECS::CameraMatricesComponent* cameraMatrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(
		_entityManager.getByTags("CameraFreeLook")[0]
	);

	lightView = glm::lookAt(directionalLight->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lsm->LightSpaceMatrix = lightProjection * lightView;

	depthShader->Program.use();
	depthShader->Program.setMat4("lightSpaceMatrix", lsm->LightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, buffers->DepthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	ECS::RenderPassComponent* renderPipeline = _entityManager.getComponent<ECS::RenderPassComponent>(
		_entityManager.getByTags("RenderPipeline")[0]
	);

	std::vector<int> entities = _entityManager.getByTags("Mesh");
	for (int entityId : entities) {
		// Retrieve the components required for rendering
		ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
		ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
		ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
		ECS::TexturesComponent* textures = _entityManager.getComponent<ECS::TexturesComponent>(entityId);

		if (transform && mesh && buffers && textures) {
			depthShader->Program.setMat4("model", transform->Transformation);

			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			for (unsigned int i = 0; i < textures->Textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::string number;
				std::string name = textures->Textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);

				glBindTexture(GL_TEXTURE_2D, textures->Textures[i].id);
			}
			glActiveTexture(GL_TEXTURE0);

			// draw mesh
			glBindVertexArray(buffers->VAO);
			glDrawElements(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);


		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderPipeline->State = ECS::RenderPassComponent::COLOUR_MAP;
}

void ECS::RenderOutputSystem::_renderForColourMap()
{
	std::vector<int> entities = _entityManager.getByTags("Mesh");

	int e = _entityManager.getByTag("ShadowMapColourShader")[0];
	ECS::ProgramComponent* shader = _entityManager.getComponent<ECS::ProgramComponent>(e);

	ECS::DirectionalLightComponent* directionalLight = _entityManager.getComponent<ECS::DirectionalLightComponent>(
		_entityManager.getByTags("DirectionalLight")[0]
	);
	ECS::TextureComponent* depthTexture = _entityManager.getComponent<ECS::TextureComponent>(
		_entityManager.getByTags("DepthTexture")[0]
	);
	ECS::LightSpaceMatrixComponent* lsm = _entityManager.getComponent<ECS::LightSpaceMatrixComponent>(
		_entityManager.getByTags("LightSpaceMatrix")[0]
	);
	ECS::CameraMatricesComponent* cameraMatrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(
		_entityManager.getByTags("CameraFreeLook")[0]
	);
	ECS::OrientationComponent* cameraOrientation = _entityManager.getComponent<ECS::OrientationComponent>(
		_entityManager.getByTags("CameraFreeLook")[0]
	);

	if (shader) {
		shader->Program.use();
		shader->Program.setVec3("lightPos", directionalLight->Position);
		shader->Program.setVec3("viewPos", cameraOrientation->Position);
		shader->Program.setVec3("lightColor", glm::vec3(1, 1, 1));
		shader->Program.setMat4("projection", cameraMatrices->Projection);
		shader->Program.setMat4("view", cameraMatrices->View);
		shader->Program.setInt("diffuseTexture", 0);
		shader->Program.setInt("shadowMap", 1);
		shader->Program.setMat4("lightSpaceMatrix", lsm->LightSpaceMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture->Texture.id);
	}
	else {
		std::cout << "could not load shader" << std::endl;
	}

	for (int entityId : entities) {
		// Retrieve the components required for rendering
		ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
		ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
		ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
		ECS::TexturesComponent* textures = _entityManager.getComponent<ECS::TexturesComponent>(entityId);



		if (transform && mesh && buffers && textures && shader) {
			shader->Program.setMat4("model", transform->Transformation);

			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			for (unsigned int i = 0; i < textures->Textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::string number;
				std::string name = textures->Textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);

				glBindTexture(GL_TEXTURE_2D, textures->Textures[i].id);
			}
			glActiveTexture(GL_TEXTURE0);

			// draw mesh
			glBindVertexArray(buffers->VAO);
			glDrawElements(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		}
	}
}
