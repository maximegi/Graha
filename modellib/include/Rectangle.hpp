#pragma once

#include <iostream>
#include <glm.hpp>

#include "GeometricShape.hpp"

//In fact, it will be a parallelepiped
class Rectangle : public GeometricShape
{
public:
	Rectangle() : GeometricShape(), mW(2.f), mH(2.f), mD(2.f), mOrientationW(glm::vec3(1.f,0.f,0.f)), mOrientationH(glm::vec3(0.f,1.f,0.f)), mOrientationD(glm::vec3(0.f,0.f,1.f)) {}
	Rectangle(glm::mat4 place)
		: GeometricShape(), mW(2.f), mH(2.f), mD(2.f), mOrientationW(glm::vec3(1.f,0.f,0.f)), mOrientationH(glm::vec3(0.f,1.f,0.f)), mOrientationD(glm::vec3(0.f,0.f,1.f))
		{
			updateShape(place);
		}
	bool contains(glm::vec3 position);
	void updateShape(glm::mat4 transformations);
private:
	float mW, mH, mD;
	glm::vec3 mOrientationW, mOrientationH, mOrientationD; //Define the shape of the box,and its orientation
};