#include <iostream>
#include <glm.hpp>

class GeometricShape
{
public:
	GeometricShape(glm::vec3 center) : mCenter(center){}
	bool isIn(float x, float y, float z);

protected:
	glm::vec3 mCenter;
};