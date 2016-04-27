#version 330

in vec3 gNormal;
in float gEdgeType;
in float gTime;
in float gSpeed;

out vec4 oColor;

uniform float uTime;

void main(){
  float time_passed = (uTime - gTime);
  float heat = exp(-time_passed/10.0);
  
  vec3 normal = normalize(gNormal);
  oColor = vec4(heat, 1.0-heat, 0.0, 1.0)*max(normal[2], 0.0);
}