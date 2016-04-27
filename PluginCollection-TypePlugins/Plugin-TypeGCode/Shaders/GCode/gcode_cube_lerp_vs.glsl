#version 140

// shader for rendering the interpolation of the currently active segment
//  (current time is within this segment)

in vec4 inPosition;
in vec3 inNormal;

out vec3 gNormal;
out float gEdgeType;
out float gTime;
out float gSpeed;

uniform vec4 uParams;

uniform mat4 g_mWVP;
uniform mat3 g_mWVIT;

void main()
{
  gl_Position = g_mWVP * inPosition;
  
  gNormal = normalize(g_mWVIT * inNormal);
  
  gEdgeType = uParams.w;
  gTime = uParams.x;
  gSpeed = uParams.z;
}