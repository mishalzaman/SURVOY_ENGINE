#include "Resource.h"

ENGINE::Resource::Resource():
	_textureId(0),
	_shaderId(0),
	_position(glm::vec2(0,0))
{
	_renderQuad = new RenderQuad;
}

ENGINE::Resource::~Resource()
{
	delete _renderQuad;
}

void ENGINE::Resource::Create(const std::string& path)
{
	Texture::Load(_textureId, path);
}

void ENGINE::Resource::SetPosition(glm::vec2 position)
{
	_position = position;
}

void ENGINE::Resource::SetShader(GLuint id)
{
	_shaderId = id;
}

void ENGINE::Resource::Draw()
{

	_renderQuad->Render(_shaderId, _textureId, _position.x, _position.y, 256, 256, glm::vec3(1, 1, 1));
}
