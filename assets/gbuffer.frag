#version 120

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

void main() {
	vec2 uv = gl_TexCoord[0].st;
	vec4 diffuse = texture2D(diffuseTex, uv);
	vec4 normal = texture2D(normalTex, uv);
	vec4 specular = texture2D(specularTex, uv);

	gl_FragData[0] = vec4(diffuse.rgb, 1.0);
	gl_FragData[1] = vec4(normal.rgb, 1.0);
	gl_FragData[2] = vec4(specular.rgb, 1.0);
}