#pragma once

#include <iostream>
#include <glm.hpp>

#include "GeometricShape.hpp"

class Spheric : public GeometricShape
{
public:
	Spheric() : GeometricShape(glm::mat4(1)), mRad(0.f) {}
	Spheric(glm::mat4 place, float rad) : GeometricShape(place), mRad(rad) {}

	bool isIn(glm::vec3 position) { return glm::l2Norm(position,mCenter) <= mRad; }
	void updateShape(glm::mat4 transformations) //We assume that the scale is always the same on the 3 axis
	{
		//calculate translation
		glm::vec3 translation = glm::vec3(transformations * glm::vec4(0.f,0.f,0.f,1.f));

		glm::vec3 pointOnSphere = mCenter + mRad*glm::vec3(1.f,0.f,0.f);
		glm::vec3 pointAfterTransformation = glm::vec3(transformations * glm::vec4(pointOnSphere - mCenter,0.f));

		mCenter += translation;
		mRad = glm::l2Norm(mCenter,pointAfterTransformation);
	}
private:
	float mRad;
};