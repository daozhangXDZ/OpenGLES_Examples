#version 310 es
precision mediump float;
layout(location = 0) out vec4 fragColor;

in vec3 fFragPos;
in vec3 fNormal;
in vec3 camDir;
in vec3 camNormal;
in vec4 lightSpaceFragPos;
in float lightDist;

uniform vec3 viewPos;
uniform vec3 lightDir;
uniform sampler2D depthMap;

void main()
{
	float bias = 0.005f;

	float hitShadowDist = texture(depthMap, lightSpaceFragPos.xy).r;
	float scatDistance = lightDist - hitShadowDist;
	float scatPower = exp(-scatDistance * 10.0f);

	vec3 albedo = vec3(0.7f);
	vec3 N = normalize(fNormal);
	vec3 L = normalize(-lightDir);
	vec3 V = normalize(viewPos - fFragPos);
	vec3 R = reflect(L, N);

	vec3 diff = max(dot(N, L), 0.0f) * albedo;
	fragColor = vec4(scatPower); //* vec4(diff + (albedo * 0.1f), 1.0f);//
}