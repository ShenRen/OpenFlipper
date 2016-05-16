// instanced mesh is a cube with missing side faces

// instance data
in vec4 inWorld0;
in vec4 inWorld1;
in vec4 inWorld2;
in vec4 inParams; // start_time, end_time, speed, edge_type


out vec4 v2f_params;

uniform float uTime;

void main()
{
  SG_VERTEX_BEGIN;
  
  v2f_params = inParams;
  
  // transform position
  vec4 posWS;
  posWS.x = dot(inWorld0, SG_INPUT_POSOS);
  posWS.y = dot(inWorld1, SG_INPUT_POSOS);
  posWS.z = dot(inWorld2, SG_INPUT_POSOS);
  posWS.w = 1.0;

  sg_vPosVS = g_mWV * posWS;
  sg_vPosPS = g_mWVP * posWS;

#ifdef SG_INPUT_NORMALOS
  // view space normal
  vec3 normalWS;
  normalWS.x = dot(inWorld0.xyz, SG_INPUT_NORMALOS.xyz);
  normalWS.y = dot(inWorld1.xyz, SG_INPUT_NORMALOS.xyz);
  normalWS.z = dot(inWorld2.xyz, SG_INPUT_NORMALOS.xyz);


  sg_vNormalVS = g_mWVIT * normalWS;
  sg_vNormalVS = normalize(sg_vNormalVS);
#endif
  
  SG_VERTEX_END;
}