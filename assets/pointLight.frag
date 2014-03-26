#version 120

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform vec2 mousePos;

void main() {
	vec2 uv = gl_TexCoord[0].st;
	vec3 diffuse = texture2D(diffuseTex, uv).rgb;
	vec3 normal = texture2D(normalTex, uv).rgb;
	vec3 specular = texture2D(specularTex, uv).rgb;

	float distance = clamp(1.0 - length(uv - vec2(mousePos.x, 1.0 - mousePos.y)) * (1.0 / .25), 0.0, 1.0);

	gl_FragColor = vec4(vec3(distance) * normal, 1.0);
}