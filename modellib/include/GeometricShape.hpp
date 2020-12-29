#include <iostream>
#include <glm.hpp>

class GeometricShape
{
public:
	GeometricShape(glm::mat4 place) : mCenter(glm::vec3(place*glm::vec4(0.f,0.f,0.f,1.f))){}
	bool isIn(glm::vec3 position);
	void updateShape(glm::mat4 tranformations);

protected:
	glm::vec3 mCenter;
};