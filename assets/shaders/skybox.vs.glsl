#version 300 es

precision mediump float;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 uMVPMatrix;

out vec2 vTexCoords;

void main()
{
	vTexCoords = aTexCoords;
	gl_Position = uMVPMatrix*vec4(aVertexPosition,1);
}