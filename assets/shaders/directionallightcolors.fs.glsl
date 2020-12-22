#version 300 es

precision mediump float;

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec3 fFragColor;

uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDirection;
uniform vec3 uLightIntensity;

vec3 blinnPhong()
{
	vec3 lightDir = normalize(-uLightDirection);

	vec3 ambientColor = vec3(0.1,0.1,0.1)*uKa;

	vec3 diff = uLightIntensity * max(dot(normalize(lightDir),normalize(vNormal_vs)),0.f);
	vec3 diffuseColor = diff * uKd;

	vec3 spec = uLightIntensity * pow(max(dot(normalize(normalize(-vPosition_vs) + lightDir),normalize(vNormal_vs)),0.f),uShininess); 
	vec3 specularColor = spec * uKs;

	return ambientColor + diffuseColor + specularColor;
}

void main()
{
	fFragColor = blinnPhong();
}