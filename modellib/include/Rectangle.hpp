#include <iostream>
#include <glm.hpp>

#include "GeometricShape.hpp"

//In fact, it will be a parallelepiped
class Rectangle : public GeometricShape
{
public:
	Rectangle() : GeometricShape(glm::vec3(0.f,0.f,0.f)), mH(0.f), mW(0.f), mD(0.f), mOrientationW(glm::vec3(1.f,0.f,0.f)), mOrientationH(glm::vec3(0.f,1.f,0.f)), mOrientationD(glm::vec3(0.f,0.f,1.f)) {}
	Rectangle(glm::vec3 center, float w, float h, float d, glm::vec3 orientationW = glm::vec3(1.f,0.f,0.f), glm::vec3 orientationH = glm::vec3(0.f,1.f,0.f))
		: GeometricShape(center), mW(w), mH(h), mD(d)
		{
			mOrientationW = glm::normalize(orientationW);
			mOrientationH = glm::normalize(orientationH);
			mOrientationD = glm::normalize(glm::cross(mOrientationW,mOrientationH)); 
		}
	bool isIn(float x, float y, float z);
private:
	float mW, mH, mD;
	glm::vec3 mOrientationW, mOrientationH, mOrientationD; //Define the shape of the box,and its orientation
};