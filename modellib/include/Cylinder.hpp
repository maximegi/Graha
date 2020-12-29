#pragma once

#include <iostream>
#include <glm.hpp>

#include "GeometricShape.hpp"

class Cylinder : public GeometricShape
{
public:
	Cylinder() : GeometricShape(glm::mat4(1)), mRad(0.f), mH(0.f), mOrientation(glm::vec3(0.f,1.f,0.f)) {}
	Cylinder(glm::mat4 place, float rad, float h, glm::vec3 orientation = glm::vec3(0.f,1.f,0.f))
		: GeometricShape(place), mRad(rad), mH(h)
		{
			mOrientation = glm::normalize(orientation);
		}

	bool isIn(glm::vec3 position);
	void updateShape(glm::mat4 transformations); //We assume that scale will always keep circles, and never transform them into ellipses

private:
	float mRad, mH;
	glm::vec3 mOrientation;
};