#version 330

in vec3 gNormal;
in float gEdgeType;
in float gTime;
in float gSpeed;

out vec4 oColor;

uniform float uTime;
uniform vec4 uColor;

void main(){
  vec3 normal = normalize(gNormal);
  oColor = uColor*max(normal[2], 0.0);
}