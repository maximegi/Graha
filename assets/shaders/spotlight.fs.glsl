#version 300 es

precision mediump float;

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec3 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;
uniform vec3 uLightDirection;
uniform float uInnerCutOff;
uniform float uOuterCutOff;

vec3 blinnPhong()
{
	vec3 Li = uLightIntensity / (distance(uLightPos_vs,vPosition_vs) * distance(uLightPos_vs,vPosition_vs));

	vec3 Diffuse = uKd * (max(dot(normalize(uLightPos_vs - vPosition_vs),normalize(vNormal_vs)),0.f));

	vec3 Specular = uKs * pow(max(dot(normalize((normalize(-vPosition_vs) + normalize(uLightPos_vs - vPosition_vs)) / 2.f),normalize(vNormal_vs)),0.f),uShininess);

	return Li * (Diffuse + Specular);
}

void main()
{
	vec3 lightToFrag = normalize(vPosition_vs - uLightPos_vs);

	float theta = dot(normalize(lightToFrag),normalize(uLightDirection));

	float intensity = clamp((theta - uOuterCutOff) / (uInnerCutOff - uOuterCutOff),0.05,1.0);
	
	fFragColor = blinnPhong()*intensity;
}