#include <iostream>
#include <glm.hpp>

#include "GeometricShape.hpp"

class Cylinder : public GeometricShape
{
public:
	Cylinder() : GeometricShape(glm::vec3(0.f,0.f,0.f)), mRad(0.f), mH(0.f), mOrientation(glm::vec3(0.f,1.f,0.f)) {}
	Cylinder(glm::vec3 center, float rad, float h, glm::vec3 orientation = glm::vec3(0.f,1.f,0.f))
		: GeometricShape(center), mRad(rad), mH(h)
		{
			mOrientation = glm::normalize(orientation);
		}
	bool isIn(float x, float y, float z);
private:
	float mRad, mH;
	glm::vec3 mOrientation;
};