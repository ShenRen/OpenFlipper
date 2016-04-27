#version 140

// instanced mesh is a cube with missing side faces

// base cube
in vec4 inPosition;
in vec3 inNormal;

// instance data
in vec3 inTranslation;
in vec4 inParams; // start_time, end_time, speed, edge_type


out vec3 gNormal;
out float gEdgeType;
out float gTime;
out float gSpeed;

uniform float uTime;

uniform mat4 g_mWVP;
uniform mat3 g_mWVIT;

uniform vec3 g_Scale;

void main()
{
  gEdgeType = inParams.w;
  gTime = inParams.x;
  gSpeed = inParams.z;
  
  vec4 posWS;
  posWS.xyz = inPosition.xyz * g_Scale + inTranslation;
  posWS.w = 1.0;

  gl_Position = g_mWVP * posWS;

  // view space normal
  gNormal = g_mWVIT * inNormal;
  gNormal = normalize(gNormal);
}