#version 300 es
precision mediump float;
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vFragTexture;


void main() {
  vFragTexture = aVertexTexture;
  gl_Position = vec4(aVertexPosition, 0, 1);
}