#include <iostream>
#include <glm.hpp>
#include <cmath>

#include "Cylinder.hpp"

bool Cylinder::isIn(glm::vec3 position)
{
	glm::vec3 base = mCenter - (mH/2.f) * mOrientation;
	glm::vec3 baseToP = position - base;

	//Is P(x,y,z) between discs ?
	//if P is between the two discs, the distance between the base and the projection is less than the height
	//P is in not in the cylinder if it is not between its discs

	float heightP = glm::dot(glm::normalize(mOrientation),baseToP);

	//The projection of P(x,y,z) on the main axis of the cylinder
	glm::vec3 projP = base + heightP*mOrientation;

	//if the distance between the main axis and P is less than the radius, P is inside the cylinder
	float dist = glm::l2Norm(projP,position);

	return (heightP >= 0 && heightP <= mH && dist <= mRad);
}

void Cylinder::updateShape(glm::mat4 transformations) //We assume that scale will always keep circles, and never transform them into ellipses
{
	//calculate translation
	glm::vec3 translation = glm::vec3(transformations * glm::vec4(0.f,0.f,0.f,1.f));

	//calculate previous positions
	glm::vec3 pointOnDisc = mCenter + (mH/2.f) * mOrientation;

	//main axis equation is
	//mOrientation.x * x + mOrientation.y * y + mOrientation.z * z = mOrientation.x * mCenter.x + mOrientation.y * mCenter.y + mOrientation.z * mCenter.z
	//We want to calculate a directionnal vector for the plane parrallel to the discs and containing mCenter
	//We need to find a vectr v(x,y,z) verifying 
	//mOrientation.x * x + mOrientation.y * y + mOrientation.z * z = 0
	float normalX = mOrientation.x;
	float normalY = mOrientation.y;
	float c = mOrientation.z;

	glm::vec3 direction;
	if(normalX!= 0)
	{
		//In this case we resolve the equation : V = y * (-mOrientation.y/mOrientation.x,1,0) + z * (-mOrientation.z/mOrientation.x,0,1) 
		//so (-mOrientation.y/mOrientation.x,1,0) is a directional vector. We normalize it for the following steps
		direction = glm::normalize(glm::vec3(-normalY/normalX,1.f,0.f));
	}
	else
	{
		//if normal has no component on x, (1,0,0) is a directional vector of the plane, and it is already normalized
		direction = glm::vec3(1.f, 0.f,0.f);
	}
	glm::vec3 pointAround = mCenter + mRad * direction;

	//calculate new positions
	mCenter = mCenter + translation;

	glm::vec3 newDir = glm::vec3(transformations * glm::vec4(mRad*direction,0.f));
	glm::vec3 newOrientation = glm::vec3(transformations * glm::vec4((mH/2.f) * mOrientation,0.f));
	mOrientation = glm::normalize(newOrientation);

	glm::vec3 pointOnDiscAfterTransformation = mCenter + newOrientation;
	glm::vec3 pointAroundAfterTransformation = mCenter + newDir;

	//update attributes
	mH = glm::l2Norm(mCenter,pointOnDiscAfterTransformation) * 2.f;
	mRad = glm::l2Norm(mCenter,pointAroundAfterTransformation);
}