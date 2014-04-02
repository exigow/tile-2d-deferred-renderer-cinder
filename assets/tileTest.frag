#version 120

uniform sampler2D normalTex;

void main() {
	vec2 uv = gl_TexCoord[0].st;
	vec3 normalSrc = texture2D(normalTex, uv).rgb;
	gl_FragColor = vec4(normalSrc, 1.0);
}