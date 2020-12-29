#include <iostream>
#include <glm.hpp>

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

	//Calulation of a point around the previous cylinder. 
	glm::vec3 direction;
	//we are trying to get a directional vector of the plane of the dics
	if(glm::vec3(mCenter + glm::dot(glm::normalize(mOrientation),glm::vec3(0.f) - mCenter) * mOrientation) != glm::vec3(0.f))
	{
		direction = mRad * glm::normalize(mCenter + glm::dot(glm::normalize(mOrientation),glm::vec3(0.f) - mCenter) * mOrientation);
	} //Doesn't work if the point (0,0,0) is on the main axis line, we try another point
	else if(glm::vec3(mCenter + glm::dot(glm::normalize(mOrientation),glm::vec3(1.f,0.f,0.f) - mCenter) * mOrientation) != glm::vec3(0.f))
	{
		direction = mRad * (glm::vec3(1.f,0.f,0.f) - glm::normalize(mCenter + glm::dot(glm::normalize(mOrientation),glm::vec3(1.f,0.f,0.f) - mCenter) * mOrientation));
	} //If the main axis line go through points (0,0,0) and (1,0,0), we try with another point which is not on this axis.
	else
	{
		direction = mRad * (glm::vec3(0.f,1.f,0.f) - glm::normalize(mCenter + glm::dot(glm::normalize(mOrientation),glm::vec3(0.f,1.f,0.f) - mCenter) * mOrientation));
	}
	glm::vec3 pointAround = mCenter + direction;

	//calculate new positions
	mCenter += translation;

	glm::vec3 newDir = glm::vec3(transformations * glm::vec4(pointAround - mCenter,0.f));
	glm::vec3 newOrientation = glm::vec3(transformations * glm::vec4((mH/2.f) * mOrientation,0.f));
	mOrientation = glm::normalize(newOrientation);

	glm::vec3 pointOnDiscAfterTransformation = mCenter + newOrientation;
	glm::vec3 pointAroundAfterTransformation = mCenter + newDir;

	//update attributes
	mCenter += translation;
	mH = glm::l2Norm(mCenter,pointOnDiscAfterTransformation) * 2.f;
	mRad = glm::l2Norm(mCenter,pointAroundAfterTransformation);
}