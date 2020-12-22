#include <iostream>
#include <glm.hpp>

typedef struct Material
{
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float shininess;
}Material;