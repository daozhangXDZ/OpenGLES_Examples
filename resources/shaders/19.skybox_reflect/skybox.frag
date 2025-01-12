#version 310 es
precision mediump float;
layout(location = 0) out vec4 fragColor;

in vec3 fUVW;

uniform samplerCube cubemap;

void main()
{
    vec3 result = pow(texture(cubemap, fUVW).rgb, vec3(1.0 / 2.2));
	fragColor = texture(cubemap, fUVW);
}