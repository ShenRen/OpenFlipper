#version 330

in vec3 gNormal;
in float gEdgeType;
in float gTime;
in float gSpeed;

out vec4 oColor;

uniform float uTime;

void main(){
  float speed = min((gSpeed-1200.0)/(4800.0-1200.0), 1.0);
  
  vec3 normal = normalize(gNormal);
  oColor = vec4(1.0-speed, speed, 0.0, 1.0)*max(normal[2], 0.0);
}