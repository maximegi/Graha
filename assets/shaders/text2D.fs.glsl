#version 300 es
precision mediump float;

in vec2 vFragTexture;

out vec3 fFragColor;

uniform sampler2D uTexture;


void main()
{
	fFragColor = vec3(texture(uTexture, vFragTexture));
}