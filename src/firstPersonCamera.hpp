#pragma once

#include "glm.hpp"

class FreeflyCamera {

private:
	glm::vec3 mPosition;
	float mPhi;
	float mTheta;
	glm::vec3 mFrontVector;
	glm::vec3 mLeftVector;
	glm::vec3 mUpVector;
	glm::vec3

	void computeDirectionVectors()
	{
		mFrontVector = glm::vec3(glm::cos(mTheta)*glm::sin(mPhi),glm::sin(mTheta),glm::cos(mTheta)*glm::cos(mPhi));
		mLeftVector = glm::vec3(glm::sin(mPhi+glm::half_pi<float>()),0.,glm::cos(mPhi+glm::half_pi<float>()));
		mUpVector = glm::cross(mFrontVector,mLeftVector);
	}

public:
	FreeflyCamera() : mPosition(glm::vec3(0.f,0.f,0.f)), mPhi(glm::pi<float>()), mTheta(0.f) {
		computeDirectionVectors();
	}

	void moveLeft(float t){ mPosition += t*mLeftVector; }
	void moveFront(float t){ mPosition += t*mFrontVector; }
	void moveUp(float t){ mPosition += t*mUpVector; }
	void rotateLeft(float degrees){ 
		mPhi += glm::radians(degrees);
		computeDirectionVectors();
	}
	void rotateUp(float degrees){ 
		mTheta += glm::radians(degrees);
		computeDirectionVectors();
	}

	glm::mat4 getViewMatrix(){ return glm::lookAt(mPosition,mPosition+mFrontVector,mUpVector); }

	glm::vec3 getPosition(){ return mPosition; }
	glm::vec3 getDirection(){ return mFrontVector; }
};