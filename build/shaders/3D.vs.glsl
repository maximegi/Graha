#version 300 es

precision mediump float;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uModelMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uMVPMatrix;
uniform mat4 uNormalMatrix;

out vec3 vPosition_vs;
out vec3 vNormal_vs;
out vec2 vTexCoords;

void main()
{
	vec4 vertexPosition = vec4(aVertexPosition,1);
	vec4 vertexNormal = vec4(aVertexNormal,0);

	vPosition_vs = vec3(uModelMatrix*vertexPosition);
	vNormal_vs = vec3(uNormalMatrix*vertexNormal);
	vTexCoords = aTexCoords;

	gl_Position = uMVPMatrix*vertexPosition;
}