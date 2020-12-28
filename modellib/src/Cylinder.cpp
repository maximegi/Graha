#include <iostream>
#include <glm.hpp>

#include "Cylinder.hpp"

bool Cylinder::isIn(float x, float y, float z)
{
	glm::vec3 P(x,y,z);
	glm::vec3 base = mCenter - (mH/2.f) * mOrientation;
	glm::vec3 baseToP = P - base;

	//Is P(x,y,z) between discs ?
	//if P is between the two discs, the distance between the base and the projection is less than the height
	//P is in not in the cylinder if it is not between its discs

	float heightP = glm::dot(glm::normalize(mOrientation),baseToP);

	//The projection of P(x,y,z) on the main axis of the cylinder
	glm::vec3 projP = base + heightP*mOrientation;

	//if the distance between the main axis and P is less than the radius, P is inside the cylinder
	float dist = glm::l2Norm(projP,P);

	return (heightP >= 0 && heightP <= mH && dist <= mRad);
}