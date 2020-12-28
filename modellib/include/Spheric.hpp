#include <iostream>
#include <glm.hpp>

#include "GeometricShape.hpp"

class Spheric : public GeometricShape
{
public:
	Spheric() : GeometricShape(glm::vec3(0.f,0.f,0.f)), mRad(0.f) {}
	Spheric(glm::vec3 center, float rad) : GeometricShape(center), mRad(rad) {}
	bool isIn(float x, float y, float z)
	{
		return glm::l2Norm(glm::vec3(x,y,z),mCenter) <= mRad;
	}
private:
	float mRad;
};