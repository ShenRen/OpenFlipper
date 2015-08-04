in vec4 inWorld0;
in vec4 inWorld1;
in vec4 inWorld2;


in vec3 inWorldIT0;
in vec3 inWorldIT1;
in vec3 inWorldIT2;


void main()
{
  SG_VERTEX_BEGIN;
  
  // transform with instance world matrix
  vec4 arrownode_posWS;
  arrownode_posWS.x = dot(inWorld0, inPosition);
  arrownode_posWS.y = dot(inWorld1, inPosition);
  arrownode_posWS.z = dot(inWorld2, inPosition);
  arrownode_posWS.w = 1.0; 

  // transform to view and clip space
  sg_vPosVS = g_mWV * arrownode_posWS;  
  sg_vPosPS = g_mWVP * arrownode_posWS;
#ifdef SG_INPUT_NORMALOS
  sg_vNormalVS.x = dot(inWorldIT0.xyz, SG_INPUT_NORMALOS.xyz);
  sg_vNormalVS.y = dot(inWorldIT1.xyz, SG_INPUT_NORMALOS.xyz);
  sg_vNormalVS.z = dot(inWorldIT2.xyz, SG_INPUT_NORMALOS.xyz);

  sg_vNormalVS = g_mWVIT * sg_vNormalVS;
  sg_vNormalVS = normalize(sg_vNormalVS);
#endif

  SG_VERTEX_END;
}