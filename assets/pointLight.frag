#version 120

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform vec2 lightPos;
uniform float lightPropertySize;
uniform float bufferRatio;

void main() {
	vec2 uv = gl_TexCoord[0].st;

	vec3 diffuseSrc = texture2D(diffuseTex, uv).rgb;
	vec3 normalSrc = texture2D(normalTex, uv).rgb;
	vec3 specularSrc = texture2D(specularTex, uv).rgb;

	vec3 normal = normalize(vec3(1.0 - normalSrc.r, 1.0 - normalSrc.g, 1.0) * 2.0 - 1.0);

	vec3 localPos = vec3(vec2(lightPos - uv), lightPropertySize / 8);
	localPos.x *= bufferRatio;
	vec3 lightDirection = normalize(localPos);

	// Distance.
	float dist = clamp(1.0 - length(vec2(localPos.x, localPos.y) * (1 / lightPropertySize)), 0.0, 1.0);

	// Lambert.
	float lambert = clamp(dot(normal, lightDirection), 0.0, 1.0);

	// Output color.
	gl_FragColor = vec4(vec3(lambert * dist), 1.0);
}