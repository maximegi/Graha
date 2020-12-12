#version 300 es

precision mediump float;

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec3 fFragColor;

//uniform sampler2D uTextureDiffuse;
//uniform sampler2D uTextureSpecular;
uniform float uShininess;

uniform vec3 uLightDirection;
uniform vec3 uLightIntensity;

//vec3 blinnPhong()
//{
//	vec3 lightDir = normalize(-uLightDirection);
//
//	vec3 diff = uLightIntensity * max(dot(normalize(lightDir),normalize(vNormal_vs)),0.f);
//	vec3 diffuseColor = diff * vec3(texture(uTextureDiffuse, vTexCoords));
//
//	vec3 spec = uLightIntensity * pow(max(dot(normalize(normalize(-vPosition_vs) + lightDir),normalize(vNormal_vs)),0.f),uShininess); 
//	vec3 specularColor = spec * vec3(texture(uTextureSpecular, vTexCoords));
//
//	return diffuseColor + specularColor + vec3(0.4);
//}

void main()
{
	//fFragColor = blinnPhong();
	fFragColor = vec3(0.4f,0.4f,0.4f);
}