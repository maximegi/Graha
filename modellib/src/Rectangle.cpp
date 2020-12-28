#include <iostream>
#include <glm.hpp>

#include "Rectangle.hpp"

bool Rectangle::isIn(float x, float y, float z)
{
	glm::vec3 corner = mCenter - (mW/2.f) * mOrientationW - (mH/2.f) * mOrientationH - (mD/2.f) * mOrientationD;

	//Variables names are picked for a regular box

	//Is P(x,y,z) between sides W ?
	float detLeftSide = glm::determinant(glm::mat3(mOrientationH, mOrientationD, glm::vec3(x,y,z) - corner));
	float detRightSide = glm::determinant(glm::mat3(mOrientationH, mOrientationD, glm::vec3(x,y,z) - (corner + mW*mOrientationW)));
	float productW = detLeftSide * detRightSide;

	//Is P(x,y,z) between sides H ?
	float detBottomSide = glm::determinant(glm::mat3(mOrientationD, mOrientationW, glm::vec3(x,y,z) - corner));
	float detTopSide = glm::determinant(glm::mat3(mOrientationD, mOrientationW, glm::vec3(x,y,z) - (corner + mH*mOrientationH)));
	float productH = detBottomSide * detTopSide;

	//Is P(x,y,z) between sides D ?
	float detBackSide = glm::determinant(glm::mat3(mOrientationW, mOrientationH, glm::vec3(x,y,z) - corner));
	float detFrontSide = glm::determinant(glm::mat3(mOrientationW, mOrientationH, glm::vec3(x,y,z) - (corner + mD*mOrientationD)));
	float productD = detBackSide * detFrontSide;

	//if P is between 2 faces, one determinant is positive and the other is negative so their product is negative 
	//P is in the rectangle if it is between all its faces

	return (productW <= 0 && productH <= 0 && productD <= 0);
}