#pragma once

#include <iostream>
#include <glm.hpp>

#include "GeometricShape.hpp"

class Spheric : public GeometricShape
{
public:
	Spheric() : GeometricShape(), mRad(1.f) {}
	Spheric(glm::mat4 place) : GeometricShape(), mRad(1.f)
	{
		updateShape(place);
	}

	bool contains(glm::vec3 position) { return glm::l2Norm(position,mCenter) <= mRad; }
	void updateShape(glm::mat4 transformations) //We assume that the scale is always the same on the 3 axis
	{
		//calculate translation
		glm::vec3 translation = glm::vec3(transformations * glm::vec4(0.f,0.f,0.f,1.f));

		//transformation of one point on the sphere to get the new radius
		glm::vec3 direction = glm::vec3(transformations * glm::vec4(mRad*glm::vec3(1.f,0.f,0.f),0.f));
		
		//calculate new positions
		mCenter = mCenter + translation;
		glm::vec3 pointAfterTransformation = mCenter + direction;

		mRad = glm::l2Norm(mCenter,pointAfterTransformation);
	}
private:
	float mRad;
};