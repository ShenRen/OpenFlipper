
// instance data
in vec3 inTranslation;
in vec4 inParams; // start_time, end_time, speed, edge_type


out vec4 v2f_params;


uniform float uTime;

uniform vec3 g_Scale;

void main()
{
  SG_VERTEX_BEGIN;
  
  v2f_params = inParams;
  
  vec4 posWS;
  posWS.xyz = inPosition.xyz * g_Scale + inTranslation;
  posWS.w = 1.0;

  sg_vPosVS = g_mWV * posWS;
  sg_vPosPS = g_mWVP * posWS;
  

#ifdef SG_INPUT_NORMALOS
  // view space normal
  sg_vNormalVS = g_mWVIT * SG_INPUT_NORMALOS.xyz;
  sg_vNormalVS = normalize(sg_vNormalVS);
#endif

  SG_VERTEX_END;
}