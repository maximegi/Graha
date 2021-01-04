#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uModelMatrix;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = uMVPMatrix * vec4(aPos, 1.0);
}