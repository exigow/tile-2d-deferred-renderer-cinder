#version 120

uniform sampler2D positionMap;

uniform vec3 cameraPosition;

uniform vec2 lightScreenPosition;
uniform float lightRadius;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 lightTransformedPosition;

varying vec4 pos;

void main() {
	vec2 uv = pos.xy / pos.w * 0.5 + 0.5;
	
	vec3 position = texture2D(positionMap, uv).rgb;

	vec3 localPosition = lightTransformedPosition - position;
	float distance = length(localPosition);
	
	float distToCam = length(lightPosition - cameraPosition);
	float haloDistance = pow(clamp(1 - length(abs(vec2(uv.x, 1.0 - uv.y) - lightScreenPosition)) * distToCam / lightRadius, 0, 1), 2) * .125;
	
	vec3 strength = vec3(1.0 - (distance / lightRadius));
	strength.x = pow(strength.x, 2.0);
	strength.y = pow(strength.y, 2.0);
	strength.z = pow(strength.z, 2.0);
	
	vec3 outputColor = vec3(1, 0, 0);
	
	
	gl_FragColor = vec4(outputColor, 1.0);
}
