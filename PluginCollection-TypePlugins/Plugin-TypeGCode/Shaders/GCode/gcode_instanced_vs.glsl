#version 140

// instanced mesh is a cube with missing side faces

// base cube
in vec4 inPosition;
in vec3 inNormal;

// instance data
in vec4 inWorld0;
in vec4 inWorld1;
in vec4 inWorld2;
in vec4 inParams; // start_time, end_time, speed, edge_type


out vec3 gNormal;
out float gEdgeType;
out float gTime;
out float gSpeed;

uniform float uTime;

uniform mat4 g_mWVP;
uniform mat3 g_mWVIT;

void main()
{
  gEdgeType = inParams.w;
  gTime = inParams.x;
  gSpeed = inParams.z;
  
  vec4 posWS;
  posWS.x = dot(inWorld0, inPosition);
  posWS.y = dot(inWorld1, inPosition);
  posWS.z = dot(inWorld2, inPosition);
  posWS.w = 1.0;

  gl_Position = g_mWVP * posWS;

  vec3 normalWS;
  normalWS.x = dot(inWorld0.xyz, inNormal);
  normalWS.y = dot(inWorld1.xyz, inNormal);
  normalWS.z = dot(inWorld2.xyz, inNormal);

  // view space normal
  gNormal = g_mWVIT * normalWS;
  gNormal = normalize(gNormal);
}