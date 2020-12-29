#include <iostream>
#include <glm.hpp>

#include "Rectangle.hpp"

bool Rectangle::isIn(glm::vec3 position)
{
	glm::vec3 corner = mCenter - (mW/2.f) * mOrientationW - (mH/2.f) * mOrientationH - (mD/2.f) * mOrientationD;

	//Variables names are picked for a regular box

	//Is P(x,y,z) between sides W ?
	float detLeftSide = glm::determinant(glm::mat3(mOrientationH, mOrientationD, position - corner));
	float detRightSide = glm::determinant(glm::mat3(mOrientationH, mOrientationD, position - (corner + mW*mOrientationW)));
	float productW = detLeftSide * detRightSide;

	//Is P(x,y,z) between sides H ?
	float detBottomSide = glm::determinant(glm::mat3(mOrientationD, mOrientationW, position - corner));
	float detTopSide = glm::determinant(glm::mat3(mOrientationD, mOrientationW, position - (corner + mH*mOrientationH)));
	float productH = detBottomSide * detTopSide;

	//Is P(x,y,z) between sides D ?
	float detBackSide = glm::determinant(glm::mat3(mOrientationW, mOrientationH, position - corner));
	float detFrontSide = glm::determinant(glm::mat3(mOrientationW, mOrientationH, position - (corner + mD*mOrientationD)));
	float productD = detBackSide * detFrontSide;

	//if P is between 2 faces, one determinant is positive and the other is negative so their product is negative 
	//P is in the rectangle if it is between all its faces

	return (productW <= 0 && productH <= 0 && productD <= 0);
}

void Rectangle::updateShape(glm::mat4 transformations)
{
	//calculate translation
	glm::vec3 translation = glm::vec3(transformations * glm::vec4(0.f,0.f,0.f,1.f));

	//calculate new orientations
	glm::vec3 directionW = glm::vec3(transformations * glm::vec4((mW/2.f) * mOrientationW,0.f));
	glm::vec3 directionH = glm::vec3(transformations * glm::vec4((mH/2.f) * mOrientationH,0.f));
	glm::vec3 directionD = glm::vec3(transformations * glm::vec4((mD/2.f) * mOrientationD,0.f));
	
	mOrientationW = glm::normalize(directionW);
	mOrientationH = glm::normalize(directionH);
	mOrientationD = glm::normalize(directionD);

	//calculate new positions
	mCenter += translation;

	glm::vec3 pointOnWAfterTransformation = mCenter + directionW;
	glm::vec3 pointOnHAfterTransformation = mCenter + directionH;
	glm::vec3 pointOnDAfterTransformation = mCenter + directionD;

	//update attributes
	mW = glm::l2Norm(mCenter,pointOnWAfterTransformation) * 2.f;
	mH = glm::l2Norm(mCenter,pointOnHAfterTransformation) * 2.f;
	mD = glm::l2Norm(mCenter,pointOnDAfterTransformation) * 2.f;
}