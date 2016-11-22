#version 130


in vec2 inCamOriginCoeff;

uniform mat4 clipSpaceToWorld;
uniform vec4 camOriginWS; // cam origin in world space

void main()
{
  SG_VERTEX_BEGIN;
  
  
  // transform clip to world space
  vec4 posWS = clipSpaceToWorld * SG_INPUT_POSOS;
  posWS.xyz /= posWS.w;
  posWS.w = 1;
  
  posWS = inCamOriginCoeff.x * posWS + inCamOriginCoeff.y * camOriginWS;
  
  sg_vPosVS = g_mWV * posWS;
  sg_vPosPS = g_mP * sg_vPosVS;
  
  
  SG_VERTEX_END;
  
  
  #ifdef SG_OUTPUT_POSOS
  SG_OUTPUT_POSOS = posWS;
  #endif
}