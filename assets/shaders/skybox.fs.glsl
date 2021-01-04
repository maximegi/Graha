#version 300 es

precision mediump float;

out vec4 fFragColor;

in vec2 vTexCoords;

uniform sampler2D uSkybox;

void main()
{    
    fFragColor = texture(uSkybox, vTexCoords);
}