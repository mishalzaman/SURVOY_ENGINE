#include "Resource.h"

ENGINE::Resource::Resource():
	_textureId(0),
	_shaderId(0),
	_position(glm::vec2(0,0)),
	_width(0),
	_height(0),
	_scale(1),
	_channels(1)
{
}

ENGINE::Resource::~Resource()
{
}

void ENGINE::Resource::Create(const std::string& path)
{
	Texture::Load(_textureId, path, _width, _height, _channels);
}

void ENGINE::Resource::SetPosition(glm::vec2 position)
{
	_position = position;
}

void ENGINE::Resource::SetShader(GLuint id)
{
	_shaderId = id;
}
