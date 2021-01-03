#pragma once

#include <iostream>
#include <glm.hpp>

class GeometricShape
{
public:
	GeometricShape() : mCenter(glm::vec3(0.f,0.f,0.f)){}
	bool isIn(glm::vec3 position);
	void updateShape(glm::mat4 tranformations);
	inline glm::vec3 getPosition(){return mCenter;}

protected:
	glm::vec3 mCenter;
};