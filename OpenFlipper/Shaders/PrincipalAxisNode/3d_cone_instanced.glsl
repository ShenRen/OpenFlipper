#version 130

in vec4 pa_worldTransform0;
in vec4 pa_worldTransform1;
in vec4 pa_worldTransform2;

in float pa_size;


uniform float pa_cone_radius;
uniform float pa_cone_mirror;

uniform vec3 pa_scale;
uniform vec3 pa_offset;

void main()
{
  SG_VERTEX_BEGIN;
  
  vec4 pa_posOS = SG_INPUT_POSOS;
  
  pa_posOS.xy *= pa_cone_radius;
  pa_posOS.z = pa_posOS.z * pa_cone_radius + 0.85 * pa_size;
  pa_posOS.yz *= pa_cone_mirror;
  
  vec4 pa_posWS;
  pa_posWS.x = dot(pa_worldTransform0, pa_posOS);
  pa_posWS.y = dot(pa_worldTransform1, pa_posOS);
  pa_posWS.z = dot(pa_worldTransform2, pa_posOS);
  pa_posWS.w = 1.0;
  
#ifdef SG_INPUT_NORMALOS
  vec3 pa_normalOS = SG_INPUT_NORMALOS.xyz;
  pa_normalOS.yz *= pa_cone_mirror;
  
  sg_vNormalVS.x = dot(pa_worldTransform0.xyz, pa_normalOS);
  sg_vNormalVS.y = dot(pa_worldTransform1.xyz, pa_normalOS);
  sg_vNormalVS.z = dot(pa_worldTransform2.xyz, pa_normalOS);

  sg_vNormalVS = g_mWVIT * sg_vNormalVS;
  sg_vNormalVS = normalize(sg_vNormalVS);
#endif
  
  sg_vPosVS = g_mWV * pa_posWS;
  sg_vPosPS = g_mWVP * pa_posWS;

  SG_VERTEX_END;
}