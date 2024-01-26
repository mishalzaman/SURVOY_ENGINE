#include "BuffersComponent.h"

ECS::BuffersComponent::BuffersComponent(std::vector<SVertex> vertices,
	std::vector<unsigned int> indices):
	VAO(0), VBO(0), EBO(0)
{
}
